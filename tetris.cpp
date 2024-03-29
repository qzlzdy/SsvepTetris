#include "tetris.h"

#include <exception>
#include <limits>

using namespace std;
using namespace ehdu;

#define RIGHT_BOTTOM bitset<200>(0x200ull)
#define ROW_RIGHT(y) (RIGHT_BOTTOM << (y * 10))
#define RIGHT_BOUND (\
    ROW_RIGHT(0) | ROW_RIGHT(1) | ROW_RIGHT(2) | ROW_RIGHT(3) |\
    ROW_RIGHT(4) | ROW_RIGHT(5) | ROW_RIGHT(6) | ROW_RIGHT(7) |\
    ROW_RIGHT(8) | ROW_RIGHT(9) | ROW_RIGHT(10) | ROW_RIGHT(11) |\
    ROW_RIGHT(12) | ROW_RIGHT(13) | ROW_RIGHT(14) | ROW_RIGHT(15) |\
    ROW_RIGHT(16) | ROW_RIGHT(17) | ROW_RIGHT(18) | ROW_RIGHT(19))

bitset<200> Tetris::eraseLine(const bitset<200> &orin, size_t line){
    size_t below = (line + 1) * 10;
    size_t above = (20 - line) * 10;
    return ((orin >> below) << (below - 10))
           | ((orin << above) >> above);
}

constexpr bitset<200> I_hori(0xfull);
constexpr bitset<200> I_vert(0x40100401ull);
constexpr bitset<200> J_0(0x200803ull);
constexpr bitset<200> J_1(0x1c04ull);
constexpr bitset<200> J_2(0x300401ull);
constexpr bitset<200> J_3(0x407ull);
constexpr bitset<200> L_0(0x100403ull);
constexpr bitset<200> L_1(0x1007ull);
constexpr bitset<200> L_2(0x300802ull);
constexpr bitset<200> L_3(0x1c01ull);
constexpr bitset<200> O_o(0xc03ull);
constexpr bitset<200> S_0(0x1803ull);
constexpr bitset<200> S_1(0x100c02ull);
constexpr bitset<200> T_0(0x807ull);
constexpr bitset<200> T_1(0x200c02ull);
constexpr bitset<200> T_2(0x1c02ull);
constexpr bitset<200> T_3(0x100c01ull);
constexpr bitset<200> Z_0(0xc06ull);
constexpr bitset<200> Z_1(0x200c01ull);

const bitset<200> &Tetris::getPattern(PieceKind kind, size_t rotate){
    switch(kind){
    case I:
        switch(rotate){
        case 0:
            return I_hori;
        case 1:
            return I_vert;
        default:
            throw invalid_argument("I-type piece has only 2 patterns");
        }
    case J:
        switch(rotate){
        case 0:
            return J_0;
        case 1:
            return J_1;
        case 2:
            return J_2;
        case 3:
            return J_3;
        default:
            throw invalid_argument("J-type piece has only 4 patterns");
        }
    case L:
        switch(rotate){
        case 0:
            return L_0;
        case 1:
            return L_1;
        case 2:
            return L_2;
        case 3:
            return L_3;
        default:
            throw invalid_argument("L-type piece has only 4 patterns");
        }
    case O:
        if(rotate != 0){
            throw invalid_argument("X-type piece has only 1 pattern");
        }
        return O_o;
    case S:
        switch(rotate){
        case 0:
            return S_0;
        case 1:
            return S_1;
        default:
            throw invalid_argument("S-type piece has only 2 patterns");
        }
    case T:
        switch(rotate){
        case 0:
            return T_0;
        case 1:
            return T_1;
        case 2:
            return T_2;
        case 3:
            return T_3;
        default:
            throw invalid_argument("T-type piece has only 4 patterns");
        }
    case Z:
        switch(rotate){
        case 0:
            return Z_0;
        case 1:
            return Z_1;
        default:
            throw invalid_argument("Z-type piece has only 2 patterns");
        }
    default:
        throw invalid_argument("unknown piece type");
    }
}

QColor Tetris::getPieceColor(PieceKind kind){
    switch(kind){
    case I:
        return QColor::fromRgb(0, 240, 240);
    case J:
        return QColor::fromRgb(0, 0, 240);
    case L:
        return QColor::fromRgb(240, 160, 0);
    case O:
        return QColor::fromRgb(240, 240, 0);
    case S:
        return QColor::fromRgb(0, 240, 0);
    case T:
        return QColor::fromRgb(160, 240, 0);
    case Z:
        return QColor::fromRgb(240, 0, 0);
    default:
        return QColor::fromRgb(0x66, 0xCC, 0xFF);
    }
}

Tetris::Tetris(QObject *parent): QObject(parent),
gen(random_device()()), distrib(I, Z){
    gaming = false;
    resetPad();
};

unsigned Tetris::getScore() const{
    return score;
}

const bitset<200> &Tetris::getIset() const{
    return iset;
}

const bitset<200> &Tetris::getJset() const{
    return jset;
}

