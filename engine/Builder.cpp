#include "Builder.h"

#include <QtMath>
#include <QRectF>
#include <QLineF>

#include "../common/Options.h"
#include "../data/FigureFactory.h"

CBuilder::CBuilder( CAppOption * const pAppOption, Logger_t * const pLogger )
: CLoggerHolder( pLogger )
, m_pAppOption( pAppOption )
{
}

ActionType_t
CBuilder::get_action_type()
{
    return m_pAppOption->getActionType();
}

bool
CBuilder::check_action_type( ActionType_t ActionType )
{
    return ActionType == m_pAppOption->getActionType();
}

void
CBuilder::hover_reset()
{
    m_pAppOption->getFigureList  ()->hover_clear();
    m_pAppOption->getRelationList()->hover_clear();
}


long
CBuilder::figure_hover( const QPoint &pos, bool set_up_hover )
{
    long n = m_pAppOption->getFigureList()->hover_index( pos );
    if( n >= 0 && set_up_hover )
    {
        auto &pItem = m_pAppOption->getFigureList()->at( n );
        //getLogger()->info( APP_LOG_LEVEL, "info: Figure hover[%d]", n );

        pItem->m_bHover       = true;

        pItem->m_bHover       =
        pItem->m_bHoverFirst  =
        pItem->m_bHoverLast   =
        pItem->m_bHoverAngle  =
        pItem->m_bHoverResize = true;
    }
    return n;
}

bool
CBuilder::figure_move( const QPoint &pt )
{
    long n = m_pAppOption->getFigureList()->hover_index( pt );
    if( n >= 0 )
    {
        getLogger()->info( APP_LOG_LEVEL, "info: Move figure [%d]", n );
        m_pAppOption->getFigureList()->at( n )->m_bHover = true;
        m_pAppOption->setLastPos( pt );
        m_pAppOption->getFigureList()->at( n )->move(
              m_pAppOption->getFirstPos()
            , m_pAppOption->getLastPos ()
            );
        m_pAppOption->setFirstPos( pt );

        m_pAppOption->getRelationList()->calculate( m_pAppOption->getFigureList() );
        return true;
    }
    return false;
}

void
CBuilder::figure_set_first_pos( const QPoint &pt )
{
    getLogger()->info( APP_LOG_LEVEL, "info: Press left button for move figure %d:%d", pt.x(), pt.y() );
    m_pAppOption->setFirstPos( pt );
}

void
CBuilder::figure_set_last_pos( const QPoint &pos )
{
    m_pAppOption->setLastPos( pos );
    //getLogger()->info( APP_LOG_LEVEL, "info: Show new figure" );
}

void
CBuilder::figure_begin_add( const QPoint &pos )
{
    getLogger()->info( APP_LOG_LEVEL, "info: Press left button for add figure %d:%d", pos.x(), pos.y() );
    m_pAppOption->setActionType( ActionTypeAddFigure );
    m_pAppOption->setFirstPos( pos );
}

bool
CBuilder::figure_release_add()
{
    getLogger()->info( APP_LOG_LEVEL, "info: Release left button for add figure" );

    auto pFigure = CFigureFactory::buildFigure(
          m_pAppOption->getFigureType()
        , m_pAppOption->getFirstPos  ()
        , m_pAppOption->getLastPos   ()
        );
    if( pFigure->valid() )
    {
        m_pAppOption->getFigureList()->Add( pFigure );
    }
    else
    {
        delete pFigure;
    }
    return true;
}

bool
CBuilder::figure_rotate_type( long n )
{
    if( n >= 0 )
    {
        if( n < (long)m_pAppOption->getFigureList()->size() )
        {
            getLogger()->info( APP_LOG_LEVEL, "info: Change figure type [%d]", n );
            auto &pFigureItem = m_pAppOption->getFigureList()->at( n );
            pFigureItem->m_nFigureType = CAppOption::rotateFigureType( pFigureItem->m_nFigureType );

            switch( pFigureItem->m_nFigureType )
            {
                case FigureTypeNone      : break;
                case FigureTypeTriangle  : pFigureItem->m_nAnglesCount  ++; break;
                case FigureTypeEllipse   : pFigureItem->m_nAnglesCount = 0; break;
                case FigureTypeRectangle : pFigureItem->m_nAnglesCount = 4; break;
            }

            m_pAppOption->getRelationList()->calculate( m_pAppOption->getFigureList() );
            return true;
        }
    }
    return false;
}

