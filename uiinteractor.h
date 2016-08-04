#pragma once

#include <QObject>
#include <QPoint>
#include <QVector>

/**
 * @brief Class for interaction with GestureRecognition library from gui
 */
class UIInteractor : public QObject {
    Q_OBJECT
   public:
    explicit UIInteractor(QObject *parent = 0);

   signals:
    /**
     * @brief Emited when new gesture recognized
     * @param description Gesture description in form of string
     */
    void newGestureRegistered(QString description);

   public slots:
    /**
     * @brief Recognize gesture from vector of points
     * @param points Vector of points in the order of drawing
     */
    void processGesture(QVector<QPoint> points);
};
