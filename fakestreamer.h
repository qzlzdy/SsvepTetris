#ifndef FAKESTREAMER_H
#define FAKESTREAMER_H

#include <QElapsedTimer>
#include <brainflow_array.h>
#include "acquiredevice.h"

namespace ehdu{

class FakeStreamer final: public AcquireDevice{
public:
    FakeStreamer(QObject *parent = nullptr);
    void stop() override;
public slots:
    void prepare() override;
    void startStream() override;
    void stopStream() override;
    void release() override;
private:
    static const qint64 threshold;
    void run() override;
    BrainFlowArray<double, 2> data;
    bool keepRunning;
    bool keepStreaming;
    QElapsedTimer timer;
    std::size_t index;
};

} // namespace ehdu

#endif // FAKESTREAMER_H
