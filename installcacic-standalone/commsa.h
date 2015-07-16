#ifndef COMMSA_H
#define COMMSA_H

#include <iostream>
#include <winsock2.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include "../src/simplejson/src/JSON.h"

#define MAX_HEADERS 10
#define MAX_ELEMENT_SIZE 500
#define BUFFERSIZE 4096

#define ROUTE_HASH "/ws/instala/hash"
#define ROUTE_DOWNLOAD "/ws/instala/download/service"
#define ROUTE_DOWNLOAD_MSI "/ws/instala/download/msi"
#define ROUTE_MAPA_LDAP "/ws/neo/mapa/ldap"
#define ROUTE_MAPA_FORM "/ws/neo/mapa/form"
#define ROUTE_MAPA_GETMAPA "/ws/neo/mapa/config"
#define ROUTE_ERRO "/ws/instala/erro"
#define ROUTE_LOG "/ws/neo/log"
#define ROUTE_COLETA "/ws/neo/coleta"
#define CACIC_SERVICE_BIN "cacic-service.exe"
#define CACIC_MSI "Cacic.msi"
#define CACIC_VERSION "3.1.15"

class CommSA
{
public:
    CommSA();
    ~CommSA();

    std::string sendReq(const char* parameters = "");
    std::string sendReq(const char* host, const char* route, const char* method);
    bool downloadFile(const char *url, const char *filePath);
    bool log(const char *message);
    bool log(double codigo, const char *user, const char *so, const char *message);
    std::string getConfig();

    const char *getHost() const;
    void setHost(const char *value);

    int getPort() const;
    void setPort(int value);

    const char *getMethod() const;
    void setMethod(const char *value);

    const char *getType() const;
    void setType(const char *value);

    const char *getRoute() const;
    void setRoute(const char *value);

    int getTimeOut() const;
    void setTimeOut(int value);

    std::string getBody(std::string request) const;
    
    const wchar_t *GetWC(const char *c);

    bool setNetworkInfo(const char *ip, const char *subnetMask);

private:

    const char* host;
    const char* user;
    const char* pass;
    const char* route;
    const char* method;
    const char* type;
    const char* parameters;
    int port;
    int timeOut;
    const char* ip;
    const char* subnetMask;

    std::string sendReq(const char* host, const char* route, const char* method, const char* type, int port, const char* parameters);
};

#endif // COMMSA_H
