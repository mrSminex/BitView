#ifndef BASEDELEGATE_H
#define BASEDELEGATE_H
//----------------------------------------------------------
#include <QObject>
#include <QColor>
// delegate - отрисовывает прямоугольник битового потока - что то делает
//----------------------------------------------------------
class Model;
class QPainter;
//----------------------------------------------------------
class BaseDelegate : public QObject
{
    Q_OBJECT
public:
    static const int MAX_SCALE = 50;
public:
    explicit        BaseDelegate(QObject *parent = nullptr);

    void            setModel( Model *m );

    int             bitsInElement() const { return m_bie; }

    int             width() const  { return m_width; }
    int             height() const { return m_height; }

    int             scale() const { return m_scale; }
    //отрисовка прямоугольника в периодической развертке - мартрассе
        //paintRectangle бует собирать из отрисованных строк - абстрактная функция будет рисовать строку
    void            paintRectangle( int period, const QRect &r, int startClm, int startRow, QPainter *p );
signals:
    void            needUpdate();
public slots:
    void            setScale( int s );
    void            setSelectedChannel( int channel );
protected:
    int             m_width;
    int             m_height;
    int             m_scale;

    QColor          m_0clr;
    QColor          m_1clr;
    QColor          m_selectedClr;

    Model           *m_model;

    int             m_bie;
    int             m_selectedChannel;

    virtual void    updateSize();
    //чисто абстрактная фукнция!!!!!!!!!!!!!!!!!
    virtual void    paintString( int periodStart, int bgn, int cnt, int x, int y, QPainter *p ) = 0;
    virtual bool    isSelectedChannel(int ind, int bgn) = 0;
};
//----------------------------------------------------------
#endif // BASEDELEGATE_H
