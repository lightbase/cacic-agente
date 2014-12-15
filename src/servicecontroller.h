#ifndef SERVICECONTROLLER_H
#define SERVICECONTROLLER_H
#include <Windows.h>
#include <stdlib.h>
#include <string>

class ServiceController
{
public:
    ServiceController();
    bool open(std::wstring name);
    bool start();
    bool stop();
    void close();
private:
    SC_HANDLE schSCManager, schService;

    bool waitPending();
};

#endif // SERVICECONTROLLER_H
