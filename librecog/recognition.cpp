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

Recognizer::Recognizer() {
    m_gestMt = new quint8* [IMG_MAX_WIDTH];
    for (int i = 0; i < IMG_MAX_WIDTH; ++i) {
        m_gestMt[i] = new quint8[IMG_MAX_HEIGHT];
    }
}

Recognizer::~Recognizer() {
    for (int i = 0; i < IMG_MAX_WIDTH; ++i) {
        delete[] m_gestMt[i];
    }
    delete[] m_gestMt;
}

void Recognizer::loadGesture(const QVector<QPoint> &points) {
    m_gestPoints = points;
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
    drawPointsInGestMatrix(points);
    return m_ht
        .lineHoughTransform(m_gestMt, QSize(IMG_MAX_WIDTH, IMG_MAX_HEIGHT))
        .getImageFromHoughMatrix();
}

int Recognizer::detectLineWithScore(const QVector<QPoint> &points, QLine *line) {
    drawPointsInGestMatrix(points);
    m_ht.lineHoughTransform(m_gestMt, QSize(IMG_MAX_WIDTH, IMG_MAX_HEIGHT));
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

void Recognizer::clearGestMatrix() {
    for (int x = 0; x < IMG_MAX_WIDTH; ++x) {
        memset(m_gestMt[x], 0, sizeof(m_gestMt[0][0]) * IMG_MAX_HEIGHT);
    }
}

void Recognizer::drawPointsInGestMatrix(const QVector<QPoint> &points, bool continuous) {
    if (points.empty()) {
        return;
    }

    clearGestMatrix();

    QVector<QPoint>::ConstIterator next = points.begin();
    QVector<QPoint>::ConstIterator cur = next++;
    bool isValidPoint;
    while (cur != points.end()) {
        isValidPoint = (   cur->x() >= 0 && cur->x() < IMG_MAX_WIDTH
                        && cur->y() >= 0 && cur->y() < IMG_MAX_HEIGHT);
        if (isValidPoint) {
            m_gestMt[cur->x()][cur->y()] = 1;
        }
        if (continuous && next != points.end()) {
            drawLineInMatrix(m_gestMt, QSize(IMG_MAX_WIDTH, IMG_MAX_HEIGHT), *cur, *next, 1);
        }
        cur = next++;
    }

}

}
