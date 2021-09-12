#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    serial = new Serial(this);
    waveSerial = new Serial(this);
    pidSerial = new Serial(this);

    waveData = new WaveData(this);
    arr = new QByteArray();
    pidData = new QByteArray();

    this->resize(QSize(951,837));
    this->setWindowIcon(QIcon(":/Image/serial.png"));

    ui->setupUi(this);

    initWaves();

    this->tabModel = new QStandardItemModel(this);
    ui->tableView->setModel(this->tabModel);

    this->ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    this->tabModel->setHorizontalHeaderItem(0, new QStandardItem("Kp"));
    this->tabModel->setHorizontalHeaderItem(1, new QStandardItem("Ki"));
    this->tabModel->setHorizontalHeaderItem(2, new QStandardItem("Kd"));

    ui->setPID->setDisabled(true);

//    ui->tableView->setColumnWidth(0, this->ui->tableView->width() / 3);
//    ui->tableView->setColumnWidth(1, this->ui->tableView->width() / 3);
//    ui->tableView->setColumnWidth(2, this->ui->tableView->width() / 3);

    //更新波形定时器, 作用是在串口未接受到数据时也能更新波形
    dataTimer = new QTimer(this);

    //100ms为周期
    dataTimer->setInterval(100);

//    qDebug()<< QString("start: %1").arg(timeStampStart);

