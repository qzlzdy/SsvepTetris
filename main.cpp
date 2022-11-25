#include <QApplication>
#include "ssvepviewer.h"

using namespace ehdu;

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    SsvepViewer w;
    w.show();
    return a.exec();
}
