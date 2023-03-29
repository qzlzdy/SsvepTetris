#include "fakestreamer.h"

#include <chrono>
#include <data_filter.h>

using namespace std;
using namespace ehdu;

FakeStreamer::FakeStreamer(QObject *parent): AcquireDevice(parent),
keepRunning(true), keepStreaming(false), index(0){
    data = DataFilter::read_file("sample.csv");
}

void FakeStreamer::stop(){
    keepRunning = false;
}

void FakeStreamer::prepare(){
    index = 0;
}

void FakeStreamer::startStream(){
    keepStreaming = true;
}

void FakeStreamer::stopStream(){
    keepStreaming = false;
}

void FakeStreamer::release(){}

const qint64 FakeStreamer::threshold = (1000000000ns / 250).count();

void FakeStreamer::run(){
    timer.start();
    qint64 count = 0;
    while(keepRunning){
        qint64 elapsed = timer.nsecsElapsed();
        timer.restart();
        if(keepStreaming){
            count += elapsed;
            if(count >= threshold){
                count %= threshold;
                BrainFlowArray<double, 2> d(9, 1);
                for(int c = 0; c < 8; ++c){
                    d.at(c + 1, 0) = data.at(c + 1, index);
                }
                emit dataReady(d);
                ++index;
                if(index >= data.get_size(1)){
                    index = 0;
                }
            }
        }
        usleep(400);
    }
}
