#pragma once

#include "houghtransform.h"

#include <QVector>
#include <QPoint>
#include <QImage>

namespace recog {

class iShape;

class Recognizer {
public:

    void loadGesture(const QVector<QPoint> &points);

    iShape* detectShape();

    QImage houghTransformView();

private:
    QImage createHoughLineViewImage(const QVector<QPoint> &points);

    QVector<QPoint> makeContinuous(const QVector<QPoint> &points);

    int detectLineWithScore(const QVector<QPoint> &points, QLine *line);

    int detectRectangleWithScore(const QVector<QPoint> &points, QRect *rect);

private:
    HoughTransform m_ht;
    QVector<QPoint> m_gestPoints;
    QVector< QVector<int> > m_gestMt;
};

}
