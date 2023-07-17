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



private:
    // variables
    int lightNumber = 4;
    int startDelay_m = 3000;
    int attemptNum_m = 10;

    int currentLight_m = 2;

    int randomIntervalFrom_m = 1000;
    int randomIntervalTo_m = 1000;

    bool m_noReps = false;

    QList<QString> m_availableSerialPorts = {};

    QString m_currentSerialPort;


    QString m_currentSerialPortDescription;

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
};

#endif // MAINLOGIC_H
