#include "ssveptimer.h"

#include <chrono>

using namespace std;
using namespace ehdu;

SsvepTimer::SsvepTimer(QObject *parent): QThread(parent){
    keepRunning = true;
}

void SsvepTimer::stop(){
    keepRunning = false;
}

void SsvepTimer::run(){
    timer.start();
    qint64 count_8hz = 0;
    qint64 count_10hz = 0;
    qint64 count_12hz = 0;
    qint64 count_14hz = 0;
    while(keepRunning){
        qint64 elapsed = timer.nsecsElapsed();
        timer.restart();
        count_8hz += elapsed;
        if(count_8hz >= threshold_8hz){
            count_8hz %= threshold_8hz;
            emit timeout_8hz();
        }
        count_10hz += elapsed;
        if(count_10hz >= threshold_10hz){
            count_10hz %= threshold_10hz;
            emit timeout_10hz();
        }
        count_12hz += elapsed;
        if(count_12hz >= threshold_12hz){
            count_12hz %= threshold_12hz;
            emit timeout_12hz();
        }
        count_14hz += elapsed;
        if(count_14hz >= threshold_14hz){
            count_14hz %= threshold_14hz;
            emit timeout_14hz();
        }
        msleep(4);
    }
}

const qint64 SsvepTimer::threshold_8hz = (500000000ns / 8).count();
const qint64 SsvepTimer::threshold_10hz = (500000000ns / 10).count();
const qint64 SsvepTimer::threshold_12hz = (500000000ns / 12).count();
const qint64 SsvepTimer::threshold_14hz = (500000000ns / 14).count();
