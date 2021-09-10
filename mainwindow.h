#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTextEdit>
#include <QDialog>
#include <QDebug>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QList>
#include <QComboBox>
#include <QScrollBar>
#include <QThread>
#include <QMutex>
#include <QtMath>
#include <QQueue>

#include "serial.h"
#include "errorpage.h"
#include "mythread.h"
#include "wavedata.h"

#define POP_FRONT_N(A, N)         \
for(size_t i = 0; i < N ; i++){   \
    A->pop_front();                \
}

#define CONNECT_CHANNEL_ENABLE(I)   \
    connect(ui->enableChannel##I, &QCheckBox::clicked, [&](){   \
        ui->enableChannel##I->isChecked() ? ui->waveWidget->graph(I - 1)->setVisible(true) : ui->waveWidget->graph(I - 1)->setVisible(false);   \
    })

#define SET_VISABLE(INDEX) ui->enableChannel##INDEX->isChecked() ? ui->waveWidget->graph(INDEX - 1)->setVisible(true) : ui->waveWidget->graph(INDEX - 1)->setVisible(false)

#define SET_ALL_VISABLE \
    SET_VISABLE(1);     \
    SET_VISABLE(2);     \
    SET_VISABLE(3);     \
    SET_VISABLE(4);     \
    SET_VISABLE(5);     \
    SET_VISABLE(6);     \
    SET_VISABLE(7);     \
    SET_VISABLE(8);     \
    SET_VISABLE(9);     \
    SET_VISABLE(10);     \
    SET_VISABLE(11);     \
    SET_VISABLE(12);     \
    SET_VISABLE(13);     \
    SET_VISABLE(14);     \
    SET_VISABLE(15);     \
    SET_VISABLE(16)

#define REVERSE_CHANNEL(INDEX) ui->enableChannel##INDEX->isChecked() == true ? ui->enableChannel##INDEX->setChecked(false) : ui->enableChannel##INDEX->setChecked(true)

#define CONNECT_ALL_CHANNEL \
    CONNECT_CHANNEL_ENABLE(1);  \
    CONNECT_CHANNEL_ENABLE(2);  \
    CONNECT_CHANNEL_ENABLE(3);  \
    CONNECT_CHANNEL_ENABLE(4);  \
    CONNECT_CHANNEL_ENABLE(5);  \
    CONNECT_CHANNEL_ENABLE(6);  \
    CONNECT_CHANNEL_ENABLE(7);  \
    CONNECT_CHANNEL_ENABLE(8);  \
    CONNECT_CHANNEL_ENABLE(9);  \
    CONNECT_CHANNEL_ENABLE(10); \
    CONNECT_CHANNEL_ENABLE(11); \
    CONNECT_CHANNEL_ENABLE(12); \
    CONNECT_CHANNEL_ENABLE(13); \
    CONNECT_CHANNEL_ENABLE(14); \
    CONNECT_CHANNEL_ENABLE(15); \
    CONNECT_CHANNEL_ENABLE(16)  \

#define UPDATE_WAVE(WIDGHT, CHANNEL)    \
    WIDGHT->graph(CHANNEL - 1)->addData(bias, this->waveData->getChannelData(CHANNEL))

QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    Serial *serial;
    Serial *waveSerial;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initWaves();
    double getChannelMax();

public slots:
    void updateReceive();
    void sendData();
    void openFialed();
    void waveReceiveUpdate();
    void clearWaves();
    void updateWaves();
    void updatePorts();

private:
    QTimer *dataTimer;
    QTimer *scanTimer;
    bool flag;
    bool wave_flag;
    Ui::MainWindow *ui;
    qint64 timeStampStart;
    qint64 recerveStamp;

    QByteArray *arr;
    WaveData *waveData;

    QMutex m_mutex;
    double curMax;
    double rangeMax;

    QQueue<double> maxQueue;
    QTimer *maxTimer;

    double max;

    int fps = 0;

};
#endif // MAINWINDOW_H
