#include "Model.h"
#include <QFile>
#include <QString>
#include <QtGlobal>
#include <array>
//----------------------------------------------------------
namespace {
// Предсозданная таблица для разворота бит в байте
std::array<unsigned char, 256> createReverseTable() {
    std::array<unsigned char, 256> table;

    // Заполняем таблицу для всех возможных значений байта (0-255)
    for (size_t i = 0; i < 256; i++) {
        unsigned char original = static_cast<unsigned char>(i);
        unsigned char reversed = 0;

        // Медленно вычисляем разворот битов (делается только один раз!)
        for (int bit = 0; bit < 8; bit++) {
            reversed = static_cast<unsigned char>((reversed << 1) | (original & 1));
            original >>= 1;
        }

        table[i] = reversed; // Сохраняем результат в таблицу
    }

    return table;
}

const std::array<unsigned char, 256> reverseTable = createReverseTable();
} // namespace

//----------------------------------------------------------
Model::Model(QFile *f,QObject *parent) :
    QObject(parent)
{
    m_file      = f;
    m_size      = static_cast<int>(m_file->size()) * 8;
    m_shift     = 0;
    m_reverse   = false;

    m_page      = new qint8[PAGE_SIZE_TO_READ];
    m_pageNum   = -1;
    m_pageSize  = 0;
}
//----------------------------------------------------------
Model::~Model()
{
    delete m_file;
    delete[] m_page;
}
//----------------------------------------------------------
int Model::getBits(int firstBitNum, qint64 *bits, int bitCnt)
{
    int result = 0;
    int indx;

    *bits = 0;
    firstBitNum += m_shift;
    if( bitCnt > BIT_MAX ) bitCnt = BIT_MAX;

    if( firstBitNum >= 0 &&
        firstBitNum < m_size &&
        bitCnt > 0 )
    {
     // вычисляем сквозной номер байта, содержащего требуемый бит
        indx = firstBitNum  >>  3;// разделить на 8
        readPage( indx / PAGE_SIZE ); //читаем страницу содержащую этот байт // кэширование
        indx = indx % PAGE_SIZE; // номер байта в странице прочитанной
        if( indx < m_pageSize ) {
         // сколько бит будет прочитано
            result = m_size - firstBitNum;
            if( result > bitCnt ) result = bitCnt;
         // выделяем биты
            // указатель на байт где находится интересующий нас бит
            qint64 tmp64 = 0;
            if( m_reverse ){
                // копируем байты с разворотом бит в каждом
                quint8 *dst = reinterpret_cast<quint8*>(&tmp64);  // куда
                quint8 *src = reinterpret_cast<quint8*>(m_page + indx);  // откуда
                for( int i = 0; i < 8; ++i ){
                    dst[i] = reverseTable[ src[i] ];
                }
            } else {
                tmp64 = *(reinterpret_cast<qint64*>(m_page + indx));
            }
            // выделили битики.  firstBitNum & 0x7 остаток от деления на 8 - сколько бит в первом байте мы игнорируем и сдвигаем в право
            *bits = tmp64 >> (firstBitNum & 0x7);
        }
    }
    return result;
}
//----------------------------------------------------------
void Model::setShift(int val)
{
    if( val < 0 ) val = 0;

    if( m_shift != val ) {
        m_shift = val;

        emit needUpdate();
    }
}
//----------------------------------------------------------
void Model::setReverse(bool val)  // +++
{
    if( m_reverse != val ){  // если точно отличается, то газ
        m_reverse = val;
        emit needUpdate();
    }
}
//----------------------------------------------------------
void Model::readPage(int p)
{
    // проверяем ни эта ли страница загружена
    if( m_pageNum != p ) {
        m_pageNum  = p;
        m_pageSize = 0;
        // читаем с перекрытием
        if( m_file->seek( m_pageNum * PAGE_SIZE ) ) {
            m_pageSize = m_file->read( reinterpret_cast<char*>(m_page), PAGE_SIZE_TO_READ );
            size_t n = static_cast<size_t>(PAGE_SIZE_TO_READ - m_pageSize);
            // если осталось место то заполняем нулями
            if( n ) memset( m_page + m_pageSize, 0, n );
        }
    }
}
//----------------------------------------------------------
unsigned char Model::getReverseBytes(unsigned char byte) const
{
    return reverseTable[byte];
}
//----------------------------------------------------------
bool Model::demultiplexChannel(int channel, const QString &outputFileName)
{
    if( outputFileName.isEmpty() ){
        return false;
    }

    QFile outFile( outputFileName );
    if( !outFile.open(QIODevice::WriteOnly) ){
        return false;
    }

    outFile.resize(0);
    unsigned char byteChannel = 0;
    int    startPos = (channel-1) * 8;
    qint64 bits;
    int    bitCnt = 1;

    while( bitCnt ) {
        bitCnt = this->getBits(startPos, &bits, 8);

        if( bitCnt > 0 ){
            // Преобразуем 8 бит в байт (извлекаем младший байт)
            byteChannel = static_cast<unsigned char>(bits & 0xFF);
            if(m_reverse){  // уже развернут
                outFile.write(reinterpret_cast<const char*>(&byteChannel), 1);
            } else {
                byteChannel = this->getReverseBytes(byteChannel);
                outFile.write(reinterpret_cast<const char*>(&byteChannel), 1);
            }
            startPos += 256;
        }
    }
    outFile.close();
    return true;
}
//----------------------------------------------------------





