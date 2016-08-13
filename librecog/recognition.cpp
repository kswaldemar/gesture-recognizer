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

/// Maximum point deviation from rectangle line in pixels
static const quint8 RECTANGLE_DEVIATION_PX = 4;
/// Maximum deviation from 90 and 0 degrees for rectangle lines
const qreal RECTANGLE_ANGLE_DEVIATION = 5;

/// Maximum point deviation from ellipse border in pixels
static const qreal ELLIPSE_DEVIATION_PX = 4;

Recognizer::Recognizer() {
    m_gestMt = new quint8 *[IMG_MAX_WIDTH];
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

    const int SHAPES_COUNT = 3;

    int scores[SHAPES_COUNT] = {0};
    iShape *shapes[SHAPES_COUNT] = {NULL};

    clock_gettime(CLOCK_MONOTONIC, &ts1);
    scores[0] = detectLineWithScore(m_gestPoints, shapes[0]);
    clock_gettime(CLOCK_MONOTONIC, &ts2);
    printTimeDiff(ts1, ts2);

    clock_gettime(CLOCK_MONOTONIC, &ts1);
    scores[1] = detectRectangleWithScore(m_gestPoints, shapes[1]);
    clock_gettime(CLOCK_MONOTONIC, &ts2);
    printTimeDiff(ts1, ts2);

    clock_gettime(CLOCK_MONOTONIC, &ts1);
    scores[2] = detectEllipseWithScore(m_gestPoints, shapes[2]);
    clock_gettime(CLOCK_MONOTONIC, &ts2);
    printTimeDiff(ts1, ts2);

    int maxInd = 0;
    for (int i = 0; i < SHAPES_COUNT; ++i) {
        qDebug().nospace() << "Shape(" << i << ") score " << scores[i]
                           << ". Description: " << (shapes[i] ? shapes[i]->toString() : "None");
        if (scores[i] > scores[maxInd]) {
            maxInd = i;
        }
    }

    // Free non used shapes
    for (int i = 0; i < SHAPES_COUNT; ++i) {
        if (i != maxInd && shapes[i]) {
            delete shapes[i];
        }
    }

    return shapes[maxInd];
}

QImage Recognizer::houghTransformView() {
    return createHoughLineViewImage(m_gestPoints);
}

QImage Recognizer::createHoughLineViewImage(const QVector<QPoint> &points) {
    drawPointsInGestMatrix(points);
    return m_ht.lineHoughTransform(m_gestMt, m_gestMtSize).getImageFromHoughMatrix();
}

int Recognizer::detectLineWithScore(const QVector<QPoint> &points, iShape *&sLine) {
    drawPointsInGestMatrix(points);
    m_ht.lineHoughTransform(m_gestMt, m_gestMtSize);
    int score = m_ht.getMaxValue();
    QPoint maxP = m_ht.getMaxValuePoint();

    QLine line;
    line = m_ht.angleRadiusToLine(maxP.x(), maxP.y());
    cutLineWithBbox(line, minSizeFromPointSet(points), maxSizeFromPointSet(points));
    sLine = new SLine(line.p1(), line.p2());

    return score;
}

