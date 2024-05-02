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
    static QString readFileContents(const QString& filePath);
};

#endif // FILEINTERACTIONHANDLER_H
