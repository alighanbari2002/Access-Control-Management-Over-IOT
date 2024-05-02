#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>
#include <QCoreApplication>
#include "webserver.h"

const int port = 80;



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


