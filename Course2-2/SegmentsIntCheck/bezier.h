#ifndef BEZIER_H
#define BEZIER_H

#include <QVector>
#include <QPointF>

namespace Bezier
{
QPointF getBezierPoint(const QVector<QPointF> & pts, qreal t);
}

#endif // BEZIER_H
