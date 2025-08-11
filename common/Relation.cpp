#include "Relation.h"

#include <QRect>
#include <QPainterPath>
#include <QtMath>

/*                */
/* Class Relation */
/*                */

__Relation::__Relation( RelationType_t nRelationType )
: m_nFirstPos{}, m_nLastPos{}
{
    assert( nRelationType != RelationTypeNone );

    m_nRelationType = nRelationType;
    m_nFrom         = -1;
    m_nTo           = -1;
    m_bHover        = false;
}

void
__Relation::calculate( Figure_t &pFrom, Figure_t &pTo )
{
    assert( m_nRelationType != RelationTypeNone );

    pFrom.center( &m_nFirstPos );
    pTo  .center( &m_nLastPos  );
}

float
__Relation::distancePointToLine( const QPoint &p1, const QPoint &p2, const QPoint &p )
{
    float dx = p2.x() - p1.x();
    float dy = p2.y() - p1.y();

    if( qFuzzyIsNull( dx ) && qFuzzyIsNull( dy ) )
    {
        return QLineF(p1, p).length();
    }

    float numerator   = qAbs(dy * p.x() - dx * p.y() + p2.x() * p1.y() - p2.y() * p1.x());
    float denominator = qSqrt(dx * dx + dy * dy);

    return numerator / denominator;
}

bool
__Relation::isPointNearLine(const QPoint &p1, const QPoint &p2, const QPoint &p, qreal tolerance )
{
    if( distancePointToLine(p1, p2, p) < tolerance )
    {
        qreal minX = qMin(p1.x(), p2.x());
        qreal maxX = qMax(p1.x(), p2.x());
        qreal minY = qMin(p1.y(), p2.y());
        qreal maxY = qMax(p1.y(), p2.y());

        return (p.x() >= minX && p.x() <= maxX &&
                p.y() >= minY && p.y() <= maxY);
    }
    return false;
}

bool
__Relation::contain( const QPoint &point )
{
    assert( m_nRelationType != RelationTypeNone );

    return isPointNearLine( m_nFirstPos, m_nLastPos, point );
}

/*                     */
/* Class Relation List */
/*                     */

bool
RelationList_t::validate( long nFrom, long nTo )
{
    auto found = std::find_if( __EXECUTION_POLICY_BUILDER__, rbegin(), rend(), [ nFrom, nTo ]( auto &pItem ) { return pItem->m_nFrom == nFrom && pItem->m_nTo == nTo; } );
    return found == rend();
    // for( long n = size() - 1; n >= 0; n -- )
    // {
    //     auto &pp = at( n );
    //     assert( pp->m_nRelationType != RelationTypeNone );

    //     if( pp->m_nFrom == nFrom && pp->m_nTo == nTo )
    //     {
    //         return false;
    //     }
    // }
    // return true;
}

void
RelationList_t::calculate( FigureList_t *pFigureList )
{
    std::for_each( __EXECUTION_POLICY_BUILDER__, begin(), end(), [ pFigureList ]( auto &pItem ) { pItem->calculate( *pFigureList->at( pItem->m_nFrom ), *pFigureList->at( pItem->m_nTo ) ); } );
    // for( long n = size() - 1; n >= 0; n -- )
    // {
    //     auto &pp = at( n );
    //     pp->calculate( *pFigureList->at( pp->m_nFrom ), *pFigureList->at( pp->m_nTo ) );
    // }
}

void
RelationList_t::hover_clear() { for( auto &p : *this ) { p->m_bHover = false; } }

long
RelationList_t::hover_index( const QPoint &pos )
{
    auto found = std::find_if( __EXECUTION_POLICY_BUILDER__, rbegin(), rend(), [ pos ]( auto &pItem ) { return pItem->contain( pos ); } );
    return ( found != rend() ) ? std::distance( begin(), -- found.base() ) : -1;
    // for( long n = size() - 1; n >= 0; n -- )
    // {
    //     assert( at( n )->m_nRelationType != RelationTypeNone );

    //     if( at( n )->contain( pos ) )
    //     {
    //         return n;
    //     }
    // }
    // return -1;
}
