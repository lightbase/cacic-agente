#include "logfile.h"
#include <QMetaType>


LogFile::LogFile(QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType<QtMsgType>("QtMsgType");
    qWarning() << "Sem caminho de arquivo passado como parâmetro.";
    qWarning() << "Utilizando arquivo-padrão logfile.txt.";

    logFile = new QFile("./logfile.txt");

    bool fileOpened = logFile->open(QFile::ReadWrite);
    if(!fileOpened) {
        qWarning() << "Não foi possível abrir arquivo de log.";
        return;
    }
}

LogFile::LogFile(const QString &location, QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType<QtMsgType>("QtMsgType");

    logFile = new QFile(location);

    bool fileOpened = logFile->open(QFile::ReadWrite);
    if(!fileOpened) {
        qWarning() << "Não foi possível abrir arquivo de log.";
        return;
    }
}

LogFile::~LogFile()
{
    logFile->close();
    delete logFile;
}


void LogFile::outputMessage(QtMsgType type, const QString &msg)
{


    if (type == QtDebugMsg) {
        QByteArray localMsg = QString("Debug: ").append(msg).append("\n").toLocal8Bit();
        logFile->write(localMsg);
    } else if(type == QtWarningMsg) {
        QByteArray localMsg = QString("Warning: ").append(msg).append("\n").toLocal8Bit();
        logFile->write(localMsg);
    } else if(type == QtCriticalMsg) {
        QByteArray localMsg = QString("Critical: ").append(msg).append("\n").toLocal8Bit();
        logFile->write(localMsg);
    } else if(type == QtFatalMsg) {
        QByteArray localMsg = QString("Fatal: ").append(msg).append("\n").toLocal8Bit();
        logFile->write(localMsg);
    }
}
