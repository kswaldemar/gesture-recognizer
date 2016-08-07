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
    for (QVector<QPoint>::const_iterator it = points.begin(); it != points.end(); ++it) {
        mX = qMax(mX, it->x());
        mY = qMax(mY, it->y());
    }
    return QPoint(mX, mY);
}

QPoint minSizeFromPointSet(const QVector<QPoint> &points) {
    int mX = 1e9;
    int mY = 1e9;
    for (QVector<QPoint>::const_iterator it = points.begin(); it != points.end(); ++it) {
        mX = qMin(mX, it->x());
        mY = qMin(mY, it->y());
    }
    return QPoint(mX, mY);
}

void cutLineWithBbox(QLine &line, const QPoint &ul, const QPoint &br) {

}

}
