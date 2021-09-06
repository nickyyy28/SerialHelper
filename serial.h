#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QIODevice>

#define getStopBit(str) (                                               \
    str == "1"   ? QSerialPort::OneStop : (                             \
    str == "1.5" ? QSerialPort::OneAndHalfStop : QSerialPort::TwoStop   \
    ))

#define getDataBit(str) (                                   \
    str == "8" ? QSerialPort::Data8 : (                     \
    str == "7" ? QSerialPort::Data7 : (                     \
    str == "6" ? QSerialPort::Data6 : QSerialPort::Data5    \
    )))

#define getParity(str) (                                                \
    str == "None" ? QSerialPort::NoParity : (                           \
    str == "Odd" ? QSerialPort::OddParity : QSerialPort::EvenParity     \
    ))

class Serial : public QObject
{
    Q_OBJECT
private:
    QSerialPort *SerialPort = nullptr;
    qint32 boundRate;
    QString portName;
    QSerialPort::DataBits dataBit;
    QSerialPort::StopBits stopBit;
    QSerialPort::Parity parity;
    QSerialPortInfo *portInfo =nullptr;
public:
    QByteArray *receive;

public:
    explicit Serial(QObject *parent = nullptr);
    void initSerial();
    void setParams(qint32 boundRate, QString portName, QSerialPort::DataBits dataBit,
                   QSerialPort::StopBits stopBit, QSerialPort::Parity parity);
    bool openSerial();
    void closeSerial();
    void loadCogfiger(const QString &configPath = nullptr);
    bool isOpen();

signals:
    void update();
    void openFialed();

};

#endif // SERIAL_H
