#include "BaseDelegate.h"
#include "Model.h"
#include <QRect>
//----------------------------------------------------------
BaseDelegate::BaseDelegate(QObject *parent) : QObject(parent)
{
    m_width     = 0;
    m_height    = 0;
    m_scale     = 0;

    m_bie       = 1;

    m_0clr      = Qt::darkMagenta;
    m_1clr      = Qt::green;
    m_selectedClr = Qt::red;

    m_model     = nullptr;
    m_selectedChannel = -1;

    setScale( 1 );
}
//----------------------------------------------------------
void BaseDelegate::setModel(Model *m)
{
    m_model = m;
}
//----------------------------------------------------------
void BaseDelegate::paintRectangle(int period,
                                  const QRect &r,
                                  int startClm, int startRow,
                                  QPainter *p)
{
    int bgn;
    int x = (r.x() - startClm)*width();
    int y = (r.y() - startRow)*height();

    bgn = r.y()*period + r.x();
    for( int i = 0 ; i < r.height() ; ++i ) {
        paintString( startClm,
                     bgn, r.width(),
                     x, y,
                     p );
        bgn += period;
        y   += height();
    }
}
//----------------------------------------------------------
void BaseDelegate::setScale(int s)
{
    s = qBound( 1, s, int(MAX_SCALE) ); // ограничение величины

    if( m_scale != s ) {
        m_scale  = s;

        updateSize();

        emit needUpdate();
    }
}
//----------------------------------------------------------
void BaseDelegate::setSelectedChannel(int channel)
{
    if (m_selectedChannel != channel) {
        m_selectedChannel = channel;
        emit needUpdate();
    }
}
//----------------------------------------------------------
void BaseDelegate::updateSize()
{
    m_width = m_height = m_scale;
}
//----------------------------------------------------------
