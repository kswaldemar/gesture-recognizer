#pragma once

#include <QVector>
#include <QPoint>
#include <QSize>
#include <QLine>

namespace recog {

/**
 * @brief Return point, constructed from maximum values between points coordinates
 * @param points vector of points from which calculate value
 * @return point in form (max(x), max(y))
 */
QPoint maxSizeFromPointSet(const QVector<QPoint> &points);

/**
 * @brief Return point, constructed from minimum values between points coordinates
 * @param points vector of points from which calculate value
 * @return point in form (min(x), min(y))
 */
QPoint minSizeFromPointSet(const QVector<QPoint> &points);

/**
 * @brief Convert from radian to degrees
 * @param rad angle value in radian
 * @return angle value in degrees
 */
double radToDeg(double rad);

/**
 * @brief Convert from degrees to radian
 * @param deg angle value in degrees
 * @return angle value in radian
 */
double degToRad(double deg);

/**
 * @brief Fit line in bounding box
 * @param line[inout] line to fit. Pass by reference, so value will be changed
 * @param ul upper left point of bounding box
 * @param br bottom right point of bounding box
 */
void cutLineWithBbox(QLine &line, const QPoint &ul, const QPoint &br);

/**
 * @brief Caclulate difference between two timespec and print it into stderr
 * @param t1 first timespec, to substract from
 * @param t2 second timespec
 * @param prefix log prefix, to print before time (default string "Time consumed")
 */
void printTimeDiff(timespec t1, timespec t2, const char *prefix="Time consumed");

/**
 * @brief Draw line in matrix.
 * Set all points belongs to line to `val`. Each points not belonging to matrix is ignored.
 * @param mt matrix of values in form of (x, y) array
 * @param mtSize maximum size of matrix
 * @param pt1 "from" point
 * @param pt2 "to" point
 * @param val which value set to matrix cell
 * @param extraWidth extra area of pixels to fill around each point (default 0)
 */
void drawLineInMatrix(quint8 **mt,
                      const QSize &mtSize,
                      const QPoint &pt1,
                      const QPoint &pt2,
                      quint8 val,
                      int extraWidth = 0);
/**
 * @brief Check that point coordinates in range [0, size.width) and [0, size.height)
 * @param pt point to check
 * @param size upper limit
 * @return true if point in range, false otherwise
 */
bool isValidPoint(const QPoint &pt, const QSize &size);

template<typename T>
inline T sqr(T val) {
    return val * val;
}

}
