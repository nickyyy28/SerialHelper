#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    serial = new Serial(this);
    waveSerial = new Serial(this);


    waveData = new WaveData(this);
    arr = new QByteArray();

    this->resize(QSize(951,837));
    this->setWindowIcon(QIcon(":/Image/serial.png"));
    setWindowState(Qt::WindowMaximized);

    ui->setupUi(this);

    initWaves();

    dataTimer = new QTimer(this);

    dataTimer->setInterval(100);

//    qDebug()<< QString("start: %1").arg(timeStampStart);

//    dataTimer->start();

    //串口扫描函数
    scanTimer = new QTimer(this);
    scanTimer->setInterval(500);

    scanTimer->start();

    ui->tabWidget->setTabText(0, QString("串口助手"));
    ui->tabWidget->setTabText(1, QString("波形助手"));

    flag = true;
    wave_flag = true;
    ui->clickSerial->setText("打开串口");
    ui->openSerial->setText("打开串口");

    ui->actionrefresh->setIcon(QIcon(":/Image/refresh.png"));

    ui->receiveText->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    ui->receiveText->setReadOnly(true);


    MyThread *thread = new MyThread(this->waveData);

    thread->start();

    //波特率
    QStringList boundrateList;
    boundrateList << "110" << "300" << "600" << "1200" << "2400" << "4800" << "9600" << "14400" << "19200" << "38400" << "43000" << "57600"
                    << "76800" << "115200" << "128000" << "230400" << "256000" << "460800" << "921600" << "1000000" << "2000000" << "3000000";

    ui->boundrateBox->addItems(boundrateList);
    ui->boundrateBox_2->addItems(boundrateList);

    ui->boundrateBox->setCurrentIndex(13);
    ui->boundrateBox_2->setCurrentIndex(13);

    //停止位
    QStringList stopBitList;
    stopBitList << "1" << "1.5" << "2";

    ui->stopByteBox->addItems(stopBitList);
    ui->stopByteBox_2->addItems(stopBitList);

    //数据位
    QStringList dataBitList;
    dataBitList << "8" << "7" << "6" << "5";

    ui->dataByteBox->addItems(dataBitList);
    ui->dataByteBox_2->addItems(dataBitList);

    //校验位
    QStringList checkBitList;
    checkBitList << "None" << "Odd" << "Even";

    ui->checkByteBox->addItems(checkBitList);
    ui->checkByteBox_2->addItems(checkBitList);

    //计算FPS定时器
    QTimer *fpsTimer = new QTimer(this);
    fpsTimer->setInterval(1000);

    connect(fpsTimer, &QTimer::timeout, [this](){
       qDebug() << "fps = " << this->fps;
       this->ui->label_FPS->setNum(this->fps);
       this->fps = 0;
    });

    //开启FPS计算
    fpsTimer->start();

    maxTimer = new QTimer(this);
    maxTimer->setInterval(50);

    connect(maxTimer, &QTimer::timeout, [this](){
        if(this->maxQueue.size() > 20){
            this->maxQueue.pop_front();
        }
        this->maxQueue.push_back(this->getChannelMax());

        this->max = this->maxQueue.at(0);
        for(double item : this->maxQueue){
            if(this->max < item){
                this->max = item;
            }
        }

        if(this->max < 10) this->max = 10;

        this->ui->waveWidget->yAxis->setRange(- this->max - 5, this->max + 5);
    });

    maxTimer->start();

    //更新波形
    connect(dataTimer, &QTimer::timeout, this, [&](){
        qint64 err = QDateTime::currentMSecsSinceEpoch() - this->recerveStamp;
        qDebug() << "err = " << err;
        if(err > 100){
            this->updateWaves();
        }
    });
    connect(thread, &MyThread::receiveFinished, this, [&](){
        this->recerveStamp = QDateTime::currentMSecsSinceEpoch();
        this->updateWaves();
        this->fps++;
    });


    //点击刷新按钮
    connect(ui->actionrefresh,&QAction::triggered,[&](){
        ui->portBox->clear();
        ui->portBox_2->clear();
        foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
        {   //在portBox中显示可用串口
            ui->portBox->addItem(info.portName());
            ui->portBox_2->addItem(info.portName());
        }
    });

    connect(this->scanTimer, &QTimer::timeout, this, &MainWindow::updatePorts);

    //点击保存按钮
    connect(ui->actionsaveFile, &QAction::triggered, [](){

    });

    //串口助手点击打开/关闭按钮
    connect(ui->clickSerial,&QPushButton::clicked,[&](){
        if(flag){
            qDebug() << "串口已打开";
            ui->boundrateBox->backgroundRole();
            qint32 boundtate = ui->boundrateBox->currentText().toInt();
            QSerialPort::StopBits stopbit = getStopBit(ui->stopByteBox->currentText());
            QSerialPort::DataBits databit = getDataBit(ui->dataByteBox->currentText());
            QSerialPort::Parity parity = getParity(ui->checkByteBox->currentText());

            this->serial->setParams(boundtate, ui->portBox->currentText(), databit, stopbit, parity);

            bool isOpen = this->serial->openSerial();
            if(isOpen){
                //打开成功
                qDebug() << "打开成功!";
                ui->sendData->setEnabled(flag);
                ui->actionrefresh->setDisabled(flag);
                ui->boundrateBox->setDisabled(flag);
                ui->dataByteBox->setDisabled(flag);
                ui->stopByteBox->setDisabled(flag);
                ui->portBox->setDisabled(flag);
                ui->checkByteBox->setDisabled(flag);
                ui->clickSerial->setText("关闭串口");
                flag = !flag;


            } else {
                //打开失败
                emit this->serial->openFialed();
                ui->sendData->setDisabled(true);
            }

        } else {
            qDebug() << "串口已关闭";
            ui->actionrefresh->setDisabled(false);
            this->serial->closeSerial();
            ui->clickSerial->setText("打开串口");
            ui->sendData->setDisabled(true);

            ui->boundrateBox->setDisabled(flag);
            ui->dataByteBox->setDisabled(flag);
            ui->stopByteBox->setDisabled(flag);
            ui->portBox->setDisabled(flag);
            ui->checkByteBox->setDisabled(flag);

            flag = !flag;
        }
    });

    //波形助手点击打开/关闭按钮
    connect(ui->openSerial,&QPushButton::clicked,[&](){
        if(wave_flag){
            qDebug() << "串口已打开";
            ui->boundrateBox_2->backgroundRole();
            qint32 boundtate = ui->boundrateBox_2->currentText().toInt();
            QSerialPort::StopBits stopbit = getStopBit(ui->stopByteBox_2->currentText());
            QSerialPort::DataBits databit = getDataBit(ui->dataByteBox_2->currentText());
            QSerialPort::Parity parity = getParity(ui->checkByteBox_2->currentText());

            this->waveSerial->setParams(boundtate, ui->portBox_2->currentText(), databit, stopbit, parity);

            bool isOpen = this->waveSerial->openSerial();
            if(isOpen){
                //打开成功
                qDebug() << "打开成功!";

                this->dataTimer->start();

                this->timeStampStart = QDateTime::currentMSecsSinceEpoch();

                ui->actionrefresh->setDisabled(wave_flag);
                ui->boundrateBox_2->setDisabled(wave_flag);
                ui->dataByteBox_2->setDisabled(wave_flag);
                ui->stopByteBox_2->setDisabled(wave_flag);
                ui->portBox_2->setDisabled(wave_flag);
                ui->checkByteBox_2->setDisabled(wave_flag);
                ui->openSerial->setText("关闭串口");
                wave_flag = !wave_flag;


            } else {
                //打开失败
                emit this->waveSerial->openFialed();
            }

        } else {
            qDebug() << "串口已关闭";
            ui->actionrefresh->setDisabled(false);
            this->waveSerial->closeSerial();
            ui->openSerial->setText("打开串口");
            ui->sendData->setDisabled(true);

            dataTimer->stop();
            clearWaves();

            ui->boundrateBox_2->setDisabled(wave_flag);
            ui->dataByteBox_2->setDisabled(wave_flag);
            ui->stopByteBox_2->setDisabled(wave_flag);
            ui->portBox_2->setDisabled(wave_flag);
            ui->checkByteBox_2->setDisabled(wave_flag);

            wave_flag = !wave_flag;
        }
    });

    connect(this->serial, &Serial::openFialed, this, &MainWindow::openFialed);
    connect(this->waveSerial, &Serial::openFialed, this, &MainWindow::openFialed);

    connect(this->serial, &Serial::update, this, &MainWindow::updateReceive);
    connect(this->waveSerial, &Serial::update, this, &MainWindow::waveReceiveUpdate);

    connect(ui->clearReceive, &QPushButton::clicked, [&](){
        ui->receiveText->clear();
        this->arr->clear();
    });

    connect(ui->hexShow, &QCheckBox::clicked, [&](){
        if(ui->hexShow->isChecked()){
            ui->receiveText->clear();
            ui->receiveText->insertPlainText(QString(this->arr->toHex()).toUpper());
        } else {
            ui->receiveText->clear();
            ui->receiveText->insertPlainText(QString(*this->arr));
        }
    });


    connect(ui->sendData, &QPushButton::clicked, this, &MainWindow::sendData);

    connect(ui->clearSend, &QPushButton::clicked, [&](){
        ui->sendDataText->clear();
    });

    //设置波形使能问题
    connect(ui->enableChannel1, &QCheckBox::clicked, [&](){
        ui->enableChannel1->isChecked() ? ui->waveWidget->graph(0)->setVisible(true) : ui->waveWidget->graph(0)->setVisible(false);
    });

    connect(ui->clearWaves, &QPushButton::clicked, [&](){
        this->clearWaves();
        this->timeStampStart = QDateTime::currentMSecsSinceEpoch();
        initWaves();
        this->ui->waveWidget->replot();
    });

    connect(ui->enableAll, &QPushButton::clicked, [&](){
        ui->enableChannel1->setChecked(true);
        ui->enableChannel2->setChecked(true);
        ui->enableChannel3->setChecked(true);
        ui->enableChannel4->setChecked(true);
        ui->enableChannel5->setChecked(true);
        ui->enableChannel6->setChecked(true);
        ui->enableChannel7->setChecked(true);
        ui->enableChannel8->setChecked(true);
        ui->enableChannel9->setChecked(true);
        ui->enableChannel10->setChecked(true);
        ui->enableChannel11->setChecked(true);
        ui->enableChannel12->setChecked(true);
        ui->enableChannel13->setChecked(true);
        ui->enableChannel14->setChecked(true);
        ui->enableChannel15->setChecked(true);
        ui->enableChannel16->setChecked(true);
        SET_ALL_VISABLE;
    });

    connect(ui->reverseAll, &QPushButton::clicked, [&](){
        REVERSE_CHANNEL(1);
        REVERSE_CHANNEL(2);
        REVERSE_CHANNEL(3);
        REVERSE_CHANNEL(4);
        REVERSE_CHANNEL(5);
        REVERSE_CHANNEL(6);
        REVERSE_CHANNEL(7);
        REVERSE_CHANNEL(8);
        REVERSE_CHANNEL(9);
        REVERSE_CHANNEL(10);
        REVERSE_CHANNEL(11);
        REVERSE_CHANNEL(12);
        REVERSE_CHANNEL(13);
        REVERSE_CHANNEL(14);
        REVERSE_CHANNEL(15);
        REVERSE_CHANNEL(16);
        SET_ALL_VISABLE;
    });

    CONNECT_ALL_CHANNEL;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendData()
{
    QString data = this->ui->sendDataText->toPlainText();
    this->serial->sendData(data);
}

