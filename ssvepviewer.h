#ifndef SSVEPVIEWER_H
#define SSVEPVIEWER_H

#include <QGraphicsScene>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class SsvepViewer; }
QT_END_NAMESPACE

namespace ehdu{

class SsvepViewer: public QMainWindow{
    Q_OBJECT
public:
    SsvepViewer(QWidget *parent = nullptr);
    ~SsvepViewer();
private:
    void populateScene();
    Ui::SsvepViewer *ui;
    QGraphicsScene *scene;
};

}

#endif // SSVEPVIEWER_H
