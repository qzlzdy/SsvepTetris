#ifndef SSVEPTIMER_H
#define SSVEPTIMER_H

#include <QElapsedTimer>
#include <QThread>

namespace ehdu{

class SsvepTimer: public QThread{
    Q_OBJECT
public:
    SsvepTimer(QObject *parent = nullptr);
    void stop();
signals:
    void timeout_8hz();
    void timeout_10hz();
    void timeout_12hz();
    void timeout_14hz();
protected:
    void run() override;
private:
    static const qint64 threshold_8hz;
    static const qint64 threshold_10hz;
    static const qint64 threshold_12hz;
    static const qint64 threshold_14hz;
    QElapsedTimer timer;
    bool keepRunning;
};

} // namespace ehdu

#endif // SSVEPTIMER_H
