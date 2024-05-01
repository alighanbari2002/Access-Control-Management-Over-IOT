#include "cpsapplication.h"

namespace CPS {

Application::Application(QObject *parent)
    : QObject{parent},
    _window(new MainWindow),
    _history(new HistoryWindow),
    _socket(new CPSWebSocket)
{
    setWindowsThemeToDark<MainWindow>(*_window);
    QObject::connect(_window, &MainWindow::historyuBtnClicked,
                     this, &Application::showHistoryWindow);
    QObject::connect(_socket, &CPSWebSocket::serverSentHistory,
                     this, &Application::showHistoryAfterSent);

    // TODO:
    QObject::connect(_socket, &CPSWebSocket::newUser, _window,
                     &MainWindow::showUserDetails);
    // QObject::connect(&YourSocketClassInstance, &YourSocketClass::newUser, &window, &MainWindow::showUserDetails);
    QObject::connect(_window, &MainWindow::connectBtnClicked, _socket,
                     &CPSWebSocket::connectToServer);
    // QObject::connect(&window, &MainWindow::connectBtnClicked, &YourSocketClassInstance, &YourSocketClass::connectToServer)
    // QObject::connect(_socket, )
    QObject::connect(_socket, &CPSWebSocket::connectionChanged,
                     _window, &MainWindow::changeRightPanelEnabled);
    // QObject::connect(&YourSocketClassInstance, &YourSocketClass::connectionChanged, &window, &MainWindow::changeRightPanelEnabled);

}

Application::~Application()
{
    delete this->_window;
    delete this->_history;

    //TODO:
    delete this->_socket;
}

void Application::show()
{
    this->_window->show();
}

void Application::showHistoryWindow()
{
    setWindowsThemeToDark<HistoryWindow>(*_history);

    _socket->fetchHistoryInfo();

    // TODO:
    /*
         * fetch data from server and show it in history window.
         * your data must be in QJsonArray format.
         * something like this:
         *
         * [
         *     {
         *          username: string,
         *          date: string,
         *          time: string,
         *     },
         *
         *     {
         *          username: string,
         *          date: string,
         *          time: string,
         *     }
         * ]
         *
         *  below is an example of how to create a QJsonArray from QVariantList: (beginer level)
         *  please erase this horrible example and implement your own logic.
         *  you must fetch a json from server
         *
         * */

    // QJsonObject obj1;
    // QJsonObject obj2;
    // QJsonObject obj3;

    // obj1["username"] = "1234567890";
    // obj1["date"] = "12/12/2024";
    // obj1["time"] = "10:00";

    // obj2["username"] = "0987654321";
    // obj2["date"] = "03/28/2024";
    // obj2["time"] = "12:00";

    // obj3["username"] = "5432167890";
    // obj3["date"] = "09/08/2024";
    // obj3["time"] = "14:00";

    // QVariantList list;
    // list.append(obj1);
    // list.append(obj2);
    // list.append(obj3);

    // QJsonArray data = QJsonArray::fromVariantList(list);

    // _history->show(data);
}

void Application::showHistoryAfterSent(const QString &receivedMessage)
{
    QJsonArray data = CPSTextFormatManip::jsonToQJsonArray(receivedMessage);
    _history->show(data);
}

} // end of CPS
