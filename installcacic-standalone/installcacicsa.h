#ifndef INSTALLCACICSA_H
#define INSTALLCACICSA_H

#include "commsa.h"
#include <windows.h>
#include <wincrypt.h>
#include <iostream>
#include <servicecontroller.h>
#include <shlwapi.h>
#include <../src/tinydir.h>

#define BUFSIZE 1024
#define MD5LEN  16


class InstallCacicSA
{
public:
    InstallCacicSA(const std::string &url, const std::string &user, const std::string &pass);
    ~InstallCacicSA();

    bool registryExists(HKEY RootKey, LPCTSTR SubKey);
    bool downloadService(const std::string &rota, const std::string &path);
    bool downloadMsi(const std::string &rota, const std::string &path);
    bool installService(const std::string &serviceName, const std::string &serviceBinPath);
    bool ping();
    bool getConfig();
    bool comparaHash();
    bool verificaServico();
    bool installCacic(const std::string &msiPath);
    bool removeCacic(const std::string &msiPath);
    bool deleteCacic26();
    bool deleteCacic28();
    bool setHashLocal(const std::string &hash);
    bool setHashRemoto(const std::string &hash);
    bool setPort(int port);
    bool fileExists(const std::string &filePath);
    bool informaGerente(const std::string &error);
    bool runProgram(const std::string &applicationPath, const std::string &parameters);
    std::string getHashFromFile(const std::string &filePath);
    std::string getHashLocal();
    std::string getHashRemoto();
    std::string getUrl() const;
    void setUrl(const std::string &value);
    bool delFolder(const std::string &path, const std::string fileException[], const int &numException, bool *exceptionFound = NULL);

private:
    std::string url;
    std::string user;
    std::string pass;
    std::string hashLocal;
    std::string hashRemoto;
    std::string cacicPath;
    CommSA comm;
};

#endif // INSTALLCACICSA_H
