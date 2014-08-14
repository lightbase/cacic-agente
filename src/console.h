#ifndef CONSOLE_H
#define CONSOLE_H

#include <QDebug>
#include <QProcess>
#include <QString>
#include <QSysInfo>

class ConsoleObject:public std::unary_function<std::string,QString>
{

public:

#if defined(Q_OS_LINUX)
    QString operator() (std::string input)
    {
        QProcess* process = new QProcess();

        QStringList options;
        options << "-c" << QString::fromStdString(input);
        process->start("/bin/sh", options);
        process->waitForFinished();
        QString output = process->readAll();
//        qDebug() << output;

        delete process;
        return output;
    }
#endif
};

#endif // CONSOLE_H
