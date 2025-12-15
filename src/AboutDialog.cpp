#include "AboutDialog.h"
#include "ui_AboutDialog.h"
#include <QMovie>
//----------------------------------------------------------
AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    setWindowFlag( Qt::WindowContextHelpButtonHint, false );

    QMovie  *mv = new QMovie;
    mv->setFileName( ":/img/fly.gif" );
    ui->imageLBL->setMovie( mv );
    mv->start();
}
//----------------------------------------------------------
AboutDialog::~AboutDialog()
{
    delete ui;
}
//----------------------------------------------------------
