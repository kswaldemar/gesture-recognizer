#include "recognition.h"
#include "common.h"
#include "houghtransform.h"
#include "shape.h"

#include <QDebug>
#include <QPainter>
#include <QPoint>
#include <QString>
#include <QVector>

void printTimeDiff(timespec t1, timespec t2) {
    static const quint64 E9 = 1e9;
    quint64 timePassed = ((t2.tv_sec * E9) + t2.tv_nsec) - ((t1.tv_sec * E9) + t1.tv_nsec);
    fprintf(stderr, "Time cosumed: %2llu.%08llu\n", timePassed / E9, timePassed % E9);
}

namespace recog {

void Recognizer::loadGesture(const QVector<QPoint> &points) {
    m_gestPoints = makeContinuous(points);

    m_gestMt.clear();
    QPoint imSize = maxSizeFromPointSet(points);
    m_gestMt.resize(imSize.x());
    for (int col = 0; col < imSize.x(); ++col) {
        m_gestMt[col].resize(imSize.y());
    }
}

iShape *Recognizer::detectShape() {
    timespec ts1, ts2;
    QLine line;
    clock_gettime(CLOCK_MONOTONIC, &ts1);
    int lineScore = detectLineWithScore(m_gestPoints, &line);
    clock_gettime(CLOCK_MONOTONIC, &ts2);
    qDebug() << "Line " << line << " with score " << lineScore;
    printTimeDiff(ts1, ts2);

    QRect rect;
    clock_gettime(CLOCK_MONOTONIC, &ts1);
    int rectScore = detectRectangleWithScore(m_gestPoints, &rect);
    clock_gettime(CLOCK_MONOTONIC, &ts2);
    qDebug() << "Rectangle " << rect << "with score" << rectScore;
    printTimeDiff(ts1, ts2);

    if (lineScore > rectScore) {
        return new SLine(line.p1(), line.p2());
    } else {
        return new SRect(rect);
    }
}

QImage Recognizer::houghTransformView() {
    return createHoughLineViewImage(m_gestPoints);
}

QImage Recognizer::createHoughLineViewImage(const QVector<QPoint> &points) {
    return m_ht.lineHoughTransform(points).getImageFromHoughMatrix();
}

QVector<QPoint> Recognizer::makeContinuous(const QVector<QPoint> &points) {
    QVector<QPoint> ret;
    QVector<QPoint>::const_iterator next = points.begin();
    ret.append(*next);

    QPoint shiftX(1, 0);
    QPoint shiftY(0, 1);
    QPoint curPt = *next;
    ++next;
    while (next != points.end()) {
        const QPoint dist = QPoint(*next - curPt);
        if (dist.manhattanLength() > 1) {
            if (qAbs(dist.x()) > qAbs(dist.y())) {
                curPt += shiftX * (dist.x() > 0 ? 1 : -1);
            } else {
                curPt += shiftY * (dist.y() > 0 ? 1 : -1);
            }
        } else {
            curPt = *next;
            ++next;
        }
        ret.append(curPt);
    }

    return ret;
}

int Recognizer::detectLineWithScore(const QVector<QPoint> &points, QLine *line) {
    m_ht.lineHoughTransform(points);
    int score = m_ht.getMaxValue();
    QPoint maxP = m_ht.getMaxValuePoint();
    if (line) {
        *line = m_ht.angleRadiusToLine(maxP.x(), maxP.y());
        cutLineWithBbox(*line, minSizeFromPointSet(points), maxSizeFromPointSet(points));
    }
    return score;
}

int Recognizer::detectRectangleWithScore(const QVector<QPoint> &points, QRect *rect) {
    return 0;
}

}
