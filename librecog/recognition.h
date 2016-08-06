#pragma once

#include <QVector>
#include <QPoint>

namespace recog {

struct Gesture {
    enum GestureType {
        TYPE_LINE,
        TYPE_UNDEFINED
    };
    GestureType type;
};

Gesture recognizePointSet(const QVector<QPoint> &points);

}
