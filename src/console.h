/*-----------------------------------------------------------------------------------------
 *
 * Project Cacic Agente
 *    File created by Lightbase
 *
 * Developers: Eric Menezes Noronha (eric.m.noronha@lightbase.com.br); GitHub: ericmenezes
 *             Thiago Rocha         (thiago.rocha@lightbase.com.br)  ;
 *
 *-----------------------------------------------------------------------------------------*/
#ifndef CONSOLE_H
#define CONSOLE_H

#include <QDebug>
#include <QProcess>
#include <QString>
#include <QSysInfo>

class ConsoleObject:public std::unary_function<QString,QString>
{

public:


    QString operator() (QString input)
    {
        QProcess* process = new QProcess();

#if defined(Q_OS_LINUX)
        QStringList options;
        options << "-c" << input;
        process->start("/bin/sh", options);
#elif defined(Q_OS_WIN)
        process->start(input);
#endif
        process->waitForFinished();
        QString output = process->readAll();
        output.append(process->readAllStandardError());

        delete process;
        return output;
    }
};

#endif // CONSOLE_H
