#include "HexDelegate.h"
#include <QFontMetrics>
#include <QPainter>
#include "Model.h"
#include <QColor>
//--------------------------------------------------------------------------
HexDelegate::HexDelegate(QObject *parent) : BaseDelegate(parent)
{
    // число бит в одном элемнете
    m_bie = 8;   // bit eat element

    font.setFamily("Courier new");
    //установка новых размеров
    m_scale = 0;
    setScale(1);
    //сделаем преобразования один раз
    for(int i = 0; i < 256; i++){
        //table[i] = QString::number(i, 16).toUpper();
        table[i] = QString("%1").arg(i,2,16, QChar('0')).toUpper();

    }

}
//--------------------------------------------------------------------------
void HexDelegate::updateSize()
{
    // подгоняем размер под текст
    font.setPixelSize(m_scale*6); // эмпирическая

    QFontMetrics metric(font);
    QRect r = metric.boundingRect("FF");

    m_width  = r.width();
    m_height = r.height();

}
//--------------------------------------------------------------------------
void HexDelegate::paintString(int periodStart, int bgn, int cnt, int x, int y, QPainter *p)
{
    //для хранения битиков
    qint64 bits;
    int    bCnt = 1;
    int    c = periodStart;
    if(m_model){
        p->setFont(font);
        p->setPen(m_1clr);

        while(cnt > 0){
            bCnt = m_model->getBits(bgn, &bits, 8); // получаем 4 бита - (откуда, куда, сколько)
            if(bCnt == 8){
                // Проверяем, пересекаются ли эти 4 бита с выбранным каналом
                bool _isSelectedChannel = isSelectedChannel(0, bgn);

                QColor bgColor = _isSelectedChannel ? m_selectedClr :
                               ((c & 0x8) ? m_0clr : QColor(m_0clr).darker());
                QColor textColor = _isSelectedChannel ? QColor(m_selectedClr).lighter() : m_1clr;
                //рисуем
                //p->fillRect(x, y,  m_width, m_height, (c & 0x8) ? m_0clr : QColor(m_0clr).darker());
                p->fillRect(x, y,  m_width, m_height, bgColor);
                p->setPen(textColor);
                p->drawText(x, y + m_height, table[bits & 0xFF]); // младщие 4 бита

            }
            cnt -= 8;
            bgn += 8;
            x   += m_width;
            c   += 8;
        }

    }
}
//--------------------------------------------------------------------------
bool HexDelegate::isSelectedChannel(int ind, int bgn)
{
    bool _isSelectedChannel = false;
    if (m_selectedChannel > 0) {
        int channelStart = (m_selectedChannel - 1) * 8; // Начало канала в периоде
        int elementStart = bgn % 256; // Начало элемента в периоде
        int elementEnd = (elementStart + 3) % 256; // Конец элемента в периоде

        // Проверяем пересечение с каналом [channelStart, channelStart+7]
        if (elementStart <= elementEnd) {
            // Элемент не пересекает границу периода
            _isSelectedChannel = (elementStart <= channelStart + 7) && (elementEnd >= channelStart);
        } else {
            // Элемент пересекает границу периода
            _isSelectedChannel = (elementStart <= channelStart + 7) || (elementEnd >= channelStart);
        }
    }
    return _isSelectedChannel;
}
//--------------------------------------------------------------------------
