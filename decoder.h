#ifndef DECODER_H
#define DECODER_H

#include <array>
#include <QObject>
#include <QTimer>
#include <boost/circular_buffer.hpp>
#include <brainflow_array.h>

namespace ehdu{

class Decoder: public QObject{
    Q_OBJECT
public:
    explicit Decoder(QObject *parent = nullptr);
    virtual ~Decoder() = default;
public slots:
    void start();
    void stop();
    void processData(const BrainFlowArray<double, 2> &data);
signals:
    void cmd0();
    void cmd1();
    void cmd2();
    void cmd3();
protected:
    virtual std::array<double, 4>
    decode(std::array<std::array<double, 500>, 8> &data) = 0;
private:
    std::array<boost::circular_buffer<double>, 8> buffers;
    boost::circular_buffer<size_t> cmds;
    QTimer timer;
private slots:
    void predict();
};

} // namespace ehdu

#endif // DECODER_H
