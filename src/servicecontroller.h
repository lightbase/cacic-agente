#if defined(__WIN32)

#ifndef SERVICECONTROLLER_H
#define SERVICECONTROLLER_H
#include <Windows.h>
#include <string>

class ServiceController
{
public:
    ServiceController(std::wstring serviceName);
    //Instala o serviço solicitado.
    bool install(std::wstring servicePath, std::wstring displayName = L"");
    //Starta o serviço.
    bool start();
    //Para o serviço.
    bool stop();
    //Desinstala o serviço aberto.
    bool uninstall();
    bool isInstalled();
    bool isRunning();
    std::string getLastError() const;
    int getILastError() const;

private:
    SC_HANDLE schSCManager, schService;
    std::string lastError;
    int iLastError;
    std::wstring serviceName;

    bool open(DWORD accessManager, DWORD accessService);
    bool waitPending();
    bool openMananger(DWORD managerAccess);
    int trataErro(DWORD error, std::string detailError);
    //Fecha o controle do serviço.
    void close();
};

#endif // SERVICECONTROLLER_H
#endif // defined(Q_OS_WIN)
