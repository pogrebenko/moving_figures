#ifndef BUILDER_H
#define BUILDER_H

#include "../common/Consts.h"
#include "../common/Logger.h"

class CAppOption;
class QPoint;

class CBuilder : public CLoggerHolder
{
public:
    CBuilder( CAppOption * const pAppOption, Logger_t * const pLogger );
    virtual ~CBuilder() {}

    ActionType_t get_action_type();
    bool check_action_type( ActionType_t ActionType );

    void hover_reset();
    void edit_reset();

    // relation section
    long relation_hover( const QPoint &pos, bool set_up_hover );
    bool relation_delete( long n );
    bool relation_rotate_type( long n );
    void relation_begin_add( long n );
    bool relation_release_add( const QPoint &pos );
    bool relation_set_last_pos( const QPoint &pos );
    void relation_rebuild();

    // figure section
    long figure_hover( const QPoint &pos, bool set_up_hover );
    bool figure_delete( long n );
    bool figure_rotate_type( long n );
    void figure_begin_add( const QPoint &pos );
    bool figure_release_add();
    void figure_set_first_pos( const QPoint &pos );
    void figure_set_last_pos( const QPoint &pos );
    bool figure_move( const QPoint &pos );

    long resize_hover( const QPoint &pos, bool set_up_hover, bool set_up_resize );
    long rotate_hover( const QPoint &pos, bool set_up_hover, bool set_up_resize );
    long center_hover( const QPoint &pos, bool set_up_hover );

    long relation_oscillation( QPoint &pt, int dx, int dy );
    long figure_oscillation( QPoint &pt, int dx, int dy );

private:
    CAppOption *m_pAppOption;
};

#endif // BUILDER_H
