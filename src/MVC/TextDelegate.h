#ifndef TEXTDELEGATE_H
#define TEXTDELEGATE_H
//----------------------------------------------------------
#include <BaseDelegate.h>
#include <QObject>
#include <QFont>
// Отображение бит в виде текста UTF-8
//----------------------------------------------------------
class TextDelegate : public BaseDelegate
{
    Q_OBJECT
public:
    explicit TextDelegate(QObject *parent = nullptr);

protected:
    QFont       font;

    // BaseDelegate interface
protected:
    virtual void updateSize() override;
    virtual void paintString(int periodStart, int bgn, int cnt, int x, int y, QPainter *p) override;

    virtual bool isSelectedChannel(int ind, int bgn) override;
};
//----------------------------------------------------------
#endif // TEXTDELEGATE_H
