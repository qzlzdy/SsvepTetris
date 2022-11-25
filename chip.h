#ifndef CHIP_H
#define CHIP_H

#include <QColor>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QList>
#include <QPainter>
#include <QPointF>
#include <QStyleOptionGraphicsItem>

namespace ehdu{

class Chip: public QGraphicsItem{
public:
    Chip(const QColor &color, int x, int y);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item,
               QWidget *) override;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
private:
    int x;
    int y;
    QColor color;
    QList<QPointF> stuff;
};

}

#endif // CHIP_H
