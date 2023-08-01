#include "savemanager.h"




SaveManager::SaveManager()
{


}


void SaveManager::appendAttempt(AttemptData data, QString name, QString text)
{
    if (getIndex(&name) >= 0) {

        qDebug() << "tried to append existing attempt";
        return; // this attempt name already exists

    }
    // attempt doesn't exist
    attemptParameters.append(data);
    attemptTexts.append(text);
    attemptNames.append(name);

    // update qml  listView
    emit attemptNamesChanged(attemptNames);
}


void SaveManager::writeAttempt(AttemptData data, QString name, QString text)
{
    int index = getIndex(&name);




    if (index < 0) {

        // attempt doesn't exist
        appendAttempt(data, name, text);
        return;

    }
    // attempt exists
    attemptParameters[index] = data;
    attemptTexts[index] = text;

}


void SaveManager::removeAttempt(QString *name)
{
    // easier to use when selecting from qml

    removeAttempt(getIndex(name));
}


void SaveManager::removeAttempt(int index)
{
    if (index < 0){
        // invalid index
        qDebug() << "tried to remove attempt with invalid index";
        return;
    }

    attemptParameters.remove(index);
    attemptTexts.remove(index);
    attemptNames.remove(index);
}


QJsonObject SaveManager::getJson()
{
    QJsonObject json;

    // array containing all attempts
    QJsonArray attemptJsonArray;



    for (int i = 0; i < attemptNames.size(); i++){

        QJsonObject jsonSingleAttempt;

        // single string with name
        jsonSingleAttempt["AttemptName"] = attemptNames[i];
        // AttemptData struct
        // called attemptNumbers instad of attemptData to make attemptName show before (Na < Nu)
        jsonSingleAttempt["AttemptNumbers"] = attemptParameters[i].toJson();
        // single string with text
        jsonSingleAttempt["AttemptText"] = attemptTexts[i];

        // add to main array
        attemptJsonArray.append(jsonSingleAttempt);
    }


    json["Attempts"] = attemptJsonArray;

    return json;
}


void SaveManager::saveRun() // JSON
{
    QFile saveFile("save.json");

    if (!saveFile.open(QIODevice::WriteOnly)){
        qDebug() << "cuold not open save file";
        return;
    }

    QJsonObject object = getJson();
    saveFile.write(QJsonDocument(object).toJson());
}


QString SaveManager::getText(QString *name)
{
    // if name doesn't exist get index return -1 and getText returns "invalid"
    return getText(getIndex(name));
}

QString SaveManager::getText(int index)
{
    if (index < 0){
        // invalid index
        qDebug() << "tried to get text with invalid index";
        return "invalid :(";
    }
    return attemptTexts[index];
}


int SaveManager::getIndex(QString *name)
{
    // returns -1  if index doesn't exist
    return attemptNames.indexOf(*name);
}

QList<int> SaveManager::getMeans()
{
    QList<int> Means{};

    for (int i = 0; i < attemptParameters.size(); i++){
        Means.append(attemptParameters[i].mean);
    }

    return Means;
}

QString SaveManager::getName(int index)
{
    return "AAAA";
}

void SaveManager::setAttemptNames(QList<QString> value)
{
    if (value == attemptNames) {
        return;
    }
    attemptNames = value;
    emit attemptNamesChanged(value);
}
