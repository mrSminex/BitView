#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "AboutDialog.h"
#include "DemultStreamDialog.h"

#include "DemultStream.h"
#include <QSettings>
#include <QDebug>
//----------------------------------------------------------
#include "BitDelegate.h"
#include "HexDelegate.h"
#include "TextDelegate.h"
#include "Model.h"
//----------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    file( nullptr ),
    model( nullptr )
{
    ui->setupUi(this);
    ui->reverseACT->setCheckable(true);

    ui->periodHSL->setRange( 1, 32640 );
    ui->periodSBX->setRange( 1, 32640 );
    ui->scaleHSL->setRange(1, BaseDelegate::MAX_SCALE);
    ui->scaleSBX->setRange(1, BaseDelegate::MAX_SCALE);

    defState = saveState();
    ui->closeACT->trigger();

    AboutDialog *ab = new AboutDialog( this );
    connect( ui->aboutACT, &QAction::triggered,
             ab,           &AboutDialog::show );


    QSettings s;
    restoreGeometry( s.value("geometry").toByteArray() );
    restoreState(    s.value("state"   ).toByteArray() );

    fileName = s.value( "fileName" ).toString();
    if( s.value("openFlag",true).toBool() ) {
        openFile();
    }

    ui->periodHSL->setValue(  s.value("period",256).toInt()  );
    ui->shiftHSL->setValue(   s.value("shift",   0).toInt()  );
    ui->scaleHSL->setValue(   s.value("scale",   5).toInt()  );
    ui->rowVSB->setValue(     s.value("row",     0).toInt()  );
    ui->clmHSB->setValue(     s.value("clm",     0).toInt()  );
}
//----------------------------------------------------------
MainWindow::~MainWindow()
{
    QSettings s;

    s.setValue("geometry", saveGeometry() );
    s.setValue("state",    saveState()    );

    s.setValue("period", ui->periodHSL->value() );
    s.setValue("shift",  ui->shiftHSL->value()  );
    s.setValue("scale",  ui->scaleHSL->value()  );
    s.setValue("row",    ui->rowVSB->value()    );
    s.setValue("clm",    ui->clmHSB->value()    );

    s.setValue("fileName", fileName );
    s.setValue("openFlag", file && file->isOpen() );

    ui->closeACT->trigger();

    delete ui;
}
//----------------------------------------------------------
void MainWindow::on_openACT_triggered()
{
    QString str = QFileDialog::getOpenFileName(
                    this,
                    "Выберете файл",
                    QFileInfo(fileName).path()
                );
    if( !str.isEmpty() ) {
        fileName = str;

        openFile();
    }
}
//----------------------------------------------------------
void MainWindow::on_closeACT_triggered()
{
    setInterfaceEnable( false );
    ui->dataVWR->setModel(nullptr); // пустышка
    model = nullptr;
    file = nullptr;
}
//----------------------------------------------------------
void MainWindow::on_restoreACT_triggered()
{
    restoreState( defState );
}
//----------------------------------------------------------
void MainWindow::on_qtInfoACT_triggered()
{
    qApp->aboutQt();
}
//----------------------------------------------------------
void MainWindow::openFile()
{
    if( !fileName.isEmpty() ) {
        ui->closeACT->trigger();

        file = new QFile( fileName );

        if( file->open(QIODevice::ReadOnly) ){
            setWindowTitle( QDir::toNativeSeparators(fileName) );

            model = new Model(file);  // Model *model = new Model(file)???????????????????
            model->setShift(ui->shiftHSL->value());

            connect(ui->shiftHSL, &QSlider::valueChanged, model, &Model::setShift);
            connect(model, &Model::needUpdate, [=](){
                ui->shiftHSL->setValue(model->shift());
            });

            ui->dataVWR->setModel(model);

//            BaseDelegate *delegate = new BitDelegate; // создали, установили
//            delegate->setScale(ui->scaleHSL->value()); // связали с изменением на польз интерфейсе
//            connect(ui->scaleHSL, &QSlider::valueChanged, delegate, &BaseDelegate::setScale );
            setDelegate();

//            ui->dataVWR->setDelegate(delegate);   // установили


        } else {
            ui->closeACT->trigger();
        }
    }
    setInterfaceEnable( file && file->isOpen() );
}
//----------------------------------------------------------
void MainWindow::setInterfaceEnable(bool val)
{
    //если конетйнер выключается,то все остальные компоненты тоже отключаются
    ui->paramDWG->setEnabled( val );

    ui->closeACT->setEnabled( val );

    ui->rowVSB->setEnabled( val );
    ui->clmHSB->setEnabled( val );

    if( !val ) {
        setWindowTitle( "BitView" );
    }
}
//----------------------------------------------------------
void MainWindow::on_shiftHSL_rangeChanged(int min, int max)
{
    //изменение диапазона привязаваем к спин боксу
    ui->shiftSBX->setRange(min, max);

}
//----------------------------------------------------------
void MainWindow::printinfo()
{
    ui->rowVSB->setPageStep(ui->dataVWR->rowVisible());
    ui->clmHSB->setPageStep(ui->dataVWR->clmVisible());

    ui->statusBar->showMessage(QString("СТРОКА = %1 (от 0 до %2)   |   "
                                       "СТОЛБЕЦ = %11 (от 0 до %12)").
                                 arg(ui->dataVWR->row()).
                                 arg(ui->dataVWR->rowMAX()).
                                 arg(ui->dataVWR->clm()).
                                 arg(ui->dataVWR->clmMAX())
                               );

}
//----------------------------------------------------------
// чтобы действия были взаимоисключающими
void MainWindow::on_bitACT_toggled(bool arg1)
{
    ui->bitACT->setChecked(arg1);
    ui->hexACT->setChecked(false);
    ui->actionOxT->setChecked(false);
    setDelegate();
}
//----------------------------------------------------------
void MainWindow::on_hexACT_toggled(bool arg1)
{
    ui->bitACT->setChecked(false);
    ui->hexACT->setChecked(arg1);
    ui->actionOxT->setChecked(false);
    setDelegate();
}
//----------------------------------------------------------
void MainWindow::on_actionOxT_toggled(bool arg1)
{
    ui->bitACT->setChecked(false);
    ui->hexACT->setChecked(false);
    ui->actionOxT->setChecked(arg1);
    setDelegate();
}
//----------------------------------------------------------
void MainWindow::setDelegate()
{
    BaseDelegate *delegate = nullptr;

    if(ui->bitACT->isChecked()){
        delegate = new BitDelegate(this); // создали,
    } else if(ui->hexACT->isChecked()){
        delegate = new HexDelegate(this);
    } else if(ui->actionOxT->isChecked()){
        delegate = new TextDelegate(this);
    }
    if(delegate){
        delegate->setScale(ui->scaleHSL->value()); // связали с изменением на польз интерфейсе
        connect(ui->scaleHSL, &QSlider::valueChanged, delegate, &BaseDelegate::setScale );
        connect(delegate, &BaseDelegate::needUpdate, [=](){
            ui->scaleHSL->setValue(delegate->scale());
        } );
    }

    ui->dataVWR->setDelegate(delegate);
}
//----------------------------------------------------------
void MainWindow::on_reverseACT_triggered()
{

    if( model ){
        const bool reversed = !model->reverse();      // правильно ли так???? что модель глобально
        model->setReverse( reversed );                //
        ui->reverseACT->setChecked( reversed );
    } else {
        ui->reverseACT->setChecked( false );
    }
}
//------------------------------------------------------------------
void MainWindow::on_demultACT_triggered()
{
    if( !model ){
        qDebug() << "Модель не загружена";
        return;
    }

   DemultStreamDialog *dsd = new DemultStreamDialog( this );

    // Получаем текущий делегат для связи с диалогом
   BaseDelegate *currentDelegate = nullptr;
   if (model) {
       // Получаем делегат через viewer
       currentDelegate = dynamic_cast<Viewer*>(ui->dataVWR)->getDelegate();
   }

   // Подключаем сигнал изменения канала к делегату
   if (currentDelegate) {
       connect(dsd, &DemultStreamDialog::channelChanged,
               currentDelegate, &BaseDelegate::setSelectedChannel);
   }


    dsd->exec();
    if( dsd->result()){
        int channel = dsd->getChannel();
        QString outputFileName = dsd->getFileName();
        
        if( model->demultiplexChannel(channel, outputFileName) ){
            qDebug() << "+++Демультеплисксирование успешно завершено+++";
        } else {
            qDebug() << "error on_demultACT_triggered";
        }
    }else {
        // Если диалог был отменен, сбрасываем выделение канала
        if (currentDelegate) {
            currentDelegate->setSelectedChannel(-1);
        }
    }

    delete dsd;
}
//------------------------------------------------------------------

