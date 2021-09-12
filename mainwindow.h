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
#include <QMap>
#include <QDesktopServices>
#include <QUrl>
#include <QStandardItemModel>

#include "serial.h"
#include "errorpage.h"
#include "mythread.h"
#include "wavedata.h"
#include "errorpage_pid.h"

#define POP_FRONT_N(A, N)         \
for(size_t i = 0; i < N ; i++){   \
    A->pop_front();                \
}

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

//设置通道失能
#define CHANNEL_DISABLE(INDEX) ui->enableChannel##INDEX->setChecked(false)

//设置通道使能
#define CHANNEL_ENABLE(INDEX) ui->enableChannel##INDEX->setChecked(true)

//设置通道使能反转
#define CHANNEL_REVERSE(INDEX) ui->enableChannel##INDEX->isChecked() == true ? ui->enableChannel##INDEX->setChecked(false) : ui->enableChannel##INDEX->setChecked(true)

//根据动作(ENABLE/DISABLE/REVERSE)设置所有通道状态
#define SET_ALL_CHANNEL(ACTION) \
    CHANNEL_##ACTION(1);        \
    CHANNEL_##ACTION(2);        \
    CHANNEL_##ACTION(3);        \
    CHANNEL_##ACTION(4);        \
    CHANNEL_##ACTION(5);        \
    CHANNEL_##ACTION(6);        \
    CHANNEL_##ACTION(7);        \
    CHANNEL_##ACTION(8);        \
    CHANNEL_##ACTION(9);        \
    CHANNEL_##ACTION(10);        \
    CHANNEL_##ACTION(12);        \
    CHANNEL_##ACTION(13);        \
    CHANNEL_##ACTION(14);        \
    CHANNEL_##ACTION(15);        \
    CHANNEL_##ACTION(16)

//连接通道时能使能信号
#define CONNECT_CHANNEL_ENABLE(I)   \
    connect(ui->enableChannel##I, &QCheckBox::clicked, [&](){   \
        ui->enableChannel##I->isChecked() ? ui->waveWidget->graph(I - 1)->setVisible(true) : ui->waveWidget->graph(I - 1)->setVisible(false);   \
    })

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

#define SET_VALUE(CHANNEL, VALUE) this->ui->value##CHANNEL->setText(QString("%1").arg(VALUE))

#define SET_ALL_VALUES(VALUES)  \
    SET_VALUE(1, VALUES[0]);    \
    SET_VALUE(2, VALUES[1]);    \
    SET_VALUE(3, VALUES[2]);    \
    SET_VALUE(4, VALUES[3]);    \
    SET_VALUE(5, VALUES[4]);    \
    SET_VALUE(6, VALUES[5]);    \
    SET_VALUE(7, VALUES[6]);    \
    SET_VALUE(8, VALUES[7]);    \
    SET_VALUE(9, VALUES[8]);    \
    SET_VALUE(10, VALUES[9]);   \
    SET_VALUE(11, VALUES[10]);  \
    SET_VALUE(12, VALUES[11]);  \
    SET_VALUE(13, VALUES[12]);  \
    SET_VALUE(14, VALUES[13]);  \
    SET_VALUE(15, VALUES[14]);  \
    SET_VALUE(16, VALUES[15])   \

//设置单个通道文本框只读
#define DISABLE_VALUE(CHANNEL)  this->ui->value##CHANNEL->setReadOnly(true)

//设置所有通道文本框只读
#define DISABLE_ALL_VALUE   \
    DISABLE_VALUE(1);       \
    DISABLE_VALUE(2);       \
    DISABLE_VALUE(3);       \
    DISABLE_VALUE(4);       \
    DISABLE_VALUE(5);       \
    DISABLE_VALUE(6);       \
    DISABLE_VALUE(7);       \
    DISABLE_VALUE(8);       \
    DISABLE_VALUE(9);       \
    DISABLE_VALUE(10);       \
    DISABLE_VALUE(11);       \
    DISABLE_VALUE(12);       \
    DISABLE_VALUE(13);       \
    DISABLE_VALUE(14);       \
    DISABLE_VALUE(15);       \
    DISABLE_VALUE(16)

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
    Serial *pidSerial;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initWaves();
    double getChannelMax();

signals:
    void setDataType(int dataType);

public slots:
    void updateReceive();
    void sendData();
    void openFialed();
    void waveReceiveUpdate();
    void clearWaves();
    void updateWaves();
    void updatePorts();
    void setPID();

private:
    QTimer *dataTimer;
    QTimer *scanTimer;
    bool flag;
    bool wave_flag;
    bool pid_flag;
    Ui::MainWindow *ui;
    qint64 timeStampStart;
    qint64 recerveStamp;

    QByteArray *arr;
    QByteArray *pidData;
    WaveData *waveData;

    QMutex m_mutex;
    double curMax;

    QQueue<double> maxQueue;
    QTimer *maxTimer;

    double max;

    int lineNum = 0;

    int fps = 0;

    QMap<QString, int> dataType;

    QStandardItemModel *tabModel;

};
#endif // MAINWINDOW_H
