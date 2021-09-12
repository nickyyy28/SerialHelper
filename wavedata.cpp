#include "wavedata.h"
#include <QDebug>
#include <QDateTime>

WaveData::WaveData(QObject *parent) : QObject(parent)
{

}

void WaveData::lock()
{
    this->m_mutex.lock();
}

void WaveData::unlock()
{
    this->m_mutex.unlock();
}


void WaveData::appendData(const QByteArray &data)
{
    lock();
    this->waveRev.append(data);
    unlock();
}

bool WaveData::processData(int datasize)
{
    bool res = false;
//    if(this->waveRev.size() == 0){
//        QThread::msleep(20);
//    }

    lock();

    qint64 start = QDateTime::currentMSecsSinceEpoch();

    qDebug() << "处理前剩余数据:" <<this->waveRev.size();

    while(this->waveRev.size() > 0){
        if(this->waveRev.at(0) != '\x5A'){
            this->waveRev.remove(0, 1);
            continue;
        } else if (this->waveRev.size() >= 2 + 16 * datasize && this->waveRev.at(1 + 16 * datasize) == '\xA5'){
            for(uint8_t i = 0 ; i < 16 ; i++){
                this->channels[i] = *(double *)(this->waveRev.begin() + 1 + i * datasize);
            }

            this->waveRev.remove(0, 2 + 16 * datasize);
            res = true;
            break;
        } else {
            this->waveRev.remove(0, 1);
        }
    }

    qint64 end = QDateTime::currentMSecsSinceEpoch();

    qDebug() << "处理后剩余数据:" <<this->waveRev.size();

    qDebug() << "耗时: " << end - start << "ms";

    unlock();
    return res;
}

double WaveData::getChannelData(int index)
{
    return this->channels[index - 1];
}

size_t WaveData::getSize()
{
    return this->waveRev.size();
}

double WaveData::getMax()
{
    double max = abs(this->channels[0]);
    for(double i : this->channels){
        if(abs(i) > max){
            max = i;
        }
    }
    return max;
}

double* WaveData::getValues()
{
    return this->channels;
}
