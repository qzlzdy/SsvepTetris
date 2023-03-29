#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

namespace ehdu{

class Controller: public QObject{
    Q_OBJECT;
public:
    explicit Controller(QObject *parent = nullptr);
    virtual ~Controller() = default;
public slots:
    virtual void cmdUp() = 0;
    virtual void cmdDown() = 0;
    virtual void cmdLeft() = 0;
    virtual void cmdRight() = 0;
};

} // namespace ehdu

#endif // CONTROLLER_H
