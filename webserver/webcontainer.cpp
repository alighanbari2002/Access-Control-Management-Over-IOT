#include "webcontainer.h"

WebContainer::WebContainer(QHostAddress hostaddr, int webserverport,
                           int websocketport, QObject *parent)
    : QObject{parent},
    _webserver(new Webserver),
    _webSocketServer(new Websocket(hostaddr, websocketport))

{
    // QObject::connect();
    if (!_webserver->listen(hostaddr, webserverport)) {
        qDebug() << "Failed to start server.";
    }
    qDebug() << "Server started, listening on port " + std::to_string(webserverport) + ".";
}
