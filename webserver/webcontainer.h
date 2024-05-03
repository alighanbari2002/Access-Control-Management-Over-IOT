#ifndef WEBCONTAINER_H
#define WEBCONTAINER_H

#include <QObject>

#include "websocket.h"
#include "webserver.h"

class WebContainer : public QObject
{
    Q_OBJECT
private:
    Websocket *_webSocketServer;
    Webserver *_webserver;
public:
    explicit WebContainer(QHostAddress hostaddr, int webserverport,
                          int websocketport, QObject *parent = nullptr);

signals:
    void relayNewUserToWebsocketserver(const QString &data);
};

#endif // WEBCONTAINER_H
