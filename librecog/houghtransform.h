#pragma once

#include <QVector>
#include <QPoint>
#include <QLine>
#include <QImage>

namespace recog {

#define HOUGH_TH_DIM        (480)
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

    /**
     * @brief Perform line hough transformation over image.
     * It find lines, using the formula: x * cos(th) + y * sin(th) = r
     * After this call, internal hough matrix filled with apropriate values.
     *
     * @param matrix image representation, non zero values correspond pixel presence
     * @param mtSize size of matrix dimension, in form of matrix[width][height]
     * @return reference to *this for further chained calls
     */
    HoughTransform &lineHoughTransform(quint8 *matrix[], QSize mtSize);
    /**
     * @brief Convert [theta, radius] to [x, y] decart coords.
     * Respectively to image size, which was calculated from last hough transform call.
     * There should be previous call of lineHoughTransform, or result is invalid.
     * @param theta x coord in hough matrix
     * @param radius y coord in hough matrix
     * @return Line in decart coordinate system
     */
    QLine angleRadiusToLine(int theta, int radius) const;
    /**
     * @brief Get point with maximum value from hough matrix.
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
     * Image size same as hough matrix size (HOUGH_TH_DIM x HOUGH_R_DIM)
     * @return image with hough parametric field representation
     */
    QImage getImageFromHoughMatrix() const;

private:
    /// Image size, calculated from points in last hough transformation call
    QSize m_imSize;
    /// Hough parametric field
    HoughMatrix m_hmt;
    /// Precalculated table of sin function
    qreal m_sin[HOUGH_TH_DIM];
    /// Precalculated table of cos function
    qreal m_cos[HOUGH_TH_DIM];
};

}
