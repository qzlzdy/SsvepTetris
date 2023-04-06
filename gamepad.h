#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <bitset>
#include <QColor>
#include <QPainter>
#include <QWidget>

namespace ehdu{

class Tetris;

class Gamepad: public QWidget{
public:
    explicit Gamepad(Tetris &c, QWidget *parent = nullptr);
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    void drawPattern(QPainter &p, const std::bitset<200> &bs, QColor c);
    void drawPattern(QPainter &p, const std::bitset<16> &bs, QColor c);
    Tetris &ctrl;
};

} // namespace ehdu

#endif // GAMEPAD_H
