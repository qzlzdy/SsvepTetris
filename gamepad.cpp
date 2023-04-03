#include "gamepad.h"

#include <QPainter>
#include <QPen>

using namespace ehdu;

Gamepad::Gamepad(QWidget *parent): QWidget(parent){

}

QSize Gamepad::sizeHint() const{
    return QSize(500, 600);
}

QSize Gamepad::minimumSizeHint() const{
    return QSize(500, 600);
}

void Gamepad::draw(size_t x, size_t y, QColor c){
    QPainter painter(this);
    painter.setPen(QPen(c));
    painter.drawRect(30 * x, 30 * y, 30, 30);
}

void Gamepad::setScore(unsigned score){
    Q_UNUSED(score);
}

void Gamepad::resetPad(){
    QPainter painter(this);
    painter.setPen(QPen(QColorConstants::Black));
    painter.drawRect(0, 0, 300, 600);
}
