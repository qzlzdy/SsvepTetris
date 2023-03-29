#ifndef TETRIS_H
#define TETRIS_H

#include "controller.h"

namespace ehdu{

class Tetris: public Controller{
public:
    explicit Tetris(QObject *parent = nullptr);
public slots:
    void cmdUp() override;
    void cmdDown() override;
    void cmdLeft() override;
    void cmdRight() override;
};

} // namespace ehdu

#endif // TETRIS_H
