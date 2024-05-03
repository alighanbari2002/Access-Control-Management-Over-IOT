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

Q_SIGNALS:

public slots:
    void onNewConnection();
    void socketDisconnected();
    void processMessage(const QString &request);
public:
    Websocket(QHostAddress hosturl, int port, QObject* parent = nullptr);
};

#endif // WEBSOCKET_H
