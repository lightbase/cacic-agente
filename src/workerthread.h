#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include "logcacic.h"
#include "identificadores.h"

class WorkerThread: public QThread
{
public:
    WorkerThread();
    void run();
};

#endif // WORKERTHREAD_H
