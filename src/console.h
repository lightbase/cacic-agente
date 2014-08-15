#ifndef CONSOLE_H
#define CONSOLE_H

#include <QDebug>
#include <QProcess>
#include <QString>
#include <QSysInfo>

class ConsoleObject:public std::unary_function<std::string,QString>
{

public:


    QString operator() (std::string input)
    {
        QProcess* process = new QProcess();

#if defined(Q_OS_LINUX)
        QStringList options;
        options << "-c" << QString::fromStdString(input);
        process->start("/bin/sh", options);
#elif defined(Q_OS_WIN)
        process->start(QString::fromStdString(input));
#endif
        process->waitForFinished();
        QString output = process->readAll();
//        qDebug() << output;

        delete process;
        return output;
    }
};

#endif // CONSOLE_H
