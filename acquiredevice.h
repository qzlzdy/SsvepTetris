#ifndef ACQUIREDEVICE_H
#define ACQUIREDEVICE_H

#include <string>
#include <QThread>
#include <brainflow_array.h>

namespace ehdu{

class AcquireDevice: public QThread{
    Q_OBJECT
public:
    explicit AcquireDevice(QObject *parent = nullptr);
    virtual ~AcquireDevice() = default;
    /**
     * @brief stop thread running
     */
    virtual void stop() = 0;
public slots:
    /**
     * @brief connect device
     */
    virtual void prepare() = 0;
    /**
     * @brief start stream
     */
    virtual void startStream() = 0;
    /**
     * @brief stop stream
     */
    virtual void stopStream() = 0;
    /**
     * @brief disconnect device
     */
    virtual void release() = 0;
signals:
    /**
     * @brief send acquired data
     * @param data channel X timepoint matrix
     */
    void dataReady(const BrainFlowArray<double, 2> &data);
    /**
     * @brief send error message
     * @param msg
     */
    void errorMessage(const std::string &msg);
};

} // namespace ehdu

#endif // ACQUIREDEVICE_H
