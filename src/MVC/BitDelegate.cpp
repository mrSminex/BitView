#include "BitDelegate.h"
#include "Model.h"
#include <QPainter>
//----------------------------------------------------------
BitDelegate::BitDelegate(QObject *parent) :
    BaseDelegate(parent)
{

}
//----------------------------------------------------------
void BitDelegate::paintString(int periodStart, int bgn, int cnt, int x, int y, QPainter *p)
{
    qint64  bits;
    int     bitCnt = 1;
    int     c = periodStart;
    if(m_scale > 5){
        p->setRenderHint(QPainter::Antialiasing); // сглаживание
    }
    if( m_model ) {
        while( bitCnt ) {
         // получаем биты
            bitCnt = m_model->getBits( bgn, &bits, cnt );
         // визуализируем биты
            for( int i = 0 ; i < bitCnt ; ++i ) {

                // Проверяем, принадлежит ли бит выбранному каналу
                bool _isSelectedChannel = isSelectedChannel(i, bgn);

                QColor bgColor, bitColor;

                if (_isSelectedChannel) {
                    // Выделенный канал - красный цвет
                    bgColor = m_selectedClr;
                    bitColor = QColor(m_selectedClr).lighter();
                } else {
                    // Обычные цвета
                    bgColor = (c&0x8) ? m_0clr : QColor(m_0clr).darker();
                    bitColor = (bits>>i)&1 ? m_1clr : m_0clr;
                }

                if( m_scale > 5 ) {
//                    p->fillRect( x, y,
//                                 m_width, m_height,(c&0x8) ?  m_0clr : QColor(m_0clr).darker() );
//                    if( (bits>>i)&1 ) {
//                        p->setPen( Qt::NoPen );
//                        p->setBrush( m_1clr );
//                        p->drawEllipse( x+1, y+1, m_width - 2, m_height - 2 );
//                    }
                     p->fillRect( x, y, m_width, m_height, bgColor );
                     if( (bits>>i)&1 ) {
                         p->setPen( Qt::NoPen );
                         p->setBrush( bitColor );
                         p->drawEllipse( x+1, y+1, m_width - 2, m_height - 2 );
                     }
                } else {
//                    p->fillRect( x, y,
//                             m_width, m_height,
//                             (bits>>i)&1 ? m_1clr : m_0clr );
//                    p->fillRect( x, y,
//                             m_width, m_height,
//                             (bits>>i)&1 ? ((c&0x8) ?  m_1clr : QColor(m_1clr).lighter()) : ((c&0x8) ?  m_0clr : QColor(m_0clr).darker()) );
                        QColor finalColor = (bits>>i)&1 ? bitColor : bgColor;
                        if ((bits>>i)&1 && !_isSelectedChannel) {
                            finalColor = (c&0x8) ? bitColor : QColor(bitColor).lighter();
                        }
                        p->fillRect( x, y, m_width, m_height, finalColor );

                }
                x += m_width;
                c++;
            }
//         // итерируемся
            bgn += bitCnt;
            cnt -= bitCnt;
        }
    }
}
//----------------------------------------------------------
bool BitDelegate::isSelectedChannel(int ind, int bgn)
{
    bool _isSelectedChannel = (m_selectedChannel > 0) &&
                            (((bgn + ind) % 256) / 8 == (m_selectedChannel - 1));
    return _isSelectedChannel;
}
//----------------------------------------------------------








