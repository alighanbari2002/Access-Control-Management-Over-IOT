#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>
#include <QCoreApplication>
#include "webcontainer.h"
#include "defs.h"

int main(int argc, char *argv[])
{
    QCoreApplication application(argc, argv);

    new WebContainer(HOST_ADDR, WEBSERVER_PORT, WEBSOCKET_PORT);

    return application.exec();
}

#include "main.moc"


