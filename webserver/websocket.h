#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <QObject>
#include <QWebSocket>
#include <QWebSocketServer>
#include "defs.h"
#include "websocketresponsehandler.h"

class Websocket : public QObject
{
    Q_OBJECT
private:
    QWebSocketServer *_webSocketServer;
    QList<QWebSocket *> _clients;
    WebsocketResponseHandler* _handler;

public slots:
    void onNewConnection();
    void socketDisconnected();
    void processMessage(const QString &request);
    void sendNewUserNotif(const QString &rfid, const QString &time,
                          const QString &date);

public:
    Websocket(QHostAddress hosturl, int port, QObject* parent = nullptr);
    ~Websocket();
};

#endif // WEBSOCKET_H
