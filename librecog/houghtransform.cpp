#include "houghtransform.h"
#include "common.h"

#include <QGenericMatrix>
#include <QLine>
#include <QPointF>

#include <cmath>

namespace recog {

void fillHoughMatrix(HoughMatrix mt, int val) {
    for (int th = 0; th < HOUGH_TH_DIM; ++th) {
        for (int r = 0; r < HOUGH_R_DIM; ++r) {
            mt[th][r] = val;
        }
    }
}

QLine translateToLine(int theta, int radius, const QPoint &imgSize) {
    // Normalize coordinates
    double th = theta;
    th = theta * M_PI / HOUGH_TH_DIM;
    th = radToDeg(th);
    double rMax = hypot(imgSize.x(), imgSize.y());
    double r = rMax - (radius * rMax) / (HOUGH_R_DIM / 2);

    qDebug() << "Normalized: " << th << " " << r;
    // Translate line to two point form
    th = degToRad(th);
    double a = cos(th);
    double b = sin(th);
    QPoint p0(static_cast<int>(r * a), static_cast<int>(r * b));

    const int LINE_LEN = 2000;
    QPoint shift(static_cast<int>(LINE_LEN * (-b)), static_cast<int>(LINE_LEN * a));

    return QLine(p0 + shift, p0 - shift);
}

void lineHoughTransform(const QVector<QPoint> &points, HoughMatrix houghMt) {
    fillHoughMatrix(houghMt, 0);

    QPoint imSize = maxSizeFromPointSet(points);

    const double rMax = hypot(imSize.x(), imSize.y());
    const double dth = M_PI / HOUGH_TH_DIM;

    double theta;
    double radius;
    const double halfR = HOUGH_R_DIM / 2;
    for (QVector<QPoint>::const_iterator it = points.begin(); it != points.end(); ++it) {
        for (int jth = 0; jth < HOUGH_TH_DIM; ++jth) {
            theta = jth * dth;
            radius = it->x() * cos(theta) + it->y() * sin(theta);
            int ir = static_cast<int>(halfR - ceil(radius * halfR / rMax));
            houghMt[jth][ir] += HOUGH_ONE_SCORE;
        }
    }
}

QPoint getMaxValuePoint(const HoughMatrix mt) {
    int mth = 0;
    int mr = 0;
    for (int th = 0; th < HOUGH_TH_DIM; ++th) {
        for (int r = 0; r < HOUGH_R_DIM; ++r) {
            if (mt[th][r] > mt[mth][mr]) {
                mth = th;
                mr = r;
            }
        }
    }
    return QPoint(mth, mr);
}

}

