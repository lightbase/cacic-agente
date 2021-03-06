#ifndef INSTALLCACICSA_H
#define INSTALLCACICSA_H

#include "commsa.h"
#include <windows.h>
#include <tlhelp32.h>
#include <wincrypt.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include "../src/servicecontroller.h"
#include <shlwapi.h>
#include "../src/tinydir.h"
#include "../src/wcomputer.h"

#define BUFSIZE 1024
#define MD5LEN  16
#define CACIC_REGISTRY L"SOFTWARE\\Lightbase\\Cacic"


class InstallCacicSA
{
public:
    InstallCacicSA(const std::string &url, const std::string &user, const std::string &pass);
    ~InstallCacicSA();

    bool registryExists(HKEY RootKey, LPCTSTR SubKey);
    bool downloadService(const std::string &path);
    bool downloadMsi(const std::string &path);
    bool installService(const std::string &serviceName, const std::string &serviceBinPath);
    bool ping();
    bool getConfig();
    bool comparaHash();
    bool verificaServico();
    bool installCacic(const std::string &msiPath);
    bool removeCacic(const std::string &msiPath);
    bool deleteCacicAntigo();
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
    std::string getInstallDir();
    void setUrl(const std::string &value);
    bool delFolder(const std::string &path, const std::string fileException[], const int &numException, bool *exceptionFound = NULL);
    bool log(const char *message);
    bool log(const char *message, const char *level);
    bool log(double codigo, const char *user, const char *so, const char *message);
    bool log(double codigo, const char *user, const char *so, const char *message, const char *level);
    std::string createInstallDir();
    bool removeInstallDir();
    std::string getStrTime();
    std::string createLogFile();
    bool removeLogFile();
    BOOL isAdmin();
    bool cacicInstalado();
    void stopProc(const std::string &procName);
    void stopProc(const std::string *procName, int &numProc);
    bool removeTPPrograms();
    bool dirExists(const std::string &path);
    bool setValueToRegistry(const std::string &company, const std::string &software, const std::string &key, const std::string &value);

    // Informações do SO
    std::string getSo();
    std::string getUsuarioSo();
    int getNetworkInfo(struct networkInfo *ni);
    int getValidNetwork(struct networkInfo *net);

    // Instalação
    bool exec();
    bool execRemove();

    // Debug
    bool debug;

private:
    std::string url;
    std::string user;
    std::string pass;
    std::string hashLocal;
    std::string hashRemoto;
    std::string cacicPath;
    std::string installDir;
    std::string logFile;
    CommSA comm;
    Computer comp;
};

#endif // INSTALLCACICSA_H