void MainWindow::updateReceive()
{
//    myTextEdit ->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
    ui->receiveText->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);

    this->arr->append(*this->serial->receive);


    if(ui->hexShow->isChecked()){
        this->ui->receiveText->insertPlainText(QString(this->serial->receive->toHex()).toUpper());
    } else {
        this->ui->receiveText->insertPlainText(QString(*(this->serial->receive)));
    }

    QScrollBar *scrollbar = this->ui->receiveText->verticalScrollBar();
    if(scrollbar){
        scrollbar->setSliderPosition(scrollbar->maximum());
    }
}

void MainWindow::openFialed()
{
    errorPage *page = new errorPage();
    page->setWindowTitle("串口打开失败");
    //page->show();
    page->exec();
}

void MainWindow::waveReceiveUpdate()
{
    this->waveData->appendData(*this->waveSerial->receive);
}

void MainWindow::initWaves()
{
    ui->waveWidget->rescaleAxes();

    QPen pen;
    pen.setWidth(3);

    for (int i = 0 ; i < 16 ; i++){
        ui->waveWidget->addGraph()->setVisible(false);
    }

    pen.setColor(Qt::yellow);
    ui->waveWidget->graph(0)->setPen(pen);

    pen.setColor(Qt::red);
    ui->waveWidget->graph(1)->setPen(pen);

    pen.setColor(Qt::blue);
    ui->waveWidget->graph(2)->setPen(pen);

    pen.setColor(Qt::black);
    ui->waveWidget->graph(3)->setPen(pen);

    pen.setColor(Qt::cyan);
    ui->waveWidget->graph(4)->setPen(pen);

    pen.setColor(Qt::green);
    ui->waveWidget->graph(5)->setPen(pen);

    pen.setColor(Qt::gray);
    ui->waveWidget->graph(6)->setPen(pen);

    SET_ALL_VISABLE;
}

