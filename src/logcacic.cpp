#include "logcacic.h"

LogCacic::LogCacic(QString identificadorLOG, QString enderecoArquivoLog)
{
    this->identificador = identificadorLOG;
    this->enderecoArquivoLog = enderecoArquivoLog;
}

LogCacic::~LogCacic()
{
}

void LogCacic::escrever(LogCacic::CacicLogLevel level, QString msg)
{
    QLogger::QLoggerManager::getInstance()->addDestination(resolverEnderecoArquivo(level),
                                                           this->identificador,
                                                           resolverLevel(level));
    QLogger::QLog_(this->identificador, resolverLevel(level), msg);
    QLogger::QLoggerManager::getInstance()->cleanDestination();
}

QString LogCacic::resolverEnderecoArquivo(LogCacic::CacicLogLevel level){
    switch (level) {
    case LogCacic::CacicLogLevel::TraceLevel:
        return this->enderecoArquivoLog + "/cacic_trace.log";
        break;
    case LogCacic::CacicLogLevel::DebugLevel:
        return this->enderecoArquivoLog + "/cacic_debug.log";
        break;
    case LogCacic::CacicLogLevel::InfoLevel:
        return this->enderecoArquivoLog + "/cacic.log";
        break;
    case LogCacic::CacicLogLevel::WarnLevel:
        return this->enderecoArquivoLog + "/cacic_warn.log";
        break;
    case LogCacic::CacicLogLevel::ErrorLevel:
        return this->enderecoArquivoLog + "/cacic_error.log";
        break;
    case LogCacic::CacicLogLevel::FatalLevel:
        return this->enderecoArquivoLog + "/cacic_fatalerror.log";
        break;
    }
    return this->enderecoArquivoLog + "/cacic.log";
}

QLogger::LogLevel LogCacic::resolverLevel(LogCacic::CacicLogLevel level){
    switch (level) {
    case LogCacic::CacicLogLevel::TraceLevel:
        return QLogger::LogLevel::TraceLevel;
        break;
    case LogCacic::CacicLogLevel::DebugLevel:
        return QLogger::LogLevel::DebugLevel;
        break;
    case LogCacic::CacicLogLevel::InfoLevel:
        return QLogger::LogLevel::InfoLevel;
        break;
    case LogCacic::CacicLogLevel::WarnLevel:
        return QLogger::LogLevel::WarnLevel;
        break;
    case LogCacic::CacicLogLevel::ErrorLevel:
        return QLogger::LogLevel::ErrorLevel;
        break;
    case LogCacic::CacicLogLevel::FatalLevel:
        return QLogger::LogLevel::FatalLevel;
        break;
    }
    return QLogger::LogLevel::InfoLevel;
}

QString LogCacic::getLevelEmString(LogCacic::CacicLogLevel level){
    switch (level) {
    case LogCacic::CacicLogLevel::TraceLevel:
        return "TraceLevel";
        break;
    case LogCacic::CacicLogLevel::DebugLevel:
        return "DebugLevel";
        break;
    case LogCacic::CacicLogLevel::InfoLevel:
        return "InfoLevel";
        break;
    case LogCacic::CacicLogLevel::WarnLevel:
        return "WarnLevel";
        break;
    case LogCacic::CacicLogLevel::ErrorLevel:
        return "ErrorLevel";
        break;
    case LogCacic::CacicLogLevel::FatalLevel:
        return "FatalLevel";
        break;
    }
    return "InfoLevel";
}
