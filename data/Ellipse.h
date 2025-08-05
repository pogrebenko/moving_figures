#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "common/Figure.h"

class CEllipse : public Figure_t
{
public:
    CEllipse();
    virtual bool contain( const QPoint &obj ) override;
};

#endif // ELLIPSE_H
