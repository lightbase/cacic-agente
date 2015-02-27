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
    QLogger::QLoggerManager *logManager = QLogger::QLoggerManager::getInstance();
    logManager->addDestination(resolverEnderecoArquivo(level), this->identificador, resolverLevel(level));
    QLogger::QLog_(this->identificador, resolverLevel(level), msg);
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
        return this->enderecoArquivoLog + "/cacic_fatal.log";
        break;
    }
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
}
