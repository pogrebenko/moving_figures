#ifndef PAINTER_H
#define PAINTER_H

#include <QPainterPath>
#include <QPainter>
#include <QPixmap>

#include "../common/Figure.h"
#include "../common/Relation.h"

class CAppOption;

class CPainter
{
    friend class CPaintWidget;

public:
    CPainter( CAppOption *pAppOption );
    virtual ~CPainter() {}

    void painter_draw( QPainter &painter );
    void painter_resize( const QRect &r );

    void draw_figure_relation();

    void draw_figure_new   ();
    void draw_figure       ( QPainter &painter, FigureList_t   *FigureList   );
    void draw_figure_item  ( QPainter &painter, Figure_t       &Figure       );

    void draw_relation_new ();
    void draw_relation     ( QPainter &painter, RelationList_t *RelationList );
    void draw_relation_item( QPainter &painter, Relation_t     &Relation     );

private:
    CAppOption *m_pAppOption;
    QPixmap     m_pixmap;
};

#endif // PAINTER_H
