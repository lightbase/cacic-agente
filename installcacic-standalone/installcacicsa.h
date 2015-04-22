#ifndef INSTALLCACICSA_H
#define INSTALLCACICSA_H

#include "commsa.h"
#include <windows.h>
#include <iostream>

class InstallCacicSA
{
public:
    InstallCacicSA(const std::string &url, const std::string &user, const std::string &pass);
    ~InstallCacicSA();

    bool registryExists(HKEY RootKey, LPCTSTR SubKey);
    bool downloadService(const std::string &rota, const std::string &path);
    bool installService();
    bool getConfig();
    bool comparaHash(const std::string &hashLocal, const std::string &hashGerente);
    bool verificaServico();
    bool installCacic();
    bool deleteCacic26();
    bool deleteCacic28();
private:
    std::string url;
    std::string user;
    std::string pass;

};

#endif // INSTALLCACICSA_H
