#pragma once

#include <QVector>
#include <QPoint>
#include <QSize>
#include <QLine>

namespace recog {

QPoint maxSizeFromPointSet(const QVector<QPoint> &points);

QPoint minSizeFromPointSet(const QVector<QPoint> &points);

double radToDeg(double rad);

double degToRad(double deg);

void cutLineWithBbox(QLine &line, const QPoint &ul, const QPoint &br);

void printTimeDiff(timespec t1, timespec t2, const char *prefix="Time consumed");

/**
 * @brief Draw line in matrix.
 * Set all points belongs to line to `val`. If some points outside matrix its silently skipped.
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

bool isValidPoint(const QPoint &pt, const QSize &size);

}
