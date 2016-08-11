#pragma once

#include <QVector>
#include <QPoint>
#include <QLine>
#include <QImage>

namespace recog {

#define HOUGH_TH_DIM        (720)
#define HOUGH_R_DIM         (2048)
#define HOUGH_ONE_SCORE     (1)

typedef quint16** HoughMatrix;
/**
 * @brief Class to perform hough transformation with various formulas
 */
class HoughTransform {
public:

    HoughTransform();

    ~HoughTransform();

    HoughTransform &lineHoughTransform(quint8 *matrix[], QSize mtSize);
    /**
     * @brief Convert [theta, radius] to [x, y] decart coords.
     * Respectively to image size, which was calculated from last hough transform call.
     * If no one of hough transformation called result is invalid
     * @param theta
     * @param radius
     * @return
     */
    QLine angleRadiusToLine(int theta, int radius) const;
    /**
     * @brief Get point with maximum value from hough matrix.
     * Should be called after any hough transformation calls
     * @return point in hough matrix in form of (theta, radius)
     */
    QPoint getMaxValuePoint() const;
    /**
     * @brief Get value from most probably point in hough parametric field (matrix)
     * @return maximum value from hough matrix
     */
    qint32 getMaxValue() const;
    /**
     * @brief Create image with graphical representation of the hough matrix.
     * Image has white background, so darker pixel represents bigger values.
     * Image brightness adjusted, so full black pixel correspond most big value in matrix.
     * Dimension 1:1 with hough matrix (HOUGH_TH_DIM x HOUGH_R_DIM)
     * @return image with hough parametric field representation
     */
    QImage getImageFromHoughMatrix() const;

private:
    /// Image size, calculated from points in last hough transformation call
    QSize m_imSize;
    /// Hough parametric field
    HoughMatrix m_hmt;
    /// Sin table precalculated
    qreal m_sin[HOUGH_TH_DIM];
    /// Cos table precalculated
    qreal m_cos[HOUGH_TH_DIM];
};

}
