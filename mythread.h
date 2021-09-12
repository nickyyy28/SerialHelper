#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QMainWindow>
#include <QObject>
#include <QThread>
#include "mainwindow.h"
#include "wavedata.h"

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = nullptr);
    explicit MyThread(WaveData *data, QObject *parent = nullptr);
protected:
    void run() override;

private:
    WaveData *data;
    int dataSize;

signals:
    void receiveFinished();

public slots:
    void setDataSize(int dataSize);
};

#endif // MYTHREAD_H
