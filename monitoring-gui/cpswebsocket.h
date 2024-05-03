#ifndef CPSWEBSOCKET_H
#define CPSWEBSOCKET_H

#include <QObject>
#include <QWebSocket>
#include <QAbstractSocket>

#include "cpstextformatmanip.h"

namespace CPS {

class CPSWebSocket : public QObject
{
    Q_OBJECT
private:
    const QString GET_HISTORY = "#Fetch_History#";
    const QString GET_HISTORY_RESPONSE = "HistorySent";
    const QString CHECK_AUTH = "Checkauth";
    const QString CHECK_AUTH_RESPONSE = "Authchecked";
    const QString NEW_USER_NOTIF = "NewUserArrival";

    const QString AUTHORIZED = "Authorized";
    const QString UNAUTHORIZED = "Unauthroized";

    QString _address;
    QString _username;
    QString _password;

public:
    QWebSocket _qwsocket;
    const bool ALLOW_NEW_CONNECTION = true;
    const bool DISALLOW_NEW_CONNECTION = false;

public:
    explicit CPSWebSocket(QObject *parent = nullptr);
    void fetchHistoryInfo();
    virtual ~CPSWebSocket();

Q_SIGNALS:
    void newUser(const QString &rfid,
                 const QString &date, const QString &time);
    void connectionChanged(bool status);
    void serverSentHistory(const QString);

private slots:
    void printConnectivityError(QAbstractSocket::SocketError error);

public slots:
    void connectToServer(const QString &address, const QString &username,
                         const QString &password);
    void responseHandler(const QString &response);
    void serverClose();
    void serverConnect();
};
}
#endif // CPSWEBSOCKET_H
