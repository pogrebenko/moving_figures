#include "Options.h"

#include "Figure.h"
#include "IOStreamer.h"
#include "data/FigureFactory.h"

CAppOption::CAppOption()
{
    init();
}

void CAppOption::init()
{
    m_nAppVariant   = APP_VARIANT;
    m_nWidth        = 2.0;
    m_nWidthHover   = 4.0;
    m_PenColor      = Qt::red;
    m_PenColorRotate= Qt::darkGreen;
    m_PenColorResize= Qt::darkBlue;
    m_BrushColor    = Qt::green;
    m_BkgColor      = Qt::white;
    m_PenStyle      = Qt::SolidLine;
    m_ArrowType     = Qt::NoArrow;
    m_BrushStyle    = Qt::FDiagPattern;  // Qt::CrossPattern, Qt::DiagCrossPattern
    m_nCPRadius     = 2.0;

    m_nFigureType   = FigureTypeRectangle;
    m_nFirstPos.setX( 0 ); m_nFirstPos.setY( 0 );
    m_nLastPos .setX( 0 ); m_nLastPos .setY( 0 );

    m_nActionType   = ActionTypeAddFigure;

    m_nIndexFrom    = -1;
    m_nRelationType = RelationTypeLineNondirect;

    m_nArrowAngle   = 10;
    m_nArrowSize    = 15;
    m_FigureList  .Flush();
    m_RelationList.Flush();
}

FigureType_t
CAppOption::rotateFigureType( FigureType_t nFigureType )
{
    switch( nFigureType )
    {
        case FigureTypeNone      : break;
        case FigureTypeTriangle  : return FigureTypeTriangle ;
        case FigureTypeEllipse   : return FigureTypeRectangle;
        case FigureTypeRectangle : return FigureTypeEllipse  ;
    }
    return FigureTypeNone;
}


RelationType_t
CAppOption::rotateRelationType( RelationType_t nRelationType )
{
    switch( nRelationType )
    {
        case RelationTypeNone            : break;
        case RelationTypeLineNondirect   : return RelationTypeLineBidirect   ;
        case RelationTypeLineBidirect    : return RelationTypeLineDirectLeft ;
        case RelationTypeLineDirectLeft  : return RelationTypeLineDirectRight;
        case RelationTypeLineDirectRight : return RelationTypeLineNondirect  ;
    }
    return RelationTypeNone;
}

QJsonObject& Write( QJsonObject &ios, CAppOption &obj )
{
    QJsonObject m_nFirstPos   ; ::Write( m_nFirstPos   , obj.m_nFirstPos    );
    QJsonObject m_nLastPos    ; ::Write( m_nLastPos    , obj.m_nLastPos     );
    QJsonObject m_FigureList  ; ::Write( m_FigureList  , obj.m_FigureList   );
    QJsonObject m_RelationList; ::Write( m_RelationList, obj.m_RelationList );

    ios[ "m_nAppVariant"   ] =      obj.m_nAppVariant  ;
    ios[ "m_nWidth"        ] =      obj.m_nWidth       ;
    ios[ "m_nWidthHover"   ] =      obj.m_nWidthHover  ;
    ios[ "m_PenColor"      ] = (int)obj.m_PenColor     ;
    ios[ "m_PenColorRotate"] = (int)obj.m_PenColorRotate;
    ios[ "m_PenColorResize"] = (int)obj.m_PenColorResize;
    ios[ "m_BrushColor"    ] = (int)obj.m_BrushColor   ;
    ios[ "m_BkgColor"      ] = (int)obj.m_BkgColor     ;
    ios[ "m_PenStyle"      ] = (int)obj.m_PenStyle     ;
    ios[ "m_ArrowType"     ] =      obj.m_ArrowType    ;
    ios[ "m_BrushStyle"    ] = (int)obj.m_BrushStyle   ;
    ios[ "m_nCPRadius"     ] =      obj.m_nCPRadius    ;
    ios[ "m_nFigureType"   ] =      obj.m_nFigureType  ;
    ios[ "m_nActionType"   ] =      obj.m_nActionType  ;
    ios[ "m_nRelationType" ] =      obj.m_nRelationType;
    ios[ "m_nIndexFrom"    ] = (int)obj.m_nIndexFrom   ;
    ios[ "m_nArrowAngle"   ] =      obj.m_nArrowAngle  ;
    ios[ "m_nArrowSize"    ] =      obj.m_nArrowSize   ;
    ios[ "m_nFirstPos"     ] =          m_nFirstPos    ;
    ios[ "m_nLastPos"      ] =          m_nLastPos     ;
    ios[ "m_FigureList"    ] =          m_FigureList   ;
    ios[ "m_RelationList"  ] =          m_RelationList ;

    return ios;
}

