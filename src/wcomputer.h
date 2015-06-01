#ifndef COMPUTER_H
#define COMPUTER_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <tchar.h>
#include <strsafe.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "User32.lib")

#define BUFSIZE 256

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

#define MAX_NETINFO 5
#define VISTA TEXT("Vista")
#define WIN7  TEXT("Windows 7")
#define Win2K8 TEXT("Windows Server 2008")
typedef void (WINAPI *PGETSYSTEMINFO)(LPSYSTEM_INFO);
typedef BOOL (WINAPI *PGETPRODUCTINFO)(DWORD, DWORD, DWORD, DWORD, PDWORD);

typedef struct networkInfo{
    char ip[15];
    char subnetMask[15];
} networkInfo;

class Computer
{
public:
    Computer();
    ~Computer();

    // Recupera dados do So
    std::string getSo();
    std::string getUsuarioSo();
    int getNetworkInfo(struct networkInfo* arrayNetInfo);


private:
    std::string so;
    std::string usuarioSo;
    struct networkInfo arrayNetInfo[MAX_NETINFO];

    // Métodos para buscar parâmetros do Computador
    std::string setSo();
    std::string setUsuarioSo();
    int setNetworkAdapters(struct networkInfo arrayNetInfo[]);
    BOOL GetOsDisplayString(LPTSTR pszOS);
};


#endif // COMPUTER_H
