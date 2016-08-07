#include "uiinteractor.h"

#include <recognition.h>
#include <shape.h>

#include <QVector>
#include <QScopedPointer>

UIInteractor::UIInteractor(QObject *parent) : QObject(parent) {}

void UIInteractor::processGesture(QVector<QPoint> points) {
    PShape shape(recog::createShapeFromPoints(points));
    emit newGestureRegistered(shape);
}
