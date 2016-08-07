#pragma once

#include <QVector>
#include <QPoint>

namespace recog {

class iShape;

iShape* createShapeFromPoints(const QVector<QPoint> &points);

}
