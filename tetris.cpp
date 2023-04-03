#include "tetris.h"

#include <exception>

using namespace std;
using namespace ehdu;

constexpr bitset<200> I_hori(0xfull);
constexpr bitset<200> I_vert(0x80200802ull);
constexpr bitset<200> J_0(0x200803ull);
constexpr bitset<200> J_1(0x1c04ull);
constexpr bitset<200> J_2(0x300401ull);
constexpr bitset<200> J_3(0x407ull);
constexpr bitset<200> L_0(0x100403ull);
constexpr bitset<200> L_1(0x1007ull);
constexpr bitset<200> L_2(0x300802ull);
constexpr bitset<200> L_3(0x1c01ull);
constexpr bitset<200> X_(0xc03ull);
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
    case X:
        if(rotate != 0){
            throw invalid_argument("X-type piece has only 1 pattern");
        }
        return X_;
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
    case X:
        return QColor::fromRgb(240, 240, 0);
    case S:
        return QColor::fromRgb(240, 0, 0);
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
};

void Tetris::startGame(){
    gaming = true;
    resetPad();
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
    freefall();
}

void Tetris::moveLeft(){
    if(!gaming){
        return;
    }
    freefall();
}

void Tetris::moveRight(){
    if(!gaming){
        return;
    }
    freefall();
}

void Tetris::nop(){
    if(!gaming){
        return;
    }
    freefall();
}

void Tetris::drawPattern(const bitset<200> &orin, const bitset<200> &now){
    bitset<200> mask(1ull);
    size_t ind = 0;
    bitset<200> erase = orin & ~now;
    bitset<200> patch = now & ~orin;
    while(ind < 200){
        if((mask & erase).any()){
            emit draw(ind % 10, ind / 10, QColorConstants::Black);
        }
        if((mask & patch).any()){
            emit draw(ind % 10, ind / 10, getPieceColor(currPiece));
        }
        mask <<= 1;
        ++ind;
    }
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
    xset.reset();
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
    solid &= pattern;
    switch(currPiece){
    case I:
        iset &= pattern;
        break;
    case J:
        jset &= pattern;
        break;
    case L:
        lset &= pattern;
        break;
    case X:
        xset &= pattern;
        break;
    case S:
        sset &= pattern;
        break;
    case T:
        tset &= pattern;
        break;
    case Z:
        zset &= pattern;
        break;
    }
    currPiece = nextPiece;
    rotateMark = 0;
    nextPiece = (PieceKind)distrib(gen);
    x = 4;
    y = 20;
}
