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

    using QWidget::QWidget;

//     // update/draw tiemrs
    QTimer *drawTimer;
    QTimer *updateTimer;
    QTime  *deltaTimer;

    bool isFirstFrameFinished;
    float minTimeStepValue;
    float maxTimeStepValue;
    float deltaTimeModifier;
    float minDeltaTimeModifier;
    float maxDeltaTimeModifier;

public slots:
    void updateBuilder();
    void updatePainter();
};

#endif // CANVAS_H
