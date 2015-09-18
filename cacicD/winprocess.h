#ifndef WINPROCESS
#define WINPROCESS

#include <QtCore>

#ifdef Q_OS_WIN

#include <windows.h>
#include <tlhelp32.h>
namespace WinProcess {

    typedef enum STARTPROCESS_ERROR {
        NOT_ERROR,
        OPENPROCESS_ERROR,
        OPENPROCESSTOKEN_ERROR,
        DUPLICATETOKENEX_ERROR,
        CREATEPROCESSASUSER_ERROR
    } STARTPROCESS_ERROR;

    DWORD FindProcessId(const std::wstring& processName,const DWORD& dwSessionId);
    int StartProcessInSession(const DWORD& originPid, QString &errorCodeString);

}
#endif //ifdef Q_OS_WIN

#endif // WINPROCESS

