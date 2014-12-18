#if defined(__WIN32)
#include "servicecontroller.h"

ServiceController::ServiceController(std::wstring serviceName)
{
    this->serviceName = serviceName;
}

bool ServiceController::open(DWORD accessManager, DWORD accessService)
{
    if(!this->openMananger(accessManager)){
        return false;
    }
    // Abre o serviço

    schService = OpenServiceW(
                schSCManager,         // SCM database
                serviceName.c_str(),         // name of service
                accessService);  // full access

    if (schService == NULL)
    {
        this->trataErro(GetLastError());
        CloseServiceHandle(schSCManager);
        return false;
    }
    return true;
}

bool ServiceController::start(){
    SERVICE_STATUS_PROCESS ssStatus;
    DWORD dwBytesNeeded;

    if (!this->open(SC_MANAGER_ALL_ACCESS, SERVICE_QUERY_STATUS | SERVICE_START)){
        return false;
    }
    // Verifica o status em caso de não estar parado.

    if (!QueryServiceStatusEx(
                schService,                     // handle to service
                SC_STATUS_PROCESS_INFO,         // information level
                (LPBYTE) &ssStatus,             // address of structure
                sizeof(SERVICE_STATUS_PROCESS), // size of structure
                &dwBytesNeeded ) )              // size needed if buffer is too small
    {
        this->trataErro(GetLastError());
        this->close();
        return false;
    }

    if(ssStatus.dwCurrentState != SERVICE_STOPPED && ssStatus.dwCurrentState != SERVICE_STOP_PENDING)
    {
        this->trataErro(GetLastError());
        this->close();
        return false;
    }

    // Espera o serviço parar compeltamente.

    this->waitPending();

    // Começa a startar o serviço

    if (!StartService(
                schService,  // handle to service
                0,           // number of arguments
                NULL) )      // no arguments
    {
        this->trataErro(GetLastError());
        this->close();
        return false;
    }

    //Espera até startar de verdade
    this->waitPending();

    if (!QueryServiceStatusEx(
                schService,                     // handle to service
                SC_STATUS_PROCESS_INFO,         // info level
                (LPBYTE) &ssStatus,             // address of structure
                sizeof(SERVICE_STATUS_PROCESS), // size of structure
                &dwBytesNeeded ) )              // if buffer too small
    {
        this->trataErro(GetLastError());
        this->close();
        return false;
    }

    if (ssStatus.dwCurrentState == SERVICE_RUNNING)
    {
        this->close();
        return true;
    }
    else
    {
//        printf("Service not started. \n");
//        printf("  Current State: %d\n", ssStatus.dwCurrentState);
//        printf("  Exit Code: %d\n", ssStatus.dwWin32ExitCode);
//        printf("  Check Point: %d\n", ssStatus.dwCheckPoint);
//        printf("  Wait Hint: %d\n", ssStatus.dwWaitHint);

        this->trataErro(GetLastError());
        this->close();
        return false;
    }
}

bool ServiceController::stop()
{
    SERVICE_STATUS_PROCESS ssStatus;
    LPSERVICE_STATUS lpsStatus;
    DWORD dwBytesNeeded;

    if(!this->open(SC_MANAGER_ALL_ACCESS, SERVICE_STOP | SERVICE_QUERY_STATUS)){
        return false;
    }
    //lpsStatus deve ser inicializada antes.
    lpsStatus = (LPSERVICE_STATUS) malloc(sizeof(LPSERVICE_STATUS));

    ControlService(
      schService,
      SERVICE_CONTROL_STOP,
      lpsStatus
    );

    // Verifica o status em caso de não estar parado.
    if (lpsStatus->dwCurrentState == SERVICE_STOPPED){
//        printf("Serviço parado com sucesso");
        this->close();
        return true;
    } else if (lpsStatus->dwCurrentState != SERVICE_STOP_PENDING){
        this->trataErro(GetLastError());
        this->close();
        return false;
    }

    // Verifica o status até que não fique pendente
    this->waitPending();

    if (!QueryServiceStatusEx(
                schService,                     // handle to service
                SC_STATUS_PROCESS_INFO,         // info level
                (LPBYTE) &ssStatus,             // address of structure
                sizeof(SERVICE_STATUS_PROCESS), // size of structure
                &dwBytesNeeded ) )              // if buffer too small
    {
        this->trataErro(GetLastError());
        this->close();
        return false;
    }
    if (ssStatus.dwCurrentState == SERVICE_STOPPED){
        this->close();
        return true;
    } else {
        this->trataErro(GetLastError());
        this->close();
        return false;
    }
}

