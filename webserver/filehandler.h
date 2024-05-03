#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QObject>
#include <Qfile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>


class FileHandler : public QObject
{
    Q_OBJECT
public:
    FileHandler(QObject *parent = nullptr);
    static QList<QString> readCSVbyLine(const QString& filePath);
    static QString readFileContents(const QString& filePath);
    static void saveActivityToLog(const QString &rfid,
                                     const QString &time,
                                     const QString &date,
                                     const QString &accessStatus,
                                     const QString &filePath);

    static void saveActivityToHistory(const QString &rfid,
                                      const QString &time,
                                      const QString &date,
                                      const QString &filePath);

};

#endif // FILEINTERACTIONHANDLER_H
