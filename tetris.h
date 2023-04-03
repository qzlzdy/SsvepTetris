#ifndef TETRIS_H
#define TETRIS_H

#include <bitset>
#include <random>
#include <QColor>
#include <QObject>

namespace ehdu{

class Tetris: public QObject{
    Q_OBJECT
public:
    enum PieceKind: uint8_t{
        I, J, L, X, S, T, Z
    };
    static const std::bitset<200> &
    getPattern(PieceKind kind, std::size_t rotate);
    static QColor getPieceColor(PieceKind kind);
    explicit Tetris(QObject *parent = nullptr);
public slots:
    void startGame();
    void haltGame();
    void resumeGame();
    void rotate();
    void moveLeft();
    void moveRight();
    void nop();
signals:
    void draw(std::size_t x, std::size_t y, QColor c);
    void gameover();
private:
    void drawPattern(const std::bitset<200> &orin, const std::bitset<200> &now);
    void freefall();
    void resetPad();
    void solidfy(const std::bitset<200> &pattern);
    bool gaming;
    std::bitset<200> solid;
    std::bitset<200> iset;
    std::bitset<200> jset;
    std::bitset<200> lset;
    std::bitset<200> xset;
    std::bitset<200> sset;
    std::bitset<200> tset;
    std::bitset<200> zset;
    std::bitset<200> cursor;
    PieceKind currPiece;
    std::size_t rotateMark;
    PieceKind nextPiece;
    std::mt19937 gen;
    std::uniform_int_distribution<> distrib;
    std::size_t x, y;
};

} // namespace ehdu

#endif // TETRIS_H
