#ifndef DEFS_H
#define DEFS_H

#include <QHostAddress>

const int WEBSERVER_PORT = 80;
const int WEBSOCKET_PORT = 13051;

const QHostAddress HOST_ADDR = QHostAddress("192.168.2.1");

// Request/Response
const QString GET_HISTORY = "#Fetch_History#";
const QString GET_HISTORY_RESPONSE = "HistorySent";
const QString CHECK_AUTH = "Checkauth";
const QString CHECK_AUTH_RESPONSE = "Authchecked";
const QString NEW_USER_NOTIF = "NewUserArrival";

const QString AUTHORIZED = "Authorized";
const QString UNAUTHORIZED = "Unauthroized";

// Resources
const QString USERS_INFO_FILE = "usersinfo.csv";
const QString VALID_RFID_FILE = "ValidRfid.csv";
const QString RESOURCES_PATH = ":/resources";
const QString USERS_INFO_PATH = RESOURCES_PATH + "/" + "usersinfo.csv";
const QString VALID_RFID_PATH = RESOURCES_PATH + "/" + "ValidRfid.csv";
const int USERNAME_FIELD = 1;
const int PASSWORD_FIELD = 2;


#endif // DEFS_H
