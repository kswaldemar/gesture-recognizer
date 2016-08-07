#include "common.h"

#include <cmath>

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

}
