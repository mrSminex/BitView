#ifndef BITDELEGATE_H
#define BITDELEGATE_H
//----------------------------------------------------------
#include "BaseDelegate.h"
//----------------------------------------------------------
class BitDelegate : public BaseDelegate
{
    Q_OBJECT
public:
    explicit BitDelegate(QObject *parent = nullptr);
    // BaseDelegate interface
protected:
    virtual void paintString(int periodStart, int bgn, int cnt, int x, int y, QPainter *p) override;
    virtual bool isSelectedChannel(int ind, int bgn) override;
};
//----------------------------------------------------------
#endif // BITDELEGATE_H
