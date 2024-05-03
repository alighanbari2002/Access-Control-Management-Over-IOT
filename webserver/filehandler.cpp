#include "filehandler.h"
#include <fstream>
#include <iostream>
#include <string>
FileHandler::FileHandler(QObject *parent) : QObject{parent}{}


QString FileHandler::readFileContents(const QString &filePath)
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

void FileHandler::saveActivityToLog(const QString &rfid,
                                       const QString &time,
                                       const QString &date,
                                       const QString &accessStatus,
                                       const QString &filePath)
{
    std::ofstream file(filePath.toStdString(), std::ios::out | std::ios::app);
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << filePath.toStdString() << std::endl;
        return;
    }
    file << rfid.toStdString() << "," << time.toStdString()
         << "," << date.toStdString() << "," << accessStatus.toStdString() << ",\n";
    file.close();
}
void FileHandler::saveActivityToHistory(const QString &rfid, const QString &time, const QString &date, const QString &filePath)
{
    std::ofstream file(filePath.toStdString(), std::ios::out | std::ios::app);
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << filePath.toStdString() << std::endl;
        return;
    }
    file << rfid.toStdString() << "," << time.toStdString()
         << "," << date.toStdString() <<  ",\n";
    file.close();
}

QList<QString> FileHandler::readCSVbyLine(const QString &filePath)
{
    QList <QString> Content;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << file.errorString();
        return Content;
    }

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        Content.append(line);
        // QStringList fields = line.split(",");

        // QString rfid = fields[0];
        // QString username = fields[1];
        // QString password = fields[2];
    }

    // Close the file
    file.close();
    return Content;
}