bool ServiceController::install(std::wstring servicePath, std::wstring displayName)
{
    if (this->isInstalled()){
        this->trataErro(ERROR_SERVICE_EXISTS);
        return false;
    }
    if(!this->openMananger(SC_MANAGER_ALL_ACCESS)){
        this->trataErro(GetLastError());
        return false;
    }

    schService = CreateService(
                    schSCManager,                // SCM database
                    serviceName.c_str(),         // nome do serviço
                    (displayName != L"" ?
                        displayName.c_str() :
                        serviceName.c_str()),    // nome 'fantasia' do serviço
                    SERVICE_ALL_ACCESS,          // acesso desejado
                    SERVICE_WIN32_OWN_PROCESS |
                        SERVICE_INTERACTIVE_PROCESS, // tipo de serviço
                    SERVICE_AUTO_START,          // tipo de inicialização
                    SERVICE_ERROR_NORMAL,        // tipo de controle de erro
                    servicePath.c_str(),         // path do serviço
                    NULL,                        // no load ordering group
                    NULL,                        // no tag identifier
                    NULL,                        // no dependencies
                    NULL,                        // LocalSystem account
                    NULL);                       // no password

    if (schService == NULL)
    {
        this->trataErro(GetLastError());
        this->close();
        return false;
    }
    else{
        this->close();
        return true;
    }
}

bool ServiceController::uninstall()
{
    this->close();
    if(!this->open(SC_MANAGER_ALL_ACCESS, SERVICE_STOP | DELETE)){
        return false;
    }
    //para o serviço para não ficar rodando mesmo após a desinstalação.
    if (this->isRunning()){
        if (!this->stop()){
            this->trataErro(GetLastError());
            this->close();
            return false;
        }
    }

    if (!DeleteService(schService)){
        this->trataErro(GetLastError());
        this->close();
        return false;
    } else {
        this->close();
        return true;
    }

}

bool ServiceController::isInstalled()
{
    bool result = false;
    // Abre o mananger
    if (this->openMananger(0)) {
        // Tenta abrir o serviço
        this->schService = OpenService(this->schSCManager,
                                       this->serviceName.c_str(),
                                       SERVICE_QUERY_CONFIG);

        if (this->schService) {
            result = true;
            CloseServiceHandle(this->schService);
        }
        CloseServiceHandle(this->schSCManager);
    }
    return result;
}

bool ServiceController::isRunning()
{
    SERVICE_STATUS_PROCESS ssStatus;
    DWORD dwBytesNeeded;

    if(!this->open(SC_MANAGER_ALL_ACCESS, SERVICE_QUERY_STATUS)){
        return false;
    }

    if (!QueryServiceStatusEx(
                schService,                     // handle to service
                SC_STATUS_PROCESS_INFO,         // info level
                (LPBYTE) &ssStatus,             // address of structure
                sizeof(SERVICE_STATUS_PROCESS), // size of structure
                &dwBytesNeeded ) )              // if buffer too small
    {
        this->trataErro(GetLastError());
        this->close();
        return false;
    }
    if (ssStatus.dwCurrentState == SERVICE_RUNNING){
        this->close();
        return true;
    } else {
        this->close();
        return false;
    }
}

void ServiceController::close()
{
    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
}

std::string ServiceController::getLastError() const
{
    return lastError;
}
int ServiceController::getILastError() const
{
    return iLastError;
}

