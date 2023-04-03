#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <QColor>
#include <QWidget>

namespace ehdu{

class Gamepad: public QWidget{
public:
    explicit Gamepad(QWidget *parent = nullptr);
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
public slots:
    void draw(size_t x, size_t y, QColor c);
    void setScore(unsigned score);
    void resetPad();
};

} // namespace ehdu

#endif // GAMEPAD_H
