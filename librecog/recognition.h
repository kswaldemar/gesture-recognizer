#pragma once

#include "houghtransform.h"

#include <QVector>
#include <QPoint>
#include <QImage>

namespace recog {

class iShape;

/**
 * @brief Class for gesture recognition operations
 */
class Recognizer {
    /// Maximum possible image width
    static const int IMG_MAX_WIDTH = 2000;
    /// Maximum possible image height
    static const int IMG_MAX_HEIGHT = 2000;
public:

    Recognizer();

    ~Recognizer();

    /**
     * @brief Load gesture points
     * @param points vector of points in the order of drawing
     */
    void loadGesture(const QVector<QPoint> &points);

    /**
     * @brief Recognize shape from loaded points.
     * Points should be loaded first by loadGesture call.
     * @return pointer to Shape. Pointer allocated in heap and should be manually freed by caller
     */
    iShape* detectShape();

    /**
     * @brief Get hough transformation matrix in view of image.
     * This is usefull, when hough transformation method used to recognize gesture - for
     * line and rectangle.
     * @return Image with hough transformation representation, with brightness adjustment.
     */
    QImage houghTransformView();

private:
    /**
     * @brief Perform hough transformation to find lines and return matrix image representation.
     * @param points vector of points to work with
     * @return Image with hough transformation representation
     */
    QImage createHoughLineViewImage(const QVector<QPoint> &points);

    /**
     * @brief Detect line shape among points and get it score
     * @param points vector of points to work with
     * @param line[out] pointer to line, which fill with values
     * @return Number of points belonging to line
     */
    int detectLineWithScore(const QVector<QPoint> &points, iShape *&sLine);

    /**
     * @brief Detect rectangle shape among points and get it score
     * @param points vector of points to work with
     * @param rect[out] pointer to rectangle, which fill with values
     * @return Number of points belonging to rectangle, or zero if no rectangle found
     */
    int detectRectangleWithScore(const QVector<QPoint> &points, iShape *&sRect);

    int detectEllipseWithScore(const QVector<QPoint> &points, iShape *&sEllips);
    
    /**
     * @brief Fill internal gesture matrix with zeroes
     */
    void clearGestMatrix();

    /**
     * @brief Draw points in internal gesture matrix
     * @param points vector of points to draw
     * @param continuous if set, fill distance between points (default = true)
     */
    void drawPointsInGestMatrix(const QVector<QPoint> &points, bool continuous = true);

private:
    /// Hough transform object, to perform any hough related operations
    HoughTransform m_ht;
    /// Loaded gesture points
    QVector<QPoint> m_gestPoints;
    /// Gesture matrix, filled with 1 and 0 to
    quint8 **m_gestMt;
    /// Size of gesture matrix (calculated from gesture points)
    QSize m_gestMtSize;
};

}
