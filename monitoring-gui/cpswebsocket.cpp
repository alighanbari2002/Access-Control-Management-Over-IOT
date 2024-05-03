#include "cpswebsocket.h"

namespace CPS {

CPSWebSocket::CPSWebSocket(QObject *parent)
    : QObject{parent}
{
    QObject::connect(&_qwsocket, &QWebSocket::textMessageReceived, this,
                     &CPSWebSocket::responseHandler);
    QObject::connect(&_qwsocket, &QWebSocket::disconnected, this,
                     &CPSWebSocket::serverClose);
    QObject::connect(&_qwsocket, &QWebSocket::connected, this,
                     &CPSWebSocket::serverConnect);
    QObject::connect(&_qwsocket, &QWebSocket::errorOccurred,
                     this, &CPSWebSocket::printConnectivityError);
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
        _address = address;
        _username = username;
        _password = password;
        QString wsAddress = address;
        if(QString::compare(address.left(5), QString("ws://")) != 0)
        {
            wsAddress = QString("ws://") + wsAddress;
        }
        _qwsocket.open(QUrl(wsAddress));
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
    if(actionResponse == CHECK_AUTH_RESPONSE)
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
    else if(actionResponse == NEW_USER_NOTIF)
    {
        Q_EMIT newUser(serverResponse[0].toObject().value("username").toString(),
                       serverResponse[0].toObject().value("date").toString(),
                       serverResponse[0].toObject().value("time").toString());
    }
    else if(actionResponse == GET_HISTORY_RESPONSE)
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
    QJsonObject requestInfo;
    requestInfo.insert("action", CHECK_AUTH);
    requestInfo.insert("username", _username);
    requestInfo.insert("password", _password);
    _qwsocket.sendTextMessage(QJsonDocument(requestInfo).toJson(QJsonDocument::Compact));
}

CPSWebSocket::~CPSWebSocket()
{
    _qwsocket.close();
}

void CPSWebSocket::printConnectivityError(QAbstractSocket::SocketError error)
{
    qDebug() << error;
}
}
