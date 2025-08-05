#include "FigureFactory.h"

#include <QJsonArray>
#include <QJsonObject>

#include "Rectangle.h"
#include "Triangle.h"
#include "Ellipse.h"

extern void        Write( CIOStreamer &ios, CEllipse &obj );
extern CEllipse&   Read ( CIOStreamer &ios, CEllipse &obj, long version );

extern void        Write( CIOStreamer &ios, CRectangle &obj );
extern CRectangle& Read ( CIOStreamer &ios, CRectangle &obj, long version );

extern void        Write( CIOStreamer &ios, CTriangle &obj );
extern CTriangle&  Read ( CIOStreamer &ios, CTriangle &obj, long version );

CFigureFactory:: CFigureFactory() {}
CFigureFactory::~CFigureFactory() {}

Figure_t*
CFigureFactory::buildFigure( FigureType_t nFigureType )
{
    switch( nFigureType )
    {
        case FigureTypeTriangle  : return new CTriangle ();
        case FigureTypeEllipse   : return new CEllipse  ();
        case FigureTypeRectangle : return new CRectangle();
        case FigureTypeNone      : throw "Exception: figure with type none";
    }
    throw "Exception: unknown figure";
}

Figure_t*
CFigureFactory::buildFigure( FigureType_t nFigureType, const QPoint &nFirsPos, const QPoint &nLastPos )
{
    Figure_t *pObj = CFigureFactory::buildFigure( nFigureType );
              pObj->m_nFirstPos =
              pObj->m_nAnglePos = nFirsPos;
              pObj->m_nResizePos=
              pObj->m_nLastPos  = nLastPos;

              pObj->calc_angle_point( nFirsPos );

              pObj->m_nResizePos.rx()+= DELTA_RESIZE;
              pObj->m_nResizePos.ry()+= DELTA_RESIZE;

    return pObj;
}

Relation_t*
CFigureFactory::buildRelation( RelationType_t nRelationType )
{
    return new Relation_t( nRelationType );
}

Relation_t*
CFigureFactory::buildRelation( RelationType_t nRelationType, unsigned int nFrom, unsigned int nTo, const QPoint &nFirstPos, const QPoint &nLastPos )
{
    Relation_t  *pObj = CFigureFactory::buildRelation( nRelationType );
                 pObj->m_nFrom     = nFrom;
                 pObj->m_nTo       = nTo;
                 pObj->m_nFirstPos = nFirstPos;
                 pObj->m_nLastPos  = nLastPos;
    return pObj;
}



QJsonObject&
CFigureFactory::Write( QJsonObject &ios, Figure_t &obj )
{
    ios[ "m_nFigureType" ] = obj.m_nFigureType;

    switch( obj.m_nFigureType )
    {
        case FigureTypeTriangle  : CFigureFactory::Write( ios, (CTriangle &)obj ); break;
        case FigureTypeEllipse   : CFigureFactory::Write( ios, (CEllipse  &)obj ); break;
        case FigureTypeRectangle : CFigureFactory::Write( ios, (CRectangle&)obj ); break;
        case FigureTypeNone      : throw "Exception: figure with type none";
        default                  : throw "Exception: unknown figure";
    }
    return ios;
}

Figure_t&
CFigureFactory::Read ( QJsonObject &ios, Figure_t &obj, long version )
{
    obj.m_nFigureType = (FigureType_t) ios[ "m_nFigureType" ].toInt();

    switch( obj.m_nFigureType )
    {
        case FigureTypeTriangle  : return CFigureFactory::Read( ios, (CTriangle &)obj, version );
        case FigureTypeEllipse   : return CFigureFactory::Read( ios, (CEllipse  &)obj, version );
        case FigureTypeRectangle : return CFigureFactory::Read( ios, (CRectangle&)obj, version );
        case FigureTypeNone      : throw "Exception: figure with type none";
    }
    throw "Exception: unknown figure";
}


void CFigureFactory::Write( CIOStreamer &ios, Figure_t &obj )
{
    ::WriteInt( ios, obj.m_nFigureType );
    switch( obj.m_nFigureType )
    {
        case FigureTypeTriangle  : CFigureFactory::Write( ios, (CTriangle &)obj ); break;
        case FigureTypeEllipse   : CFigureFactory::Write( ios, (CEllipse  &)obj ); break;
        case FigureTypeRectangle : CFigureFactory::Write( ios, (CRectangle&)obj ); break;
        case FigureTypeNone      : throw "Exception: figure with type none";
        default                  : throw "Exception: unknown figure";
    }
}

Figure_t& CFigureFactory::Read( CIOStreamer &ios, Figure_t &obj, long version )
{
    switch( obj.m_nFigureType )
    {
        case FigureTypeTriangle  : return CFigureFactory::Read( ios, (CTriangle &)obj, version );
        case FigureTypeEllipse   : return CFigureFactory::Read( ios, (CEllipse  &)obj, version );
        case FigureTypeRectangle : return CFigureFactory::Read( ios, (CRectangle&)obj, version );
        case FigureTypeNone      : throw "Exception: figure with type none";
    }
    throw "Exception: unknown figure";
}


