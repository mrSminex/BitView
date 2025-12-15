#include "Viewer.h"
#include <QPaintEvent>
#include <QResizeEvent>
#include <QDebug>
#include <QPainter>
//----------------------------------------------------------
Viewer::Viewer(QWidget *parent) :
    QWidget(parent),
    vScreen(nullptr),
    m_model(nullptr),
    m_delegate(nullptr)
{
    m_period        = 256;

    m_row           = 0;
    m_rowMAX        = 0;
    m_rowVisible    = 0;

    m_clm           = 0;
    m_clmMAX        = 0;
    m_clmVisible    = 0;

    m_mouseFlag     = false;
    m_startX        = 0;
    m_startY        = 0;
    m_startRow      = 0;
    m_startClm      = 0;



    vScreen = new QPixmap( this->size() );

    recalculate();
}
//----------------------------------------------------------
Viewer::~Viewer()
{
    delete vScreen;
}
//----------------------------------------------------------
void Viewer::setModel(Model *m)
{
    if( m_model != m ) {
        delete m_model;
        m_model = m;
        if( m_delegate ) m_delegate->setModel( m_model );
        if( m_model ) {
            m_model->setParent( this );
            connect( m_model, &Model::needUpdate,
                     this,    &Viewer::recalculate );
            m_row           =  0;
            m_rowMAX        = -1;
            m_rowVisible    = -1;

            m_clm           =  0;
            m_clmMAX        = -1;
            m_clmVisible    = -1;
        }
        recalculate();
    }
}
//----------------------------------------------------------
void Viewer::setDelegate(BaseDelegate *d)
{
    if( m_delegate != d ) {
        delete m_delegate;
        m_delegate = d;

        if( m_delegate ) {
            m_delegate->setModel( m_model );
            m_delegate->setParent( this );
            connect( m_delegate, &BaseDelegate::needUpdate, this, &Viewer::recalculate );
            m_row           =  0;
            m_rowMAX        = -1;
            m_rowVisible    = -1;

            m_clm           =  0;
            m_clmMAX        = -1;
            m_clmVisible    = -1;
        }
        recalculate();
    }
}
//----------------------------------------------------------
void Viewer::setPeriod(int val)
{
    if( val < 1 ) val = 1;

    if( m_period != val ) {
        m_period =  val;
        emit shiftRangeChanged( 0, m_period-1 );
        recalculate();
    }
}
//----------------------------------------------------------
void Viewer::setStartRow(int val)
{
    val = qBound( 0, val, m_rowMAX );

    if( m_row != val ) {
        m_row  = val;
        emit rowChanged( m_row );
        recalculate();
    }
}
//----------------------------------------------------------
void Viewer::setStartClm(int val)
{
    val *= bitsInElement();
    val = qBound( 0, val, m_clmMAX );

    if( m_clm != val ) {
        m_clm  = val;
        emit clmChanged( m_clm / bitsInElement() );
        recalculate();
    }
}
//----------------------------------------------------------
void Viewer::recalculate()
{
    int mr, mc, tmpI;

    if(m_model && m_delegate){

        //максимальное число строк
        tmpI = m_model->size();
        mr = tmpI / m_period;  // число полных строк
        if(tmpI % m_period) mr++; // есть ещё одна строка, если остаток не ноль

        //число строк на экране
        m_rowVisible = size().height() / m_delegate->height(); // размер виджета поделить на высоту делегата(отображаемого квадратика)
        if(size().height() % m_delegate->height()) m_rowVisible++;

        //число столбцов на экране
        m_clmVisible = size().width() / m_delegate->width(); // размер виджета поделить на высоту делегата(отображаемого квадратика)
        if(size().width() % m_delegate->width()) m_clmVisible++;
        m_clmVisible *= bitsInElement();  // (ширинав пикселях / ширину элемента) * на ширину бита
        //корректировка максимума
        mr -= m_rowVisible / 2;  //отступ
        if(mr < 0) mr = 0;

        //максимальное число столбцов
        mc = m_period - m_clmVisible / 2;
        if(mc < 0) mc = 0;

        //корректировка текущих значений
        if(m_row > mr) {
            m_row = mr;
            emit rowChanged(m_row);
        }

        if(m_clm > mc) {
            m_clm = mc;
            emit clmChanged(m_clm / bitsInElement());
        }

        //изменяем диапазон
        if(m_rowMAX != mr){
            m_rowMAX = mr;
            emit rowRangeChanged(0, m_rowMAX);
        }

        if(m_clmMAX != mc){
            m_clmMAX = mc;
            emit clmRangeChanged(0, m_clmMAX);
        }

    }


    draw();
}
//----------------------------------------------------------
void Viewer::draw()
{
    // метод отрисовки
    vScreen->fill( Qt::black );
    QPainter p(vScreen);

    if(m_model && m_delegate){
        //формируем отображаемый прямоуголбник
        QRect r(m_clm, m_row, qMin(m_clmVisible, m_period - m_clm), m_rowVisible);
        //отрисовываем данные внутри прямоуголбника
        m_delegate->paintRectangle(m_period, r, m_clm, m_row, &p);
    } else {
        p.fillRect( vScreen->rect(), QBrush(Qt::darkGreen,Qt::DiagCrossPattern) );
    }

    update();

}
//----------------------------------------------------------
void Viewer::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    p.drawPixmap( event->rect(), *vScreen, event->rect() );
}
//----------------------------------------------------------
void Viewer::resizeEvent(QResizeEvent *event)
{
    delete vScreen; vScreen = nullptr;

    vScreen = new QPixmap( event->size() );

    recalculate();

    //закрашивание
//    vScreen->fill( Qt::black );
//    QPainter p(vScreen);
    //    p.fillRect( vScreen->rect(), QBrush(Qt::darkGreen,Qt::DiagCrossPattern) );
}
//----------------------------------------------------------
void Viewer::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        m_mouseFlag     = true;
        m_startX        = event->x();
        m_startY        = event->y();
        m_startRow      = row();
        m_startClm      = clm();
    }
}
//----------------------------------------------------------
void Viewer::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        m_mouseFlag = false;
    }
}
//----------------------------------------------------------
void Viewer::mouseMoveEvent(QMouseEvent *event)
{
    if(m_mouseFlag && m_delegate){
        int dx = (m_startX - event->x()) / m_delegate->width();
        int dy = (m_startY - event->y()) / m_delegate->height();

        setStartClm(m_startClm + dx);
        setStartRow(m_startRow + dy);
    }

}
//----------------------------------------------------------
void Viewer::wheelEvent(QWheelEvent *event)
{
    // битовое И!!!!!
    if(event->modifiers() & Qt::ControlModifier){
        if(m_delegate){
            m_delegate->setScale(m_delegate->scale() + static_cast<int>((event->delta() / 120.)));
        }
    } else{
        if(event->modifiers() & Qt::ShiftModifier){
            int d = static_cast<int>(event->delta() / 120.) * 16; // сколько столбиков хотим сместить
            //d *= bitsInElement(); // смещаем в элементах

            setStartClm(clm() - d);
        }else{

            setStartRow(m_row - static_cast<int>(event->delta() / 120.) * 16); // +- 120 градусов. 16 сколько  прокручиваем на один щелчек колесика мыши
        }
    }
}
//----------------------------------------------------------




