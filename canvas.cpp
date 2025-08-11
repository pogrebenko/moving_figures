#include "canvas.h"

#include <QTimer>
#include <QTime>

#include <cmath>

#include <QObject>
#include <QAction>
#include <QtGui/QCursor>

#include "common/Options.h"

#include "mainwindow.h"

#ifdef __SMART__
    extern CLogStream __Logger;
    extern std::unique_ptr<CAppOption> __AppOption__;
    #define __AppOption     (*__AppOption__.get())
#else
    extern CLogStream __Logger;
    extern CAppOption __AppOption;
#endif

CCanvas::CCanvas( QWidget *pParent, CAppOption * const pAppOption, Logger_t * const pLogger )
: QWidget{ pParent }
, CPainter( pAppOption )
, CBuilder( pAppOption, pLogger )
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );

    setMouseTracking( true );
    setFocusPolicy( Qt::StrongFocus );

    m_bMouseLButtonMoved = false;
    m_bBreakAddFigure    = false;






//     // initialize update/draw timers
//     float updatesPerSecond = 120;
//     float drawsPerSecond = 30;
//     // simulation system
//     isFirstFrameFinished = false;
//     minDeltaTimeModifier = 0.125;
//     maxDeltaTimeModifier = 1.0;
//     deltaTimeModifier = maxDeltaTimeModifier;

//     drawTimer = new QTimer(this);
//     connect(drawTimer, &QTimer::timeout, this, &CCanvas::updatePainter );
// //    drawTimer->start(1000.0/drawsPerSecond);

//     updateTimer = new QTimer(this);
//     connect(updateTimer, &QTimer::timeout, this, &CCanvas::updateBuilder );
// //    updateTimer->start(1000.0/updatesPerSecond);

//     deltaTimer = new QTime();
// //    deltaTimer->start();
}

void
CCanvas::updatePainter()
{
    getLogger()->info( APP_LOG_LEVEL, "info: updatePainter" );

}

void
CCanvas::updateBuilder()
{
    getLogger()->info( APP_LOG_LEVEL, "info: updateBuilder" );
//     minTimeStepValue = floor(1000.0/320.0);
//     maxTimeStepValue = floor(1000.0/24.0);

//     float dt = (float) deltaTimer->elapsed() / 1000;

//     // dt might be large on the first frame
//     if (!isFirstFrameFinished) {
//         dt = minTimeStepValue / 1000.0;
//         isFirstFrameFinished = true;
//     }
//     // make sure dt does not get too big
//     if (dt > maxTimeStepValue / 1000.0) {
//         dt = maxTimeStepValue / 1000.0;
//     }
//     deltaTimer->restart();

//     dt *= deltaTimeModifier;  // speed of simulation
//     //Builder.update(dt);
}

void
CCanvas::paintEvent( QPaintEvent *pEvent )
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );
    try
    {
        QPainter painter{ this };
        painter_draw( painter );
        //__Logger.info( APP_LOG_LEVEL, "info: paintEvent" );
        QWidget::paintEvent( pEvent );
    }
    catch( ... )
    {
        getLogger()->error( APP_LOG_LEVEL, "error: unknown exception" );
    }
}

void
CCanvas::resizeEvent( QResizeEvent *pEvent )
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );
    try
    {
        painter_resize( this->rect() );
        //getLogger()->info( APP_LOG_LEVEL, "info: resizeEvent" );
        QWidget::resizeEvent( pEvent );
    }
    catch( ... )
    {
        getLogger()->error( APP_LOG_LEVEL, "error: unknown exception" );
    }
}

void
CCanvas::keyPressEvent( QKeyEvent *pEvent )
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );
    try
    {
        auto key = pEvent->key();
        if( key == Qt::Key_Escape )
        {
            getLogger()->info( APP_LOG_LEVEL, "info: Key down ESC" );
            m_bBreakAddFigure = true;
        }
        QWidget::keyPressEvent(pEvent);
    }
    catch( ... )
    {
        getLogger()->error( APP_LOG_LEVEL, "error: unknown exception" );
    }
}

void
CCanvas::keyReleaseEvent( QKeyEvent *pEvent )
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );
    try
    {
        auto key = pEvent->key();
        if( key == Qt::Key_Escape )
        {
            getLogger()->info( APP_LOG_LEVEL, "info: Key up ESC" );
            m_bBreakAddFigure = false;
        }
        QWidget::keyReleaseEvent(pEvent);
    }
    catch( ... )
    {
        getLogger()->error( APP_LOG_LEVEL, "error: unknown exception" );
    }
}

