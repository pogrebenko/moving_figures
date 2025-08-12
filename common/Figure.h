#ifndef FIGURE_H
#define FIGURE_H

// for release check #define NDEBUG
#include <assert.h>
#include <string>

#include <QPoint>

#include "Consts.h"

typedef struct __Figure
{
    FigureType_t m_nFigureType ;
    QPoint       m_nFirstPos   ;
    QPoint       m_nLastPos    ;
    QPoint       m_nAnglePos   ;
    QPoint       m_nResizePos  ;
    bool         m_bHover      ;
    bool         m_bHoverFirst ;
    bool         m_bHoverLast  ;
    bool         m_bHoverAngle ;
    bool         m_bHoverResize;
    float        m_nAngle      ;
    long         m_nAnglesCount;
    std::string  m_Name        ;

    virtual bool contain( const QPoint& );

    virtual ~__Figure() {}
    __Figure( const FigureType_t nFigureType );

    void          move( const QPoint &from, const QPoint &to );
    const QPoint& center( QPoint *point );
    bool          valid();
    bool          near_points( const QPoint &pt1, const QPoint &pt2 );
    void          calc_angle_point( const QPoint &pt );
    void          oscillation( int dx, int dy );

} Figure_t;

#ifdef __SMART__
#include <memory>
#include <vector>

class CFigureHolder : public std::shared_ptr<Figure_t>
{
public :
    inline CFigureHolder( Figure_t *pItem ) : std::shared_ptr<Figure_t>( pItem ) {}
};

class FigureList_t : public std::vector< CFigureHolder >
{
public:
    FigureList_t() : std::vector< CFigureHolder > () {}

    void hover_clear();
    long hover_index( const QPoint &pos );
    long hover_angle_index( const QPoint &pos );
    long hover_resize_index( const QPoint &pos );
    long hover_first_index( const QPoint &pos );
    long hover_last_index( const QPoint &pos );

    inline void Flush(){ clear(); }
    inline void Delete( unsigned long i ) { this->erase( this->begin() + i ); }
    inline void Add( Figure_t *pItem ) { this->push_back( CFigureHolder( pItem ) ); }
};
#else
#include "VectorOfPtr.h"

class FigureList_t : public CVectorOfPtr< Figure_t >
{
public:
    FigureList_t() : CVectorOfPtr< Figure_t > () {}

    void hover_clear();
    long hover_index( const QPoint &pos );
    inline void Add( Figure_t *pItem ) { this->push_back( pItem ); }
};
#endif

#endif // FIGURE_H
