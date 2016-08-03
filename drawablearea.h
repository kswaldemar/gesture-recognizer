#ifndef DRAWABLEAREA_H
#define DRAWABLEAREA_H

#include <QWidget>
#include <QVector>
#include <QPoint>
#include <QPainter>

class DrawableArea : public QWidget
{
    Q_OBJECT
public:
    explicit DrawableArea(QWidget *parent = 0);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

    void startNewGestureCapture();
    void addGesturePoint(const QPoint &pt);
    void stopGestureCapture();

signals:
    void gestureCaptured(QVector<QPoint>);

private:
    QVector<QPoint> m_points;
    bool m_gestureStarted;
};

#endif // DRAWABLEAREA_H
