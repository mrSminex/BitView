#ifndef MODEL_H
#define MODEL_H
//----------------------------------------------------------
#include <QObject>
//----------------------------------------------------------
class QFile;    // компилятор понимает что где то есть такое класс и уже умеет оперировать им. А include  в cpp
class QString;  //  также для demultiplexChannel
//----------------------------------------------------------
class Model : public QObject
{
    Q_OBJECT
public:
    //наша модель адресует данные по битно и умеет игнорировать сколько то бит
    explicit        Model( QFile *f, QObject *parent = nullptr);
                   ~Model();

    int             size()  const { return m_size - m_shift; }
    int             shift() const { return m_shift; }
    bool            reverse() const { return m_reverse; }

    int             getBits( int firstBitNum, qint64 *bits, int bitCnt );
    
    bool            demultiplexChannel( int channel, const QString &outputFileName ); //+++

signals:
    void            needUpdate();
public slots:
    void            setShift( int val );
    void            setReverse( bool val );  // +++
protected:
    //кэширование - разобьем весь файл на странички. и будем читать по страничкам
        // но странички должны быть с перекрытием
    static const    int PAGE_SIZE_TO_READ = 1 << 24;
    static const    int PAGE_SIZE = PAGE_SIZE_TO_READ-8;
public:
    // число бит которые можно прочитать за один раз в модели
    static const    int BIT_MAX = 64-7;
protected:
    QFile           *m_file;
    int             m_size;
    int             m_shift;
    bool            m_reverse;    // +++

    qint8           *m_page;
    int             m_pageNum;
    int             m_pageSize;

    void            readPage( int p );                          // прочитай страницу с номером
    unsigned char   getReverseBytes(unsigned char byte) const;   //+++
};
//----------------------------------------------------------
#endif // MODEL_H




