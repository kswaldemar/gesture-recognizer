#pragma once

#include <QVector>
#include <QPoint>

namespace recog {

class iShape;
iShape* createShapeFromPoints(const QVector<QPoint> &points);

//TODO: Move it outside
QVector<QPoint> makeContinious(const QVector<QPoint> &points);

}
