#include "DemultStreamDialog.h"
#include "ui_DemultStreamDialog.h"
//-------------------------------------------------------------------------
#include <QFileDialog>
#include <QDebug>
//-------------------------------------------------------------------------
DemultStreamDialog::DemultStreamDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DemultStreamDialog),
    filename("")
{
    ui->setupUi(this);


    ui->channelSL->setRange(1, 32);
    ui->channelSB->setRange(1, 32);
    ui->channelSB->setValue(1);
    ui->channelSL->setValue(1);

    connect(ui->outputPBN, &QPushButton::clicked,
            this,         &DemultStreamDialog::chooseOutputFile);
    // Соединение для изменения канала
    connect(ui->channelSB, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &DemultStreamDialog::onChannelChanged);
    connect(ui->channelSL, &QSlider::valueChanged,
            this, &DemultStreamDialog::onChannelChanged);


}
//-------------------------------------------------------------------------
DemultStreamDialog::~DemultStreamDialog()
{
    delete ui;
}
//-------------------------------------------------------------------------
void DemultStreamDialog::chooseOutputFile()
{
     this->filename = QFileDialog::getSaveFileName(
                this,
                tr("Выберите выходной файл"),
                ui->outputLED->text());

    if (!filename.isEmpty()) {
        ui->outputLED->setText(filename);
    }
}
//-------------------------------------------------------------------------
int DemultStreamDialog::getChannel() const
{
    return ui->channelSB->value();
}
//-------------------------------------------------------------------------
QString DemultStreamDialog::getFileName() const
{
    return this->filename;
}
//-------------------------------------------------------------------------
void DemultStreamDialog::onChannelChanged(int channel)
{
    emit channelChanged(channel);
}
//-------------------------------------------------------------------------
