#include "webserver.h"
#include "filehandler.h"


Webserver::Webserver(QObject *parent) : QTcpServer{parent} {}


QString Webserver::buildResponse(QString strResource,QByteArray requestData)
{
    if( QString::fromUtf8(requestData)[0] == 'G') // GET Method
        return strResource; //return users
    else { // POST Method
        QString reqString = QString::fromUtf8(requestData);


        int jsonStartIndex = reqString.indexOf("{");
        QString jsonString = reqString.mid(jsonStartIndex);

        QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
        QJsonObject json = doc.object();
        QString username = json["username"].toString();


        if(strResource.contains(username,Qt::CaseSensitive))
            strResource = "Access Granted";
        else
            strResource = "Access Denied!";
        return strResource;
    }
}

void Webserver::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, [=]() {
        QByteArray requestData = socket->readAll();
        qDebug() << "Received request:" << requestData;

        QString requestString(requestData);
        QStringList requestLines = requestString.split("\r\n");
        QString firstLine = requestLines.first();
        QStringList firstLineParts = firstLine.split(" ");
        QString path = firstLineParts.at(1);

        QString strResource = FileHandler::readFileContents(namesFile);
        QString response = buildResponse(strResource,requestData);
        QString lenght = QString(std::to_string(response.length()).c_str());
        if (path == apiService) {
            QByteArray responseData = "HTTP/1.1 200 OK\r\n"
                                      "Content-Type: text/plain\r\n"
                                      "Content-Length: " + lenght.toUtf8() + "\r\n"
                                                          "\r\n" +
                                      response.toUtf8();
            socket->write(responseData);
            socket->flush();
            socket->waitForBytesWritten();
            socket->close();
        } else {
            QByteArray responseData = "HTTP/1.1 404 Not Found\r\n"
                                      "Content-Type: text/plain\r\n"
                                      "Content-Length: 13\r\n"
                                      "\r\n"
                                      "404 Not Found";
            socket->write(responseData);
            socket->flush();
            socket->waitForBytesWritten();
            socket->close();
        }
    });
}