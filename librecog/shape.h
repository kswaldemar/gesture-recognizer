#pragma once

#include <QPoint>
#include <QRect>

class QPainter;
class QString;

namespace recog {

class iShape {
public:
    virtual ~iShape();
    virtual void draw(QPainter &painter) const = 0;
    virtual QString toString() const = 0;
};

class SLine : public iShape {
public:
    SLine(const QPoint &pt1, const QPoint &pt2);
    void draw(QPainter &painter) const;
    QString toString() const;

private:
    QPoint m_pt1;
    QPoint m_pt2;
};

class SRect : public iShape {
public:
    SRect(const QRect &rect);
    void draw(QPainter &painter) const;
    QString toString() const;

private:
    QRect m_rect;
};

class SEllipse : public iShape {
public:
    SEllipse(const QPoint &center, qreal angle, quint16 a, quint16 b);
    void draw(QPainter &painter) const;
    QString toString() const;

private:
    QPoint m_center;
    qreal m_angle;
    quint16 m_a;
    quint16 m_b;
};
}