void MainWindow::clearWaves()
{
    this->ui->waveWidget->clearGraphs();
    initWaves();
}

void MainWindow::updateWaves()
{
    double bias = (double)(QDateTime::currentMSecsSinceEpoch() - this->timeStampStart) / 1000.0;

    UPDATE_WAVE(ui->waveWidget, 1);
    UPDATE_WAVE(ui->waveWidget, 2);
    UPDATE_WAVE(ui->waveWidget, 3);
    UPDATE_WAVE(ui->waveWidget, 4);
    UPDATE_WAVE(ui->waveWidget, 5);
    UPDATE_WAVE(ui->waveWidget, 6);
    UPDATE_WAVE(ui->waveWidget, 7);
    UPDATE_WAVE(ui->waveWidget, 8);
    UPDATE_WAVE(ui->waveWidget, 9);
    UPDATE_WAVE(ui->waveWidget, 10);
    UPDATE_WAVE(ui->waveWidget, 11);
    UPDATE_WAVE(ui->waveWidget, 12);
    UPDATE_WAVE(ui->waveWidget, 13);
    UPDATE_WAVE(ui->waveWidget, 14);
    UPDATE_WAVE(ui->waveWidget, 15);
    UPDATE_WAVE(ui->waveWidget, 16);

    if(bias - 5 > 0){
        ui->waveWidget->xAxis->setRange(bias - 5 , bias + 5);
    } else {
        ui->waveWidget->xAxis->setRange(0 , bias + 5);
    }

    double max = 0;
    for(int i = 0; i < 16 ; i++){
        if(abs(this->waveData->getChannelData(i + 1)) > max){
            max = abs(this->waveData->getChannelData(i + 1));
        }
    }


//    ui->waveWidget->yAxis->setRange(-max - 5, max + 5);
//    ui->waveWidget->yAxis->setRange(-15, 15);

    ui->waveWidget->replot();
    //qDebug() << QString("timeout, channel1: %1 channel1: %2, channel1: %3").arg(this->channel1).arg(this->channel2).arg(this->channel3);
}

void MainWindow::updatePorts()
{
    QStringList comList;
    QString current = ui->portBox->currentText();
    QString current2 = ui->portBox_2->currentText();
    bool flag = false, flag2 = false;
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {   //在portBox中显示可用串口
        comList << info.portName();
        if(current == info.portName()){
            flag = true;
        }

        if(current2 == info.portName()){
            flag2 = true;
        }
    }

    ui->portBox->clear();
    ui->portBox->addItems(comList);

    ui->portBox_2->clear();
    ui->portBox_2->addItems(comList);

    if(flag){
        ui->portBox->setCurrentText(current);
    }

    if(flag2){
        ui->portBox_2->setCurrentText(current2);
    }
}

double MainWindow::getChannelMax()
{
    return this->waveData->getMax();
}
