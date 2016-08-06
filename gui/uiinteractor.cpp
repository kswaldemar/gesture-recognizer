#include "uiinteractor.h"

#include <QVector>

UIInteractor::UIInteractor(QObject *parent) : QObject(parent) {}

void UIInteractor::processGesture(QVector<QPoint> points) {
    emit newGestureRegistered(QString("Gesture ") + QString::number(points.size()));
}
