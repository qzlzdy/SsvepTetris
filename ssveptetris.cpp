#include "ssveptetris.h"
#include "ui_ssveptetris.h"

#include <chrono>
#include <mutex>
#include <QGlobalStatic>
#include <QKeySequence>
#include <QMessageBox>
//#include "brainwave.h"
#include "fakestreamer.h"
//#include "psdadecoder.h"
#include "fbccadecoder.h"
#include "tetris.h"

using namespace std;
using namespace ehdu;

namespace {

once_flag deviceStarted;

Q_GLOBAL_STATIC_WITH_ARGS(QString, authorInfo, (
    "<p align=center>作者：Qzlzdy</p>"
    "<p align=center>杭州电子科技大学</p>"
    "<p align=center>计算机学院</p>"
    "<br/>"
    "<p align=center>"
        "<a href=\"https://yangvillage.top/portfolio\">作者主页</a>"
    "</p>"
));

}

SsvepTetris::SsvepTetris(QWidget *parent):
QMainWindow(parent), ui(new Ui::SsvepTetris){
    ui->setupUi(this);

    // Menu Bar
    deviceMenu = ui->menubar->addMenu("设备");
    deviceMenu->setMinimumWidth(200);
    connectBw = deviceMenu->addAction("连接设备");
    disconnectBw = deviceMenu->addAction("断开设备");
    startStream = deviceMenu->addAction("开始采样");
    stopStream = deviceMenu->addAction("停止采样");
    disconnectBw->setEnabled(false);
    startStream->setEnabled(false);
    stopStream->setEnabled(false);

    gameMenu = ui->menubar->addMenu("游戏");
    gameMenu->setMinimumWidth(200);
    startGame = gameMenu->addAction("开始游戏");
    restartGame = gameMenu->addAction("重新开始");
    haltGame = gameMenu->addAction("暂停游戏");
    resumeGame = gameMenu->addAction("恢复游戏");
    restartGame->setEnabled(false);
    haltGame->setEnabled(false);
    resumeGame->setEnabled(false);

    helpMenu = ui->menubar->addMenu("帮助");
    helpMenu->setMinimumWidth(200);
    operation = helpMenu->addAction("操作说明");
    author = helpMenu->addAction("作者");
    aboutQt = helpMenu->addAction("关于Qt");

    // Shortcuts
    upArr = new QShortcut(Qt::Key_Up, this);
    downArr = new QShortcut(Qt::Key_Down, this);
    leftArr = new QShortcut(Qt::Key_Left, this);
    rightArr = new QShortcut(Qt::Key_Right, this);

    // Main Scene
//    ui->layout->addWidget(view, 1, 1);

    // SSVEP Stimulu
    up = new Stimulu(this);
    down = new Stimulu(this);
    left = new Stimulu(this);
    right = new Stimulu(this);
    timer = new SsvepTimer(this);
    ui->layout->addWidget(up, 0, 1);
    ui->layout->addWidget(down, 2, 1);
    ui->layout->addWidget(left, 1, 0);
    ui->layout->addWidget(right, 1, 2);
    ui->layout->setColumnMinimumWidth(0, 50);
    ui->layout->setColumnMinimumWidth(2, 50);
    ui->layout->setRowMinimumHeight(0, 50);
    ui->layout->setRowMinimumHeight(2, 50);

    // Controller
//    device = new BrainWave(this);
    device = new FakeStreamer(this);
//    decoder = new PsdaDecoder(this);
    decoder = new FbccaDecoder(this);
    ctrl = new Tetris;

    initialize();
//    timer->start();
}

void SsvepTetris::initialize(){
    connect(connectBw, &QAction::triggered, this, [&](){
        call_once(deviceStarted, [&](){
            device->start();
        });
    });
    connect(connectBw, &QAction::triggered,
            device, &AcquireDevice::prepare);
    connect(connectBw, &QAction::triggered, this, [&](){
        connectBw->setEnabled(false);
        disconnectBw->setEnabled(true);
        startStream->setEnabled(true);
    });

    connect(disconnectBw, &QAction::triggered, this, [&](){
        connectBw->setEnabled(true);
        disconnectBw->setEnabled(false);
        startStream->setEnabled(false);
    });

    connect(startStream, &QAction::triggered,
            device, &AcquireDevice::startStream);
    connect(startStream, &QAction::triggered, this, [&](){
        disconnectBw->setEnabled(false);
        startStream->setEnabled(false);
        stopStream->setEnabled(true);
    });
    connect(stopStream, &QAction::triggered,
            device, &AcquireDevice::stopStream);
    connect(stopStream, &QAction::triggered, this, [&](){
        disconnectBw->setEnabled(true);
        stopStream->setEnabled(false);
        startStream->setEnabled(true);
    });

    connect(author, &QAction::triggered, this, [&](){
        QMessageBox::information(this, "作者信息", *authorInfo);
    });
    connect(aboutQt, &QAction::triggered, this, [&](){
        QMessageBox::aboutQt(this, "关于Qt");
    });

    connect(upArr, &QShortcut::activated, ctrl, &Controller::cmdUp);
    connect(downArr, &QShortcut::activated, ctrl, &Controller::cmdDown);
    connect(leftArr, &QShortcut::activated, ctrl, &Controller::cmdLeft);
    connect(rightArr, &QShortcut::activated, ctrl, &Controller::cmdRight);

    connect(timer, &SsvepTimer::timeout_8hz, up, &Stimulu::updateColor);
    connect(timer, &SsvepTimer::timeout_10hz, down, &Stimulu::updateColor);
    connect(timer, &SsvepTimer::timeout_12hz, left, &Stimulu::updateColor);
    connect(timer, &SsvepTimer::timeout_14hz, right, &Stimulu::updateColor);

    connect(device, &AcquireDevice::errorMessage,
            this, &SsvepTetris::errorMessageHandle);
    connect(device, &AcquireDevice::dataReady, decoder, &Decoder::processData);
    connect(startStream, &QAction::triggered, decoder, &Decoder::start);
    connect(stopStream, &QAction::triggered, decoder, &Decoder::stop);
    connect(decoder, &Decoder::cmd0, ctrl, &Controller::cmdUp);
    connect(decoder, &Decoder::cmd1, ctrl, &Controller::cmdDown);
    connect(decoder, &Decoder::cmd2, ctrl, &Controller::cmdLeft);
    connect(decoder, &Decoder::cmd3, ctrl, &Controller::cmdRight);
}

SsvepTetris::~SsvepTetris(){
    // Controller
    device->stop();
    device->quit();
    device->wait();
    delete device;
    delete decoder;
    delete ctrl;

    // Shortcuts
    delete upArr;
    delete downArr;
    delete leftArr;
    delete rightArr;

    // Stimulus
    timer->stop();
    timer->wait();
    delete timer;
    delete up;
    delete down;
    delete left;
    delete right;

    // Menu Bar
    delete connectBw;
    delete disconnectBw;
    delete startStream;
    delete stopStream;
    delete deviceMenu;

    delete startGame;
    delete restartGame;
    delete haltGame;
    delete resumeGame;
    delete gameMenu;

    delete operation;
    delete author;
    delete aboutQt;
    delete helpMenu;

    delete ui;
}

void SsvepTetris::errorMessageHandle(const string &msg){
    ui->statusbar->showMessage(QString::fromStdString(msg), 2000);
}