QJsonObject& CFigureFactory::Write( QJsonObject &ios, FigureList_t &obj )
{
    QJsonArray FigureTypes;
    for( unsigned long n = 0; n < obj.size(); n ++ )
    {
        FigureTypes.append( QJsonValue( obj.at( n )->m_nFigureType ) );
    }
    ios["FigureTypes"] = FigureTypes;

    QJsonArray FigureItems;
    for( unsigned long n = 0; n < obj.size(); n ++ )
    {
        QJsonObject Item;
        FigureItems.append( ::Write( Item, *obj.at( n ) ) );
    }
    ios["FigureItems"] = FigureItems;

    return ios;
}

FigureList_t& CFigureFactory::Read( QJsonObject &ios, FigureList_t &obj, long version )
{
    obj.Flush();

    QJsonArray FigureTypes = ios["FigureTypes"].toArray();
    for( int n = 0; n < FigureTypes.size(); n ++ )
    {
        obj.Add( CFigureFactory::buildFigure( (FigureType_t) FigureTypes[ n ].toInt() ) );
    }

    QJsonArray FigureItems = ios["FigureItems"].toArray();
    for( unsigned int i = 0; i < obj.size() && i < (unsigned int)FigureItems.size(); i ++ )
    {
        QJsonObject FigureItem = FigureItems[ i ].toObject();
        ::Read( FigureItem, *obj.at( i ), version );
    }

    return obj;
}

void CFigureFactory::Write( CIOStreamer &ios, FigureList_t &obj )
{
    unsigned long n = 0, count = 0;
    count = obj.size(); ::Write( ios, count );
    for( n = 0; n < count; n ++ )
    {
        auto &pItem = obj.at( n );
        ::WriteInt( ios, pItem->m_nFigureType );
        ::Write   ( ios, *pItem );
    }
}

FigureList_t& CFigureFactory::Read( CIOStreamer &ios, FigureList_t &obj, long version )
{
    unsigned long n = 0, count = 0;
    ::Read( ios, count );

    obj.Flush();
    for( n = 0; n < count; n ++ )
    {
        FigureType_t nFigureType = FigureTypeNone;
        ::ReadInt( ios, nFigureType );
        obj.Add( &::Read( ios, *CFigureFactory::buildFigure( nFigureType ), version ) );
    }

    return obj;
}








void CFigureFactory::Write( CIOStreamer &ios, RelationList_t &obj )
{
    unsigned long n = 0, count = 0;
    count = obj.size(); ::Write( ios, count );
    for( n = 0; n < count; n ++ )
    {
        auto &pItem = obj.at( n );
        ::WriteInt( ios, pItem->m_nRelationType );
        ::Write   ( ios, *pItem );
    }
}

RelationList_t& CFigureFactory::Read( CIOStreamer &ios, RelationList_t &obj, long version )
{
    unsigned long n = 0, count = 0;
    ::Read( ios, count );

    obj.Flush();
    for( n = 0; n < count; n ++ )
    {
        RelationType_t nRelationType = RelationTypeNone;
        ::ReadInt( ios, nRelationType );
        obj.Add( &::Read( ios, *CFigureFactory::buildRelation( nRelationType ), version ) );
    }
    return obj;
}

QJsonObject& CFigureFactory::Write( QJsonObject &ios, RelationList_t &obj )
{
    QJsonArray RelationTypes;
    for( unsigned long n = 0; n < obj.size(); n ++ )
    {
        RelationTypes.append( QJsonValue( obj.at( n )->m_nRelationType ) );
    }
    ios["RelationTypes"] = RelationTypes;

    QJsonArray RelationItems;
    for( unsigned long n = 0; n < obj.size(); n ++ )
    {
        QJsonObject Item;
        RelationItems.append( ::Write( Item, *obj.at( n ) ) );
    }
    ios["RelationItems"] = RelationItems;

    return ios;
}

RelationList_t& CFigureFactory::Read( QJsonObject &ios, RelationList_t &obj, long version )
{
    obj.Flush();

    QJsonArray RelationTypes = ios["RelationTypes"].toArray();
    for( int n = 0; n < RelationTypes.size(); n ++ )
    {
        obj.Add( CFigureFactory::buildRelation( (RelationType_t) RelationTypes[ n ].toInt() ) );
    }

    QJsonArray RelationItems = ios["RelationItems"].toArray();
    for( unsigned int i = 0; i < obj.size() && i < (unsigned int)RelationItems.size(); i ++ )
    {
        QJsonObject RelationItem = RelationItems[ i ].toObject();
        ::Read( RelationItem, *obj.at( i ), version );
    }

    return obj;
}
