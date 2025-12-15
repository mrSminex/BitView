#ifndef VIEWER_H
#define VIEWER_H
//----------------------------------------------------------
#include <QWidget>
//----------------------------------------------------------
#include "BaseDelegate.h"
#include "Model.h"
//----------------------------------------------------------
class Viewer : public QWidget
{
    Q_OBJECT
public:
    explicit        Viewer(QWidget *parent = nullptr);
                   ~Viewer() override;

    void            setModel( Model *m );
    void            setDelegate( BaseDelegate *d );
    BaseDelegate*   getDelegate() const { return m_delegate; }

    int             period() const { return m_period; }

    int             bitsInElement() const { return m_delegate ?
                                            m_delegate->bitsInElement() : 1;
                                          }
    int             row()        const { return m_row; }
    int             rowMAX()     const { return m_rowMAX; }
    int             rowVisible() const { return m_rowVisible; }

    int             clm()        const { return m_clm       / bitsInElement(); }
    int             clmMAX()     const { return m_clmMAX    / bitsInElement(); }
    int             clmVisible() const { return m_clmVisible/ bitsInElement(); }
signals:
    void            rowRangeChanged( int min, int max );
    void            rowChanged( int row );
    void            clmRangeChanged( int min, int max );
    void            clmChanged( int clm );
    void            shiftRangeChanged( int min, int max );
public slots:
    void            setPeriod( int val );
    void            setStartRow( int val );
    void            setStartClm( int val );
    void            recalculate();
protected:
    QPixmap         *vScreen;

    Model           *m_model;
    BaseDelegate    *m_delegate;

    int             m_period;

    int             m_row;
    int             m_rowMAX;
    int             m_rowVisible; // сколько столбиков видно

    int             m_clm;
    int             m_clmMAX;
    int             m_clmVisible;

    void            draw();

    virtual void    paintEvent(QPaintEvent *event)        override;
    virtual void    resizeEvent(QResizeEvent *event)      override;

    // QWidget interface
protected:

    bool            m_mouseFlag;
    int             m_startX;
    int             m_startY;
    int             m_startRow;
    int             m_startClm;

    virtual void    mousePressEvent(QMouseEvent *event)   override;
    virtual void    mouseReleaseEvent(QMouseEvent *event) override;
    virtual void    mouseMoveEvent(QMouseEvent *event)    override;
    virtual void    wheelEvent(QWheelEvent *event)        override;
};
//----------------------------------------------------------
#endif // VIEWER_H
