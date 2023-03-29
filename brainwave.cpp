#include "brainwave.h"

#include <string>

using namespace std;
using namespace ehdu;

BrainWave::BrainWave(QObject *parent): AcquireDevice(parent){
    BrainFlowInputParams params;
    params.serial_port = string("/dev/ttyUSB0");
    board = new BoardShim(static_cast<int>(BoardIds::CYTON_BOARD), params);
    keepRunning = true;
}

BrainWave::~BrainWave(){
    if(board->is_prepared()){
        board->release_session();
    }
    delete board;
}

void BrainWave::stop(){
    keepRunning = false;
}

void BrainWave::prepare(){
    if(!board->is_prepared()){
        try{
            board->prepare_session();
        }
        catch(const BrainFlowException &err){
            emit errorMessage(err.what());
        }
    }
}

void BrainWave::startStream(){
    try{
        board->start_stream();
    }
    catch(const BrainFlowException &err){
        emit errorMessage(err.what());
    }
}

void BrainWave::stopStream(){
    try{
        board->stop_stream();
    }
    catch(const BrainFlowException &err){
        emit errorMessage(err.what());
    }
}

void BrainWave::release(){}

void BrainWave::run(){
    while(keepRunning){
        if(board->get_board_data_count() > 0){
            BrainFlowArray<double, 2> data = board->get_board_data();
            emit dataReady(data);
        }
    }
}
