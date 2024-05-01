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
    QObject::connect(_window, &MainWindow::connectBtnClicked, _socket,
                     &CPSWebSocket::connectToServer);
    QObject::connect(_socket, &CPSWebSocket::connectionChanged,
                     _window, &MainWindow::changeRightPanelEnabled);
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
}

void Application::showHistoryAfterSent(const QString &receivedMessage)
{
    QJsonArray data = CPSTextFormatManip::jsonToQJsonArray(receivedMessage);
    _history->show(data);
}

} // end of CPS
