#include "uiinteractor.h"

#include <recognition.h>
#include <shape.h>
#include <common.h>

#include <QVector>
#include <QScopedPointer>

UIInteractor::UIInteractor(QObject *parent) : QObject(parent) {}

void UIInteractor::processGesture(QVector<QPoint> points) {
    timespec t1, t2;

    clock_gettime(CLOCK_MONOTONIC, &t1);
    m_rcg.loadGesture(points);
    emit newGestureRegistered(m_rcg.detectShape());
    clock_gettime(CLOCK_MONOTONIC, &t2);
    recog::printTimeDiff(t1, t2, "processGesture");

    clock_gettime(CLOCK_MONOTONIC, &t1);
    emit updateHoughTransformView(m_rcg.houghTransformView());
    clock_gettime(CLOCK_MONOTONIC, &t2);
    recog::printTimeDiff(t1, t2, "updateHoughTransformView");
}
