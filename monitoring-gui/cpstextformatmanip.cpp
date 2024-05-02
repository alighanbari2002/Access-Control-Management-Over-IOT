#include "cpstextformatmanip.h"

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

}



