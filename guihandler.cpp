#include "guihandler.h"

#include <QVector>

GuiHandler::GuiHandler(QObject *parent) : QObject(parent)
{

}

void GuiHandler::processGesture(QVector<QPoint> points)
{
    emit newGestureRegistered(QString("Gesture ") + QString::number(points.size()));
}
