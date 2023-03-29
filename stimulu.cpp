#include "stimulu.h"

#include <QColor>
#include <QPainter>

using namespace std;
using namespace ehdu;

namespace{

QColor positive(255, 0, 255);
QColor negtive(255, 0, 0);

}

Stimulu::Stimulu(QWidget *parent): QWidget(parent){
    flipped = false;
}

void Stimulu::updateColor(){
    flipped = !flipped;
    update();
}

void Stimulu::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QColor color = flipped ? negtive : positive;
    painter.fillRect(event->rect(), color);
}
