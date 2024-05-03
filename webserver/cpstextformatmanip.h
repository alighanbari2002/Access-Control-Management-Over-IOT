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
    static QJsonArray historyCSVLinesToJsonArray(const QList<QString> &csvLines);
};
}
#endif // CPSTEXTFORMATMANIP_H
