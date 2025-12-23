#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//----------------------------------------------------------
#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include "BaseDelegate.h"
//----------------------------------------------------------
namespace Ui {
class MainWindow;
}
class Model;
//----------------------------------------------------------
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_openACT_triggered();

    void on_closeACT_triggered();

    void on_restoreACT_triggered();

    void on_qtInfoACT_triggered();

    void on_shiftHSL_rangeChanged(int min, int max);

    void printinfo();

    void on_bitACT_toggled(bool arg1);

    void on_hexACT_toggled(bool arg1);


    void on_reverseACT_triggered(); //+++

    void on_demultACT_triggered(); //+++

    void on_actionOxT_toggled(bool arg1);

private:
    Ui::MainWindow  *ui;

    QByteArray      defState;

    QString         fileName;
    QFile           *file;
    Model           *model; //+++


    void            openFile();
    void            setInterfaceEnable(bool val);

    void            setDelegate();

};
//----------------------------------------------------------
#endif // MAINWINDOW_H







