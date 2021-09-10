#ifndef WAVEDATA_H
#define WAVEDATA_H

#include <QObject>
#include <QMutex>
#include <QThread>


class WaveData : public QObject
{
    Q_OBJECT
public:
    explicit WaveData(QObject *parent = nullptr);
    void lock();
    void unlock();
    bool processData();
    void appendData(const QByteArray &data);
    double getChannelData(int index = 1);
    size_t getSize();
    double getMax();

private:
    QMutex m_mutex;
    QByteArray waveRev;

    double channels[16] = {0};

public:


signals:

};

#endif // WAVEDATA_H
