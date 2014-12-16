#ifndef SERVICECONTROLLER_H
#define SERVICECONTROLLER_H
#include <Windows.h>
#include <stdlib.h>
#include <string>

class ServiceController
{
public:
    ServiceController();
    //Abre o serviço. Não deve ser chamada se a intenção é instalar o serviço.
    bool open(std::wstring serviceName);
    //Instala o serviço solicitado.
    bool install(std::wstring serviceName, std::wstring servicePath, std::wstring displayName = L"");
    //Starta o serviço.
    bool start();
    //Para o serviço.
    bool stop();
    //Desinstala o serviço aberto.
    bool uninstall();
    //Fecha o controle do serviço.
    void close();
    std::string getLastError() const;
    int getILastError() const;

private:
    SC_HANDLE schSCManager, schService;
    std::string lastError;
    int iLastError;

    bool waitPending();
    bool openMananger();
    int trataErro(DWORD error);
};

#endif // SERVICECONTROLLER_H
