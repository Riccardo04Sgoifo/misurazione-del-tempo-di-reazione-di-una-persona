#include "mainlogic.h"

void MainLogic::setLightNum(int num)
{
    if (lightNumber == num){
        return;
    }
    lightNumber = num;
    emit lightNumChanged(num);
}

void MainLogic::setCurrentLight(int value)
{
    currentLight_m = value;
    emit currentLightChanged(value);
}

void MainLogic::setAttemptNum(int value)
{
    if (attemptNum_m != value){
        attemptNum_m = value;
        emit attemptNumChanged(value);

    }

}

void MainLogic::setStartDelay(int value)
{
    if (startDelay_m != value) {
        startDelay_m = value;
        emit startDelayChanged(value);

    }

}

void MainLogic::setRandomIntervalFrom(int value)
{
    if (randomIntervalFrom_m != value){
        randomIntervalFrom_m = value;
        emit randomIntervalChangedFrom(value);

    }
}

void MainLogic::setRandomIntervalTo(int value)
{
    if (randomIntervalTo_m != value){
        randomIntervalTo_m = value;
        emit randomIntervalChangedTo(value);
    }
}



MainLogic::MainLogic()
{
    worker = new MainLogicWorker;
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &MainLogic::startAttemptsSignal, worker, &MainLogicWorker::doWork);
    connect(worker, &MainLogicWorker::lightChanged, this, &MainLogic::setCurrentLight);
    connect(this, &MainLogic::startDelayChanged, worker, &MainLogicWorker::setStartDelay);
    connect(this, &MainLogic::attemptNumChanged, worker, &MainLogicWorker::setAttemptNum);
    connect(this, &MainLogic::randomIntervalChangedFrom, worker, &MainLogicWorker::setRandomIntervalFrom);
    connect(this, &MainLogic::randomIntervalChangedTo, worker, &MainLogicWorker::setRandomIntervalTo);
    connect(this, &MainLogic::noRepsChanged, worker, &MainLogicWorker::setNoReps);
    connect(worker, &MainLogicWorker::availableSerialPortsChanged, this, &MainLogic::setAvailableSerialPorts);
    connect(worker, &MainLogicWorker::currentSerialPortsDescriptionChanged, this, &MainLogic::setCurrentSerialPortDescription);
    connect(this, &MainLogic::currentSerialPortChanged, worker, &MainLogicWorker::setCurrentSerialPort);
    connect(this, &MainLogic::connectPortSignal, worker, &MainLogicWorker::connectSerialPort);
    connect(this, &MainLogic::requestUpdateAvailableSerialPortsSignal, worker, &MainLogicWorker::updateAvailableSerialPorts);
    connect(worker, &MainLogicWorker::requestOpenErrorDialog, this, &MainLogic::requestOpenErrorDialog);
    connect(worker, &MainLogicWorker::appendScoreBoardTextLine, this, &MainLogic::appendScoreBoardTextLine);
    connect(worker, &MainLogicWorker::clearScoreBoardText, this, &MainLogic::clearScoreBoardText);
    connect(worker, &MainLogicWorker::isConnectedChanged, this, &MainLogic::setIsConnected);

    //connect worker output signal and this slopt handler
    workerThread.start();
}

MainLogic::~MainLogic()
{
    workerThread.quit();
    workerThread.wait();

    delete worker;
}

void MainLogic::startAttempts()
{
    emit startAttemptsSignal();
}

void MainLogic::connectPort()
{
    emit connectPortSignal();
}

bool MainLogic::noReps() const
{
    return m_noReps;
}

void MainLogic::setNoReps(bool newNoReps)
{
    if (m_noReps == newNoReps)
        return;
    m_noReps = newNoReps;
    emit noRepsChanged(newNoReps);
}

QList<QString> MainLogic::availableSerialPorts() const
{
    return m_availableSerialPorts;
}

void MainLogic::setAvailableSerialPorts(QList<QString> newAvailableSerialPorts)
{
    if (m_availableSerialPorts == newAvailableSerialPorts){
        return;
    }
    m_availableSerialPorts = newAvailableSerialPorts;
    emit availableSerialPortsChanged(m_availableSerialPorts);
}

void MainLogic::requestUpdateAvailableSerialPorts()
{
    emit requestUpdateAvailableSerialPortsSignal();
}

QString MainLogic::currentSerialPort() const
{
    return m_currentSerialPort;
}

void MainLogic::setCurrentSerialPort(const QString newCurrentSerialPort)
{
    if (m_currentSerialPort == newCurrentSerialPort)
        return;
    m_currentSerialPort = newCurrentSerialPort;
    emit currentSerialPortChanged(m_currentSerialPort);
}

QString MainLogic::currentSerialPortDescription() const
{
    return m_currentSerialPortDescription;
}

void MainLogic::setCurrentSerialPortDescription(const QString newCurrentSerialPortDescription)
{
    if (m_currentSerialPortDescription == newCurrentSerialPortDescription)
        return;
    m_currentSerialPortDescription = newCurrentSerialPortDescription;
    emit currentSerialPortDescriptionChanged(m_currentSerialPortDescription);
}

QString MainLogic::scoreBoardText() const
{
    return m_scoreBoardText;
}

void MainLogic::setScoreBoardText(const QString newScoreBoardText)
{
    if (m_scoreBoardText == newScoreBoardText)
        return;
    m_scoreBoardText = newScoreBoardText;
    emit scoreBoardTextChanged(m_scoreBoardText);
}

void MainLogic::appendScoreBoardTextLine(const QString line)
{
    m_scoreBoardText += "\n";
    m_scoreBoardText += line;
    emit scoreBoardTextChanged(m_scoreBoardText);
}

void MainLogic::clearScoreBoardText()
{
    m_scoreBoardText.clear();
    emit scoreBoardTextChanged(m_scoreBoardText);
}

bool MainLogic::isConnected() const
{
    return m_isConnected;
}

void MainLogic::setIsConnected(bool newIsConnected)
{
    if (m_isConnected == newIsConnected)
        return;
    m_isConnected = newIsConnected;
    emit isConnectedChanged(m_isConnected);
}
