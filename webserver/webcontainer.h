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
    ~WebContainer();
signals:
    void relayNewUserToWebsocketserver(const QString &rfid, const QString &time,
                                       const QString &date);
};

#endif // WEBCONTAINER_H
