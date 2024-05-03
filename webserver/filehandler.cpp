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



QList<QString> FileHandler::readAuthfile(const QString &filePath)
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

