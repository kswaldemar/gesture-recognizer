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

    QVector<QPoint> makeContinious(const QVector<QPoint> &points);

    int detectLineWithScore(const QVector<QPoint> &points, QLine *line);

private:
    HoughTransform m_ht;
    QVector<QPoint> m_gestPoints;
    QVector< QVector<QPoint> > m_gestMt;
};

}
