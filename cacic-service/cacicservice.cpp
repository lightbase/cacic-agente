#include "cacicservice.h"

CacicService::CacicService(int argc, char **argv)
    : QtService<QCoreApplication>(argc, argv, "CacicDaemon")
{
    setServiceDescription("A dummy Cacic service implemented with Qt");
    setServiceFlags(QtServiceBase::CanBeSuspended);
}

void CacicService::pause()
{
    daemon->pause();
}

void CacicService::resume()
{
    daemon->resume();
}

void CacicService::stop()
{
    daemon->quit();
    app->quit();
}

void CacicService::start()
{
    app = application();

    daemon = new CacicDaemon(app);
    daemon->start();

qDebug() << daemon->isFinished();
qDebug() << daemon->isRunning();

    if (!daemon->isRunning()) {
        qDebug() << "Inicialização do daemon falhou";
        app->quit();
    }
}
