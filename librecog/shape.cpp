#include "shape.h"

#include <QPainter>
#include <QString>
#include <QPoint>
#include <QDebug>

namespace recog {

iShape::~iShape() { }

SLine::SLine(const QPoint &pt1, const QPoint &pt2)
    : m_pt1(pt1), m_pt2(pt2) { }

void SLine::draw(QPainter &painter) const {
    painter.drawLine(m_pt1, m_pt2);
}

QString SLine::toString() const {
    char buf[100];
    snprintf(buf, sizeof(buf),
             "LINE from (%d, %d) to (%d, %d)",
             m_pt1.x(), m_pt1.y(), m_pt2.x(),m_pt2.y());
    return buf;
}


}
