#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QUrl>
#include <Qfile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>


class Webserver : public QTcpServer
{
    Q_OBJECT
public:
    explicit Webserver(QObject *parent = nullptr);


    QString buildResponse(QString strResource,QByteArray requestData);
private:
    const QString apiService = "/service";
    const QString namesFile = ":/resources/ValidRfid.csv";

protected:
    void incomingConnection(qintptr socketDescriptor) override;

};


#endif // WEBSERVERMAIN_H
