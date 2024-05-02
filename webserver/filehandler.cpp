#include "filehandler.h"

FileHandler::FileHandler(QObject *parent) : QObject{parent}{}


QString FileHandler::readFileContents(const QString& filePath)
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
