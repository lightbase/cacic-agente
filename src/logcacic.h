#ifndef LOGCACIC_H
#define LOGCACIC_H

#include "QLogger.h"

class LogCacic
{

public:
    LogCacic(QString identificadorLOG, QString enderecoArquivoLog);
    ~LogCacic();

    enum CacicLogLevel {
        TraceLevel = 0,
        DebugLevel = 1,
        InfoLevel = 2,
        WarnLevel = 3,
        ErrorLevel = 4,
        FatalLevel = 5,
        MessageControler = 6
    };

    void escrever(CacicLogLevel level, QString msg);
    QString getLevelEmString(LogCacic::CacicLogLevel level);
    QString resolverEnderecoArquivo(LogCacic::CacicLogLevel level);

private:
    QString identificador;
    QString enderecoArquivoLog;
    QLogger::LogLevel resolverLevel(LogCacic::CacicLogLevel level);

};

#endif // LOGCACIC_H
