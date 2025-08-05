#ifndef FIGUREFACTORY_H
#define FIGUREFACTORY_H

#include "common/IOStreamer.h"
#include "common/Figure.h"
#include "common/Relation.h"
#include "common/Utils.h"

#include <QJsonObject>

class CFigureFactory
{
public:
             CFigureFactory();
    virtual ~CFigureFactory();

    static Figure_t  * buildFigure  ( FigureType_t );
    static Figure_t  * buildFigure  ( FigureType_t, const QPoint&, const QPoint& );

    static Relation_t* buildRelation( RelationType_t );
    static Relation_t* buildRelation( RelationType_t, unsigned int, unsigned int, const QPoint&, const QPoint& );


    static QJsonObject    & Write( QJsonObject &ios, Figure_t &obj );
    static Figure_t       & Read ( QJsonObject &ios, Figure_t &obj, long version );

    static QJsonObject    & Write( QJsonObject &ios, FigureList_t &obj );
    static FigureList_t   & Read ( QJsonObject &ios, FigureList_t &obj, long version );


    static void             Write( CIOStreamer &ios, Figure_t &obj );
    static Figure_t       & Read ( CIOStreamer &ios, Figure_t &obj, long version );

    static void             Write( CIOStreamer &ios, FigureList_t &obj );
    static FigureList_t   & Read ( CIOStreamer &ios, FigureList_t &obj, long version );



    static QJsonObject    & Write( QJsonObject &ios, Relation_t &obj );
    static Relation_t     & Read ( QJsonObject &ios, Relation_t &obj, long version );

    static QJsonObject    & Write( QJsonObject &ios, RelationList_t &obj );
    static RelationList_t & Read ( QJsonObject &ios, RelationList_t &obj, long version );


    static void             Write( CIOStreamer &ios, Relation_t &obj );
    static Relation_t     & Read ( CIOStreamer &ios, Relation_t &obj, long version );

    static void             Write( CIOStreamer &ios, RelationList_t &obj );
    static RelationList_t & Read ( CIOStreamer &ios, RelationList_t &obj, long version );
};




inline void Write( CIOStreamer &ios, FigureList_t &obj ){
    CFigureFactory::Write( ios, obj );
}
inline FigureList_t& Read( CIOStreamer &ios, FigureList_t &obj, long version ){
    return CFigureFactory::Read( ios, obj, version );
}

inline void Write( CIOStreamer &ios, RelationList_t &obj ){
    CFigureFactory::Write( ios, obj );
}
inline RelationList_t& Read ( CIOStreamer &ios, RelationList_t &obj, long version ){
    return CFigureFactory::Read( ios, obj, version );
}




inline QJsonObject& Write( QJsonObject &ios, FigureList_t &obj ){
    return CFigureFactory::Write( ios, obj );
}
inline FigureList_t& Read( QJsonObject &ios, FigureList_t &obj, long version ){
    return CFigureFactory::Read( ios, obj, version );
}

inline QJsonObject& Write( QJsonObject &ios, RelationList_t &obj ){
    return CFigureFactory::Write( ios, obj );
}
inline RelationList_t& Read ( QJsonObject &ios, RelationList_t &obj, long version ){
    return CFigureFactory::Read( ios, obj, version );
}

#endif // FIGUREFACTORY_H
