#include "filehandler.h"

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
    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        qDebug() << "Error opening file:" << file.errorString();
    }
    QTextStream out(&file);
    out << rfid << "," << time << "," << date << "," << accessStatus << ",\n";
    file.close();
}

void FileHandler::saveActivityToHistory(const QString &rfid, const QString &time, const QString &date, const QString &filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        qDebug() << "Error opening file:" << file.errorString();
    }
    QTextStream out(&file);
    out << rfid << "," << time << "," << date << ",\n";
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

