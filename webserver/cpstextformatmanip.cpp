#include "cpstextformatmanip.h"
#include "defs.h"

namespace CPS {

CPSTextFormatManip::CPSTextFormatManip() {}

QJsonArray CPSTextFormatManip::jsonToQJsonArray(const QString jsonString)
{
    QJsonDocument jsdoc = QJsonDocument::fromJson(jsonString.toUtf8());
    if(!jsdoc.isNull() && !jsdoc.isEmpty())
    {
        if(jsdoc.isArray())
        {
            return jsdoc.array();
        }
        else if(jsdoc.isObject())
        {
            QJsonArray jsarray;
            jsarray.append(jsdoc.object());
            return jsarray;
        }
        else
        {
            qDebug() << "json format incorrect";
            return QJsonArray();
        }
    }
    else
    {
        qDebug() << "json is empty";
    }
}

QJsonArray CPSTextFormatManip::historyCSVLinesToJsonArray(const QList<QString> &csvLines)
{
    QJsonArray res;
    for(auto &line : csvLines)
    {
        QList<QString> rawInfo = line.split(',');
        QJsonObject formatedInfo = {
            {"rfid", rawInfo[HISTORY_RFID_INDEX]},
            {"time", (rawInfo.count()>=2) ? rawInfo[HISTORY_TIME_INDEX] : ""},
            {"date", (rawInfo.count()>=3) ? rawInfo[HISTORY_DATE_INDEX] : ""}
        };
        res.append(formatedInfo);
    }
    return res;
}

}



