#include "recognition.h"
#include "common.h"
#include "houghtransform.h"
#include "shape.h"

#include <QDebug>
#include <QPainter>
#include <QPoint>
#include <QString>
#include <QVector>

#include <cmath>

namespace recog {

static const quint8 RECTANGLE_DEVIATION_PX = 10;

Recognizer::Recognizer() {
    m_gestMt = new quint8* [IMG_MAX_WIDTH];
    for (int i = 0; i < IMG_MAX_WIDTH; ++i) {
        m_gestMt[i] = new quint8[IMG_MAX_HEIGHT];
    }
    m_gestMtSize = QSize(IMG_MAX_WIDTH, IMG_MAX_HEIGHT);
}

Recognizer::~Recognizer() {
    for (int i = 0; i < IMG_MAX_WIDTH; ++i) {
        delete[] m_gestMt[i];
    }
    delete[] m_gestMt;
}

void Recognizer::loadGesture(const QVector<QPoint> &points) {
    m_gestPoints = points;
    QPoint trick = maxSizeFromPointSet(points);
    m_gestMtSize.setHeight(trick.y() + 1);
    m_gestMtSize.setWidth(trick.x() + 1);
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

    if (lineScore >= rectScore) {
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
        .lineHoughTransform(m_gestMt, m_gestMtSize)
        .getImageFromHoughMatrix();
}

int Recognizer::detectLineWithScore(const QVector<QPoint> &points, QLine *line) {
    drawPointsInGestMatrix(points);
    m_ht.lineHoughTransform(m_gestMt, m_gestMtSize);
    int score = m_ht.getMaxValue();
    QPoint maxP = m_ht.getMaxValuePoint();
    if (line) {
        *line = m_ht.angleRadiusToLine(maxP.x(), maxP.y());
        cutLineWithBbox(*line, minSizeFromPointSet(points), maxSizeFromPointSet(points));
    }
    return score;
}

int Recognizer::detectRectangleWithScore(const QVector<QPoint> &points, QRect *rect) {
    drawPointsInGestMatrix(points);

    QLine lines[4];
    int scores[4] = {0};
    qreal angles[4] = {0};
    for (int i = 0; i < 4; ++i) {
        QPoint maxP = m_ht.lineHoughTransform(m_gestMt, m_gestMtSize).getMaxValuePoint();
        lines[i] = m_ht.angleRadiusToLine(maxP.x(), maxP.y());
        cutLineWithBbox(lines[i], QPoint(0, 0), QPoint(m_gestMtSize.width(), m_gestMtSize.height()));
        qDebug() << lines[i];
        scores[i] = m_ht.getMaxValue();
        angles[i] = radToDeg(maxP.x() * M_PI / HOUGH_TH_DIM);

        // Remove line from image
        drawLineInMatrix(m_gestMt, m_gestMtSize,
                         lines[i].p1(), lines[i].p2(),
                         0, RECTANGLE_DEVIATION_PX);
    }

    // Make sure this is rectangle
    const qreal DEVIATION = 5;
    for (int i = 0; i < 4; ++i) {
        if (angles[i] >= 180 - DEVIATION) {
            angles[i] = 180 - angles[i];
        }
    }
    qSort(angles, angles + 4);
    //Two angles near to zero and two near to 90
    const bool isSureRectangle =
               qAbs(angles[0])      <= DEVIATION && qAbs(angles[1])      <= DEVIATION
            && qAbs(angles[2] - 90) <= DEVIATION && qAbs(angles[3] - 90) <= DEVIATION;
    if (!isSureRectangle) {
        return 0;
    }


    // Try to intersect lines for points
    QVector<QPointF> vertex;
    QPointF pt;
    for (int i = 0; i < 4; ++i) {
        for (int j = i + 1; j < 4; ++j) {
            QLineF l1(lines[i]);
            QLineF l2(lines[j]);
            if (l1.intersect(l2, &pt) == QLineF::BoundedIntersection) {
                vertex.append(pt);
            }
        }
    }

    if (vertex.size() != 4) {
        // Not enough vertexes, this is not rectangle
        return 0;
    }

    int fullScore = 0;
    for (int i = 0; i < 4; ++i) {
        fullScore += scores[i];
    }

    if (rect) {
        QPoint ul(100000, 100000);
        QPoint br(-1, -1);
        foreach (const QPointF &pt, vertex) {
            ul.setX(qMin(ul.x(), static_cast<int>(pt.x())));
            ul.setY(qMin(ul.y(), static_cast<int>(pt.y())));
            br.setX(qMax(br.x(), static_cast<int>(pt.x())));
            br.setY(qMax(br.y(), static_cast<int>(pt.y())));
        }
        rect->setCoords(ul.x(), ul.y(), br.x(), br.y());
    }

    return fullScore;
}

void Recognizer::clearGestMatrix() {
    for (int x = 0; x < IMG_MAX_WIDTH; ++x) {
        for (int y = 0; y < IMG_MAX_HEIGHT; ++y) {
            m_gestMt[x][y] = 0;
        }
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
            drawLineInMatrix(m_gestMt, m_gestMtSize, *cur, *next, 1);
        }
        cur = next++;
    }
}

}
