#ifndef BRAINWAVE_H
#define BRAINWAVE_H

#include <board_shim.h>
#include "acquiredevice.h"

namespace ehdu{

class BrainWave final: public AcquireDevice{
public:
    explicit BrainWave(QObject *parent = nullptr);
    ~BrainWave();
    void stop() override;
public slots:
    void prepare() override;
    void startStream() override;
    void stopStream() override;
    void release() override;
private:
    void run() override;
    BoardShim *board;
    bool keepRunning;
};

} // namespace ehdu

#endif // BRAINWAVE_H
