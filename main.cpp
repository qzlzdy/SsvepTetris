#include <QApplication>
#include "ssveptetris.h"

using namespace ehdu;

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    SsvepTetris w;
    w.show();
    return a.exec();
}
