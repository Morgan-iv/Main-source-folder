#ifndef CONVEXHULL_H
#define CONVEXHULL_H

#include <algorithm>
#include <QPair>
#include <QVector>
#include <QStack>
#include <QPointF>

namespace ConvexHull
{
void calculate(QVector<QPointF> & arr);
}

#endif // CONVEXHULL_H
