#include "mainwindow.h"
#include <QApplication>
//----------------------------------------------------------
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName( "Mattress-2025-2" );
    a.setOrganizationName( "theCompany" );

    MainWindow w;
    w.show();

    return a.exec();
}
//----------------------------------------------------------
