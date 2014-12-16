#include "servicecontroller.h"

ServiceController::ServiceController()
{
}

bool ServiceController::open(std::wstring name)
{
    // Abre o ServiceControlerMananger

    schSCManager = OpenSCManager(
                NULL,                    // local computer
                NULL,                    // servicesActive database
                SC_MANAGER_ALL_ACCESS);  // full access rights

    if (NULL == schSCManager)
    {
//        printf("OpenSCManager failed (%d)\n", GetLastError());
        return false;
    }

    // Abre o serviço

    schService = OpenService(
                schSCManager,         // SCM database
                name.c_str(),         // name of service
                SERVICE_ALL_ACCESS);  // full access

    if (schService == NULL)
    {
//        printf("OpenService failed (%d)\n", GetLastError());
        CloseServiceHandle(schSCManager);
        return false;
    }
    return true;
}

bool ServiceController::start(){
    SERVICE_STATUS_PROCESS ssStatus;
    DWORD dwBytesNeeded;

    // Verifica o status em caso de não estar parado.

    if (!QueryServiceStatusEx(
                schService,                     // handle to service
                SC_STATUS_PROCESS_INFO,         // information level
                (LPBYTE) &ssStatus,             // address of structure
                sizeof(SERVICE_STATUS_PROCESS), // size of structure
                &dwBytesNeeded ) )              // size needed if buffer is too small
    {
//        printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
        return false;
    }

    if(ssStatus.dwCurrentState != SERVICE_STOPPED && ssStatus.dwCurrentState != SERVICE_STOP_PENDING)
    {
//        printf("Cannot start the service because it is already running\n");
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
//        printf("StartService failed (%d)\n", GetLastError());
        return false;
    }
//    else printf("Service start pending...\n");

    // Verifica o status até que não fique pendente

    if (!QueryServiceStatusEx(
                schService,                     // handle to service
                SC_STATUS_PROCESS_INFO,         // info level
                (LPBYTE) &ssStatus,             // address of structure
                sizeof(SERVICE_STATUS_PROCESS), // size of structure
                &dwBytesNeeded ) )              // if buffer too small
    {
//        printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
        return false;
    }
    // Save the tick count and initial checkpoint.
    this->waitPending();

    // Determine whether the service is running.

    if (ssStatus.dwCurrentState == SERVICE_RUNNING)
    {
        printf("Service started successfully.\n");
        return true;
    }
    else
    {
//        printf("Service not started. \n");
//        printf("  Current State: %d\n", ssStatus.dwCurrentState);
//        printf("  Exit Code: %d\n", ssStatus.dwWin32ExitCode);
//        printf("  Check Point: %d\n", ssStatus.dwCheckPoint);
//        printf("  Wait Hint: %d\n", ssStatus.dwWaitHint);
        return false;
    }
}

bool ServiceController::stop()
{
    SERVICE_STATUS_PROCESS ssStatus;
    LPSERVICE_STATUS lpsStatus;
    DWORD dwBytesNeeded;

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
        return true;
    } else if (lpsStatus->dwCurrentState != SERVICE_STOP_PENDING){
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

        return false;
    }
    if (ssStatus.dwCurrentState == SERVICE_STOPPED){

        return true;
    } else {
        return false;
    }
}

void ServiceController::close()
{
    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
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
//            printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
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
                printf("Timeout waiting\n");
                return false;
            }
        }
    }
    return true;
}

