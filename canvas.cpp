#include "canvas.h"

#include <QTime>
#include <QTimer>

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

    m_nDir = 1; m_nX = 0L; m_nY = 0L;
    m_pOscilationTimer = new QTimer( this );
    connect(m_pOscilationTimer, &QTimer::timeout, this, &CCanvas::oscilationBuilder );

    m_pNameEdit = new QTextEdit( this );
    m_pNameEdit->hide();
    connect( m_pNameEdit, &QTextEdit::textChanged, this, &CCanvas::onTextChanged );
}

void CCanvas::onTextChanged()
{
    std::string str = m_pNameEdit->toPlainText().toStdString();
    char ch = str.back();
    if( ch == '\n' || ch == 27 )
    {
        auto cl = __AppOption.getFigureList();
        auto found = std::find_if( __EXECUTION_POLICY_BUILDER__, cl->begin(), cl->end(), []( auto pItem ) { return pItem->m_bEditText; } );
        if( found != cl->end() )
        {
            found->get()->m_bEditText = false;
        }

        m_pNameEdit->hide();

        draw_figure_relation();
        update();
    }
    else
    {
        auto cl = __AppOption.getFigureList();
        auto found = std::find_if( __EXECUTION_POLICY_BUILDER__, cl->begin(), cl->end(), []( auto pItem ) { return pItem->m_bEditText; } );
        if( found != cl->end() )
        {
            found->get()->m_Name = str;
        }
    }
}

CCanvas::~CCanvas()
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );

    delete m_pOscilationTimer;
    delete m_pNameEdit;
}

void
CCanvas::oscilationBuilder()
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );
    //getLogger()->info( APP_LOG_LEVEL, "info: tick updateBuilder" );

    QPoint globalMousePos{ QCursor::pos() };
    QPoint pt = this->mapFromGlobal(globalMousePos);
    int x = m_nDir * ( m_nX % 4 ), y = m_nDir * ( m_nY % 4 );
    if(    figure_oscillation  ( pt, x, y ) >= 0
        || relation_oscillation( pt, x, y ) >= 0 )
    {
        if( x == 0 )
        {
            m_nDir *= -1;
        }
        m_nX ++; m_nY ++;

        draw_figure_relation();
        update();
    }
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
            long n = -1;
            if( (n = center_hover( pt, false )) >= 0 )
            {
                auto fm = m_pNameEdit->fontMetrics();
                int  w = fm.horizontalAdvance( "here we enter the name" );
                int  h = fm.height() * 1.7;
                int  x = pt.x();
                int  y = pt.y();
                m_pNameEdit->setGeometry( x - w/2, y - h/2, w, h );
                auto fl = __AppOption.getFigureList();
                     fl->at( n )->m_bEditText = true;
                m_pNameEdit->setText( fl->at( n )->m_Name.c_str() );
                m_pNameEdit->show();
                m_pNameEdit->setFocus();
                m_pNameEdit->moveCursor( QTextCursor::End );

                draw_figure_relation();
                update();
            }
            else
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
            long fh = -1, rh = -1;
            this->setCursor( Qt::ArrowCursor );

            if( center_hover( pt, true ) >= 0 )
                this->setCursor( Qt::IBeamCursor );
            else
            if( rotate_hover( pt, true, false ) >= 0 && check_action_type( ActionTypeMove ) )
                this->setCursor( Qt::PointingHandCursor );
            else
            if( resize_hover( pt, true, false ) >= 0 && check_action_type( ActionTypeMove ) )
                this->setCursor( Qt::SizeFDiagCursor );
            else
            if( (fh = figure_hover( pt, true )) >= 0 && check_action_type( ActionTypeMove ) )
                this->setCursor( Qt::PointingHandCursor );

            rh = relation_hover( pt, true );

            if( check_action_type( ActionTypeDelete ) )
            {
                if( fh >= 0 || rh >= 0 )
                {
                    if( !m_pOscilationTimer->isActive() )
                    {
                        m_nDir = 1; m_nX = 0L; m_nY = 0L;
                        m_pOscilationTimer->start(1000/20);
                    }
                }
                else
                {
                    if( m_pOscilationTimer->isActive() )
                    {
                        m_pOscilationTimer->stop();
                        relation_rebuild();
                    }
                }
            }

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
