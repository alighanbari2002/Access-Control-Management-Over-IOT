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
    // TODO
}

bool WebsocketResponseHandler::authenticateUser(const QString &username, const QString &password)
{
    QStringList userInfoList = FileHandler::
                               readFileContents(USERS_INFO_PATH).
                               split(',');
    if(!userInfoList.isEmpty())
    {
        // remove csv titles
        userInfoList.removeFirst();
    }
    for(auto userinfo : userInfoList)
    {
        if(userinfo[USERNAME_FIELD] == username &&
            userinfo[PASSWORD_FIELD] == password)
        {
            return true;
        }
    }
    return false;
}


