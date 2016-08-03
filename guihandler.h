#ifndef GUIHANDLER_H
#define GUIHANDLER_H

#include <QObject>
#include <QVector>
#include <QPoint>

class GuiHandler : public QObject
{
    Q_OBJECT
public:
    explicit GuiHandler(QObject *parent = 0);

signals:
    void newGestureRegistered(QString);

public slots:
    void processGesture(QVector<QPoint>);
};

#endif // GUIHANDLER_H
