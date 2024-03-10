#include "mainlogicworker.h"


void MainLogicWorker::setNoReps(bool newNoReps)
{
    if (noReps == newNoReps)
        return;
    noReps = newNoReps;
}

void MainLogicWorker::setCurrentStimulationMode(QString value)
{
    if (value == currentStimulationMode) {
        return;
    }
    currentStimulationMode = value;
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
    serial->setBaudRate(9600 );
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    qDebug() << "paramenters setted";
    if (!serial->open(QSerialPort::QIODevice::ReadWrite)){
        qDebug() << "can't open serial port" << serial->error();
        return;
    }
    serial->setDataTerminalReady(true);
    qDebug() << "data terminal ready";


    qDebug() << "opened serial port";

    isConnected = true;
    emit isConnectedChanged(isConnected);
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

QByteArray MainLogicWorker::getMessage(char command, unsigned int from, unsigned int to, char mode, unsigned int attempts, unsigned int sensors)
{


    char data[12] = {0};

    data[0] |= command;

    data[4] |= from & 0x000000ff;
    data[3] |= (from & 0x0000ff00) >> 8;
    data[2] |= (from & 0x00ff0000) >> 16;
    data[1] |= (from & 0xff000000) >> 24;

    data[8] |= from & 0x000000ff;
    data[7] |= (from & 0x0000ff00) >> 8;
    data[6] |= (from & 0x00ff0000) >> 16;
    data[5] |= (from & 0xff000000) >> 24;

    data[9] |= mode;

    data[10] |= attempts & 0x000000ff;

    data[11] |= sensors & 0x000000ff;

    QByteArray message = QByteArray(data, sizeof(data));

    return message;
}


void MainLogicWorker::attemptFinished()
{
    emit lightChanged(-1);
    // for backUp and to make the listView work

    float mean = 0;

    for (int i = 0; i < currentAttempts.size(); i++) {
        mean += currentAttempts[i];
    }

    mean /= (float)currentAttempts.size();

    emit requestSaveAttempt(currentAttempts, (int)mean);
    // enables start button and disables stop button
    emit isRunningChanged(false);
}


char MainLogicWorker::getStimulationModeForMessage(){


    if (QList<QString> {"Visual", "visual", "Visuale", "visuale", "V", "v"}.indexOf(currentStimulationMode) >= 0 ){
        return VISUAL_MODE;
    }
    if (QList<QString> {"Auditory", "auditory", "Auditiva", "auditiva", "A", "a"}.indexOf(currentStimulationMode) >= 0 ){
        return AUDIO_MODE;
    }

    // default
    return COMBINED_MODE;

}

void MainLogicWorker::doWork()
{

    // check if board is open
    if (!serial->isOpen()){
        emit requestOpenErrorDialog("tried to start the loop without connection to the board");
        return;
    }

    // start on esp8266

    QThread::msleep(startDelay);

    serial->readAll();

    const QByteArray dataToSendBytes = getMessage(START_LOOP, randomIntervalFrom, randomIntervalTo, getStimulationModeForMessage(), attemptNum, lightNum);

    serial->write(dataToSendBytes);


    if (!serial->waitForBytesWritten(100)){
        emit requestOpenErrorDialog("Time out while writing to serial");
    }


    if (!serial->waitForReadyRead(100)){
        emit requestOpenErrorDialog("Time out while waiting for a response");
    }

    QByteArray dataReceived = serial->readAll();

    if (dataReceived[0] != COPY) { // ok code
        emit requestOpenErrorDialog("can't start loop on esp8266");
        emit appendScoreBoardTextLine(QString(dataReceived));
        return;
    }

    // everything went well up to here so the attempt is started

    // disables start button and enables stop button
    emit isRunningChanged(true);

    bool run = true;
    int i = 0;

    while (run) {

        if (shallStop){
            shallStop = false;

            // stop, set mode to visual to hopefully stop the noise
            const QByteArray stopMessageBytes = getMessage(BREAK_LOOP, 0, 0, STIMULATION_MODE_VISUAL, 0, 0);

            serial->write(stopMessageBytes);

            if (!serial->waitForBytesWritten(100)){
                emit requestOpenErrorDialog("Time out while writing to serial");
            }


            if (!serial->waitForReadyRead(100)){
                emit requestOpenErrorDialog("Time out while waiting for a response");
            }

            QByteArray dataReceived = serial->readAll();

            if (dataReceived[0] != COPY) { // ok code
                emit requestOpenErrorDialog("can't stop loop on esp8266");
                emit appendScoreBoardTextLine(QString(dataReceived));
                // tell mainLogic that the attempt is finished (not in the best way)
                attemptFinished();
                return;
            }

            break;
        }

        if (serial->bytesAvailable() <= 0){
            if (!serial->waitForReadyRead(10000)){
                emit requestOpenErrorDialog("Time out while waiting for a response");
                qDebug() << (QString(serial->readAll()));
            }
        }

        qDebug() << "WHATTTT";

        dataReceived = serial->read(1);

        qDebug() << QString("command is %1").arg(dataReceived[0]);

        if (dataReceived[0] == REACTION_TIME_PRINT) {
            qDebug() << "Into the loop...";
            while (serial->bytesAvailable() < REACTION_TIME_MESSAGE_LENGTH - 1) { } // wait for 4 bytes (reaction time unsigned int)
            qDebug() << "Out of the loop!";
            QByteArray dataReceived = serial->read(REACTION_TIME_MESSAGE_LENGTH - 1);
            qDebug() << dataReceived[0];
            qDebug() << dataReceived[1];
            qDebug() << dataReceived[2];
            qDebug() << dataReceived[3];

            unsigned char *dr = new unsigned char[REACTION_TIME_MESSAGE_LENGTH - 1];
            std::memcpy(dr, dataReceived.constData(), REACTION_TIME_MESSAGE_LENGTH - 1); // using the QByteArray gives 0xffffff.. for numbers lower than 0xff ?*/

            //char *dr = new char[REACTION_TIME_MESSAGE_LENGTH - 1] {0};
            //qDebug() << "reaction time:";

            //qDebug() << serial->read(dr, REACTION_TIME_MESSAGE_LENGTH - 1);

            uint32_t reactionTime = 0u;

            reactionTime |= dr[0];
            reactionTime <<= 8;
            reactionTime |= dr[1];
            reactionTime <<= 8;
            reactionTime |= dr[2];
            reactionTime <<= 8;
            reactionTime |= dr[3];

            delete[] dr;

            i++;
            emit appendScoreBoardTextLine(QString("tentativo %1:  %2ms").arg(i).arg(reactionTime));
            emit lightChanged(-1); // turn off lights
        }

        else if (dataReceived[0] == FINISHED){
            qDebug() << "received finish signal";
            run = false;
            emit appendScoreBoardTextLine(QString("finish"));
        }

        else if (dataReceived[0] == ACTIVE_SENSOR){
            dataReceived = QByteArray();
            while (serial->bytesAvailable() < ACTIVE_SENSOR_MESSAGE_LENGTH - 1) { }
            dataReceived = serial->read(ACTIVE_SENSOR_MESSAGE_LENGTH - 1);

            unsigned char* msg = new unsigned char[ACTIVE_SENSOR_MESSAGE_LENGTH - 1];
            std::memcpy(msg, dataReceived.constData(), ACTIVE_SENSOR_MESSAGE_LENGTH - 1);

            uint8_t activeSensor = msg[0];

            delete[] msg;

            emit lightChanged((int) activeSensor + 1);
        }

        else{
            qDebug() << QString("Unrecognized command %1").arg(dataReceived[0]);
            qDebug() << (QString(serial->readAll()));
        }
    }

    // tell mainLogic that the attempt is finished
    attemptFinished();


}

void MainLogicWorker::setLightNum(int value)
{
    if (lightNum != value) {
        lightNum = value;
        emit lightNumChanged(value);

    }
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
