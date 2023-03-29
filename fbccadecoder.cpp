#include "fbccadecoder.h"

#include <algorithm>
#include <cmath>
#include <functional>
#include <numbers>
#include <thread>
#include <Eigen/Dense>
#include <Eigen/QR>
#include <Eigen/SVD>
#include <brainflow_constants.h>
#include <data_filter.h>

using namespace std;
using namespace Eigen;
using namespace ehdu;

FbccaDecoder::FbccaDecoder(QObject *parent): Decoder(parent){}

template<int rows, int cols>
static Matrix<double, rows, cols>
decomposeQ(const Matrix<double, rows, cols> &A){
    return HouseholderQR<Matrix<double, rows, cols>>(A)
        .householderQ() * Matrix<double, rows, cols>::Identity();
}

static double calculateCca(Matrix<double, 8, 500> &Q1T, double freq){
    Matrix<double, 4, 500> reference;
    for(size_t h = 0; h < 2; ++h){
        double step = 2 * numbers::pi * freq * (h + 1) / 250;
        for(size_t i = 0; i < 500; ++i){
            reference(2 * h, i) = sin(step * i);
            reference(2 * h + 1, i) = cos(step * i);
        }
    }
    Matrix<double, 8, 4> Q = Q1T * decomposeQ<500, 4>(reference.transpose());
    return JacobiSVD<Matrix<double, 8, 4>>(Q).singularValues()[0];
}

void FbccaDecoder::subbandDecode(array<array<double, 500>, 8> &data,
                                 double downbound, double upbound,
                                 double weight){
    Matrix<double, 8, 500> signal;
    for(size_t c = 0; c < 8; ++c){
        array<double, 500> channel;
        copy(data[c].begin(), data[c].end(), channel.begin());
        DataFilter::perform_bandpass(channel.data(), channel.size(),
            250, downbound, upbound, 1, (int)FilterTypes::BUTTERWORTH, 0);
        for(size_t i = 0; i < 500; ++i){
            signal(c, i) = channel[i];
        }
    }
    signal = decomposeQ<500, 8>(signal.transpose()).transpose();

    double r = weight * calculateCca(signal, 8);
    m.lock(); rho[0] += r; m.unlock();
    r = weight * calculateCca(signal, 11);
    m.lock(); rho[1] += r; m.unlock();
    r = weight * calculateCca(signal, 13);
    m.lock(); rho[2] += r; m.unlock();
    r = weight * calculateCca(signal, 15);
    m.lock(); rho[3] += r; m.unlock();
}

array<double, 4> FbccaDecoder::decode(array<array<double, 500>, 8> &data){
    for(size_t c = 0; c < 8; ++c){
        DataFilter::remove_environmental_noise(data[c].data(), data[c].size(),
            250, (int)NoiseTypes::FIFTY);
    }
    rho.fill(0);
    array<std::thread, 7> threads;
    for(size_t n = 0; n < 7; ++n){
        double downbound = 8 * n + 8;
        double weight = pow(n + 1, -1.25) + 0.25;
        threads[n] = std::thread(&FbccaDecoder::subbandDecode, this,
            std::ref(data), downbound - 2, 66, weight);
    }
    for_each(threads.begin(), threads.end(), [](std::thread &t){t.join();});
    return rho;
}
