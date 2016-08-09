#pragma once

#include <shape.h>
#include <recognition.h>

#include <QObject>
#include <QPoint>
#include <QVector>
#include <QImage>
#include <QSharedDataPointer>

typedef recog::iShape* PShape;

/**
 * @brief Class for interaction with GestureRecognition library from gui
 */
class UIInteractor : public QObject {
    Q_OBJECT
public:
    explicit UIInteractor(QObject *parent = 0);

signals:
    /**
     * @brief Emited when new gesture recognized. Contain shape, taken from gesture
     * @param shape raw pointer
     */
    void newGestureRegistered(PShape shape);

    void updateHoughTransformView(const QImage &image);

public slots:
    /**
     * @brief Recognize gesture from vector of points
     * @param points Vector of points in the order of drawing
     */
    void processGesture(QVector<QPoint> points);

private:
    recog::Recognizer m_rcg;
};
