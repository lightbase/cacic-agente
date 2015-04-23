#include "http-server.h"

void HttpService::start()
{
    QCoreApplication *app = application();

    //quint16 port = (app->argc() > 1) ?
    //        QString::fromLocal8Bit(app->argv()[1]).toUShort() : 8080;
    quint16 port = 8080;
    daemon = new HttpDaemon(port, app);

    if (!daemon->isListening()) {
        logMessage(QString("Failed to bind to port %1").arg(daemon->serverPort()), QtServiceBase::Error);
        app->quit();
    }
}

void HttpService::pause()
{
    daemon->pause();
}

void HttpService::resume()
{
    daemon->resume();
}
