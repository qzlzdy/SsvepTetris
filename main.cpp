#include "ssvepviewer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SsvepViewer w;
    w.show();
    return a.exec();
}
