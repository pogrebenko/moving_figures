#include "Rectangle.h"

#include <QtMath>
#include <QRectF>
#include <QPainterPath>

CRectangle::CRectangle()
: Figure_t( FigureTypeRectangle )
{
    m_nAnglesCount = 4;
}

bool CRectangle::contain( const QPoint &mousePos )
{
    QRectF       rcf;
                 rcf.setTopLeft    ( m_nFirstPos );
                 rcf.setBottomRight( m_nLastPos  );

    QPointF      center( rcf.center() );

    QPainterPath rectanglePath;
                 rectanglePath.addRect( rcf );

    //return rectanglePath.contains( mousePos );

    QTransform   transform;
                 transform.translate( center.x(), center.y() );
                 transform.rotate( -qRadiansToDegrees( m_nAngle ) );
                 transform.translate( -center.x(), -center.y() );

     return transform.map( rectanglePath ).contains( mousePos );
}