int Recognizer::detectRectangleWithScore(const QVector<QPoint> &points, iShape *&sRect) {
    drawPointsInGestMatrix(points);

    QLine lines[4];
    int scores[4] = {0};
    qreal angles[4] = {0};
    for (int i = 0; i < 4; ++i) {
        QPoint maxP = m_ht.lineHoughTransform(m_gestMt, m_gestMtSize).getMaxValuePoint();
        lines[i] = m_ht.angleRadiusToLine(maxP.x(), maxP.y());
        cutLineWithBbox(lines[i], QPoint(0, 0),
                        QPoint(m_gestMtSize.width(), m_gestMtSize.height()));
        qDebug() << lines[i];
        scores[i] = m_ht.getMaxValue();
        angles[i] = radToDeg(maxP.x() * M_PI / HOUGH_TH_DIM);

        // Remove line from image
        drawLineInMatrix(m_gestMt, QSize(IMG_MAX_WIDTH, IMG_MAX_HEIGHT),
                         lines[i].p1(), lines[i].p2(),
                         0, RECTANGLE_DEVIATION_PX);
    }

    // Make sure this is rectangle
    for (int i = 0; i < 4; ++i) {
        if (angles[i] >= 180 - RECTANGLE_ANGLE_DEVIATION) {
            angles[i] = 180 - angles[i];
        }
    }
    qSort(angles, angles + 4);
    // Two angles near to zero and two near to 90
    const bool isSureRectangle = qAbs(angles[0]) <= RECTANGLE_ANGLE_DEVIATION &&
                                 qAbs(angles[1]) <= RECTANGLE_ANGLE_DEVIATION &&
                                 qAbs(angles[2] - 90) <= RECTANGLE_ANGLE_DEVIATION &&
                                 qAbs(angles[3] - 90) <= RECTANGLE_ANGLE_DEVIATION;
    if (!isSureRectangle) {
        qDebug() << "Rectanlge fail: I'm not sure this is rectangle";
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
        qDebug() << "Rectanlge fail: Found only" << vertex.size() << "vertexes";
        return 0;
    }

    int fullScore = 0;
    for (int i = 0; i < 4; ++i) {
        fullScore += scores[i];
    }

    // Create shape
    QRect rect;
    QPoint ul(100000, 100000);
    QPoint br(-1, -1);
    foreach (const QPointF &pt, vertex) {
        ul.setX(qMin(ul.x(), static_cast<int>(pt.x())));
        ul.setY(qMin(ul.y(), static_cast<int>(pt.y())));
        br.setX(qMax(br.x(), static_cast<int>(pt.x())));
        br.setY(qMax(br.y(), static_cast<int>(pt.y())));
    }

    // If difference <= deviation this might be not rectangle
    if ((br.x() - ul.x()) <= RECTANGLE_DEVIATION_PX ||
        (br.y() - ul.y()) <= RECTANGLE_DEVIATION_PX) {
        qDebug() << "Rectangle fail: Its very small relative to deviation";
        return 0;
    }

    rect.setCoords(ul.x(), ul.y(), br.x(), br.y());
    sRect = new SRect(rect);

    return fullScore;
}

int Recognizer::detectEllipseWithScore(const QVector<QPoint> &points, iShape *&sEllips) {
    // Spatial moments
    qreal m00 = 0, m10 = 0, m20 = 0, m01 = 0, m02 = 0, m11 = 0;

    clearGestMatrix();
    drawPointsInGestMatrix(points, true);

    // Calculate moments with horizontal intersector line from up to down
    QPoint minSz = minSizeFromPointSet(points);
    QPoint maxSz = maxSizeFromPointSet(points);
    for (int y = minSz.y(); y <= maxSz.y(); ++y) {
        int firstX = -1;
        int lastX = -1;
        for (int x = minSz.x() - 1; x <= maxSz.x(); ++x) {
            if (m_gestMt[x][y]) {
                if (firstX < 0) {
                    firstX = x;
                } else {
                    lastX = x;
                }
            }
        }
        if (lastX == -1) {
            continue;
        }
        // Like through outer contour
        for (int x = firstX; x <= lastX; ++x) {
            m00 += 1;
            m10 += x;
            m01 += y;
            m11 += x * y;
            m20 += x * x;
            m02 += y * y;
        }
    }

    if (m00 == 0) {
        return 0;
    }

    // Mass center
    QPointF mc(m10 / m00, m01 / m00);

    // Central moments
    qreal u11 = m11 / m00 - mc.x() * mc.y();
    qreal u20 = m20 / m00 - mc.x() * mc.x();
    qreal u02 = m02 / m00 - mc.y() * mc.y();

    // Rotation angle
    qreal angle = 0.5 * atan(2.0 * u11 / (u20 - u02));

    // Half axes
    qreal du = u20 - u02;
    qreal l1 = sqrt((u20 + u02 + sqrt(4 * u11 * u11 + du * du)) / 2.0);
    qreal l2 = sqrt((u20 + u02 - sqrt(4 * u11 * u11 + du * du)) / 2.0);
    if (u20 < u02) {
        std::swap(l1, l2);
    }

    sEllips = new SEllipse(QPoint(mc.x(), mc.y()), angle, 2.0 * l1, 2.0 * l2);

    int score = 0;
    l2 *= 2;
    l1 *= 2;
    const qreal cosa = cos(angle);
    const qreal sina = sin(angle);
    const qreal deviation = ELLIPSE_DEVIATION_PX / sqrt(l1 * l1 + l2 * l2);
    for (int x = 0; x < m_gestMtSize.width(); ++x) {
        for (int y = 0; y < m_gestMtSize.height(); ++y) {
            if (m_gestMt[x][y] > 0) {
                qreal dx = x - mc.x();
                qreal dy = y - mc.y();
                qreal t1 = sqr((cosa * dx + sina * dy) / l1);
                qreal t2 = sqr((sina * dx - cosa * dy) / l2);

                qreal ellipse = t1 + t2;
                score +=   (ellipse <= 1.0 + deviation)
                        && (ellipse >= 1.0 - deviation);
            }
        }
    }

    return score;
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
