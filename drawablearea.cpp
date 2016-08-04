#include "drawablearea.h"

#include <QDebug>
#include <QMouseEvent>
#include <QStyleOption>

DrawableArea::DrawableArea(QWidget *parent) : QWidget(parent) {}

void DrawableArea::mouseMoveEvent(QMouseEvent *event) {
    // Register points only inside widget
    if (this->rect().contains(event->pos())) {
        addGesturePoint(event->pos());
        update();
        qDebug() << event->pos();
    }
}

void DrawableArea::mousePressEvent(QMouseEvent *) {
    startNewGestureCapture();
}

void DrawableArea::mouseReleaseEvent(QMouseEvent *) {
    stopGestureCapture();
}

void DrawableArea::paintEvent(QPaintEvent *) {
    QStyleOption opt;
    opt.init(this);

    QColor color("blue");
    QPen pen(color, 2);
    QPainter painter(this);
    painter.setPen(pen);
    painter.setRenderHints(QPainter::Antialiasing, true);

    // For applying custom stylesheet
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    if (!m_points.empty()) {
        QVector<QPoint>::iterator it1 = m_points.begin();
        QVector<QPoint>::iterator it2 = it1;
        ++it2;
        while (it2 != m_points.end()) {
            painter.drawLine(*it1, *it2);
            it1 = it2;
            ++it2;
        }
    }
}

void DrawableArea::startNewGestureCapture() {
    m_points.clear();
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
    if (!m_points.empty()) {
        emit gestureCaptured(m_points);
    }
}
