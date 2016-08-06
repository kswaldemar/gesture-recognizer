#include "recognition.h"

#include <QVector>
#include <QPoint>

namespace recog {

Gesture recognizePointSet(const QVector<QPoint> &points) {
    Gesture ret;
    ret.type = Gesture::TYPE_UNDEFINED;
    return ret;
}

}
