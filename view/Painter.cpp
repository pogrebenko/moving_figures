#include "Painter.h"

#ifdef __THREADS__
    #include <thread>
    #include <mutex>
#endif

#include <cmath>

#include <QtMath>

#include "../common/Options.h"
#include "../data/FigureFactory.h"

extern CLogStream __Logger;

CPainter::CPainter( CAppOption *pAppOption )
: m_pAppOption( pAppOption )
{
}

void
CPainter::painter_draw( QPainter &painter )
{
    painter.drawPixmap( 0, 0, m_pixmap );
}

void
CPainter::painter_resize( const QRect &rc )
{
    auto newRect = m_pixmap.rect().united( rc );
    if( newRect == m_pixmap.rect() )
    {
        return;
    }

    QPixmap newPixmap{ newRect.size() };

    QPainter painter{ &newPixmap };
    painter.fillRect( newPixmap.rect(), m_pAppOption->getBkgColor() );
    painter.drawPixmap( 0, 0, m_pixmap );

    m_pixmap = newPixmap;
}

void
CPainter::draw_figure_relation()
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );

    QPainter painter{ &m_pixmap };
             painter.setRenderHint( QPainter::Antialiasing, true );
             painter.fillRect( m_pixmap.rect(), m_pAppOption->getBkgColor() );
    draw_figure  ( painter, m_pAppOption->getFigureList  () );
    draw_relation( painter, m_pAppOption->getRelationList() );
}

void
CPainter::draw_figure_new()
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );

    QPainter painter{ &m_pixmap };
             painter.setRenderHint( QPainter::Antialiasing, true );
             painter.fillRect( m_pixmap.rect(), m_pAppOption->getBkgColor() );

    draw_figure( painter, m_pAppOption->getFigureList() );

    std::unique_ptr<Figure_t> Item( CFigureFactory::buildFigure(
          m_pAppOption->getFigureType()
        , m_pAppOption->getFirstPos  ()
        , m_pAppOption->getLastPos   ()
        ) );
    draw_figure_item( painter, *Item.get() );

    draw_relation( painter, m_pAppOption->getRelationList() );
}

void
CPainter::draw_relation_new()
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );

    QPainter painter{ &m_pixmap };
             painter.setRenderHint( QPainter::Antialiasing, true );
             painter.fillRect( m_pixmap.rect(), m_pAppOption->getBkgColor() );

    draw_figure  ( painter, m_pAppOption->getFigureList  () );
    draw_relation( painter, m_pAppOption->getRelationList() );

    long nIndexFrom = m_pAppOption->getIndexFrom();
    if( nIndexFrom >= 0 )
    {
        QPoint from;
        std::unique_ptr<Relation_t> Item( CFigureFactory::buildRelation(
              m_pAppOption->getRelationType()
            , nIndexFrom
            , -1
            , m_pAppOption->getFigureList()->at( nIndexFrom )->center( &from )
            , m_pAppOption->getLastPos()
            ) );
        draw_relation_item( painter, *Item.get() );
    }
}

void
CPainter::draw_relation( QPainter &painter, RelationList_t *RelationList )
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );

    //__Logger.info( APP_LOG_LEVEL, "info: draw_relation %d", RelationList->size() );

#ifdef __THREADS__
    std::mutex Mutex;
    std::vector< std::unique_ptr<std::thread> > Threads;
    for( unsigned long n = 0, size = RelationList->size(); n < size; n ++ )
    {
        Threads.push_back( std::make_unique<std::thread>(
            [this]( QPainter *painter, Relation_t *pRelation, std::mutex *pMutex )
            {
                std::lock_guard<std::mutex> lg( *pMutex );
                this->draw_relation_item( *painter, *pRelation );
            }, &painter, RelationList->at( n ).get(), &Mutex )
        );
    }

    for_each( __EXECUTION_POLICY_PAINTER__, Threads.begin(), Threads.end(), [&]( auto &pItem ){ if( pItem->joinable() ) pItem->join(); } );
#else

    for_each( __EXECUTION_POLICY_PAINTER__, RelationList->begin(), RelationList->end(), [&]( auto &pItem ){ draw_relation_item( painter, *pItem ); } );

    // for( unsigned long n = 0, size = RelationList->size(); n < size; n ++ )
    // {
    //     draw_relation_item( painter, *RelationList->at( n ) );
    // }
#endif
}

