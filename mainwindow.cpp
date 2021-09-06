#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    serial = new Serial(this);
    this->resize(QSize(951,837));
//    this->setFixedSize()

    this->setWindowIcon(QIcon(":/Image/serial.png"));
    setWindowState(Qt::WindowMaximized);

    ui->setupUi(this);
    flag = true;
    ui->clickSerial->setText("打开串口");

    ui->actionrefresh->setIcon(QIcon(":/Image/refresh.png"));

    ui->receiveText->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    ui->receiveText->setReadOnly(true);

    //波特率
//    ui->boundrateBox->addAction();
    ui->boundrateBox->addItem("110");
    ui->boundrateBox->addItem("300");
    ui->boundrateBox->addItem("600");
    ui->boundrateBox->addItem("1200");
    ui->boundrateBox->addItem("2400");
    ui->boundrateBox->addItem("4800");
    ui->boundrateBox->addItem("9600");
    ui->boundrateBox->addItem("14400");
    ui->boundrateBox->addItem("19200");
    ui->boundrateBox->addItem("38400");
    ui->boundrateBox->addItem("43000");
    ui->boundrateBox->addItem("57600");
    ui->boundrateBox->addItem("76800");
    ui->boundrateBox->addItem("115200");
    ui->boundrateBox->addItem("128000");
    ui->boundrateBox->addItem("230400");
    ui->boundrateBox->addItem("256000");
    ui->boundrateBox->addItem("460800");
    ui->boundrateBox->addItem("921600");
    ui->boundrateBox->addItem("1000000");
    ui->boundrateBox->addItem("2000000");
    ui->boundrateBox->addItem("3000000");

    ui->boundrateBox->setCurrentIndex(13);

    //停止位
    ui->stopByteBox->addItem("1");
    ui->stopByteBox->addItem("1.5");
    ui->stopByteBox->addItem("2");

    //数据位
    ui->dataByteBox->addItem("8");
    ui->dataByteBox->addItem("7");
    ui->dataByteBox->addItem("6");
    ui->dataByteBox->addItem("5");

    //校验位
    ui->checkByteBox->addItem("None");
    ui->checkByteBox->addItem("Odd");
    ui->checkByteBox->addItem("Even");

    connect(ui->actionrefresh,&QAction::triggered,[&](){
        ui->portBox->clear();
        foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
        {   //在portBox中显示可用串口
            ui->portBox->addItem(info.portName());
        }
    });

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
                ui->sendData->setEnabled(true);
                ui->clickSerial->setText("关闭串口");
                flag = !flag;
                ui->actionrefresh->setDisabled(true);
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
            flag = !flag;
        }
    });

    connect(this->serial, &Serial::openFialed, [](){
        errorPage *page = new errorPage();
        page->setWindowTitle("串口打开失败");
        //page->show();
        page->exec();
    });

    connect(this->serial, &Serial::update, this, &MainWindow::updateReceive);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateReceive()
{
//    myTextEdit ->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
    ui->receiveText->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
    this->ui->receiveText->insertPlainText(QString(*(this->serial->receive)));
    QScrollBar *scrollbar = this->ui->receiveText->verticalScrollBar();
    if(scrollbar){
        scrollbar->setSliderPosition(scrollbar->maximum());
    }
}

