#include "houghtransform.h"
#include "common.h"

#include <QGenericMatrix>
#include <QLine>
#include <QPixmap>
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

HoughTransform::HoughTransform() {
    fillHoughMatrix(m_hmt, 0);
}

HoughTransform &HoughTransform::lineHoughTransform(const QVector<QPoint> &points) {
    fillHoughMatrix(m_hmt, 0);

    m_imSize = maxSizeFromPointSet(points);

    const double rMax = hypot(m_imSize.x(), m_imSize.y());
    const double dth = M_PI / HOUGH_TH_DIM;

    /*
     * sin, cos tables precalc
     */
    static qreal prCos[HOUGH_TH_DIM];
    static qreal prSin[HOUGH_TH_DIM];
    static bool called = false;
    if (!called) {
        called = true;
        for (int i = 0; i < HOUGH_TH_DIM; ++i) {
            prCos[i] = cos(i * dth);
            prSin[i] = sin(i * dth);
        }
    }

    double radius;
    const double halfR = HOUGH_R_DIM / 2;
    for (QVector<QPoint>::const_iterator it = points.begin(); it != points.end(); ++it) {
        for (int jth = 0; jth < HOUGH_TH_DIM; ++jth) {
            radius = it->x() * prCos[jth] + it->y() * prSin[jth];
            int ir = static_cast<int>(halfR - ceil(radius * halfR / rMax));
            m_hmt[jth][ir] += HOUGH_ONE_SCORE;
        }
    }

    return *this;
}

QLine HoughTransform::angleRadiusToLine(int theta, int radius) const {
    // Normalize coordinates
    double th = theta;
    th = theta * M_PI / HOUGH_TH_DIM;
    th = radToDeg(th);
    double rMax = hypot(m_imSize.x(), m_imSize.y());
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

QPoint HoughTransform::getMaxValuePoint() const {
    int mth = 0;
    int mr = 0;
    for (int th = 0; th < HOUGH_TH_DIM; ++th) {
        for (int r = 0; r < HOUGH_R_DIM; ++r) {
            if (m_hmt[th][r] > m_hmt[mth][mr]) {
                mth = th;
                mr = r;
            }
        }
    }
    return QPoint(mth, mr);
}

qint32 HoughTransform::getMaxValue() const {
    QPoint mvp = getMaxValuePoint();
    return m_hmt[mvp.x()][mvp.y()];
}

QImage HoughTransform::getImageFromHoughMatrix() const {
    QImage img(HOUGH_TH_DIM, HOUGH_R_DIM, QImage::Format_RGB32);

    QPoint mPt = getMaxValuePoint();
    qint32 maxVal = m_hmt[mPt.x()][mPt.y()];
    qreal scale = static_cast<double>(255.0) / maxVal;

    for (int th = 0; th < HOUGH_TH_DIM; ++th) {
        for (int r = 0; r < HOUGH_R_DIM; ++r) {
            uint exVal = static_cast<uint>(m_hmt[th][r] * scale);
            uint rgb = (0xff & exVal) | (0xff00 & (exVal << 8)) | (0xff0000 & (exVal << 16));
            img.setPixel(th, r, rgb);
        }
    }
    img.invertPixels(QImage::InvertRgb);
    return img;
}
}  // namespace recog
