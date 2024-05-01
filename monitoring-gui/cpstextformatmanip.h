#ifndef CPSTEXTFORMATMANIP_H
#define CPSTEXTFORMATMANIP_H

#include <QObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

namespace CPS {

class CPSTextFormatManip : public QObject
{
    Q_OBJECT
public:
    CPSTextFormatManip();

public:
    static QJsonArray jsonToQJsonArray(const QString jsonString);
};
}
#endif // CPSTEXTFORMATMANIP_H
