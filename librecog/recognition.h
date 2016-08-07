#pragma once

#include <QVector>
#include <QPoint>
#include <QImage>

namespace recog {

class iShape;
iShape* createShapeFromPoints(const QVector<QPoint> &points);

QImage createHoughLineViewImage(const QVector<QPoint> &points);

//TODO: Move it outside
QVector<QPoint> makeContinious(const QVector<QPoint> &points);

}
