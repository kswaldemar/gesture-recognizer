#pragma once

#include <QVector>
#include <QPoint>
#include <QLine>


namespace recog {

QPoint maxSizeFromPointSet(const QVector<QPoint> &points);

QPoint minSizeFromPointSet(const QVector<QPoint> &points);

double radToDeg(double rad);

double degToRad(double deg);

void cutLineWithBbox(QLine &line, const QPoint &ul, const QPoint &br);

}
