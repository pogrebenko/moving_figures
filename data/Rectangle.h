#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "common/Figure.h"

class CRectangle : public Figure_t
{
public:
    CRectangle();
    virtual bool contain( const QPoint &obj ) override;
};

#endif // RECTANGLE_H
