#ifndef DEMULTSTREAM_H
#define DEMULTSTREAM_H
//----------------------------------------------------------
#include <QWidget>
//----------------------------------------------------------
namespace Ui {
class DemultStream;
}
//----------------------------------------------------------
class DemultStream : public QWidget
{
    Q_OBJECT

public:
    explicit DemultStream(QWidget *parent = nullptr);
    ~DemultStream();

private:
    Ui::DemultStream *ui;
};
//----------------------------------------------------------
#endif // DEMULTSTREAM_H
