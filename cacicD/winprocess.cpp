#include "winprocess.h"

#ifdef Q_OS_WIN

using namespace WinProcess;

/**
 * @brief WinProcess::FindProcessId
 * @param processName
 * @param dwSessionId
 *
 * Função para achar o PID do winlogon.exe da sessão do usuário atual.
 * @return
 */
DWORD WinProcess::FindProcessId(const std::wstring& processName, const DWORD& dwSessionId)
{
    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(processInfo);

    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if ( processesSnapshot == INVALID_HANDLE_VALUE )
        return 0;

    Process32First(processesSnapshot, &processInfo);
    if ( !processName.compare(processInfo.szExeFile) )
    {
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processInfo.th32ProcessID);

        HANDLE hToken;
        if( OpenProcessToken(hProcess,TOKEN_READ,&hToken) ) {
            DWORD ssidToken;
            DWORD returnLength = sizeof(DWORD);

            if ( GetTokenInformation(hToken,TokenSessionId,&ssidToken,returnLength,&returnLength) ) {
                if(ssidToken == dwSessionId) {
                    CloseHandle(hProcess);
                    CloseHandle(hToken);
                    CloseHandle(processesSnapshot);
                    return processInfo.th32ProcessID;
                }
            }
        }
    }

    while ( Process32Next(processesSnapshot, &processInfo) )
    {
        if ( !processName.compare(processInfo.szExeFile) )
        {
            //            CloseHandle(processesSnapshot);
            //            return processInfo.th32ProcessID;
            HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processInfo.th32ProcessID);

            HANDLE hToken;
            if( OpenProcessToken(hProcess,TOKEN_READ,&hToken) ) {
                DWORD ssidToken;
                DWORD returnLength = sizeof(DWORD);

                if ( GetTokenInformation(hToken,TokenSessionId,&ssidToken,returnLength,&returnLength) ) {
                    if(ssidToken == dwSessionId) {
                        CloseHandle(hProcess);
                        CloseHandle(hToken);
                        CloseHandle(processesSnapshot);
                        return processInfo.th32ProcessID;
                    }
                }
            }
        }
    }

    CloseHandle(processesSnapshot);
    return 0;
}

/**
 * @brief WinProcess::StartProcessInSession
 * @param winlogonPID
 * @param errorCodeString
 *
 * Função para iniciar processo do CacicUi, baseando no processo do winlogon.exe
 * com usuário System e SSID do usuário atual.
 * @return
 */
int WinProcess::StartProcessInSession(const DWORD& winlogonPID, QString &errorCodeString)
{
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, winlogonPID);
    if(hProcess == NULL) {
        errorCodeString = QString::number(GetLastError());
        return OPENPROCESS_ERROR;
    }

    HANDLE hToken;
    if(OpenProcessToken(hProcess,TOKEN_ALL_ACCESS,&hToken) == 0) {
        errorCodeString = QString::number(GetLastError());
        return OPENPROCESSTOKEN_ERROR;
    }

    // copy the access token of the winlogon process;
    // the newly created token will be a primary token
    HANDLE hUserTokenDup;
    if (!DuplicateTokenEx(hToken,MAXIMUM_ALLOWED,NULL,
                            SecurityIdentification,TokenPrimary,&hUserTokenDup)) {
        errorCodeString = QString::number(GetLastError());
        CloseHandle(hProcess);
        CloseHandle(hToken);
        return DUPLICATETOKENEX_ERROR;
    }

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    si.cb = static_cast<DWORD>(sizeof(STARTUPINFO));

    // interactive window station parameter; basically this indicates
    // that the process created can display a GUI on the desktop
    wchar_t auxBuffer[16] = L"winsta0\\default";
    si.lpDesktop = auxBuffer;

    // flags that specify the priority and creation method of the process
    int dwCreationFlags = NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_BREAKAWAY_FROM_JOB;

    // create a new process in the current User's logon session
    bool result = CreateProcessAsUser(hUserTokenDup,            // client's access token
                                    L"c://cacic//cacic-ui.exe", // file to execute
                                    NULL,                       // command line
                                    NULL,                       // pointer to process SECURITY_ATTRIBUTES
                                    NULL,                       // pointer to thread SECURITY_ATTRIBUTES
                                    false,                      // handles are not inheritable
                                    dwCreationFlags,            // creation flags
                                    NULL,                       // pointer to new environment block
                                    NULL,                       // name of current directory
                                    &si,                        // pointer to STARTUPINFO structure
                                    &pi                         // receives information about new process
                                    );

    if(!result) {
        errorCodeString = QString::number(GetLastError());
        CloseHandle(hProcess);
        CloseHandle(hToken);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        return CREATEPROCESSASUSER_ERROR;
    }

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return NOT_ERROR;
}

#endif //ifdef Q_OS_WIN
