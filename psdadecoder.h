#ifndef PSDADECODER_H
#define PSDADECODER_H

#include <array>
#include "decoder.h"

namespace ehdu{

class PsdaDecoder: public Decoder{
public:
    explicit PsdaDecoder(QObject *parent = nullptr);
protected:
    std::array<double, 4>
    decode(std::array<std::array<double, 500>, 8> &data) override;
};

} // namespace ehdu

#endif // PSDADECODER_H
