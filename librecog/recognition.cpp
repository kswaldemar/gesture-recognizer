#include "recognition.h"
#include "houghtransform.h"
#include "common.h"
#include "shape.h"

#include <QDebug>
#include <QPainter>
#include <QPoint>
#include <QString>
#include <QVector>

namespace recog {

int extractLineWithScore(const QVector<QPoint> &points, QLine *line) {
    HoughMatrix mt;
    lineHoughTransform(points, mt);
    QPoint maxP = getMaxValuePoint(mt);
    int score = mt[maxP.x()][maxP.y()];
    if (line) {
        QPoint imSize = maxSizeFromPointSet(points);
        *line = translateToLine(maxP.x(), maxP.y(), imSize);
        cutLineWithBbox(*line, minSizeFromPointSet(points), imSize);
    }
    return score;
}

iShape *createShapeFromPoints(const QVector<QPoint> &points) {
    QVector<QPoint> masterPoints = makeContinious(points);
    // Here check probabilities and take the big one
    QLine line;
    int lineScore = extractLineWithScore(masterPoints, &line);
    qDebug() << "Line " << line << " with score " << lineScore;
    return new SLine(line.p1(), line.p2());
}

QVector<QPoint> makeContinious(const QVector<QPoint> &points) {
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

QImage createHoughLineViewImage(const QVector<QPoint> &points) {
    HoughMatrix mt;
    lineHoughTransform(points, mt);
    return getImageFromHoughMatrix(mt);
}

}
