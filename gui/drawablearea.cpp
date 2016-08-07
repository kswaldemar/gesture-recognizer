#include "drawablearea.h"

#include <shape.h>

#include <QDebug>
#include <QMouseEvent>
#include <QStyleOption>


/*
 * Some global variables
 */
const QColor GESTURE_COLOR = "blue";
const QColor SHAPE_COLOR = "red";
const int PEN_WIDTH = 2;


DrawableArea::DrawableArea(QWidget *parent) : QWidget(parent), m_shape(NULL) {}

void DrawableArea::mouseMoveEvent(QMouseEvent *event) {
    // Register points only inside widget
    bool isValidPoint = rect().contains(event->pos());
    if (isValidPoint && !m_points.empty()) {
        isValidPoint = m_points.back() != event->pos();
    }
    if (isValidPoint) {
        addGesturePoint(event->pos());
        update();
    }
}

void DrawableArea::mousePressEvent(QMouseEvent *) {
    startNewGestureCapture();
}

void DrawableArea::mouseReleaseEvent(QMouseEvent *) {
    stopGestureCapture();
}

void DrawableArea::drawGesture(QPainter &painter) {
    QVector<QPoint>::iterator it1 = m_points.begin();
    QVector<QPoint>::iterator it2 = it1;
    ++it2;
    while (it2 != m_points.end()) {
        painter.drawLine(*it1, *it2);
        it1 = it2;
        ++it2;
    }
}

void DrawableArea::drawShape(QPainter &painter) {
    m_shape->draw(painter);
}

void DrawableArea::paintEvent(QPaintEvent *) {
    QStyleOption opt;
    opt.init(this);

    QPainter painter(this);
    // For applying custom stylesheet
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    painter.setRenderHints(QPainter::Antialiasing, true);

    // Draw gesture
    painter.setPen(QPen(GESTURE_COLOR, PEN_WIDTH));
    if (!m_points.empty()) {
        drawGesture(painter);
    }

    // Draw shape
    painter.setPen(QPen(SHAPE_COLOR, PEN_WIDTH));
    if (m_shape) {
        drawShape(painter);
    }
}

void DrawableArea::startNewGestureCapture() {
    m_points.clear();
    delete m_shape;
    m_shape = NULL;
    m_gestureStarted = true;
    update();
    qDebug() << "Start new gesture capture";
}

void DrawableArea::addGesturePoint(const QPoint &pt) {
    bool validPoint = m_gestureStarted;
    if (validPoint) {
        m_points.append(pt);
    }
}

void DrawableArea::stopGestureCapture() {
    m_gestureStarted = false;
    qDebug() << "Stop gesture capture";
//    qDebug() << "Captured points: " << m_points;
    if (!m_points.empty()) {
        emit gestureCaptured(m_points);
    }
}

void DrawableArea::setShape(PShape shape) {
    m_shape = shape;
    update();
}
