#ifndef CPSAPPLICATION_H
#define CPSAPPLICATION_H

#include <QObject>
#include <QApplication>
#include <QWebSocket>

#include "cpsmainwindow.h"
#include "cpshistorywindow.h"
#include "cpswindowsapitools.h"
#include "cpstextformatmanip.h"
#include "cpswebsocket.h"

namespace CPS {

class Application : public QObject
{
    Q_OBJECT

public:

    explicit Application(QObject *parent = nullptr);
    ~Application();

    void show();

public slots: // Correctly declare slots here
    void showHistoryWindow();
    void showHistoryAfterSent(const QString &receivedMessage);

private: // members
    MainWindow    *_window;
    HistoryWindow *_history;
    CPSWebSocket  *_socket;
};

} // end of CPS

#endif // CPSAPPLICATION_H