void
CPainter::draw_relation_item( QPainter &painter, Relation_t &pRelation )
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );

    float nWidth = pRelation.m_bHover ? m_pAppOption->getWidthHover() : m_pAppOption->getWidth();

    QPen     pen;
    pen.setWidthF( nWidth );
    pen.setColor ( m_pAppOption->getPenColor  () );
    pen.setStyle ( m_pAppOption->getPenStyle  () );
    pen.setCosmetic( true );

    QBrush   brush;
    brush.setColor( m_pAppOption->getBrushColor() );
    brush.setStyle( m_pAppOption->getBrushStyle() );

    painter.setPen( pen );
    painter.setBrush( brush );

    painter.drawLine( pRelation.m_nFirstPos, pRelation.m_nLastPos );


    QPointF startPoint = pRelation.m_nFirstPos
          , endPoint   = pRelation.m_nLastPos;
    int   d         = m_pAppOption->getArrowAngle();
    qreal arrowSize = m_pAppOption->getArrowSize();

    /*             */
    /* Draw arrows */
    /*             */
    if( pRelation.m_nRelationType == RelationTypeLineBidirect || pRelation.m_nRelationType == RelationTypeLineDirectLeft )
    {
        QLineF line1(endPoint, startPoint);
        double angle1 = std::atan2(line1.dy(), -line1.dx()) + M_PI / 2;

        QPointF arrowP1_start = startPoint + QPointF(std::sin(angle1 + M_PI / d) * arrowSize,
                                                     std::cos(angle1 + M_PI / d) * arrowSize);
        QPointF arrowP2_start = startPoint + QPointF(std::sin(angle1 - M_PI / d) * arrowSize,
                                                     std::cos(angle1 - M_PI / d) * arrowSize);

        QPolygonF arrowHead1;
        arrowHead1 << startPoint << arrowP1_start << arrowP2_start;

        painter.drawPolygon(arrowHead1);

        QPainterPath path1; path1.addPolygon(arrowHead1);
        painter.fillPath(path1, painter.pen().color()); // Fill arrowhead
    }

    if( pRelation.m_nRelationType == RelationTypeLineBidirect || pRelation.m_nRelationType == RelationTypeLineDirectRight )
    {
        QLineF line2(startPoint, endPoint);
        double angle2 = std::atan2(line2.dy(), -line2.dx()) + M_PI / 2;

        QPointF arrowP1_end = endPoint + QPointF(std::sin(angle2 + M_PI / d) * arrowSize,
                                                 std::cos(angle2 + M_PI / d) * arrowSize);
        QPointF arrowP2_end = endPoint + QPointF(std::sin(angle2 - M_PI / d) * arrowSize,
                                                 std::cos(angle2 - M_PI / d) * arrowSize);

        QPolygonF arrowHead2;
        arrowHead2 << endPoint << arrowP1_end << arrowP2_end;

        painter.drawPolygon(arrowHead2);

        QPainterPath path2; path2.addPolygon(arrowHead2);
        painter.fillPath(path2, painter.pen().color()); // Fill arrowhead
    }
}

void
CPainter::draw_figure( QPainter &painter, FigureList_t *FigureList )
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );

    //__Logger.info( APP_LOG_LEVEL, "info: draw_figure %d", FigureList->size() );

#ifdef __THREADS__
    std::mutex Mutex;
    std::vector< std::unique_ptr< std::thread > > Threads;

    for( unsigned long n = 0, size = FigureList->size(); n < size; n ++ )
    {
        Threads.push_back( std::make_unique<std::thread>(
            [this]( QPainter *painter, Figure_t *pFigure, std::mutex *pMutex )
            {
                std::lock_guard<std::mutex> lg( *pMutex );
                this->draw_figure_item( *painter, *pFigure );
            }, &painter, FigureList->at( n ).get(), &Mutex )
        );
    }

    for_each( __EXECUTION_POLICY_PAINTER__, Threads.begin(), Threads.end(), [&]( auto &pItem ){ if( pItem->joinable() ) pItem->join(); } );
#else

    for_each( __EXECUTION_POLICY_PAINTER__, FigureList->begin(), FigureList->end(), [&]( auto &pItem ){ draw_figure_item( painter, *pItem ); } );

    // for( unsigned long n = 0, size = FigureList->size(); n < size; n ++ )
    // {
    //     draw_figure_item( painter, *FigureList->at( n ) );
    // }
#endif
}

