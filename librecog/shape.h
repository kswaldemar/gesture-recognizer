#pragma once

#include <QPoint>

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

}
