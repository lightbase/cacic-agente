#ifndef CACICSERVICE_H
#define CACICSERVICE_H

#include <QDebug>
#include <QString>
#include "qtservice.h"
#include "cacicdaemon.h"

class CacicService : public QtService<QCoreApplication>
{

public:
    CacicService(int argc, char **argv);

protected:

    void pause();
    void resume();
    void start();
    void stop();

private:
    CacicDaemon *daemon;
    QCoreApplication *app;
};

#endif // CACICSERVICE_H
