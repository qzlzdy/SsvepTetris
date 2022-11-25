#ifndef VIEW_H
#define VIEW_H

#include <QFrame>
#include <QGraphicsView>
#include <QString>
#include <QWheelEvent>

namespace ehdu{

class View: public QFrame{
    Q_OBJECT
public:
    explicit View(QWidget *parent = nullptr);
    QGraphicsView *view() const;
private:
    QGraphicsView *graphicsView;
};

}

#endif // VIEW_H
