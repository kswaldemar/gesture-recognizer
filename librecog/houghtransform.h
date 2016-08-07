#pragma once

#include <QVector>
#include <QPoint>
#include <QLine>
#include <QImage>

namespace recog {

#define HOUGH_TH_DIM        (480)
#define HOUGH_R_DIM         (360)
#define HOUGH_ONE_SCORE     (1)

typedef int HoughMatrix[HOUGH_TH_DIM][HOUGH_R_DIM];


void lineHoughTransform(const QVector<QPoint> &points, HoughMatrix houghMt);

QLine translateToLine(int theta, int radius, const QPoint &imgSize);

QPoint getMaxValuePoint(const HoughMatrix mt);

QImage getImageFromHoughMatrix(const HoughMatrix mt);

}
