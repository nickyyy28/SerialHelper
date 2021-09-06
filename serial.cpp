#include "serial.h"

Serial::Serial(QObject *parent) : QObject(parent)
{
    this->SerialPort = new QSerialPort(this);
    receive = new QByteArray;

    initSerial();
}

void Serial::initSerial()
{
    connect(this->SerialPort, &QSerialPort::readyRead, [&](){
        receive->clear();
        receive->append(this->SerialPort->readAll());
        emit update();
    });

}

void Serial::setParams(qint32 boundRate, QString portName, QSerialPort::DataBits dataBit,
                       QSerialPort::StopBits stopBit, QSerialPort::Parity parity)
{
    this->boundRate = boundRate;
    this->portName = portName;
    this->dataBit = dataBit;
    this->stopBit = stopBit;
    this->parity = parity;
}

bool Serial::openSerial()
{
    this->SerialPort->setBaudRate(this->boundRate); /*set bound rate*/
    this->SerialPort->setPortName(this->portName);  /*set port name*/
    this->SerialPort->setDataBits(this->dataBit);   /*set data bit*/
    this->SerialPort->setStopBits(this->stopBit);   /*set stop bit*/
    this->SerialPort->setParity(this->parity);      /*set parity*/

    return this->SerialPort->open(QIODevice::ReadWrite);
}

void Serial::loadCogfiger(const QString &configPath)
{

}

bool Serial::isOpen()
{
    return this->SerialPort->isOpen();
}

void Serial::closeSerial()
{
    if(isOpen()){
        this->SerialPort->close();
    }
}
