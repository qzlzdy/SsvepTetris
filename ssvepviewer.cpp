#include "ssvepviewer.h"
#include "ui_ssvepviewer.h"

SsvepViewer::SsvepViewer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SsvepViewer)
{
    ui->setupUi(this);
}

SsvepViewer::~SsvepViewer()
{
    delete ui;
}