bool
CBuilder::figure_delete( long n )
{
    if( n >= 0 )
    {
        auto f = m_pAppOption->getFigureList();
        if( n < (long)f->size() )
        {
            getLogger()->info( APP_LOG_LEVEL, "info: Delete figure with relations [%d]", n );

            auto r = m_pAppOption->getRelationList();

            for( long i = r->size() - 1; i >= 0; i -- )
            {
                auto &p = r->at( i );
                if( p->m_nFrom == (long)n || p->m_nTo == (long)n )
                {
                    r->Delete( i );
                }
            }

            for( unsigned long j = n + 1; j < f->size(); j ++ )
                for( long jj = r->size() - 1; jj >= 0; jj -- )
                {
                    auto &p = r->at( jj );
                    if( p->m_nFrom == (long)j ) p->m_nFrom --;
                    if( p->m_nTo   == (long)j ) p->m_nTo   --;
                }

            f->Delete( n );
            return true;
        }
    }
    return false;
}

long
CBuilder::resize_hover( const QPoint &pos, bool set_up_hover, bool set_up_resize )
{
    long n = m_pAppOption->getFigureList()->hover_resize_index( pos );
    if( n >= 0 && set_up_hover )
    {
        //getLogger()->info( APP_LOG_LEVEL, "info: last point hover[%d]", n );
        auto &pItem = m_pAppOption->getFigureList()->at( n );
        pItem->m_bHoverResize = true;
        if( set_up_resize )
        {
            pItem->m_nLastPos   =
            pItem->m_nResizePos = pos;

            pItem->m_nLastPos.rx()-= DELTA_RESIZE;
            pItem->m_nLastPos.ry()-= DELTA_RESIZE;

            pItem->calc_angle_point( pItem->m_nAnglePos );

            m_pAppOption->getRelationList()->calculate( m_pAppOption->getFigureList() );
        }
    }
    return n;
}

long
CBuilder::rotate_hover( const QPoint &pos, bool set_up_hover, bool set_up_rotate )
{
    long n = m_pAppOption->getFigureList()->hover_angle_index( pos );
    if( n >= 0 && set_up_hover )
    {
        auto &pItem = m_pAppOption->getFigureList()->at( n );
        pItem->m_bHoverAngle = true;
        if( set_up_rotate )
        {
            pItem->calc_angle_point( pos );
            getLogger()->info( APP_LOG_LEVEL, "info: angle[%d]", qRadiansToDegrees( pItem->m_nAngle ) );
        }
    }
    return n;
}

long
CBuilder::relation_hover( const QPoint &pos, bool set_up_hover )
{
    long n = m_pAppOption->getRelationList()->hover_index( pos );
    if( n >= 0 && set_up_hover )
    {
        //getLogger()->info( APP_LOG_LEVEL, "info: Relation hover[%d]", n );
        m_pAppOption->getRelationList()->at( n )->m_bHover = true;
    }
    return n;
}

void
CBuilder::relation_begin_add( long n )
{
    if( n >= 0 )
    {
        getLogger()->info( APP_LOG_LEVEL, "info: Press left button for relation from figure [%d]", n );
        QPoint from;
        m_pAppOption->setFirstPos( m_pAppOption->getFigureList()->at( n )->center( &from ) );
        m_pAppOption->setIndexFrom( n );
    }
}

bool
CBuilder::relation_release_add( const QPoint &pt )
{
    long nIndexFrom = m_pAppOption->getIndexFrom();
    if( nIndexFrom >= 0 )
    {
        FigureList_t *figures = m_pAppOption->getFigureList();
        long nIndexTo = figures->hover_index( pt );
        if( nIndexTo >= 0 && nIndexFrom != nIndexTo )
        {
            if( m_pAppOption->getRelationList()->validate( nIndexFrom, nIndexTo ) )
            {
                getLogger()->info( APP_LOG_LEVEL, "info: Press right button for relation to figure [%d]->[%d] %d:%d", nIndexFrom, nIndexTo, pt.x(), pt.y() );

                QPoint from, to;
                m_pAppOption->getRelationList()->Add(
                    CFigureFactory::buildRelation(
                        m_pAppOption->getRelationType()
                        , nIndexFrom
                        , nIndexTo
                        , figures->at( nIndexFrom )->center( &from )
                        , figures->at( nIndexTo   )->center( &to   )
                        )
                    );
                return true;
            }
        }
    }
    return false;
}

