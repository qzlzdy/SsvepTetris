#include "gamepad.h"

#include <QTransform>
#include <QPen>
#include "tetris.h"

using namespace std;
using namespace ehdu;

Gamepad::Gamepad(Tetris &c, QWidget *parent): QWidget(parent), ctrl(c){}

QSize Gamepad::sizeHint() const{
    return QSize(500, 600);
}

QSize Gamepad::minimumSizeHint() const{
    return QSize(500, 600);
}

void Gamepad::drawPattern(QPainter &p, const bitset<200> &bs, QColor c){
    bitset<200> mask(1ull);
    size_t ind = 0;
    while(ind < 200){
        if((mask & bs).any()){
            size_t x = ind % 10;
            size_t y = ind / 10;
            p.fillRect(30 * x, 570 - 30 * y, 30, 30, c);
        }
        mask <<= 1;
        ++ind;
    }
}

void Gamepad::drawPattern(QPainter &p, const bitset<16> &bs, QColor c){
    bitset<16> mask(1ull);
    size_t ind = 0;
    while(ind < 16){
        if((mask & bs).any()){
            size_t x = ind % 4;
            size_t y = ind / 4;
            p.fillRect(340 + 30 * x, 210 - 30 * y, 30, 30, c);
        }
        mask <<= 1;
        ++ind;
    }
}

void Gamepad::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QFont font("思源宋体", 20);
    painter.setFont(font);

    painter.fillRect(0, 0, 300, 600, QColorConstants::Black);
    auto currPattern = ctrl.getCurrPiece();
    drawPattern(painter, currPattern.first, currPattern.second);
    drawPattern(painter, ctrl.getIset(), Tetris::getPieceColor(Tetris::I));
    drawPattern(painter, ctrl.getJset(), Tetris::getPieceColor(Tetris::J));
    drawPattern(painter, ctrl.getLset(), Tetris::getPieceColor(Tetris::L));
    drawPattern(painter, ctrl.getOset(), Tetris::getPieceColor(Tetris::O));
    drawPattern(painter, ctrl.getSset(), Tetris::getPieceColor(Tetris::S));
    drawPattern(painter, ctrl.getTset(), Tetris::getPieceColor(Tetris::T));
    drawPattern(painter, ctrl.getZset(), Tetris::getPieceColor(Tetris::Z));

    painter.drawText(360, 100, "下一个");
    painter.fillRect(340, 120, 120, 120, QColorConstants::Black);
    auto nextPattern = ctrl.getNextPiece();
    drawPattern(painter, nextPattern.first, nextPattern.second);

    painter.drawText(375, 300, "分数");
    painter.fillRect(340, 320, 120, 40, QColorConstants::Black);
    painter.drawText(350, 350, QString::number(ctrl.getScore()));
}
