#include "webcontainer.h"

WebContainer::WebContainer(QHostAddress hostaddr, int webserverport,
                           int websocketport, QObject *parent)
    : QObject{parent},
    _webserver(new Webserver),
    _webSocketServer(new Websocket(hostaddr, websocketport))

{
    QObject::connect(_webserver, &Webserver::newUserArrived, this,
                     &WebContainer::relayNewUserToWebsocketserver);

    QObject::connect(this, &WebContainer::relayNewUserToWebsocketserver,
                     _webSocketServer, &Websocket::sendNewUserNotif);
    if (!_webserver->listen(hostaddr, webserverport))
    {
        qDebug() << "Failed to start server.";
    }
    qDebug() << "Server started, listening on port " + std::to_string(webserverport) + ".";
}

WebContainer::~WebContainer()
{
    delete _webserver;
    delete _webSocketServer;
}
