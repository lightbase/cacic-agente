#include "workerthread.h"

WorkerThread::WorkerThread()
{
}

void WorkerThread::run()
{
    //Inicia o EventLoop da thread
    exec();
}
