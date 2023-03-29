#ifndef STIMULU_H
#define STIMULU_H

#include <QPaintEvent>
#include <QWidget>

namespace ehdu{

class Stimulu: public QWidget{
    Q_OBJECT
public:
    Stimulu(QWidget *parent = nullptr);
public slots:
    void updateColor();
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    bool flipped;
};

} // namespace ehdu

#endif // STIMULUS_H
