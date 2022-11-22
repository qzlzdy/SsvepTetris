#ifndef SSVEPVIEWER_H
#define SSVEPVIEWER_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class SsvepViewer; }
QT_END_NAMESPACE

class SsvepViewer : public QMainWindow
{
    Q_OBJECT

public:
    SsvepViewer(QWidget *parent = nullptr);
    ~SsvepViewer();

private:
    Ui::SsvepViewer *ui;
};
#endif // SSVEPVIEWER_H
