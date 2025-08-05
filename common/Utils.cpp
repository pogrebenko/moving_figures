#include <QPoint>

#include "IOStreamer.h"

#include "Figure.h"
#include "Relation.h"

#include <QJsonObject>
#include <QVariantMap>

QJsonObject& Write( QJsonObject &ios, QPoint &obj )
{
    ios[ "x" ] = obj.rx();
    ios[ "y" ] = obj.ry();
    return ios;
}

QPoint& Read( QJsonObject &ios, QPoint &obj, long )
{
    obj.setX( ios[ "x" ].toInt() );
    obj.setY( ios[ "y" ].toInt() );
    return obj;
}

void Write( CIOStreamer &ios, QPoint &obj )
{
    int x = obj.rx(); ::Write( ios, x );
    int y = obj.ry(); ::Write( ios, y );
}

QPoint& Read( CIOStreamer &ios, QPoint &obj, long )
{
    int x = 0; ::Read( ios, x ); obj.setX( x );
    int y = 0; ::Read( ios, y ); obj.setY( y );
    return obj;
}


QJsonObject& Write( QJsonObject &ios, Figure_t &obj )
{
    QJsonObject m_nFirstPos ; ::Write( m_nFirstPos , obj.m_nFirstPos  );
    QJsonObject m_nLastPos  ; ::Write( m_nLastPos  , obj.m_nLastPos   );
    QJsonObject m_nAnglePos ; ::Write( m_nAnglePos , obj.m_nAnglePos  );
    QJsonObject m_nResizePos; ::Write( m_nResizePos, obj.m_nResizePos );

    ios[ "m_nFigureType" ] =      obj.m_nFigureType ;
    ios[ "m_bHover"      ] =      obj.m_bHover      ;
    ios[ "m_nAnglesCount"] = (int)obj.m_nAnglesCount;
    ios[ "m_nAngle"      ] =      obj.m_nAngle      ;
    ios[ "m_nFirstPos"   ] =          m_nFirstPos   ;
    ios[ "m_nLastPos"    ] =          m_nLastPos    ;
    ios[ "m_nAnglePos"   ] =          m_nAnglePos   ;
    ios[ "m_nResizePos"  ] =          m_nResizePos  ;

    return ios;
}

Figure_t& Read( QJsonObject &ios, Figure_t &obj, long version )
{
    obj .m_nFigureType = (FigureType_t)ios[ "m_nFigureType" ].toInt   ();
    obj .m_bHover      =               ios[ "m_bHover"      ].toBool  ();
    obj .m_nAnglesCount=               ios[ "m_nAnglesCount"].toInt   ();
    obj .m_nAngle      =               ios[ "m_nAngle"      ].toDouble();
    auto m_nFirstPos   =               ios[ "m_nFirstPos"   ].toObject();
    auto m_nLastPos    =               ios[ "m_nLastPos"    ].toObject();
    auto m_nAnglePos   =               ios[ "m_nAnglePos"   ].toObject();
    auto m_nResizePos  =               ios[ "m_nResizePos"  ].toObject();

    ::Read( m_nFirstPos , obj.m_nFirstPos , version );
    ::Read( m_nLastPos  , obj.m_nLastPos  , version );
    ::Read( m_nAnglePos , obj.m_nAnglePos , version );
    ::Read( m_nResizePos, obj.m_nResizePos, version );
    return obj;
}

CIOStreamer& Write( CIOStreamer &ios, Figure_t &obj )
{
    ::WriteInt( ios, obj.m_nFigureType );
    ::Write   ( ios, obj.m_nFirstPos   );
    ::Write   ( ios, obj.m_nLastPos    );
    ::Write   ( ios, obj.m_nAnglePos   );
    ::Write   ( ios, obj.m_nResizePos  );
    ::Write   ( ios, obj.m_nAngle      );
    ::Write   ( ios, obj.m_nAnglesCount);
    ::Write   ( ios, obj.m_bHover      );
    return ios;
}

Figure_t& Read( CIOStreamer &ios, Figure_t &obj, long version )
{
    ::ReadInt( ios, obj.m_nFigureType          );
    ::Read   ( ios, obj.m_nFirstPos  , version );
    ::Read   ( ios, obj.m_nLastPos   , version );
    ::Read   ( ios, obj.m_nAnglePos  , version );
    ::Read   ( ios, obj.m_nResizePos , version );
    ::Read   ( ios, obj.m_nAngle               );
    ::Read   ( ios, obj.m_nAnglesCount         );
    ::Read   ( ios, obj.m_bHover               );
    return obj;
}



QJsonObject& Write( QJsonObject &ios, Relation_t &obj )
{
    QJsonObject m_nFirstPos;  ::Write( m_nFirstPos, obj.m_nFirstPos );
    QJsonObject m_nLastPos ;  ::Write( m_nLastPos , obj.m_nLastPos  );

    ios[ "m_nRelationType" ] =      obj.m_nRelationType;
    ios[ "m_nFrom"         ] = (int)obj.m_nFrom        ;
    ios[ "m_nTo"           ] = (int)obj.m_nTo          ;
    ios[ "m_bHover"        ] =      obj.m_bHover       ;
    ios[ "m_nFirstPos"     ] =          m_nFirstPos    ;
    ios[ "m_nLastPos"      ] =          m_nLastPos     ;
    return ios;
}

Relation_t& Read( QJsonObject &ios, Relation_t &obj, long version )
{
    obj .m_nRelationType = (RelationType_t)ios[ "m_nRelationType" ].toInt ();
    obj .m_nFrom         =                 ios[ "m_nFrom"         ].toInt ();
    obj .m_nTo           =                 ios[ "m_nTo"           ].toInt ();
    obj .m_bHover        =                 ios[ "m_bHover"        ].toBool();
    auto m_nFirstPos     =                 ios[ "m_nFirstPos"     ].toObject();
    auto m_nLastPos      =                 ios[ "m_nLastPos"      ].toObject();

    ::Read( m_nFirstPos, obj.m_nFirstPos, version );
    ::Read( m_nLastPos , obj.m_nLastPos , version );
    return obj;
}

void Write( CIOStreamer &ios, Relation_t &obj )
{
    ::WriteInt( ios, obj.m_nRelationType );
    ::Write   ( ios, obj.m_nFrom         );
    ::Write   ( ios, obj.m_nTo           );
    ::Write   ( ios, obj.m_nFirstPos     );
    ::Write   ( ios, obj.m_nLastPos      );
    ::Write   ( ios, obj.m_bHover        );
}

Relation_t& Read( CIOStreamer &ios, Relation_t &obj, long version )
{
    ::ReadInt( ios, obj.m_nRelationType          );
    ::Read   ( ios, obj.m_nFrom                  );
    ::Read   ( ios, obj.m_nTo                    );
    ::Read   ( ios, obj.m_nFirstPos    , version );
    ::Read   ( ios, obj.m_nLastPos     , version );
    ::Read   ( ios, obj.m_bHover                 );
    return obj;
}
