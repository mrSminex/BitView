#ifndef HEXDELEGATE_H
#define HEXDELEGATE_H
//--------------------------------------------------------------------------
#include <BaseDelegate.h>
#include <QFont>
//--------------------------------------------------------------------------
class HexDelegate : public BaseDelegate
{
    Q_OBJECT
public:
    explicit HexDelegate(QObject *parent = nullptr);

protected:
    QFont           font;       // шрифт
    QString         table[256];  // соответствие число - как отображается

    virtual void    updateSize() override;
    virtual void    paintString(int periodStart, int bgn, int cnt, int x, int y, QPainter *p) override;
    virtual bool    isSelectedChannel(int ind, int bgn) override;
};
//--------------------------------------------------------------------------
#endif // HEXDELEGATE_H
