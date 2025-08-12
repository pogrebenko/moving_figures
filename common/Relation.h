#ifndef RELATION_H
#define RELATION_H

#include "Figure.h"

typedef struct __Relation
{
    RelationType_t m_nRelationType;
    long           m_nFrom        ;
    long           m_nTo          ;
    QPoint         m_nFirstPos    ;
    QPoint         m_nLastPos     ;
    bool           m_bHover       ;
    std::string    m_Name         ;

    __Relation( RelationType_t nRelationType );
    virtual ~__Relation() {}

    void calculate( Figure_t &pFrom, Figure_t &pTo );

    float distancePointToLine( const QPoint &p1, const QPoint &p2, const QPoint &p );
    bool  isPointNearLine(const QPoint &p1, const QPoint &p2, const QPoint &p, qreal tolerance = DEFAULT_TOLERANCE );
    bool  contain( const QPoint &point );
    void  oscillation( int dx, int dy );

} Relation_t;

#ifdef __SMART__

class CRelationHolder : public std::unique_ptr<Relation_t>
{
public :
    inline CRelationHolder( Relation_t *pItem ) : std::unique_ptr<Relation_t>( pItem ) {}
};

class RelationList_t : public std::vector< CRelationHolder >
{
public:
    RelationList_t() : std::vector< CRelationHolder > () {}

    bool validate( long nFrom, long nTo );
    void calculate( FigureList_t *pFigureList );

    void hover_clear();
    long hover_index( const QPoint &pos );

    inline void Flush(){ clear(); }
    inline void Delete( unsigned long i ) { this->erase( this->begin() + i ); }
    inline void Add( Relation_t *pItem ) { this->push_back( CRelationHolder( pItem ) ); }
};
#else

class RelationList_t : public CVectorOfPtr< Relation_t >
{
public:
    RelationList_t() : CVectorOfPtr< Relation_t > () {}

    bool validate( long nFrom, long nTo );
    void calculate( FigureList_t *pFigureList );

    void hover_clear();
    long hover_index( const QPoint &pos );
    inline void Add( Relation_t *pItem ) { this->push_back( pItem ); }
};
#endif

#endif // RELATION_H
