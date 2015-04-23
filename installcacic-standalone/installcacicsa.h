#ifndef INSTALLCACICSA_H
#define INSTALLCACICSA_H

#include "commsa.h"
#include <windows.h>
#include <iostream>
#include <openssl/md5.h>


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
    bool setPort(int port);
    std::string getHashFromFile(const std::string &filePath);
private:
    std::string url;
    std::string user;
    std::string pass;
    std::string hashLocal;
    std::string hashRemoto;
    CommSA comm;
};

#endif // INSTALLCACICSA_H