bool
CBuilder::relation_delete( long n )
{
    if( n >= 0 )
    {
        if( n < (long)m_pAppOption->getRelationList()->size() )
        {
            auto &pRelation = m_pAppOption->getRelationList()->at( n );
            getLogger()->info( APP_LOG_LEVEL, "info: Delete relation [%d]->[%d]", pRelation->m_nFrom, pRelation->m_nTo );

            m_pAppOption->getRelationList()->Delete( n );
            return true;
        }
    }
    return false;
}

bool
CBuilder::relation_rotate_type( long n )
{
    if( n >= 0 )
    {
        if( n < (long)m_pAppOption->getRelationList()->size() )
        {
            auto &pRelation = m_pAppOption->getRelationList()->at( n );
            getLogger()->info( APP_LOG_LEVEL, "info: Change relation type [%d]->[%d]", pRelation->m_nFrom, pRelation->m_nTo );

            pRelation->m_nRelationType = CAppOption::rotateRelationType( pRelation->m_nRelationType );
            return true;
        }
    }
    return false;
}

bool
CBuilder::relation_set_last_pos( const QPoint &pt )
{
    long nIndexFrom = m_pAppOption->getIndexFrom();
    if( nIndexFrom >= 0 )
    {
        m_pAppOption->setLastPos( pt );
        //getLogger()->info( APP_LOG_LEVEL, "info: Show new relation" );
        return true;
    }
    return false;
}

long
CBuilder::relation_oscillation( QPoint &pt, int dx, int dy )
{
    RelationList_t *rl = m_pAppOption->getRelationList();
    long n = rl->hover_index( pt );
    if( n >= 0 )
    {
        auto &r = rl->at( n );
        r->oscillation( dx, dy );

        // FigureList_t *fl = m_pAppOption->getFigureList();
        // auto c1 = fl->at(r->m_nFrom);
        // auto c2 = fl->at(r->m_nTo  );

        // c1->oscillation( dx, dy );
        // c2->oscillation( dx, dy );
    }
    return n;
}

long
CBuilder::figure_oscillation( QPoint &pt, int dx, int dy )
{
    FigureList_t *fl = m_pAppOption->getFigureList();
    long n = fl->hover_index( pt );
    if( n >= 0 )
    {
        fl->at( n )->oscillation( dx, dy );

        auto rl = m_pAppOption->getRelationList();

        for( long i = rl->size() - 1; i >= 0; i -- )
        {
            auto &p = rl->at( i );
            if( p->m_nFrom == (long)n || p->m_nTo == (long)n )
            {
                p->oscillation( dx, dy );
            }
        }
    }
    return n;
}

void
CBuilder::relation_rebuild()
{
    auto fl = m_pAppOption->getFigureList();
    auto rl = m_pAppOption->getRelationList();

    for( long i = fl->size() - 1; i >= 0; i -- )
    {
        auto &f = fl->at( i );

        QRect rc;
        rc.setTopLeft    ( f->m_nFirstPos );
        rc.setBottomRight( f->m_nLastPos  );

        for( long ii = rl->size() - 1; ii >= 0; ii -- )
        {
            auto &r = rl->at( ii );
            if( r->m_nFrom == (long)i )
            {
                r->m_nFirstPos = rc.center();
            }
            else
            if( r->m_nTo == (long)i )
            {
                r->m_nLastPos = rc.center();
            }
        }
    }
}

long
CBuilder::center_hover( const QPoint &pos, bool set_up_hover )
{
    long n = m_pAppOption->getFigureList()->hover_center_index( pos );
    if( n >= 0 && set_up_hover )
    {
        auto &pItem = m_pAppOption->getFigureList()->at( n );
        pItem->m_bHoverCenter = true;
    }
    return n;
}
