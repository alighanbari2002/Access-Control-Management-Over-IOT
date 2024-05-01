#include "cpstextformatmanip.h"

namespace CPS {

CPSTextFormatManip::CPSTextFormatManip() {}

QJsonArray CPSTextFormatManip::jsonToQJsonArray(const QString jsonString)
{
    QJsonDocument jsdoc = QJsonDocument::fromJson(jsonString.toUtf8());
    if(jsdoc.isArray())
    {
        return jsdoc.array();
    }
    else
    {
        qDebug() << "json format incorrect";
    }
}

}



