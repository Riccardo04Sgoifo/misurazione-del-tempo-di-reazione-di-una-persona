#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <QObject>
#include <QList>
#include <QString>
#include <QDebug>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>


struct AttemptData {
    int mean = 0;
    int errors = 0;
    int meanWithoutErrors = 0;
    int attemptNum = 0;
    QString mode = "";
    int sensors = 0;
    bool randomIntervals = false;
    int intervalFrom = 0;
    int intervalTo = 0;
    QList<int> attempts {0};


    // convert to json object for storing
    QJsonObject toJson() {
        QJsonObject json;

        json["Mean"] = mean;
        json["Errors"] = errors;
        json["MeanWithoutErrors"] = meanWithoutErrors;
        json["attemptNum"] = attemptNum;
        json["Mode"] = mode;
        json["Sensors"] = sensors;
        json["RandomIntervals"] = randomIntervals;
        json["IntervalFrom"] = intervalFrom;
        json["IntervalTo"] = intervalTo;
        QJsonArray attemptJsonList;
        for (int i = 0; i < attemptNum; i++){
            attemptJsonList.append(attempts[i]);
        }
        json["Attempts"] = attemptJsonList;

        return json;
    };

    // return an AttemptData object initialized with values from json
    AttemptData fromJson(QJsonObject json){
        AttemptData result;

        if (const QJsonValue value = json["Mean"]; value.isDouble()){
            result.mean = value.toInt();
        }

        if (const QJsonValue value = json["Errors"]; value.isDouble()){
            result.errors = value.toInt();
        }

        if (const QJsonValue value = json["MeanWithoutErrors"]; value.isDouble()){
            result.meanWithoutErrors = value.toInt();
        }

        if (const QJsonValue value = json["attemptNum"]; value.isDouble()){
            result.attemptNum = value.toInt();
        }

        if (const QJsonValue value = json["Mode"]; value.isString()){
            result.mode = value.toString();
        }

        if (const QJsonValue value = json["Sensors"]; value.isDouble()){
            result.sensors = value.toInt();
        }

        if (const QJsonValue value = json["RandomIntervals"]; value.isBool()){
            result.randomIntervals = value.toBool();
        }

        if (const QJsonValue value = json["IntervalFrom"]; value.isDouble()){
            result.intervalFrom = value.toInt();
        }

        if (const QJsonValue value = json["IntervalTo"]; value.isDouble()){
            result.intervalTo = value.toInt();
        }

        if (const QJsonValue value = json["Attempts"]; value.isArray()){
            QJsonArray array = value.toArray();
            for (int i = 0; i < array.size(); i++){
                result.attempts.append(array[i].toInt());
            }
        }

        return result;
    }

};




class SaveManager : public QObject
{
    Q_OBJECT

private:

    // used also for json value names
    QList<QString> attemptNames;

    // stored in json
    QList<QString> attemptTexts;
    QList<AttemptData> attemptParameters;

    // get json object for storing
    QJsonObject getJson();


public:

    SaveManager();

    // add or remove attempt
    void appendAttempt(AttemptData data, QString name, QString text);
    // add if it doesn't exists else overwrite it
    void writeAttempt(AttemptData data, QString name, QString text);
    void removeAttempt(QString *name); // remove by name
    void removeAttempt(int index); // remove by index

    // save every stored attempt in JSON
    void saveRun();

    // get by index or name for displaying
    QString getText(QString *name);
    QString getText(int index);

    // get attempt index by searching in attemptNames
    int getIndex(QString *name);

    // get all means in current file
    QList<int> getMeans();

public slots:

    // used to modify from qml
    void setAttemptNames(const QList<QString> value);

    QString getName(int index);

signals:

    void attemptNamesChanged(const QList<QString> neams);

};

#endif // SAVEMANAGER_H
