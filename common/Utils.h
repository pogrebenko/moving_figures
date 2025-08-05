#ifndef UTILS_H
#define UTILS_H

#include "Relation.h"

#include <QVariantMap>

class CIOStreamer;
class QPoint;

extern void        Write( CIOStreamer &ios, QPoint &obj );
extern QPoint&     Read ( CIOStreamer &ios, QPoint &obj, long );

extern void        Write( CIOStreamer &ios, Figure_t &obj );
extern Figure_t&   Read ( CIOStreamer &ios, Figure_t &obj, long version );

extern void        Write( CIOStreamer &ios, Relation_t &obj );
extern Relation_t& Read ( CIOStreamer &ios, Relation_t &obj, long version );


extern QJsonObject&Write( QJsonObject &ios, QPoint &obj );
extern QPoint&     Read ( QJsonObject &ios, QPoint &obj, long );

extern QJsonObject&Write( QJsonObject &ios, Figure_t &obj );
extern Figure_t&   Read ( QJsonObject &ios, Figure_t &obj, long version );

extern QJsonObject&Write( QJsonObject &ios, Relation_t &obj );
extern Relation_t& Read ( QJsonObject &ios, Relation_t &obj, long version );

#endif // UTILS_H
