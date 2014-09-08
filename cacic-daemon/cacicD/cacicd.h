#ifndef CACICD_H
#define CACICD_H

#include <QCoreApplication>
#include "qtservice.h"
#include "cacictimer.h"


class cacicD : public QtService<QCoreApplication>
{
public:
    QCoreApplication *app;
    CacicTimer *Ocacictimer;
    CCacic *ccacic;
    cacicD(int argc, char **argv);
    ~cacicD();
    void start();
    void pause();
    void resume();
    void stop();
};
#endif // CACICD_H
