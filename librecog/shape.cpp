#include "shape.h"
#include "common.h"

#include <QDebug>
#include <QPainter>
#include <QPoint>
#include <QString>

namespace recog {

iShape::~iShape() {}

SLine::SLine(const QPoint &pt1, const QPoint &pt2) : m_pt1(pt1), m_pt2(pt2) {}

void SLine::draw(QPainter &painter) const {
    painter.drawLine(m_pt1, m_pt2);
}

QString SLine::toString() const {
    char buf[100];
    qsnprintf(buf, sizeof(buf),
              "LINE from (%d, %d) to (%d, %d)",
              m_pt1.x(), m_pt1.y(), m_pt2.x(), m_pt2.y());
    return buf;
}



SRect::SRect(const QRect &rect) : m_rect(rect) {}

void SRect::draw(QPainter &painter) const {
    painter.drawRect(m_rect);
}

QString SRect::toString() const {
    char buf[100];
    qsnprintf(buf, sizeof(buf),
              "RECTANGLE upper left (%d, %d); bottom right (%d, %d)",
              m_rect.topLeft().x(), m_rect.topLeft().y(),
              m_rect.bottomRight().x(), m_rect.bottomRight().y());
    return buf;
}

SEllipse::SEllipse(const QPoint &center, qreal angle, quint16 a, quint16 b)
    : m_center(center), m_angle(angle), m_a(a), m_b(b) { }

void SEllipse::draw(QPainter &painter) const {
    QTransform wts;
    wts.translate(m_center.x(), m_center.y());
    wts.rotateRadians(m_angle);
    painter.setTransform(wts);
    painter.drawEllipse(QPoint(0, 0), m_a, m_b);
}

QString SEllipse::toString() const {
    char buf[100];
    qsnprintf(buf, sizeof(buf),
              "ELLIPSE center=(%d, %d); rotation angle=%.2f; a=%d; b=%d",
              m_center.x(), m_center.y(),
              recog::radToDeg(m_angle),
              m_a, m_b);
    return buf;
}

} // namespace recog
