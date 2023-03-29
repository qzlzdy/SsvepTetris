#ifndef FBCCADECODER_H
#define FBCCADECODER_H

#include <array>
#include <mutex>
#include "decoder.h"

namespace ehdu{

class FbccaDecoder: public Decoder{
public:
    FbccaDecoder(QObject *parent = nullptr);
protected:
    std::array<double, 4>
    decode(std::array<std::array<double, 500>, 8> &data) override;
private:
    void subbandDecode(std::array<std::array<double, 500>, 8> &data,
                       double downbound, double upbound, double weight);
    std::array<double, 4> rho;
    std::mutex m;
};

} // namespace ehdu

#endif // FBCCADECODER_H
