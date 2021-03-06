#pragma once

#include <QPoint>
#include <QRect>

class QPainter;
class QString;

namespace recog {

/**
 * @brief Abstract shape interface
 */
class iShape {
public:
    virtual ~iShape();
    /**
     * @brief Draw yourself using custom painter
     * @param painter painter which use to draw
     */
    virtual void draw(QPainter &painter) const = 0;
    /**
     * @brief Self description
     * @return string with descriprion
     */
    virtual QString toString() const = 0;
};

/**
 * @brief Shape representing the cut line
 */
class SLine : public iShape {
public:
    /**
     * @brief Constructor
     * @param pt1 first point of cut
     * @param pt2 second point of cut
     */
    SLine(const QPoint &pt1, const QPoint &pt2);

    void draw(QPainter &painter) const;

    QString toString() const;
private:
    /// First point of cut
    QPoint m_pt1;
    /// Second point of cut
    QPoint m_pt2;
};

/**
 * @brief Shape representing the rectangle
 */
class SRect : public iShape {
public:
    /**
     * @brief Constructor
     * @param rect Qt rectangle to build from
     */
    SRect(const QRect &rect);

    void draw(QPainter &painter) const;

    QString toString() const;

private:
    /// Underlying Qt rectangle
    QRect m_rect;
};

/**
 * @brief Shape representing the ellipse
 */
class SEllipse : public iShape {
public:
    /**
     * @brief Constructor
     * @param center ellipse center
     * @param angle rotation angle in radian
     * @param a half of the first axe
     * @param b half of the second axe
     */
    SEllipse(const QPoint &center, qreal angle, quint16 a, quint16 b);

    void draw(QPainter &painter) const;

    QString toString() const;

private:
    /// Ellipse center
    QPoint m_center;
    /// Rotation angle in radian
    qreal m_angle;
    /// Half of the first ellipse axe
    quint16 m_a;
    /// Half of the second ellipse axe
    quint16 m_b;
};
}
