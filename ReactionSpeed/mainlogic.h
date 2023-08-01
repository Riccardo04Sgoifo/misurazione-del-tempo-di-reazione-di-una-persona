#ifndef MAINLOGIC_H
#define MAINLOGIC_H
#include <QObject>
#include <QThread>
#include <QDebug>
#include <mainlogicworker.h>
#include <QList>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QString>
#include <savemanager.h>

class MainLogic : public QObject
{
    Q_OBJECT
    QThread workerThread;
    Q_PROPERTY(int lightNum READ lightNum WRITE setLightNum NOTIFY lightNumChanged);
    Q_PROPERTY(int currentLight READ currentLight WRITE setCurrentLight NOTIFY currentLightChanged);
    Q_PROPERTY(int attemptNum READ attemptNum WRITE setAttemptNum NOTIFY attemptNumChanged);
    Q_PROPERTY(int startDelay READ startDelay WRITE setStartDelay NOTIFY startDelayChanged);

    Q_PROPERTY(int randomIntervalFrom READ randomIntervalFrom WRITE setRandomIntervalFrom NOTIFY randomIntervalChangedFrom);
    Q_PROPERTY(int randomIntervalTo READ randomIntervalTo WRITE setRandomIntervalTo NOTIFY randomIntervalChangedTo);
    Q_PROPERTY(bool noReps READ noReps WRITE setNoReps NOTIFY noRepsChanged);
    Q_PROPERTY(QList<QString> availableSerialPorts READ availableSerialPorts WRITE setAvailableSerialPorts NOTIFY availableSerialPortsChanged);
    Q_PROPERTY(QString currentSerialPort READ currentSerialPort WRITE setCurrentSerialPort NOTIFY currentSerialPortChanged);
    Q_PROPERTY(QString currentSerialPortDescription READ currentSerialPortDescription WRITE setCurrentSerialPortDescription NOTIFY currentSerialPortDescriptionChanged);
    Q_PROPERTY(QString scoreBoardText READ scoreBoardText WRITE setScoreBoardText NOTIFY scoreBoardTextChanged);
    Q_PROPERTY(bool isConnected READ isConnected WRITE setIsConnected NOTIFY isConnectedChanged);
    Q_PROPERTY(bool isRunning READ isRunning WRITE setIsRunning NOTIFY isRunningChanged);
    Q_PROPERTY(QString currentStimulationMode READ currentStimulationMode WRITE setCurrentStimulationMode NOTIFY currentStimulationModeChanged);

    // names to display on listView

    Q_PROPERTY(QList<QString> attemptNames READ attemptNames WRITE setAttemptNames NOTIFY attemptNamesChanged);



private:
    // variables

    MainLogicWorker *worker;
    SaveManager saveManager;

    int lightNumber = 4;
    int startDelay_m = 3000;
    int attemptNum_m = 10;

    int currentLight_m = 2;

    int randomIntervalFrom_m = 1000;
    int randomIntervalTo_m = 1000;

    bool m_noReps = false;

    // used for currentAttemptName
    int currentAttemptNum = 0;
    QString currentAttemptName = "Tentativo 1";

    QList<QString> m_availableSerialPorts = {};

    QString m_currentSerialPort;


    QString m_currentSerialPortDescription;

    QString m_scoreBoardText = "Prova 1 \n\nTentativo 1: 234ms\nTentativo2: 842ms\nTentativo 3: 922ms";
    AttemptData currentAttempt;

    bool m_isConnected = false;
    bool m_isRunning = false;

    // prevent saving nothing by disabling the save button
    //bool hasAtLeastOneAttempt = false;

    QString m_currentStimulationMode;

    QList<QString> m_attemptNames;

public:

    int lightNum() const {return lightNumber; }
    int currentLight() const {return currentLight_m; }
    int attemptNum() const {return attemptNum_m; }
    int startDelay() const {return startDelay_m; }

    int randomIntervalFrom() const {return randomIntervalFrom_m; }
    int randomIntervalTo() const {return randomIntervalTo_m; }


    MainLogic();
    ~MainLogic();

    bool noReps() const;


    QList<QString> availableSerialPorts() const;

    QString currentSerialPort() const;

    QString currentSerialPortDescription() const;


    QString scoreBoardText() const;


    bool isConnected() const;




    bool isRunning() const;
    void setIsRunning(bool newIsRunning);

    QString currentStimulationMode() const;

    QList<QString> attemptNames() const;


public slots:
    void startAttempts();
    void connectPort();
    void setLightNum(int num);
    void setCurrentLight(int value);
    void setAttemptNum(int value);
    void setStartDelay(int value);

    void setRandomIntervalFrom(int value);
    void setRandomIntervalTo(int value);
    void setNoReps(bool newNoReps);
    void setAvailableSerialPorts(QList<QString> newAvailableSerialPorts);
    void requestUpdateAvailableSerialPorts();
    void setCurrentSerialPort(const QString newCurrentSerialPort);
    void setCurrentSerialPortDescription(const QString newCurrentSerialPortDescription);

    void requestOpenErrorDialog(QString error) {emit errorDialogRequested(error); }

    void setScoreBoardText(const QString newScoreBoardText);
    void appendScoreBoardTextLine(const QString line);
    void clearScoreBoardText();
    void setIsConnected(bool newIsConnected);
    void stopAttempts() { worker->shallStop = true; }
    void setCurrentStimulationMode(const QString newCurrentStimulationMode);
    void setAttemptNames(const QList<QString> newAttemptNames);

    // give all data about current attempt to saveManager
    void saveAttempt(QList<int> attempts, int mean);
    // save from qml
    Q_INVOKABLE void saveAttemptText();
    // ask saveManager to save everything to json
    void saveAll();

    // ask saveManager to get all mean in current file
    Q_INVOKABLE QList<int> getMeans();

    // save attempt and load text
    Q_INVOKABLE void loadAttempt(int index);

signals:
    void lightNumChanged(int value);
    void currentLightChanged(int value);
    void startAttemptsSignal();
    void attemptNumChanged(int value);
    void startDelayChanged(int value);

    void randomIntervalChangedFrom(int value);
    void randomIntervalChangedTo(int value);
    void noRepsChanged(int value);
    void availableSerialPortsChanged(QList<QString>  value);
    void currentSerialPortChanged(QString value);
    void connectPortSignal();
    void requestUpdateAvailableSerialPortsSignal();
    void currentSerialPortDescriptionChanged(QString value);

    void errorDialogRequested(QString error);
    void scoreBoardTextChanged(QString value);
    void isConnectedChanged(bool value);

    void isRunningChanged(bool value);
    void currentStimulationModeChanged(QString value);
    void attemptNamesChanged(const QList<QString> value);
};

#endif // MAINLOGIC_H
