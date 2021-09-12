#include "mythread.h"

MyThread::MyThread(QObject *parent) : QThread(parent)
{

}

void MyThread::run()
{
    while(1){
        if(this->data->getSize() < (2 + 16 * this->dataSize)){
            continue;
        }
        bool res = this->data->processData(this->dataSize);
        if(res){
            emit receiveFinished();
        }
        //qDebug() << QString("ch1: %1, ch2:%2, ch3:%3").arg(this->data->getChannelData(1)).arg(this->data->getChannelData(2)).arg(this->data->getChannelData(3));
        qDebug() << "dataSize : " << this->dataSize;
    }
}

void MyThread::setDataSize(int dataSize)
{
    this->dataSize = dataSize;
}

MyThread::MyThread(WaveData *data, QObject *parent) : QThread(parent)
{
    this->data = data;
}
