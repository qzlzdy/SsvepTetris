#include "ssvepviewer.h"
#include "ui_ssvepviewer.h"

#include <QImage>
#include <QPushButton>
#include <QScrollBar>
#include "chip.h"
#include "view.h"

using namespace ehdu;

SsvepViewer::SsvepViewer(QWidget *parent):
QMainWindow(parent), ui(new Ui::SsvepViewer){
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    populateScene();

    View *view = new View();
    view->view()->setScene(scene);
    ui->layout->addWidget(view, 1, 1);

    QPushButton *up = new QPushButton;
    QPushButton *down = new QPushButton;
    QPushButton *left = new QPushButton;
    QPushButton *right = new QPushButton;
    ui->layout->addWidget(up, 0, 1);
    ui->layout->addWidget(down, 2, 1);
    ui->layout->addWidget(left, 1, 0);
    ui->layout->addWidget(right, 1, 2);

    QScrollBar *xAxis = view->view()->horizontalScrollBar();
    QScrollBar *yAxis = view->view()->verticalScrollBar();
    connect(up, &QPushButton::clicked, this, [=](){
        yAxis->setValue(yAxis->value() - 100);
    });
    connect(down, &QPushButton::clicked, this, [=](){
        yAxis->setValue(yAxis->value() + 100);
    });
    connect(left, &QPushButton::clicked, this, [=](){
        xAxis->setValue(xAxis->value() + 100);
    });
    connect(right, &QPushButton::clicked, this, [=](){
        xAxis->setValue(xAxis->value() - 100);
    });
}

SsvepViewer::~SsvepViewer(){
    delete ui;
}

void SsvepViewer::populateScene(){
    QImage image(":/qt4logo.png");
    int xx = 0;
    for(int i = -11000; i < 11000; i += 110){
        ++xx;
        int yy = 0;
        for(int j = -7000; j < 7000; j += 70){
            ++yy;
            qreal x = (i + 11000) / 22000.0;
            qreal y = (j + 7000) / 14000.0;
            QColor color(image.pixel(int(image.width() * x), int(image.height() * y)));
            QGraphicsItem *item = new Chip(color, xx, yy);
            item->setPos(QPointF(i, j));
            scene->addItem(item);
        }
    }
}
