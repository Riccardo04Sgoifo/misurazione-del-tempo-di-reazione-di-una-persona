#ifndef MAINLOGICWORKER_H
#define MAINLOGICWORKER_H

#include <QThread.h>
#include <QObject>
#include <QRandomGenerator>
#include <QList>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QString>
//#include <QtWidgets/QMessageBox>

#define MAX_STEPS 1000

class MainLogicWorker : public QObject
{
    Q_OBJECT
private:
    int startDelay = 3000;
    int attemptNum = 10;

    int randomIntervalFrom = 1000;
    int randomIntervalTo = 1000;

    bool noReps = false;

    QList<QString> availableSerialPorts = {};
    QList<QString> availableSerialPortsDescription = {};

    QString currentSerialPort;
    QString currentSerialPortDescription;

    QSerialPort *serial;

    virtual ~MainLogicWorker();


public:
    explicit MainLogicWorker(QObject *parent = nullptr);




public slots:
    void doWork();
    void setAttemptNum(int value);
    void setStartDelay(int value);

    void setRandomIntervalFrom(int value);
    void setRandomIntervalTo(int value);
    void setNoReps(bool newNoReps);
    void setCurrentSerialPort(QString value);
    void connectSerialPort();
    void updateAvailableSerialPorts();



signals:
    void lightChanged(int value);
    void attemptNumChanged(int value);
    void startDelayChanged(int value);

    void randomIntervalChangedFrom(int value);
    void randomIntervalChangedTo(int value);
    void availableSerialPortsChanged(QList<QString> value);
    void currentSerialPortsDescriptionChanged(QString value);
    void requestOpenErrorDialog(QString reason);


};

#endif // MAINLOGICWORKER_H
