#include "common.h"

#include <cmath>
#include <cstdio>

namespace recog {

double radToDeg(double rad) {
    return rad * 180.0 / M_PI;
}

double degToRad(double deg) {
    return deg * M_PI / 180.0;
}

QPoint maxSizeFromPointSet(const QVector<QPoint> &points) {
    int mX = -1;
    int mY = -1;
    for (QVector<QPoint>::ConstIterator it = points.begin(); it != points.end(); ++it) {
        mX = qMax(mX, it->x());
        mY = qMax(mY, it->y());
    }
    return QPoint(mX, mY);
}

QPoint minSizeFromPointSet(const QVector<QPoint> &points) {
    int mX = 1e9;
    int mY = 1e9;
    for (QVector<QPoint>::ConstIterator it = points.begin(); it != points.end(); ++it) {
        mX = qMin(mX, it->x());
        mY = qMin(mY, it->y());
    }
    return QPoint(mX, mY);
}

void cutLineWithBbox(QLine &line, const QPoint &ul, const QPoint &br) {
    QLineF fLine(line);

    bool isLineFromLeftToRight = line.p1().x() < line.p2().x();
    bool isLineBottomToUp = line.p1().y() > line.p2().y();

    QPointF ur(br.x(), ul.y());
    QPointF bl(ul.x(), br.y());

    QLineF left(ul, bl);
    QLineF right(ur, br);
    QLineF top(ul, ur);
    QLineF bottom(bl, br);

    QPointF pt;
    if (fLine.intersect(left, &pt) == QLineF::BoundedIntersection) {
        isLineFromLeftToRight ? fLine.setP1(pt)
                              : fLine.setP2(pt);
    }

    if (fLine.intersect(bottom, &pt) == QLineF::BoundedIntersection) {
        isLineBottomToUp ? fLine.setP1(pt)
                         : fLine.setP2(pt);
    }

    if (fLine.intersect(right, &pt) == QLineF::BoundedIntersection) {
        isLineFromLeftToRight ? fLine.setP2(pt)
                              : fLine.setP1(pt);
    }

    if (fLine.intersect(top, &pt) == QLineF::BoundedIntersection) {
        isLineBottomToUp ? fLine.setP2(pt)
                         : fLine.setP1(pt);
    }

    line.setLine(static_cast<int>(fLine.p1().x()), static_cast<int>(fLine.p1().y()),
                 static_cast<int>(fLine.p2().x()), static_cast<int>(fLine.p2().y()));
}

void printTimeDiff(timespec t1, timespec t2, const char *prefix) {
    static const quint64 E9 = 1e9;
    quint64 timePassed = ((t2.tv_sec * E9) + t2.tv_nsec) - ((t1.tv_sec * E9) + t1.tv_nsec);
    fprintf(stderr, "%s: %2llu.%08llu\n", prefix, timePassed / E9, timePassed % E9);
}

void drawLineInMatrix(quint8 **mt,
                      const QSize &mtSize,
                      const QPoint &pt1,
                      const QPoint &pt2,
                      int val) {
    QPoint shiftX(1, 0);
    QPoint shiftY(0, 1);
    QPoint curPt = pt1;
    QPoint endPt = pt2;

    while (curPt != endPt) {
        const QPoint dist = QPoint(endPt - curPt);
        if (dist.manhattanLength() > 1) {
            if (qAbs(dist.x()) > qAbs(dist.y())) {
                curPt += shiftX * (dist.x() > 0 ? 1 : -1);
            } else {
                curPt += shiftY * (dist.y() > 0 ? 1 : -1);
            }
        } else {
            curPt = endPt;
        }

        //Set point in matrix, if it valid
        if (   curPt.x() >= 0 && curPt.x() < mtSize.width()
            && curPt.y() >= 0 && curPt.y() < mtSize.height()) {

            mt[curPt.x()][curPt.y()] = val;
        }
    }
}



}