bool ServiceController::waitPending()
{
    DWORD dwOldCheckPoint;
    DWORD dwStartTickCount;
    DWORD dwWaitTime;
    DWORD dwBytesNeeded;
    SERVICE_STATUS_PROCESS ssStatus;

    if (!QueryServiceStatusEx(
                schService,                     // handle to service
                SC_STATUS_PROCESS_INFO,         // info level
                (LPBYTE) &ssStatus,             // address of structure
                sizeof(SERVICE_STATUS_PROCESS), // size of structure
                &dwBytesNeeded ) )              // if buffer too small
    {
        this->trataErro(GetLastError());
        return false;
    }
    // Save the tick count and initial checkpoint.

    dwStartTickCount = GetTickCount();
    dwOldCheckPoint = ssStatus.dwCheckPoint;

    while (ssStatus.dwCurrentState == SERVICE_STOP_PENDING || ssStatus.dwCurrentState == SERVICE_START_PENDING)
    {
        // Não esperar muito. Um bom tempo é um décimo do tempo do
        // waitHint, mas não menos que 1 segundo e não mais que 10.
        dwWaitTime = ssStatus.dwWaitHint / 10;

        if( dwWaitTime < 1000 )
            dwWaitTime = 1000;
        else if ( dwWaitTime > 10000 )
            dwWaitTime = 10000;
        Sleep( dwWaitTime );

        // Verifica se está pendente ainda.

        if (!QueryServiceStatusEx(
                    schService,                     // handle to service
                    SC_STATUS_PROCESS_INFO,         // information level
                    (LPBYTE) &ssStatus,             // address of structure
                    sizeof(SERVICE_STATUS_PROCESS), // size of structure
                    &dwBytesNeeded ) )              // size needed if buffer is too small
        {
            this->trataErro(GetLastError());
            return false;
        }

        if ( ssStatus.dwCheckPoint > dwOldCheckPoint )
        {
            // Continue esperando e verificando.

            dwStartTickCount = GetTickCount();
            dwOldCheckPoint = ssStatus.dwCheckPoint;
        }
        else
        {
            if(GetTickCount()-dwStartTickCount > 30000)
            {
                this->trataErro(ERROR_TIMEOUT);
                return false;
            }
        }
    }
    return true;
}

bool ServiceController::openMananger(DWORD managerAccess)
{
    // Abre o ServiceControlerMananger

    schSCManager = OpenSCManagerW(
                NULL,                    // local computer
                NULL,                    // servicesActive database
                managerAccess);  // full access rights

    if (NULL == schSCManager)
    {
        this->trataErro(GetLastError());
        return false;
    } else {
        return true;
    }
}

int ServiceController::trataErro(DWORD error)
{
    if (error == ERROR_ACCESS_DENIED){
        this->lastError = "Acesso negado";
        this->iLastError = 0;
    } else if (error == ERROR_CIRCULAR_DEPENDENCY){
        this->lastError = "A circular service dependency was specified";
        this->iLastError = 1;
    } else if (error == ERROR_DUPLICATE_SERVICE_NAME){
        this->lastError = "Nome do serviço já existente";
        this->iLastError = 2;
    } else if (error == ERROR_INVALID_HANDLE){
        this->lastError = "Service Control Mananger inválido";
        this->iLastError = 3;
    } else if (error == ERROR_INVALID_NAME){
        this->lastError = "Nome inválido";
        this->iLastError = 4;
    } else if (error == ERROR_INVALID_PARAMETER){
        this->lastError = "Parâmetro invalido";
        this->iLastError = 5;
    } else if (error == ERROR_INVALID_SERVICE_ACCOUNT){
        this->lastError = "Conta do serviço inválida";
        this->iLastError = 6;
    } else if (error == ERROR_SERVICE_EXISTS){
        this->lastError = "Serviço já existente";
        this->iLastError = 7;
    } else if (error == ERROR_SERVICE_MARKED_FOR_DELETE){
        this->lastError = "Serviço marcado para exclusão";
        this->iLastError = 8;
    } else if (error == ERROR_TIMEOUT){
        this->lastError = "Tempo limite atingido";
        this->iLastError = 9;
    } else if (error == ERROR_PATH_NOT_FOUND){
        this->lastError = "Path não encontrado";
        this->iLastError = 10;
    } else if (error == ERROR_SERVICE_ALREADY_RUNNING){
        this->lastError = "Serviço já está rodando";
        this->iLastError = 11;
    } else if (error == ERROR_SERVICE_DATABASE_LOCKED){
        this->lastError = "A base de dados de serviço está trancada";
        this->iLastError = 12;
    } else if (error == ERROR_SERVICE_DISABLED){
        this->lastError = "O serviço foi desativado";
        this->iLastError = 13;
    } else if (error == ERROR_SERVICE_REQUEST_TIMEOUT){
        this->lastError = "O processo do serviço foi iniciado, mas não completou a operação";
        this->iLastError = 14;
    } else if (error == ERROR_SERVICE_NOT_ACTIVE){
        this->lastError = "O serviço não está ativo";
        this->iLastError = 15;
    } else if (error == ERROR_SERVICE_DOES_NOT_EXIST){
        this->lastError = "O serviço não existe";
        this->iLastError = 16;
    } else {
        this->lastError = "Desconhecido: " + std::to_string(error);
        this->iLastError = -1;
    }

    return this->iLastError;
}

#endif // defined(Q_OS_WIN)
