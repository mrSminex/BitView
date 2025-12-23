#include "TextDelegate.h"
#include "Model.h"
#include <QFontMetrics>
#include <QPainter>
#include <QColor>
//----------------------------------------------------------
TextDelegate::TextDelegate(QObject *parent) : BaseDelegate(parent)
{
    this->m_bie = 8;
    font.setFamily("Courier New");
    font.setPixelSize(m_scale * 6);

    //установка новых размеров
    this->m_scale = 0;
    setScale(1);
}
//----------------------------------------------------------
void TextDelegate::updateSize()
{
    // подгоняем размер под текст
    font.setPixelSize(m_scale * 6); // эмпирическая настройка

    QFontMetrics metric(font);
    QRect r = metric.boundingRect("W"); // используем широкий символ для расчета

    m_width  = r.width();
    m_height = r.height();
}
//----------------------------------------------------------
void TextDelegate::paintString(int periodStart, int bgn, int cnt, int x, int y, QPainter *p)
{
    //для хранения бит
    qint64 bits;
    int    bCnt = 1;
    int    c = periodStart;
    if(m_model){
        p->setFont(font);
        p->setPen(m_1clr);
        QString textBuffer; // буфер для накопления текста
        while(cnt > 0 && bCnt > 0){
            bCnt = m_model->getBits(bgn, &bits,  qMin(8, cnt)); // получаем 8 бит (1 байт)
            if(bCnt >= 8){
                QString ch = ".";
                // Конвертируем 8 бит в байт
                unsigned char byteValue = static_cast<unsigned char>(bits & 0xFF);
                // Добавляем символ в буфер, если он printable или пробел
                if(byteValue >= 1 && byteValue <= 126) {
                    ch = QChar(byteValue);
                } else if(byteValue == 9) { // tab
                    textBuffer += ".";
                } else if(byteValue == 10 || byteValue == 13) { // \n or \r
                    textBuffer += ".";
                } else {
                    textBuffer += "."; // невидимый символ заменяем точкой
                }

                p->fillRect(x, y, m_width, m_height, (c & 0x8) ? m_0clr : QColor(m_0clr).darker());
                p->drawText(x, y + m_height, ch);

            } else if(bCnt > 0) {
                // Если неполный байт, рисуем пустой символ
                p->fillRect(x, y, m_width, m_height, (c & 0x8) ? m_0clr : QColor(m_0clr).darker());
                p->drawText(x, y + m_height, ".");
            }
            cnt -= bCnt;
            bgn += bCnt;
            x   += m_width;
            c   += bCnt;
        }
    }
}

bool TextDelegate::isSelectedChannel(int ind, int bgn)
{

}
//----------------------------------------------------------
