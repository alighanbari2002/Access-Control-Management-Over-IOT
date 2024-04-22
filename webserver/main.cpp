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

const QString apiService = "/service";
const int port = 10051;
const QString namesFile = "name.html";


class Webserver : public QTcpServer
{
    Q_OBJECT
public:
    explicit Webserver(QObject *parent = nullptr) : QTcpServer(parent) {}
    QString readHtmlFile(const QString& filePath)
    {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "Error opening file:" << file.errorString();
            return QString();
        }

        QTextStream in(&file);
        QString htmlContent = in.readAll();

        file.close();
        return htmlContent;
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

            QString strResource = readHtmlFile("C:\\Users\\ASUS\\Desktop\\Webserver\\webserver\\" + namesFile);
            QString lenght = QString(std::to_string(strResource.length()).c_str());
            if (path == apiService) {
                QByteArray responseData = "HTTP/1.1 200 OK\r\n"
                                          "Content-Type: text/plain\r\n"
                                          "Content-Length: " + lenght.toUtf8() + "\r\n"
                                          "\r\n" +
                                          strResource.toUtf8();
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
