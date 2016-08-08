#include "uiinteractor.h"

#include <recognition.h>
#include <shape.h>

#include <QVector>
#include <QScopedPointer>

UIInteractor::UIInteractor(QObject *parent) : QObject(parent) {}

void UIInteractor::processGesture(QVector<QPoint> points) {
    recog::Recognizer rcg;
    rcg.loadGesture(points);
    emit newGestureRegistered(rcg.detectShape());
    emit updateHoughTransformView(rcg.houghTransformView());
}
