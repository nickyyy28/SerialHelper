#include "mythread.h"

MyThread::MyThread(QObject *parent) : QThread(parent)
{

}

void MyThread::run()
{
    while(1){
        if(this->data->getSize() < 130){
            continue;
        }
        bool res = this->data->processData();
        if(res){
            emit receiveFinished();
        }
        //qDebug() << QString("ch1: %1, ch2:%2, ch3:%3").arg(this->data->getChannelData(1)).arg(this->data->getChannelData(2)).arg(this->data->getChannelData(3));
    }
}

MyThread::MyThread(WaveData *data, QObject *parent) : QThread(parent)
{
    this->data = data;
}
