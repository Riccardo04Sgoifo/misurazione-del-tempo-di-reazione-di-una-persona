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
    connect(this, &MainLogic::startDelayChanged, worker, &MainLogicWorker::setStartDelay);
    connect(this, &MainLogic::requestUpdateAvailableSerialPortsSignal, worker, &MainLogicWorker::updateAvailableSerialPorts);
    connect(worker, &MainLogicWorker::requestOpenErrorDialog, this, &MainLogic::requestOpenErrorDialog);
    connect(worker, &MainLogicWorker::appendScoreBoardTextLine, this, &MainLogic::appendScoreBoardTextLine);
    connect(worker, &MainLogicWorker::clearScoreBoardText, this, &MainLogic::clearScoreBoardText);
    connect(worker, &MainLogicWorker::isConnectedChanged, this, &MainLogic::setIsConnected);
    connect(worker, &MainLogicWorker::isRunningChanged, this, &MainLogic::setIsRunning);
    connect(worker, &MainLogicWorker::requestSaveAttempt, this, &MainLogic::saveAttempt);
    connect(&saveManager, &SaveManager::attemptNamesChanged, this, &MainLogic::setAttemptNames);
    connect(this, &MainLogic::attemptNamesChanged, &saveManager, &SaveManager::setAttemptNames);
    connect(this, &MainLogic::currentStimulationModeChanged, worker, &MainLogicWorker::setCurrentStimulationMode);

    //connect worker output signal and this slopt handler
    workerThread.start();

    // update qml stuff

}

MainLogic::~MainLogic()
{
    workerThread.quit();
    workerThread.wait();

    delete worker;
}

void MainLogic::startAttempts()
{

    // starts from 0 so first is 1
    currentAttemptNum++;
    currentAttemptName = QString("Prova %1").arg(currentAttemptNum);

    // adds name to save manager by saving a blank attempt

    saveManager.writeAttempt(AttemptData(), currentAttemptName, currentAttemptName);
    // update attemptNames
    emit attemptNamesChanged(m_attemptNames);
    // clear score board
    clearScoreBoardText();
    // write name on scoreBoard
    appendScoreBoardTextLine(currentAttemptName + "\n");

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

void MainLogic::saveAttempt(QList<int> attempts, int mean)
{
    AttemptData data;


    data.mean = mean;
    data.errors = 799999; // +
    data.meanWithoutErrors = 7; // +
    data.attemptNum = attempts.size();
    data.mode = m_currentStimulationMode;
    data.sensors = lightNumber;
    data.randomIntervals = randomIntervalFrom_m == randomIntervalTo_m;
    data.intervalFrom = randomIntervalFrom_m;
    data.intervalTo = randomIntervalTo_m;
    data.attempts = attempts;

    currentAttempt = data;

    saveManager.writeAttempt(data, currentAttemptName, m_scoreBoardText);
}

void MainLogic::saveAttemptText()
{

    saveManager.writeAttempt(currentAttempt, currentAttemptName, m_scoreBoardText);

    //emit attemptNamesChanged(QList<QString> {"papa leone", "amerigo vespucci", "cristofolo colombo"});

}

void MainLogic::saveAll()
{
    saveManager.saveRun();
}

QList<int> MainLogic::getMeans()
{
    return saveManager.getMeans();
}

void MainLogic::loadAttempt(int index)
{
    if (index >= m_attemptNames.size()){
        return;
        qDebug() << "index out of bounds for attemptNames during loadAttempt call";
    }

    // save current data

    saveManager.writeAttemptText(currentAttemptName, m_scoreBoardText);

    // load new one
    currentAttemptName = m_attemptNames[index];
    // attempt data is not set as it's useless and could only cause more bugs
    m_scoreBoardText = saveManager.getText(index);

    emit scoreBoardTextChanged(m_scoreBoardText);

    return;
}

bool MainLogic::isRunning() const
{
    return m_isRunning;
}

void MainLogic::setIsRunning(bool newIsRunning)
{
    if (m_isRunning == newIsRunning) {
        return;
    }
    m_isRunning = newIsRunning;
    emit isRunningChanged(m_isRunning);
}

QString MainLogic::currentStimulationMode() const
{
    return m_currentStimulationMode;
}

void MainLogic::setCurrentStimulationMode(const QString newCurrentStimulationMode)
{
    if (m_currentStimulationMode == newCurrentStimulationMode)
        return;
    m_currentStimulationMode = newCurrentStimulationMode;
    emit currentStimulationModeChanged(newCurrentStimulationMode);
}

QList<QString> MainLogic::attemptNames() const
{
    return m_attemptNames;
}

void MainLogic::setAttemptNames(const QList<QString> newAttemptNames)
{
    if (m_attemptNames == newAttemptNames)
        return;
    m_attemptNames = newAttemptNames;
    emit attemptNamesChanged(newAttemptNames);
}
