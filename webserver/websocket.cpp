#include "websocket.h"


void Websocket::onNewConnection()
{
    QWebSocket *thisClientSocket = _webSocketServer->
                                    nextPendingConnection();
    _clients.append(thisClientSocket);
    QObject::connect(thisClientSocket, &QWebSocket::disconnected, this,
                     &Websocket::socketDisconnected);
    QObject::connect(thisClientSocket, &QWebSocket::textMessageReceived,
                     this, &Websocket::processMessage);
}

void Websocket::socketDisconnected()
{
    QWebSocket *thisClientSocket = qobject_cast<QWebSocket *>(QObject::sender());
    if (thisClientSocket)
    {
        _clients.removeAll(thisClientSocket);
        thisClientSocket->deleteLater();
    }
}

void Websocket::processMessage(const QString &request)
{
    QString response = _handler->handleResponse(request);
    QWebSocket *requester = qobject_cast<QWebSocket*>(QObject::sender());
    requester->sendTextMessage(response);
    const QString checkauth = QJsonDocument::fromJson(response.toUtf8()).
                              object().value("auth").toString();
    if(!checkauth.isNull() && checkauth == UNAUTHORIZED)
    {
        requester->flush();
        requester->close();
    }
}

void Websocket::sendNewUserNotif(const QString &rfid, const QString &time,
                                 const QString &date)
{
    QString response = _handler->newUserNotifResponse(rfid, time, date);
    for(auto &client : _clients)
    {
        client->sendTextMessage(response);
    }
}

Websocket::~Websocket()
{
    delete _webSocketServer;
    delete _handler;
    _clients.clear();
}

Websocket::Websocket(QHostAddress hostaddr, int port, QObject *parent) :
    QObject{parent},
    _webSocketServer(new QWebSocketServer("Websocket Server",
                                            QWebSocketServer::NonSecureMode)),
    _handler(new WebsocketResponseHandler),
    _clients()
{
    if (_webSocketServer->listen(hostaddr, port))
    {
        qDebug() << "Websocket server listening on port" << port;
        QObject::connect(_webSocketServer,
                         &QWebSocketServer::newConnection,
                         this, &Websocket::onNewConnection);
    }
}



