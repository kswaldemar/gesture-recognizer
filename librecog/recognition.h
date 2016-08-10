#pragma once

#include "houghtransform.h"

#include <QVector>
#include <QPoint>
#include <QImage>

namespace recog {

class iShape;

class Recognizer {
    static const int IMG_MAX_WIDTH = 2000;
    static const int IMG_MAX_HEIGHT = 2000;
public:
    Recognizer();
    ~Recognizer();

    void loadGesture(const QVector<QPoint> &points);

    iShape* detectShape();

    QImage houghTransformView();

private:
    QImage createHoughLineViewImage(const QVector<QPoint> &points);

    int detectLineWithScore(const QVector<QPoint> &points, QLine *line);

    int detectRectangleWithScore(const QVector<QPoint> &points, QRect *rect);

    void clearGestMatrix();

    void drawPointsInGestMatrix(const QVector<QPoint> &points, bool continuous = true);
private:
    HoughTransform m_ht;
    QVector<QPoint> m_gestPoints;
    quint8 **m_gestMt;
    QSize m_gestMtSize;
};

}
