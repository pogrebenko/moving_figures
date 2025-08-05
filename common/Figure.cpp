#include "Figure.h"

#include <QRect>
#include <QPainterPath>
#include <QtMath>

/* class Figure */
__Figure::__Figure( const FigureType_t nFigureType )
: m_nFirstPos{}, m_nLastPos{}, m_nAnglePos{}, m_nResizePos{}
{
    assert( nFigureType != FigureTypeNone );

    m_nFigureType  = nFigureType;
    m_bHover       = false;
    m_bHoverFirst  = false;
    m_bHoverLast   = false;
    m_bHoverAngle  = false;
    m_bHoverResize = false;
    m_nAngle       = 0.;
    m_nAnglesCount = -1;
}

bool
__Figure::contain( const QPoint &mousePos )
{
    QRectF  rcf;
            rcf.setTopLeft    ( this->m_nFirstPos );
            rcf.setBottomRight( this->m_nLastPos  );

    QPointF center( rcf.center() );

    int n = m_nAnglesCount;

    qreal x0 = 0; qreal y0 = 0;

    double width  = abs( rcf.width () );
    double height = abs( rcf.height() );
    double radius = std::sqrt( width * width + height * height ) / 2;

    QPainterPath path;
    for( int i = 0; i < n; i ++ )
    {
        qreal fAngle = ( 2 * M_PI * i ) / n  - M_PI/2 - m_nAngle;
        qreal x = center.x() + cos( fAngle ) * radius;
        qreal y = center.y() + sin( fAngle ) * radius;

        if( i == 0 )
        {
            x0 = x; y0 = y;
            path.moveTo(x, y);
        }
        else
        {
            path.lineTo(x,y);
        }
    }
    path.lineTo( x0, y0 );

    return path.contains( mousePos );
}

void
__Figure::calc_angle_point( const QPoint &pt )
{
    QRectF rcf;
           rcf.setTopLeft    ( this->m_nFirstPos );
           rcf.setBottomRight( this->m_nLastPos  );

    double width  = abs( rcf.width () );
    double height = abs( rcf.height() );
    double radius = std::sqrt( width * width + height * height ) / 2 + DELTA_ANGLE;

    QLineF line( rcf.center(), pt );
           line.setLength( radius );

    this->m_nAnglePos.setX( line.p2().x() );
    this->m_nAnglePos.setY( line.p2().y() );

    QLineF line_size( rcf.center(), this->m_nFirstPos );
    QLineF line_angle( rcf.center(), pt );
    this->m_nAngle = line_size.angleTo( line_angle );

    this->m_nAngle = qDegreesToRadians( this->m_nAngle );
}

bool
__Figure::near_points( const QPoint &pt1, const QPoint &pt2 )
{
    QRectF rcf;
    rcf.setTopLeft    ( pt1 );
    rcf.setBottomRight( pt2 );

    double width  = abs( rcf.width () );
    double height = abs( rcf.height() );

    double radius = std::sqrt( width * width + height * height );

    return radius <= MAX_NEAR_SIZE;
}

void
__Figure::move( const QPoint &from, const QPoint &to )
{
    assert( m_nFigureType != FigureTypeNone );

    QPoint diff = to - from;

    m_nFirstPos += diff;
    m_nLastPos  += diff;
    m_nAnglePos += diff;
    m_nResizePos+= diff;
}

const QPoint&
__Figure::center( QPoint *point )
{
    assert( m_nFigureType != FigureTypeNone );

    QRect rc;
    rc.setTopLeft    ( m_nFirstPos );
    rc.setBottomRight( m_nLastPos  );

    *point = rc.center();
    return *point;
}

bool
__Figure::valid()
{
    QRect rc;
    rc.setTopLeft    ( m_nFirstPos );
    rc.setBottomRight( m_nLastPos  );
    if( m_nFigureType == FigureTypeTriangle )
    {
        return
            (abs(rc.width()) > MIN_FIGURE_SIZE && abs(rc.height()) > MIN_FIGURE_SIZE) ||
            (abs(rc.width()) < MAX_FIGURE_SIZE && abs(rc.height()) < MAX_FIGURE_SIZE);
    }
    else
    {
        return
            abs(rc.width()) > MIN_FIGURE_SIZE && abs(rc.height()) > MIN_FIGURE_SIZE &&
            abs(rc.width()) < MAX_FIGURE_SIZE && abs(rc.height()) < MAX_FIGURE_SIZE;
    }
}

/* class Figure List */
void
FigureList_t::hover_clear()
{
    for( long n = size() - 1; n >= 0; n -- )
    {
        auto &pItem = at( n );
        assert( pItem->m_nFigureType != FigureTypeNone );
        pItem->m_bHover      =
        pItem->m_bHoverFirst =
        pItem->m_bHoverLast  =
        pItem->m_bHoverAngle =
        pItem->m_bHoverResize= false;
    }
}

long
FigureList_t::hover_index( const QPoint &pos )
{
    for( long n = size() - 1; n >= 0; n -- )
    {
        assert( at( n )->m_nFigureType != FigureTypeNone );
        if( at( n )->contain( pos ) )
        {
            return n;
        }
    }
    return -1;
}

long
FigureList_t::hover_resize_index( const QPoint &pos )
{
    for( long n = size() - 1; n >= 0; n -- )
    {
        assert( at( n )->m_nFigureType != FigureTypeNone );
        if( at( n )->near_points( at( n )->m_nResizePos, pos ) )
        {
            return n;
        }
    }
    return -1;
}

long
FigureList_t::hover_angle_index( const QPoint &pos )
{
    for( long n = size() - 1; n >= 0; n -- )
    {
        assert( at( n )->m_nFigureType != FigureTypeNone );
        if( at( n )->near_points( at( n )->m_nAnglePos, pos ) )
        {
            return n;
        }
    }
    return -1;
}

long
FigureList_t::hover_first_index( const QPoint &pos )
{
    for( long n = size() - 1; n >= 0; n -- )
    {
        assert( at( n )->m_nFigureType != FigureTypeNone );
        if( at( n )->near_points( at( n )->m_nFirstPos, pos ) )
        {
            return n;
        }
    }
    return -1;
}

long
FigureList_t::hover_last_index( const QPoint &pos )
{
    for( long n = size() - 1; n >= 0; n -- )
    {
        assert( at( n )->m_nFigureType != FigureTypeNone );
        if( at( n )->near_points( at( n )->m_nLastPos, pos ) )
        {
            return n;
        }
    }
    return -1;
}