const bitset<200> &Tetris::getLset() const{
    return lset;
}

const bitset<200> &Tetris::getOset() const{
    return oset;
}

const bitset<200> &Tetris::getSset() const{
    return sset;
}

const bitset<200> &Tetris::getTset() const{
    return tset;
}

const bitset<200> &Tetris::getZset() const{
    return zset;
}

pair<bitset<200>, QColor> Tetris::getCurrPiece() const{
    return make_pair(getPattern(currPiece, rotateMark) << x << (y * 10),
                     getPieceColor(currPiece));
}

pair<bitset<16>, QColor> Tetris::getNextPiece() const{
    bitset<200> piece = getPattern(nextPiece, 0);
    bitset<16> trim;
    for(size_t i = 0; i < 4; ++i){
        for(size_t j = 0; j < 4; ++j){
            trim[4 * i + j] = piece[10 * i + j];
        }
    }
    return make_pair(trim, getPieceColor(nextPiece));
}

void Tetris::startGame(){
    gaming = true;
    resetPad();
    score = 0;
    emit updatePad();
}

void Tetris::haltGame(){
    gaming = false;
}

void Tetris::resumeGame(){
    gaming = true;
}

void Tetris::rotate(){
    if(!gaming){
        return;
    }
    size_t nextMark = 0;
    switch(currPiece){
    case I:
    case S:
    case Z:
        nextMark = (rotateMark + 1) % 2;
        break;
    case J:
    case L:
    case T:
        nextMark = (rotateMark + 1) % 4;
        break;
    case O:
        nextMark = 0;
        break;
    }
    bitset<200> nextPattern = getPattern(currPiece, nextMark) << x << (y * 10);
    // not reach bound
    if((nextPattern & RIGHT_BOUND).none()){
        // not crush
        if((nextPattern & solid).none()){
            rotateMark = nextMark;  // rotate
        }
    }
    freefall();
    emit updatePad();
}

void Tetris::moveLeft(){
    if(!gaming){
        return;
    }
    // not reach bound
    if(x != 0){
        bitset<200> nextPattern =
            getPattern(currPiece, rotateMark) << (x - 1) << (y * 10);
        // not crush
        if((nextPattern & solid).none()){
            --x;    // move
        }
    }
    freefall();
    emit updatePad();
}

void Tetris::moveRight(){
    if(!gaming){
        return;
    }
    bitset<200> nextPattern =
        getPattern(currPiece, rotateMark) << x << (y * 10);
    // not reach bound
    if((nextPattern & RIGHT_BOUND).none()){
        nextPattern <<= 1;
        // not crush
        if((nextPattern & solid).none()){
            ++x;    // move
        }
    }
    freefall();
    emit updatePad();
}

void Tetris::nop(){
    if(!gaming){
        return;
    }
    freefall();
    emit updatePad();
}

void Tetris::checkFillup(){
    bitset<200> mask(0x3ff);
    size_t ind = 0;
    unsigned combo = 0;
    while(ind < 20){
        if((mask & solid).count() == 10){
            solid = eraseLine(solid, ind);
            iset = eraseLine(iset, ind);
            jset = eraseLine(jset, ind);
            lset = eraseLine(lset, ind);
            oset = eraseLine(oset, ind);
            sset = eraseLine(sset, ind);
            tset = eraseLine(tset, ind);
            zset = eraseLine(zset, ind);
            score += ++combo * 100;
        }
        else{
            mask <<= 10;
            ++ind;
        }
    }
    currPiece = nextPiece;
    rotateMark = 0;
    nextPiece = (PieceKind)distrib(gen);
    x = 4;
    y = 20;
}

void Tetris::freefall(){
    bitset<200> currPattern = getPattern(currPiece, rotateMark);
    currPattern <<= x;
    // reach bottom
    if(y == 0){
        solidfy(currPattern);
        return;
    }

    bitset<200> nextPattern = currPattern << (y - 1) * 10;
    currPattern <<= y * 10;
    // crush
    if((nextPattern & solid).any()){
        // has out-screen part
        if(currPattern.count() != 4){
            gaming = false;
            emit gameover();
            return;
        }
        solidfy(currPattern);
        return;
    }
    // fall
    --y;
}

void Tetris::resetPad(){
    solid.reset();
    iset.reset();
    jset.reset();
    lset.reset();
    oset.reset();
    sset.reset();
    tset.reset();
    zset.reset();
    currPiece = (PieceKind)distrib(gen);
    rotateMark = 0;
    nextPiece = (PieceKind)distrib(gen);
    x = 4;
    y = 20;
}

void Tetris::solidfy(const bitset<200> &pattern){
    solid |= pattern;
    switch(currPiece){
    case I:
        iset |= pattern;
        break;
    case J:
        jset |= pattern;
        break;
    case L:
        lset |= pattern;
        break;
    case O:
        oset |= pattern;
        break;
    case S:
        sset |= pattern;
        break;
    case T:
        tset |= pattern;
        break;
    case Z:
        zset |= pattern;
        break;
    }
    checkFillup();
}
