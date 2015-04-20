#ifndef INSTALLCACICSA_H
#define INSTALLCACICSA_H

#include <windows.h>

class InstallCacicSA
{
public:
    InstallCacicSA();
    ~InstallCacicSA();

    bool registryExists(HKEY RootKey, LPCTSTR SubKey);
};

#endif // INSTALLCACICSA_H
