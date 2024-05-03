#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>

#include "defs.h"

class Webserver : public QTcpServer
{
    Q_OBJECT
public:
    explicit Webserver(QObject *parent = nullptr);
    QString buildResponse(QString strResource, QByteArray requestData);

private:
    const QString apiService = "/service";

protected:
    void incomingConnection(qintptr socketDescriptor) override;

};


#endif // WEBSERVERMAIN_H
