#ifndef INSTALLCACICSA_H
#define INSTALLCACICSA_H

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
    bool comparaHash();
    bool verificaServico();
    bool installCacic();
    bool deleteCacic26();
    bool deleteCacic28();
    bool setHashLocal(const std::string &hash);
    bool setHashRemoto(const std::string &hash);
private:
    std::string url;
    std::string user;
    std::string pass;
    std::string hashLocal;
    std::string hashRemoto;
};

#endif // INSTALLCACICSA_H