void
CCanvas::mousePressEvent( QMouseEvent *pEvent )
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );
    try
    {
        QPoint pt( pEvent->pos() );

        m_bMouseLButtonMoved = false;
        if( pEvent->button() == Qt::LeftButton )
        {
            switch( get_action_type() )
            {
                case ActionTypeAddFigure   : figure_begin_add( pt ); break;
                case ActionTypeAddRelation : relation_begin_add( figure_hover( pt, false ) ); break;
                case ActionTypeMove        : if( figure_hover( pt, false ) >= 0 ) figure_set_first_pos( pt ); break;
                case ActionTypeDelete      : if( relation_delete( relation_hover( pt, false ) ) || figure_delete( figure_hover( pt, false ) ) ) { draw_figure_relation(); update(); } break;
                case ActionTypeNone        : break;
            }
        }
        else
        if( pEvent->button() == Qt::RightButton )
        {
            relation_begin_add( figure_hover( pt, false ) );
        }
        QWidget::mousePressEvent(pEvent);
    }
    catch( ... )
    {
        getLogger()->error( APP_LOG_LEVEL, "error: unknown exception" );
    }
}

void
CCanvas::mouseDoubleClickEvent( QMouseEvent *pEvent )
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );
    try
    {
        QPoint pt( pEvent->pos() );
        if( pEvent->button() == Qt::LeftButton )
        {
            if( relation_rotate_type( relation_hover( pt, false ) ) || figure_rotate_type( figure_hover( pt, false ) ) )
            {
                draw_figure_relation();
                update();
            }
        }
        else
        if( pEvent->button() == Qt::RightButton )
        {
            if( relation_delete( relation_hover( pt, false ) ) || figure_delete( figure_hover( pt, false ) ) )
            {
                draw_figure_relation();
                update();
            }
        }
        QWidget::mouseDoubleClickEvent( pEvent );
    }
    catch( ... )
    {
        getLogger()->error( APP_LOG_LEVEL, "error: unknown exception" );
    }
}

void
CCanvas::mouseReleaseEvent( QMouseEvent *pEvent )
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );
    try
    {
        QPoint pt( pEvent->pos() );
        if( pEvent->button() == Qt::LeftButton )
        {
            if( !m_bBreakAddFigure && m_bMouseLButtonMoved )
            {
                if(
                       ( check_action_type( ActionTypeAddFigure   ) && figure_release_add() )
                    || ( check_action_type( ActionTypeAddRelation ) && relation_release_add( pt ) )
                )
                {
                    draw_figure_relation();
                    update();
                }
            }
        }
        else
        if( pEvent->button() == Qt::RightButton )
        {
            if( relation_release_add( pt ) )
            {
                draw_figure_relation();
                update();
            }
        }
        QWidget::mouseReleaseEvent( pEvent );
        m_bMouseLButtonMoved = false;
    }
    catch( ... )
    {
        getLogger()->error( APP_LOG_LEVEL, "error: unknown exception" );
    }
}

void
CCanvas::mouseMoveEvent( QMouseEvent *pEvent )
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );

    try
    {
        QPoint pt( pEvent->pos() );

        hover_reset();

        auto buttons = pEvent->buttons();
        if( buttons == Qt::LeftButton )
        {
            m_bMouseLButtonMoved = true;
            switch( get_action_type() )
            {
                case ActionTypeAddFigure   : { figure_set_last_pos( pt ); draw_figure_new(); update(); } break;
                case ActionTypeAddRelation : if( relation_set_last_pos( pt ) ) { draw_relation_new(); update(); } break;
                case ActionTypeMove        : if( rotate_hover( pt, true, true ) >= 0 || resize_hover( pt, true, true ) >= 0 || figure_move( pt ) ) { draw_figure_relation(); update(); } break;
                case ActionTypeNone        :
                case ActionTypeDelete      : break;
            }
        }
        else
        if( buttons == Qt::RightButton )
        {
            if( relation_set_last_pos( pt ) )
            {
                draw_relation_new();
                update();
            }
        }
        else
        {
            this->setCursor( Qt::ArrowCursor );

            if( rotate_hover( pt, true, false ) >= 0 && check_action_type( ActionTypeMove ) )
                this->setCursor( Qt::PointingHandCursor );
            else
            if( resize_hover( pt, true, false ) >= 0 && check_action_type( ActionTypeMove ) )
                this->setCursor( Qt::SizeFDiagCursor );
            else
            if( figure_hover( pt, true ) >= 0 && check_action_type( ActionTypeMove ) )
                this->setCursor( Qt::PointingHandCursor );


            relation_hover( pt, true );

            draw_figure_relation();
            update();
        }

        QWidget::mouseMoveEvent( pEvent );
    }
    catch( ... )
    {
        getLogger()->error( APP_LOG_LEVEL, "error: unknown exception" );
    }
}
