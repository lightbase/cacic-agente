#ifndef COMMSA_H
#define COMMSA_H

#include <winsock2.h>
#include <iostream>
#include <stdlib.h>

#define MAX_HEADERS 10
#define MAX_ELEMENT_SIZE 500

#define ROUTE_HASH "/ws/instala/hash"
#define ROUTE_ERRO "/ws/instala/erro"

class CommSA
{
public:
    CommSA();
    ~CommSA();
    std::string sendReq(char *buffer, const char* parameters = "");

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

private:
    const char* host;
    const char* user;
    const char* pass;
    const char* route;
    const char* method;
    const char* type;
    const char* parameters;
    int port;

    std::string sendReq(char *buffer, const char* host, const char* route, const char* method, const char* type, int port, const char* parameter);

};

#endif // COMMSA_H
