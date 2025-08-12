#ifndef CANVAS_H
#define CANVAS_H

#include <QMouseEvent>
#include <QWidget>

#include "common/Figure.h"
#include "common/Relation.h"

#include "engine/Builder.h"
#include "view/Painter.h"

class CCanvas
: public QWidget
, public CPainter
, public CBuilder
{
    Q_OBJECT
public:

    explicit CCanvas( QWidget *pParent, CAppOption * const, Logger_t * const );
    ~CCanvas();

protected:

    void paintEvent           ( QPaintEvent * ) override;
    void resizeEvent          ( QResizeEvent* ) override;
    void mousePressEvent      ( QMouseEvent * ) override;
    void mouseReleaseEvent    ( QMouseEvent * ) override;
    void mouseMoveEvent       ( QMouseEvent * ) override;
    void mouseDoubleClickEvent( QMouseEvent * ) override;
    void keyPressEvent        ( QKeyEvent   * ) override;
    void keyReleaseEvent      ( QKeyEvent   * ) override;

    bool  m_bMouseLButtonMoved;
    bool  m_bBreakAddFigure;
    long  m_nDir, m_nX, m_nY;

    using QWidget::QWidget;

// update/draw tiemrs
    QTimer *m_pOscilationTimer;

public slots:
    void oscilationBuilder();
};

#endif // CANVAS_H