CAppOption& Read( QJsonObject &ios, CAppOption &obj, long version )
{
    obj.        m_nAppVariant   =                  ios[ "m_nAppVariant"   ].toInt();
    obj.        m_nWidth        =                  ios[ "m_nWidth"        ].toInt();
    obj.        m_nWidthHover   =                  ios[ "m_nWidthHover"   ].toInt();
    obj.        m_PenColor      = (Qt::GlobalColor)ios[ "m_PenColor"      ].toInt();
    obj.        m_PenColorRotate= (Qt::GlobalColor)ios[ "m_PenColorRotate"].toInt();
    obj.        m_PenColorResize= (Qt::GlobalColor)ios[ "m_PenColorResize"].toInt();
    obj.        m_BrushColor    = (Qt::GlobalColor)ios[ "m_BrushColor"    ].toInt();
    obj.        m_BkgColor      = (Qt::GlobalColor)ios[ "m_BkgColor"      ].toInt();
    obj.        m_PenStyle      = (Qt::PenStyle   )ios[ "m_PenStyle"      ].toInt();
    obj.        m_ArrowType     = (Qt::ArrowType  )ios[ "m_ArrowType"     ].toInt();
    obj.        m_BrushStyle    = (Qt::BrushStyle )ios[ "m_BrushStyle"    ].toInt();
    obj.        m_nCPRadius     =                  ios[ "m_nCPRadius"     ].toInt();
    obj.        m_nFigureType   = (FigureType_t   )ios[ "m_nFigureType"   ].toInt();
    obj.        m_nActionType   = (ActionType_t   )ios[ "m_nActionType"   ].toInt();
    obj.        m_nRelationType = (RelationType_t )ios[ "m_nRelationType" ].toInt();
    obj.        m_nIndexFrom    =                  ios[ "m_nIndexFrom"    ].toInt();
    obj.        m_nArrowAngle   =                  ios[ "m_nArrowAngle"   ].toInt();
    obj.        m_nArrowSize    =                  ios[ "m_nArrowSize"    ].toInt();
    QJsonObject m_nFirstPos     =                  ios[ "m_nFirstPos"     ].toObject();
    QJsonObject m_nLastPos      =                  ios[ "m_nLastPos"      ].toObject();
    QJsonObject m_FigureList    =                  ios[ "m_FigureList"    ].toObject();
    QJsonObject m_RelationList  =                  ios[ "m_RelationList"  ].toObject();

    ::Read( m_nFirstPos   , obj.m_nFirstPos   , version );
    ::Read( m_nLastPos    , obj.m_nLastPos    , version );
    ::Read( m_FigureList  , obj.m_FigureList  , version );
    ::Read( m_RelationList, obj.m_RelationList, version );
    return obj;
}

void Write( CIOStreamer &ios, CAppOption &obj )
{
    ::Write   ( ios, obj.m_nAppVariant   );
    ::Write   ( ios, obj.m_nWidth        );
    ::Write   ( ios, obj.m_nWidthHover   );
    ::WriteInt( ios, obj.m_PenColor      );
    ::WriteInt( ios, obj.m_PenColorRotate);
    ::WriteInt( ios, obj.m_PenColorResize);
    ::WriteInt( ios, obj.m_BrushColor    );
    ::WriteInt( ios, obj.m_BkgColor      );
    ::WriteInt( ios, obj.m_PenStyle      );
    ::WriteInt( ios, obj.m_ArrowType     );
    ::WriteInt( ios, obj.m_BrushStyle    );
    ::Write   ( ios, obj.m_nCPRadius     );
    ::WriteInt( ios, obj.m_nFigureType   );
    ::Write   ( ios, obj.m_nFirstPos     );
    ::Write   ( ios, obj.m_nLastPos      );
    ::WriteInt( ios, obj.m_nActionType   );
    ::WriteInt( ios, obj.m_nRelationType );
    ::Write   ( ios, obj.m_nIndexFrom    );
    ::Write   ( ios, obj.m_nArrowAngle   );
    ::Write   ( ios, obj.m_nArrowSize    );
    ::Write   ( ios, obj.m_FigureList    );
    ::Write   ( ios, obj.m_RelationList  );
}


CAppOption& Read( CIOStreamer &ios, CAppOption &obj, long version )
{
    ::Read   ( ios, obj.m_nAppVariant             );
    ::Read   ( ios, obj.m_nWidth                  );
    ::Read   ( ios, obj.m_nWidthHover             );
    ::ReadInt( ios, obj.m_PenColor                );
    ::ReadInt( ios, obj.m_PenColorRotate          );
    ::ReadInt( ios, obj.m_PenColorResize          );
    ::ReadInt( ios, obj.m_BrushColor              );
    ::ReadInt( ios, obj.m_BkgColor                );
    ::ReadInt( ios, obj.m_PenStyle                );
    ::ReadInt( ios, obj.m_ArrowType               );
    ::ReadInt( ios, obj.m_BrushStyle              );
    ::Read   ( ios, obj.m_nCPRadius               );
    ::ReadInt( ios, obj.m_nFigureType             );
    ::Read   ( ios, obj.m_nFirstPos     , version );
    ::Read   ( ios, obj.m_nLastPos      , version );
    ::ReadInt( ios, obj.m_nActionType             );
    ::ReadInt( ios, obj.m_nRelationType           );
    ::Read   ( ios, obj.m_nIndexFrom              );
    ::Read   ( ios, obj.m_nArrowAngle             );
    ::Read   ( ios, obj.m_nArrowSize              );
    ::Read   ( ios, obj.m_FigureList    , version );
    ::Read   ( ios, obj.m_RelationList  , version );

    return obj;
}
