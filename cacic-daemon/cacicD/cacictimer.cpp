#include "cacictimer.h"

CacicTimer::CacicTimer()
{
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(mslot()));
}

void CacicTimer::iniciarTimer(int x)
{
    timer->start(x);
}

void CacicTimer::mslot()
{
    qDebug() << "timer executado";
}
