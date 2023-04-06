#ifndef TETRIS_H
#define TETRIS_H

#include <bitset>
#include <random>
#include <utility>
#include <QColor>
#include <QObject>

namespace ehdu{

class Tetris: public QObject{
    Q_OBJECT
public:
    enum PieceKind: uint8_t{
        I, J, L, X, S, T, Z
    };
    static std::bitset<200>
    eraseLine(const std::bitset<200> &orin, size_t lien);
    static const std::bitset<200> &
    getPattern(PieceKind kind, std::size_t rotate);
    static QColor getPieceColor(PieceKind kind);
    explicit Tetris(QObject *parent = nullptr);
    unsigned getScore() const;
    const std::bitset<200> &getIset() const;
    const std::bitset<200> &getJset() const;
    const std::bitset<200> &getLset() const;
    const std::bitset<200> &getXset() const;
    const std::bitset<200> &getSset() const;
    const std::bitset<200> &getTset() const;
    const std::bitset<200> &getZset() const;
    std::pair<std::bitset<200>, QColor> getCurrPattern() const;
    std::pair<std::bitset<16>, QColor> getNextPattern() const;
public slots:
    void startGame();
    void haltGame();
    void resumeGame();
    void rotate();
    void moveLeft();
    void moveRight();
    void nop();
signals:
    void gameover();
    void updatePad();
private:
    void checkFillup();
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
    unsigned score;
};

} // namespace ehdu

#endif // TETRIS_H
