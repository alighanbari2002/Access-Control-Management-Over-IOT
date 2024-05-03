#include "websocket.h"


void Websocket::onNewConnection()
{
    QWebSocket *thisClientSocket = _webSocketServer->
                                    nextPendingConnection();
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



