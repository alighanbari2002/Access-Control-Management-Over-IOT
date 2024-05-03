#include "websocketresponsehandler.h"

WebsocketResponseHandler::WebsocketResponseHandler(QObject *parent)
    : QObject{parent}
{

}

QString WebsocketResponseHandler::handleResponse(const QString &request)
{
    QJsonArray qjsarray = CPS::CPSTextFormatManip::jsonToQJsonArray(request);
    QString action = qjsarray[0].toObject().value("action").toString();
    QString username = qjsarray[0].toObject().value("username").toString();
    QString password = qjsarray[0].toObject().value("password").toString();
    bool authorization = authenticateUser(username, password);
    if(action == CHECK_AUTH)
    {
        return authRespond(authorization);
    }
    else if(action == GET_HISTORY)
    {
        return historyRespond(authorization);
    }
    else
    {
        qDebug() << "Invalid Request from client";
    }
}

QString WebsocketResponseHandler::newUserNotifResponse(const QString &rfid, const QString &time, const QString &date)
{
    QJsonObject qjsobj = {
        {"action", NEW_USER_NOTIF},
        {"rfid", rfid},
        {"time", time},
        {"date", date}
    };
    QJsonDocument jsdoc = QJsonDocument(qjsobj);
    return QString(jsdoc.toJson(QJsonDocument::JsonFormat::Compact));
}

QString WebsocketResponseHandler::authRespond(bool isAuthorized)
{
    QJsonObject respondJson = {
        {"action", CHECK_AUTH_RESPONSE},
        {"auth", QJsonValue(isAuthorized ? AUTHORIZED : UNAUTHORIZED)}
    };
    return QString(
        QJsonDocument(respondJson).
        toJson(QJsonDocument::JsonFormat::Compact));
}

QString WebsocketResponseHandler::historyRespond(bool isAuthorized)
{
    // Unlikely to happen since authority was checked in the
    // CHECK_AUTH phase of the connection
    if(!isAuthorized)
    {
        return authRespond(isAuthorized);
    }
    QJsonObject historyJsonResponse = {
        {"action", GET_HISTORY_RESPONSE}
    };
    QJsonArray historyJsonArray = CPS::
                                    CPSTextFormatManip::
        historyCSVLinesToJsonArray(FileHandler::readCSVbyLine(HISTORY_FILE_PATH));
    historyJsonResponse.insert("data", historyJsonArray);
    return QString(QJsonDocument(historyJsonResponse).
                   toJson(QJsonDocument::JsonFormat::Compact));
}

bool WebsocketResponseHandler::authenticateUser(const QString &username, const QString &password)
{
    QList <QString> userInfoList = FileHandler::
        readCSVbyLine(USERS_INFO_PATH);

    if(!userInfoList.isEmpty())
    {
        // remove csv titles
        userInfoList.removeFirst();
    }
    for(auto &userinfo : userInfoList)
    {
        QStringList fields = userinfo.split(",");
        QString username_ = fields[1];
        QString password_ = fields[2];
        if(username_ == username &&
            password_ == password)
        {
            return true;
        }
    }
    return false;
}



