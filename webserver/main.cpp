#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>
#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QUrl>
#include <Qfile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>

const QString apiService = "/service";
const int port = 80;
const QString namesFile = "name.csv";


class Webserver : public QTcpServer
{
    Q_OBJECT
public:
    explicit Webserver(QObject *parent = nullptr) : QTcpServer(parent) {}
    QString readFileContents(const QString& filePath)
    {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "Error opening file:" << file.errorString();
            return QString();
        }

        QTextStream in(&file);
        QString Content = in.readAll();

        file.close();
        return Content;
    }

    QString buildResponse(QString strResource,QByteArray requestData){
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
                strResource = "Success";
            else
                strResource = "Failed!";
            return strResource;
        }
    }
protected:
    void incomingConnection(qintptr socketDescriptor) override
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

            QString strResource = readFileContents("../../assets/" + namesFile);
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
};

int main(int argc, char *argv[])
{
    QCoreApplication application(argc, argv);

    Webserver server;
    if (!server.listen(QHostAddress::Any, port)) {
        qDebug() << "Failed to start server.";
        return 1;
    }
    qDebug() << "Server started, listening on port " + std::to_string(port) + ".";

    return application.exec();
}

#include "main.moc"
