#include "DemultStream.h"
#include "ui_DemultStream.h"
//----------------------------------------------------------
DemultStream::DemultStream(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DemultStream)
{
    ui->setupUi(this);

}
//----------------------------------------------------------
DemultStream::~DemultStream()
{
    delete ui;
}
//----------------------------------------------------------
