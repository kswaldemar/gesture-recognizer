#include "recognition.h"
#include "common.h"
#include "houghtransform.h"
#include "shape.h"

#include <QDebug>
#include <QPainter>
#include <QPoint>
#include <QString>
#include <QVector>

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
    QLine line;
    int lineScore = detectLineWithScore(m_gestPoints, &line);
    qDebug() << "Line " << line << " with score " << lineScore;

    QRect rect;
    int rectScore = detectRectangleWithScore(m_gestPoints, &rect);
    qDebug() << "Rectangle " << rect << "with score" << rectScore;

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
