#ifndef OPTIONS_H
#define OPTIONS_H

#include <qnamespace.h>
#include <QPoint>
#include <QVariantMap>

#include "Consts.h"
#include "Logger.h"
#include "Figure.h"
#include "Relation.h"

class CIOStreamer;

class CAppOption
{
    friend void         Write( CIOStreamer &ios, CAppOption &obj );
    friend CAppOption&  Read ( CIOStreamer &ios, CAppOption &obj, long load_version );

    friend QJsonObject& Write( QJsonObject &ios, CAppOption &obj );
    friend CAppOption&  Read ( QJsonObject &ios, CAppOption &obj, long load_version );

public:
    CAppOption();
    CAppOption(Logger_t * const pLogger);

    void init();

    static FigureType_t   rotateFigureType  ( FigureType_t   );
    static RelationType_t rotateRelationType( RelationType_t );

    inline float           getWidth     () { return m_nWidth     ; }
    inline float           getWidthHover() { return m_nWidthHover; }
    inline Qt::GlobalColor getPenColor  () { return m_PenColor   ; }
    inline Qt::GlobalColor getPenColorRotate() { return m_PenColorRotate; }
    inline Qt::GlobalColor getPenColorResize() { return m_PenColorResize; }
    inline Qt::GlobalColor getBrushColor() { return m_BrushColor ; }
    inline Qt::GlobalColor getBkgColor  () { return m_BkgColor   ; }
    inline Qt::PenStyle    getPenStyle  () { return m_PenStyle   ; }
    inline Qt::ArrowType   getArrowType () { return m_ArrowType  ; }
    inline Qt::BrushStyle  getBrushStyle() { return m_BrushStyle ; }
    inline float           getCPRadius  () { return m_nCPRadius  ; }

    inline FigureType_t    getFigureType() { return m_nFigureType; }
    inline void            setFigureType( FigureType_t nFigureType ) { m_nFigureType = nFigureType; }
    inline RelationType_t  getRelationType() { return m_nRelationType; }
    inline void            setRelationType( RelationType_t nRelationType ) { m_nRelationType = nRelationType; }
    inline ActionType_t    getActionType() { return m_nActionType; }
    inline void            setActionType( ActionType_t nActionType ) { m_nActionType = nActionType; }

    inline QPoint&         getFirstPos() { return m_nFirstPos; }
    inline void            setFirstPos( const QPoint &nFirstPos ) { m_nFirstPos = nFirstPos; }
    inline QPoint&         getLastPos() { return m_nLastPos; }
    inline void            setLastPos( const QPoint &nLastPos ) { m_nLastPos = nLastPos; }

    inline FigureList_t  *   getFigureList  () { return &m_FigureList  ; }
    inline RelationList_t*   getRelationList() { return &m_RelationList; }

    inline long            getIndexFrom() { return m_nIndexFrom; }
    inline void            setIndexFrom( unsigned int nIndexFrom ) { m_nIndexFrom = nIndexFrom; }
    inline int             getArrowAngle() { return m_nArrowAngle; }
    inline void            setArrowAngle( unsigned int nArrowAngle ) { m_nArrowAngle = nArrowAngle; }
    inline float           getArrowSize() { return m_nArrowSize; }
    inline void            setArrowSize( float nArrowSize ) { m_nArrowSize = nArrowSize; }

private:
    int             m_nAppVariant  ;
    float           m_nWidth       ;
    float           m_nWidthHover  ;
    Qt::GlobalColor m_PenColor     ;
    Qt::GlobalColor m_PenColorRotate;
    Qt::GlobalColor m_PenColorResize;
    Qt::GlobalColor m_BrushColor   ;
    Qt::GlobalColor m_BkgColor     ;
    Qt::PenStyle    m_PenStyle     ;
    Qt::ArrowType   m_ArrowType    ;
    Qt::BrushStyle  m_BrushStyle   ;
    float           m_nCPRadius    ;
    FigureType_t    m_nFigureType  ;
    ActionType_t    m_nActionType  ;
    RelationType_t  m_nRelationType;
    long            m_nIndexFrom   ;
    int             m_nArrowAngle  ;
    float           m_nArrowSize   ;
    QPoint          m_nFirstPos    ;
    QPoint          m_nLastPos     ;
    FigureList_t    m_FigureList   ;
    RelationList_t  m_RelationList ;
};

extern void         Write( CIOStreamer &ios, CAppOption &obj );
extern CAppOption&  Read ( CIOStreamer &ios, CAppOption &obj, long version );

extern QJsonObject& Write( QJsonObject &ios, CAppOption &obj );
extern CAppOption&  Read ( QJsonObject &ios, CAppOption &obj, long version );

#endif // OPTIONS_H
