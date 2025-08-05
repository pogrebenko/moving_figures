#include "Triangle.h"

#include <math.h>

#include <QRectF>
#include <QPainterPath>

CTriangle::CTriangle()
: Figure_t( FigureTypeTriangle )
{
    m_nAnglesCount = 3;
}