void
CPainter::draw_figure_item( QPainter &painter, Figure_t &pItem )
{
    FUNC_TRACE( &__Logger, __PRETTY_FUNCTION__ );

    QRectF  rcf;
            rcf.setTopLeft    ( pItem.m_nFirstPos );
            rcf.setBottomRight( pItem.m_nLastPos  );
    QPointF center( rcf.center() );

    // begin figures >>>>>
    // 1. N-angles
    long n = pItem.m_nAnglesCount;
    QPolygonF polygon;
    double width  = abs( rcf.width () );
    double height = abs( rcf.height() );
    double radius = std::sqrt( width * width + height * height ) / 2;
    for( long i = 0; i < n; i ++ )
    {
        qreal fAngle = ( 2 * M_PI * i ) / n  - M_PI/2 - pItem.m_nAngle;
        qreal x = center.x() + cos( fAngle ) * radius;
        qreal y = center.y() + sin( fAngle ) * radius;
        polygon << QPoint( x, y );
    }
    // 2. ellipse
    QPainterPath ellipsePath;
    ellipsePath.addEllipse( rcf );
    // 3. rectangle
    QPainterPath rectPath;
    rectPath.addRect( rcf );
    // >>>>> end figures

    // rotate figures 2.and 3.
    QTransform   transform;
    transform.translate( rcf.center().x(), rcf.center().y() );
    transform.rotate( -qRadiansToDegrees( pItem.m_nAngle ) );
    transform.translate( -rcf.center().x(), -rcf.center().y() );

    float nWidth = pItem.m_bHover ? m_pAppOption->getWidthHover() : m_pAppOption->getWidth();

    QPen     pen;
             pen.setWidthF( nWidth );
             pen.setColor ( m_pAppOption->getPenColor  () );
             pen.setStyle ( m_pAppOption->getPenStyle  () );
             pen.setCosmetic( true );

    QBrush   brush;
             brush.setColor( m_pAppOption->getBrushColor() );
             brush.setStyle( m_pAppOption->getBrushStyle() );

    painter.setPen( pen );
    painter.setBrush( brush );

    // begin paint figures >>>>>>
    switch( pItem.m_nFigureType )
    {
        case FigureTypeTriangle  :
            painter.drawPolygon( polygon );
            break;
        case FigureTypeEllipse   :
            painter.drawPath( transform.map( ellipsePath ) );
            break;
        case FigureTypeRectangle :
            painter.drawPath( transform.map( rectPath ) );
            break;
        case FigureTypeNone :
            break;
    }
    // >>>>>> end paint figures

    float r = m_pAppOption->getCPRadius();
    painter.drawEllipse( rcf.center(), r, r );

    // resize points
    float dx = 1.7;
    if( m_pAppOption->getActionType() == ActionTypeMove )
    {
        nWidth = pItem.m_bHoverFirst ? dx*m_pAppOption->getWidthHover() : m_pAppOption->getWidth();
        pen.setWidthF( nWidth );
        painter.setPen( pen );
        painter.drawEllipse( QPointF(pItem.m_nFirstPos), r, r );

        nWidth = pItem.m_bHoverLast ? dx*m_pAppOption->getWidthHover() : m_pAppOption->getWidth();
        pen.setWidthF( nWidth );
        painter.setPen( pen );
        painter.drawEllipse( QPointF(pItem.m_nLastPos), r, r );

        dx = 2.0;
        nWidth = pItem.m_bHoverAngle ? dx*m_pAppOption->getWidthHover() : m_pAppOption->getWidth();
        pen.setWidthF( nWidth );
        pen.setColor ( m_pAppOption->getPenColorRotate() );
        painter.setPen( pen );
        painter.drawEllipse( QPointF(pItem.m_nAnglePos ), r, r );

        nWidth = pItem.m_bHoverResize ? dx*m_pAppOption->getWidthHover() : m_pAppOption->getWidth();
        pen.setWidthF( nWidth );
        pen.setColor ( m_pAppOption->getPenColorResize() );
        painter.setPen( pen );
        painter.drawEllipse( QPointF(pItem.m_nResizePos ), r, r );
    }

    dx = 2.0;
    nWidth = pItem.m_bHoverCenter ? dx*m_pAppOption->getWidthHover() : m_pAppOption->getWidth();
    pen.setWidthF( nWidth );
    pen.setColor ( m_pAppOption->getPenColor() );
    painter.setPen( pen );
    painter.drawEllipse( rcf.center(), r, r );

    if( !pItem.m_Name.empty() )
    {
        painter.save();
        auto fm = painter.fontMetrics();
        int w = fm.horizontalAdvance( pItem.m_Name.c_str() );
        int h = fm.height();
        int x = rcf.center().x() - w/2;
        int y = rcf.center().y() + h/2;
        painter.translate( rcf.center().x(), rcf.center().y() );
        painter.rotate( -qRadiansToDegrees( pItem.m_nAngle ) );
        painter.translate( -rcf.center().x(), -rcf.center().y() );
        painter.drawText( x, y, pItem.m_Name.c_str() );
        painter.restore();
    }
}
