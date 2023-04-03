#ifndef SSVEPVIEWER_H
#define SSVEPVIEWER_H

#include <string>
#include <QAction>
#include <QGraphicsScene>
#include <QScrollBar>
#include <QShortcut>
#include <QMainWindow>
#include <QMenu>
#include "acquiredevice.h"
#include "decoder.h"
#include "gamepad.h"
#include "ssveptimer.h"
#include "stimulu.h"
#include "tetris.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SsvepTetris; }
QT_END_NAMESPACE

namespace ehdu{

class SsvepTetris: public QMainWindow{
    Q_OBJECT
public:
    explicit SsvepTetris(QWidget *parent = nullptr);
    ~SsvepTetris();
private:
    void initialize();
    Ui::SsvepTetris *ui;

    // Menu Bar
    QMenu *deviceMenu;
    QAction *connectBw;
    QAction *disconnectBw;
    QAction *startStream;
    QAction *stopStream;

    QMenu *gameMenu;
    QAction *startGame;
    QAction *haltGame;
    QAction *resumeGame;

    QMenu *helpMenu;
    QAction *operation;
    QAction *author;
    QAction *aboutQt;

    // Shortcuts
    QShortcut *upArr;
    QShortcut *downArr;
    QShortcut *leftArr;
    QShortcut *rightArr;

    // Stimulus
    Stimulu *up;
    Stimulu *down;
    Stimulu *left;
    Stimulu *right;
    SsvepTimer *timer;

    // Decive & Decoder
    AcquireDevice *device;
    Decoder *decoder;

    // Game
    Tetris *ctrl;
    Gamepad *pad;
private slots:
    void errorMessageHandle(const std::string &msg);
};

} // namespace ehdu

#endif // SSVEPVIEWER_H
