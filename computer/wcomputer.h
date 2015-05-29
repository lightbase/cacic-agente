#ifndef COMPUTER_H
#define COMPUTER_H

#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

#pragma comment(lib, "User32.lib")

#define BUFSIZE 256

#define VISTA TEXT("Vista")
#define WIN7  TEXT("Windows 7")
#define Win2K8 TEXT("Windows Server 2008")
typedef void (WINAPI *PGETSYSTEMINFO)(LPSYSTEM_INFO);
typedef BOOL (WINAPI *PGETPRODUCTINFO)(DWORD, DWORD, DWORD, DWORD, PDWORD);

class Computer
{
public:
    Computer();
    ~Computer();

    // Recupera dados do So
    std::string getSo();
    std::string getUsuarioSo();

private:
    std::string so;
    std::string usuarioSo;

    // Métodos para buscar parâmetros do Computador
    std::string setSo();
    std::string setUsuarioSo();
    BOOL GetOsDisplayString(LPTSTR pszOS);
};


#endif // COMPUTER_H
