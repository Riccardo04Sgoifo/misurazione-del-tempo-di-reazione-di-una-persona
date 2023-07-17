#include "mainlogicworker.h"


void MainLogicWorker::setNoReps(bool newNoReps)
{
    if (noReps == newNoReps)
        return;
    noReps = newNoReps;
}

void MainLogicWorker::setCurrentSerialPort(QString value)
{
    if(value == currentSerialPort){
        return;
    }
    currentSerialPort = value;

    currentSerialPortDescription = availableSerialPortsDescription[availableSerialPorts.indexOf(currentSerialPort)];
    emit currentSerialPortsDescriptionChanged(currentSerialPortDescription);
}

void MainLogicWorker::connectSerialPort() {
    qDebug() << "started connection function";
    if (currentSerialPort == QString("")){
        return;
    }
    qDebug() << "current serial port is not null" << currentSerialPort;

    delete serial;
    serial = new QSerialPort();


    serial->setPortName(currentSerialPort);
    qDebug() << "port name setted";
    serial->setBaudRate(115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    qDebug() << "paramenters setted";
    if (!serial->open(QSerialPort::QIODevice::ReadWrite)){
        qDebug() << "can't open serial port" << serial->error();
    }
    serial->setDataTerminalReady(true);
    qDebug() << "data terminal ready";


    qDebug() << "opened serial port";
}

void MainLogicWorker::updateAvailableSerialPorts() {
    QList<QSerialPortInfo> serialPorts = QSerialPortInfo::availablePorts();

    availableSerialPorts.clear();
    availableSerialPortsDescription.clear();

    for (int i = 0; i < serialPorts.size(); i++){
        availableSerialPorts.append(serialPorts[i].portName());
        availableSerialPortsDescription.append(serialPorts[i].description());
    }

    emit availableSerialPortsChanged(availableSerialPorts);


}

MainLogicWorker::~MainLogicWorker()
{
    serial->close();
    delete serial;
}

MainLogicWorker::MainLogicWorker(QObject *parent)
    : QObject{parent}
{
    updateAvailableSerialPorts();
    serial = new QSerialPort();
}

void MainLogicWorker::doWork()
{
    // check if board is open
    if (!serial->isOpen()){
        emit requestOpenErrorDialog("tried to start the loop without connection to the board");
        return;
    }
    // start on esp32
    QThread::msleep(startDelay);

    QString dataToSend = "dattta";

    const QByteArray dataToSendBytes = dataToSend.toUtf8();



    serial->write(dataToSendBytes);

    if (!serial->waitForBytesWritten(100)){
        emit requestOpenErrorDialog("Time out while writing to serial");
    }

    int prevLight = -1;

    for (int i = 0; i < attemptNum; i++){

        int interval= int(QRandomGenerator::global()->generateDouble() * double(randomIntervalTo - randomIntervalFrom)) + randomIntervalFrom;
        QThread::msleep(interval);

        int s = 0;
        int randomLight = prevLight;

        while (randomLight == prevLight){
            randomLight = QRandomGenerator::global()->bounded(4);

            s++;
            if (s > MAX_STEPS || !noReps) break;
        }

        prevLight = randomLight;
        emit lightChanged(randomLight);

        QThread::msleep(200); // simulate response time


    }

    emit lightChanged(-1);

}

void MainLogicWorker::setAttemptNum(int value)
{
    if (attemptNum != value) {
        attemptNum = value;
        emit attemptNumChanged(value);

    }
}

void MainLogicWorker::setStartDelay(int value)
{
    if (startDelay != value){
        startDelay = value;
        emit startDelayChanged(value);


    }
}

void MainLogicWorker::setRandomIntervalFrom(int value)
{
    if (randomIntervalFrom != value){
        randomIntervalFrom = value;
        emit randomIntervalChangedFrom(value);

    }
}

void MainLogicWorker::setRandomIntervalTo(int value)
{
    if (randomIntervalTo != value){
        randomIntervalTo = value;
        emit randomIntervalChangedTo(value);
    }
}
