#pragma once

#include "uiinteractor.h"

#include <shape.h>

#include <QPainter>
#include <QPoint>
#include <QVector>
#include <QWidget>
#include <QColor>

/**
 * @brief Widget for drawing gestures
 * Track mouse when left button pressed and draw lines
 */
class DrawableArea : public QWidget {
    Q_OBJECT
public:
    explicit DrawableArea(QWidget *parent = 0);

protected:
    /**
     * @brief Triggered when mouse moving with left button holded
     * @param event Contains information about mouse position
     */
    void mouseMoveEvent(QMouseEvent *event);
    /**
     * @brief Triggered when left mouse button pressed
     */
    void mousePressEvent(QMouseEvent *);
    /**
     * @brief Triggered when left mouse button released
     */
    void mouseReleaseEvent(QMouseEvent *);
    /**
     * @brief Triggered when need to redraw widget.
     * Called manually after the registration of each point
     * @param event Not used
     */
    void paintEvent(QPaintEvent *event);

    /**
     * @brief Remove all points from widget area and start new gesture
     */
    void startNewGestureCapture();
    /**
     * @brief Add new point to gesture
     * @param pt Point to be added
     */
    void addGesturePoint(const QPoint &pt);
    /**
     * @brief Stop registering new points for gesture
     * Emit signal gestureCaptured
     */
    void stopGestureCapture();

public slots:
    /**
     * @brief Set shape to draw on a widget
     * @param shape raw pointer to shape, function TAKE ownership over it, so it will be deleted
     * after start of new gesture capture.
     */
    void setShape(PShape shape);

signals:
    /**
     * @brief Emited when gesture registered and ready to further processing
     * @param points Vector of points in the order of drawing
     */
    void gestureCaptured(QVector<QPoint> points);

private:
    /**
     * @brief Draw points of user gesture on a widget
     * @param painter painter by means of which draw
     */
    void drawGesture(QPainter &painter);
    /**
     * @brief Draw internal shape on a widget.
     * Shape should be previously setuped via setShape function
     * @param painter painter by means of which draw
     */
    void drawShape(QPainter &painter);

    /// Points in the order of drawing
    QVector<QPoint> m_points;
    /// Recognized shape to draw
    PShape m_shape;
    /// True is we currently capturing gesture
    bool m_gestureStarted;
};
