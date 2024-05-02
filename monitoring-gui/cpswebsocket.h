#ifndef CPSWEBSOCKET_H
#define CPSWEBSOCKET_H

#include <QObject>
#include <QWebSocket>

#include "cpstextformatmanip.h"

namespace CPS {

class CPSWebSocket : public QObject
{
    Q_OBJECT
private:
    const QString GET_HISTORY = "#Fetch_History#";
    const QString AUTHORIZED = "Authorized";
    const QString UNAUTHORIZED = "Unauthroized";
    const QString CHECK_AUTHORIZATION = "Checkauth";
    const QString SEND_USER_DATA = "GetUserData";
    const QString SEND_CLIENT_INFO = "SendClientInfo";
    const QString SEND_HISTORY = "SendHistory";
public:
    QWebSocket _qwsocket;
    const bool ALLOW_NEW_CONNECTION = true;
    const bool DISALLOW_NEW_CONNECTION = false;
public:
    explicit CPSWebSocket(QObject *parent = nullptr);
    void fetchHistoryInfo();
    virtual ~CPSWebSocket();

Q_SIGNALS:
    void newUser(const QString &username,
                 const QString &date, const QString &time);
    void connectionChanged(bool status);
    void serverSentHistory(const QString);

public slots:
    void connectToServer(const QString &address,
                         const QString &username, const QString &password);
    void responseHandler(const QString &response);
    void serverClose();
    void serverConnect();
};
}
#endif // CPSWEBSOCKET_H
