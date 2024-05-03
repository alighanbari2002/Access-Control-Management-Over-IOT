#ifndef WEBSOCKETRESPONSEHANDLER_H
#define WEBSOCKETRESPONSEHANDLER_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "cpstextformatmanip.h"
#include "defs.h"
#include "filehandler.h"

class WebsocketResponseHandler : public QObject
{
    Q_OBJECT
private:
    QString authRespond(bool isAuthorized);
    QString historyRespond(bool isAuthorized);
    bool authenticateUser(const QString &username,
                          const QString &password);
public:
    WebsocketResponseHandler(QObject *parent = nullptr);
    QString handleResponse(const QString &request);

};

#endif // WEBSOCKETRESPONSEHANDLER_H
