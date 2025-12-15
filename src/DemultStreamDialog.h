#ifndef DEMULTSTREAMDIALOG_H
#define DEMULTSTREAMDIALOG_H

#include <QDialog>
#include <QString>
namespace Ui {
class DemultStreamDialog;
}

class DemultStreamDialog : public QDialog
{
    Q_OBJECT

public:
    explicit    DemultStreamDialog(QWidget *parent = nullptr);
                ~DemultStreamDialog();

private slots:
    void        chooseOutputFile();
    void        onChannelChanged(int channel);

private:
    Ui::DemultStreamDialog  *ui;
    QString                 filename;

public:
    int         getChannel()     const;
    QString     getFileName()    const;

signals:
    void        channelChanged(int channel);

};

#endif // DEMULTSTREAMDIALOG_H