//    dataTimer->start();

    //串口扫描函数
    scanTimer = new QTimer(this);
    scanTimer->setInterval(1000);

    scanTimer->start();

    ui->tabWidget->setTabText(0, QString("串口助手"));
    ui->tabWidget->setTabText(1, QString("波形助手"));
    ui->tabWidget->setTabText(2, QString("PID调试助手"));

    flag = true;
    wave_flag = true;
    pid_flag = true;
    ui->clickSerial->setText("打开串口");
    ui->openSerial->setText("打开串口");
    ui->openSerial_2->setText("打开串口");


    ui->actionrefresh->setIcon(QIcon(":/Image/refresh.png"));

    ui->receiveText->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    ui->receiveText->setReadOnly(true);


    MyThread *thread = new MyThread(this->waveData);

    thread->start();

    //数据类型
    QStringList typeList;
    typeList << "int8" << "int16" << "float32" << "float64";

    ui->dataType->addItems(typeList);
    ui->dataType->setCurrentIndex(3);

    thread->setDataSize(8);

    this->dataType.insert("int8", 1);
    this->dataType.insert("int16", 2);
    this->dataType.insert("float32", 4);
    this->dataType.insert("float64", 8);

    //波特率
    QStringList boundrateList;
    boundrateList << "110" << "300" << "600" << "1200" << "2400" << "4800" << "9600" << "14400" << "19200" << "38400" << "43000" << "57600"
                    << "76800" << "115200" << "128000" << "230400" << "256000" << "460800" << "921600" << "1000000" << "2000000" << "3000000";

    ui->boundrateBox->addItems(boundrateList);
    ui->boundrateBox_2->addItems(boundrateList);
    ui->boundrateBox_3->addItems(boundrateList);

    ui->boundrateBox->setCurrentIndex(13);
    ui->boundrateBox_2->setCurrentIndex(13);
    ui->boundrateBox_3->setCurrentIndex(13);

    //停止位
    QStringList stopBitList;
    stopBitList << "1" << "1.5" << "2";

    ui->stopByteBox->addItems(stopBitList);
    ui->stopByteBox_2->addItems(stopBitList);
    ui->stopByteBox_3->addItems(stopBitList);

    //数据位
    QStringList dataBitList;
    dataBitList << "8" << "7" << "6" << "5";

    ui->dataByteBox->addItems(dataBitList);
    ui->dataByteBox_2->addItems(dataBitList);
    ui->dataByteBox_3->addItems(dataBitList);

    //校验位
    QStringList checkBitList;
    checkBitList << "None" << "Odd" << "Even";

    ui->checkByteBox->addItems(checkBitList);
    ui->checkByteBox_2->addItems(checkBitList);
    ui->checkByteBox_3->addItems(checkBitList);

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

    //波形最大值计算定时器
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

    //开始计算最大值
    maxTimer->start();

    //定时器更新波形
    connect(dataTimer, &QTimer::timeout, this, [&](){
        qint64 err = QDateTime::currentMSecsSinceEpoch() - this->recerveStamp;
//        qDebug() << "err = " << err;
        if(err > 100){
            this->updateWaves();
        }
    });

    //收数据更新波形
    connect(thread, &MyThread::receiveFinished, this, [&](){
        this->recerveStamp = QDateTime::currentMSecsSinceEpoch();
        this->updateWaves();
        this->fps++;
    });

    connect(ui->addLine, &QPushButton::clicked, [this](){
        this->tabModel->setItem(this->lineNum, 0, new QStandardItem(""));
        this->tabModel->setItem(this->lineNum, 1, new QStandardItem(""));
        this->tabModel->setItem(this->lineNum, 2, new QStandardItem(""));
        this->lineNum++;
    });

    connect(ui->subLine, &QPushButton::clicked, [this](){
        if(this->lineNum > 0){
            this->tabModel->removeRow(this->lineNum - 1);
            this->lineNum--;
        }
    });

    connect(ui->setPID, &QPushButton::clicked, this, &MainWindow::setPID);

    //点击刷新按钮
    connect(ui->actionrefresh,&QAction::triggered,[&](){
        ui->portBox->clear();
        ui->portBox_2->clear();
        foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
        {   //在portBox中显示可用串口
            ui->portBox->addItem(info.portName());
            ui->portBox_2->addItem(info.portName());
            ui->portBox_3->addItem(info.portName());
        }
    });

    connect(this->scanTimer, &QTimer::timeout, this, &MainWindow::updatePorts);

    //点击保存按钮
    connect(ui->actionsaveFile, &QAction::triggered, [](){

    });

    connect(ui->actiongithub, &QAction::triggered, [](){
        QDesktopServices::openUrl(QUrl(QLatin1String("https://github.com/nickyyy28/SerialHelper")));
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

    //PID调试助手点击打开/关闭按钮
    connect(ui->openSerial_2,&QPushButton::clicked,[&](){
        if(pid_flag){
            qDebug() << "串口已打开";
            ui->boundrateBox_3->backgroundRole();
            qint32 boundtate = ui->boundrateBox_3->currentText().toInt();
            QSerialPort::StopBits stopbit = getStopBit(ui->stopByteBox_3->currentText());
            QSerialPort::DataBits databit = getDataBit(ui->dataByteBox_3->currentText());
            QSerialPort::Parity parity = getParity(ui->checkByteBox_3->currentText());

            this->pidSerial->setParams(boundtate, ui->portBox_3->currentText(), databit, stopbit, parity);

            bool isOpen = this->pidSerial->openSerial();
            if(isOpen){
                //打开成功
                qDebug() << "打开成功!";
                ui->setPID->setDisabled(false);

                ui->actionrefresh->setDisabled(pid_flag);
                ui->boundrateBox_3->setDisabled(pid_flag);
                ui->dataByteBox_3->setDisabled(pid_flag);
                ui->stopByteBox_3->setDisabled(pid_flag);
                ui->portBox_3->setDisabled(pid_flag);
                ui->checkByteBox_3->setDisabled(pid_flag);
                ui->openSerial_2->setText("关闭串口");
                pid_flag = !pid_flag;


            } else {
                //打开失败
                emit this->pidSerial->openFialed();
            }

        } else {
            qDebug() << "串口已关闭";

            ui->setPID->setDisabled(true);
            ui->actionrefresh->setDisabled(false);
            this->pidSerial->closeSerial();
            ui->openSerial_2->setText("打开串口");

            ui->boundrateBox_3->setDisabled(pid_flag);
            ui->dataByteBox_3->setDisabled(pid_flag);
            ui->stopByteBox_3->setDisabled(pid_flag);
            ui->portBox_3->setDisabled(pid_flag);
            ui->checkByteBox_3->setDisabled(pid_flag);

            pid_flag = !pid_flag;
        }
    });

    connect(this->serial, &Serial::openFialed, this, &MainWindow::openFialed);
    connect(this->waveSerial, &Serial::openFialed, this, &MainWindow::openFialed);
    connect(this->pidSerial, &Serial::openFialed, this, &MainWindow::openFialed);

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

    connect(ui->clearWaves, &QPushButton::clicked, [&](){
        this->clearWaves();
        this->timeStampStart = QDateTime::currentMSecsSinceEpoch();
        initWaves();
        this->ui->waveWidget->replot();
    });

    connect(ui->enableAll, &QPushButton::clicked, [&](){
        SET_ALL_CHANNEL(ENABLE);
        SET_ALL_VISABLE;
    });

    connect(ui->reverseAll, &QPushButton::clicked, [&](){
        SET_ALL_CHANNEL(REVERSE);
        SET_ALL_VISABLE;
    });

    connect(ui->cancelEnableAll, &QPushButton::clicked, [&](){
        SET_ALL_CHANNEL(DISABLE);
        SET_ALL_VISABLE;
    });

    connect(this, &MainWindow::setDataType, thread, &MyThread::setDataSize);

    connect(ui->dataType, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [&](){
        emit this->setDataType(this->dataType.value(this->ui->dataType->currentText()));
        qDebug() << "aaaa";
    });

    //设置波形使能信号处理函数
    CONNECT_ALL_CHANNEL;

    DISABLE_ALL_VALUE;


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

    //设置所有通道的值
    SET_ALL_VALUES(this->waveData->getValues());

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
    QString current3 = ui->portBox_3->currentText();
    bool flag = false, flag2 = false, flag3 = false;
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {   //在portBox中显示可用串口
        comList << info.portName();
        if(current == info.portName()){
            flag = true;
        }

        if(current2 == info.portName()){
            flag2 = true;
        }

        if(current3 == info.portName()){
            flag3 = true;
        }
    }

    ui->portBox->clear();
    ui->portBox->addItems(comList);

    ui->portBox_2->clear();
    ui->portBox_2->addItems(comList);

    ui->portBox_3->clear();
    ui->portBox_3->addItems(comList);

    if(flag){
        ui->portBox->setCurrentText(current);
    }

    if(flag2){
        ui->portBox_2->setCurrentText(current2);
    }

    if(flag3){
        ui->portBox_3->setCurrentText(current3);
    }
}

void MainWindow::setPID()
{
    pidData->clear();
    pidData->push_back('\x5A');

    for(int row = 0 ; row < this->lineNum ; row++){
        for(int column = 0 ; column < 3 ; column++){
            bool isOk;
            qDebug() << "row = " << row << " column = " << column;
            qDebug() << "test = " << this->tabModel->item(row, column)->text();
            float item = this->tabModel->item(row, column)->text().toFloat(&isOk);
            qDebug() << "test = " << this->tabModel->item(row, column)->text() << "number = " << item;
            if(isOk){
                qDebug() << "aaaa";
                QByteArray temp;
                temp.resize(sizeof(item));
                memcpy(temp.data(), &item, sizeof(item));
                pidData->append(temp);
            } else {
                qDebug() << QString("the row %1 , column %2 is not a float number").arg(row + 1).arg(column + 1);
                errorpage_pid *page = new errorpage_pid();
                page->setWindowTitle("数据错误");
                page->setRowAndColumn(row + 1, column + 1);
                page->exec();
                return;
            }
        }
    }

    qDebug() << "当前有" << this->lineNum << "行";


    pidData->push_back('\xA5');

    qDebug() << "send" << pidData->size() << "byte data";

    this->pidSerial->sendData(*pidData);
}

double MainWindow::getChannelMax()
{
    return this->waveData->getMax();
}
