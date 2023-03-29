#include "psdadecoder.h"

#include <utility>
#include <brainflow_constants.h>
#include <brainflow_exception.h>
#include <data_filter.h>

using namespace std;
using namespace ehdu;

PsdaDecoder::PsdaDecoder(QObject *parent): Decoder(parent){}

array<double, 4> PsdaDecoder::decode(array<array<double, 500>, 8> &data){
    array<double, 4> power;
    power.fill(0);
    for(size_t c = 0; c < 8; ++c){
        double *head = data[c].data();
        DataFilter::remove_environmental_noise(head, 500, 250,
                                               (int)NoiseTypes::FIFTY);
        int psd_len;
        pair<double *, double *> psd = DataFilter::get_psd_welch(
            head, 500, 256, 128, 250, (int)WindowOperations::HANNING, &psd_len);
        try{
            power[0] += DataFilter::get_band_power(psd, psd_len, 7, 9);
        }
        catch(const BrainFlowException &){}
        try{
            power[1] += DataFilter::get_band_power(psd, psd_len, 10, 12);
        }
        catch(const BrainFlowException &){}
        try{
            power[2] += DataFilter::get_band_power(psd, psd_len, 12, 14);
        }
        catch(const BrainFlowException &){}
        try{
            power[3] += DataFilter::get_band_power(psd, psd_len, 14, 16);
        }
        catch(const BrainFlowException &){}
    }
    return power;
}
