#include "recognition.h"
#include "shape.h"

#include <QVector>
#include <QPoint>
#include <QPainter>
#include <QString>

namespace recog {

iShape* createShapeFromPoints(const QVector<QPoint> &points) {
    return new SLine(points.first(), points.last());
}




}
