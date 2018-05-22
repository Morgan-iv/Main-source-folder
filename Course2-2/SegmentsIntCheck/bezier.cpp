#include "bezier.h"

namespace Bezier
{
QPointF getBezierPoint(const QVector<QPointF> & pts, qreal t)
{
    QVector<QPointF> tmp(pts);
    for (int i = pts.size() - 1; i > 0; --i)
        for (int k = 0; k < i; k++)
            tmp[k] = tmp[k] + t * (tmp[k + 1] - tmp[k]);
    return tmp[0];
}
}
