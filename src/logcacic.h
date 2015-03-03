#ifndef LOGCACIC_H
#define LOGCACIC_H

#include "QLogger.h"
#include "ccacic.h"


class LogCacic
{

public:
    LogCacic(QString identificadorLOG, QString enderecoArquivoLog);
    enum CacicLogLevel {
        TraceLevel = 0,
        DebugLevel,
        InfoLevel,
        WarnLevel,
        ErrorLevel,
        FatalLevel
    };
    ~LogCacic();
    void escrever(CacicLogLevel level, QString msg);

private:
    QString identificador;
    QString enderecoArquivoLog;
    QLogger::LogLevel resolverLevel(LogCacic::CacicLogLevel level);
    QString resolverEnderecoArquivo(LogCacic::CacicLogLevel level);

};

#endif // LOGCACIC_H
