#include "cpswebsocket.h"

namespace CPS {

CPSWebSocket::CPSWebSocket(QObject *parent)
    : QObject{parent}
{
    QObject::connect(&_qwsocket, &QWebSocket::textMessageReceived, this,
                     &responseHandler);
    QObject::connect(&_qwsocket, &QWebSocket::disconnected, this,
                     &serverClose);
    QObject::connect(&_qwsocket, &QWebSocket::connected, this,
                     &serverConnect);
}

void CPSWebSocket::fetchHistoryInfo()
{
    if(_qwsocket.isValid())
    {
        _qwsocket.sendTextMessage(GET_HISTORY);
    }
    else
    {
        qDebug() << "Connection has not been established";
    }
}

void CPSWebSocket::connectToServer(const QString &address,
                                   const QString &username,
                                   const QString &password)
{
    if(!_qwsocket.isValid())
    {
        _qwsocket.open(QUrl(address));
        QJsonObject requestInfo;
        requestInfo.insert("action", SEND_CLIENT_INFO);
        requestInfo.insert("username", username);
        requestInfo.insert("password", password);
    }
    else
    {
        qDebug() << "Socket has already been connected";
    }
}

void CPSWebSocket::responseHandler(const QString &response)
{
    QJsonArray serverResponse = CPSTextFormatManip::jsonToQJsonArray(response);
    QString actionResponse = serverResponse[0].toObject().value("action").toString();
    if(actionResponse == CHECK_AUTHORIZATION)
    {
        if(serverResponse[0].toObject().value("auth").toString() == AUTHORIZED)
        {
            qDebug() << "User is Authorized to view the history";
        }
        else if(serverResponse[0].toObject().value("auth").toString() == UNAUTHORIZED)
        {
            qDebug() << "User is not Authorized to view the history";
        }
        else
        {
            qDebug() << "Invalid response";
        }
    }
    else if(actionResponse == GET_USER_DATA)
    {
        Q_EMIT newUser(serverResponse[0].toObject().value("username").toString(),
                       serverResponse[0].toObject().value("date").toString(),
                       serverResponse[0].toObject().value("time").toString());
    }
    else if(actionResponse == SEND_HISTORY)
    {
        // Remove action to end up with history iteself
        serverResponse.removeAt(0);
        Q_EMIT serverSentHistory(QString(QJsonDocument(serverResponse).toJson()));
    }
    else
    {
        qDebug() << "Invalid action";
    }
}

void CPSWebSocket::serverClose()
{
    Q_EMIT connectionChanged(ALLOW_NEW_CONNECTION);
}

void CPSWebSocket::serverConnect()
{
    Q_EMIT connectionChanged(DISALLOW_NEW_CONNECTION);
}

CPSWebSocket::~CPSWebSocket()
{
    _qwsocket.close();
}
}
