#include "decoder.h"

#include <algorithm>
#include <cstdlib>

using namespace std;
using namespace ehdu;

Decoder::Decoder(QObject *parent): QObject(parent){
    auto setCapacity = [&](boost::circular_buffer<double> &buf){
        buf.set_capacity(500);
    };
    for_each(buffers.begin(), buffers.end(), setCapacity);
    cmds.set_capacity(3);
    connect(&timer, &QTimer::timeout, this, &Decoder::predict);
}

void Decoder::start(){
    timer.start(1000);
}

void Decoder::stop(){
    timer.stop();
}

void Decoder::processData(const BrainFlowArray<double, 2> &data){
    size_t availableSamples = data.get_size(1);
    size_t start = 0;
    if(availableSamples > 500){
        start = availableSamples - 500;
    }
    for(size_t c = 0; c < 8; ++c){
        for(size_t i = start; i < availableSamples; ++i){
            buffers[c].push_back(data(c + 1, i));
        }
    }
}

void Decoder::predict(){
    if(!buffers[0].full()){
        return;
    }
    array<array<double, 500>, 8> data;
    for(size_t c = 0; c < 8; ++c){
        copy(buffers[c].begin(), buffers[c].end(), data[c].begin());
    }
    array<double, 4> pred = decode(data);
    size_t result = distance(pred.begin(),
                             max_element(pred.begin(), pred.end()));
    cmds.push_back(result);
    size_t realRes = 3;
    if(cmds.full()){
        if(cmds[0] == cmds[1] || cmds[0] == cmds[2]){
            realRes = cmds[0];
        }
        else if(cmds[1] == cmds[2]){
            realRes = cmds[1];
        }
    }
    switch(realRes){
    case 0:
        emit cmd0();
        break;
    case 1:
        emit cmd1();
        break;
    case 2:
        emit cmd2();
        break;
    case 3:
        emit cmd3();
        break;
    }
}
