#include "commsa.h"

CommSA::CommSA()
{
    this->type = "text/plain";
    this->port = 80;
    this->method = "GET";
}

CommSA::~CommSA()
{

}

std::string CommSA::sendReq(char* buffer, const char* parameters)
{
    return this->sendReq(buffer, this->host, this->route, this->method, this->type, this->port, parameters);
}

std::string CommSA::sendReq(char* buffer, const char* host, const char* route, const char* method, const char* type, int port, const char* parameters)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
//        cout << "WSAStartup failed.\n";
        return false;
    }
    SOCKET Socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    struct hostent *shost;
    shost = gethostbyname(host);
    SOCKADDR_IN SockAddr;
    SockAddr.sin_port = htons(port);
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_addr.s_addr = *((unsigned long*)shost->h_addr);

    // Ajusta o timeout para a conexão
    int timeoutBuffer = 1000;
    setsockopt(Socket, SOL_SOCKET, SO_SNDTIMEO, (const char *)&timeoutBuffer, sizeof(timeoutBuffer));
    setsockopt(Socket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeoutBuffer, sizeof(timeoutBuffer));

    if(connect(Socket,(SOCKADDR*)(&SockAddr),sizeof(SockAddr)) != 0){
//        cout << "Could not connect";
        // Throw exception if it was not possible to connect
        return "CONNECTION_ERROR";
    }

    std::string req;
    req.append("GET");
    req.append(" ");
    req.append(route);
    req.append(" HTTP/1.0\n");
    req.append("Host: ");
    req.append(host);
    req.append(" \nConnection: close\n");
    req.append("Content-Type: ");
    req.append(type);
    req.append("; charset=utf-8\n\n\n");
    req.append(parameters);

    send(Socket, req.c_str(), strlen(req.c_str()),0);
    char buff[10000];
    int nDataLength;
    while ((nDataLength = recv(Socket,buff,10000,0)) > 0){
        int i = 0;
        while (buff[i] >= 32 || buff[i] == '\n' || buff[i] == '\r') {
//            cout << buff[i];
            i += 1;
        }
    }
    buffer = buff;
    closesocket(Socket);
    WSACleanup();
    return buff;
}
const char *CommSA::getHost() const
{
    return host;
}

void CommSA::setHost(const char *value)
{
    host = value;
}
int CommSA::getPort() const
{
    return port;
}

void CommSA::setPort(int value)
{
    port = value;
}
const char *CommSA::getMethod() const
{
    return method;
}

void CommSA::setMethod(const char *value)
{
    method = value;
}
const char *CommSA::getType() const
{
    return type;
}

void CommSA::setType(const char *value)
{
    type = value;
}
const char *CommSA::getRoute() const
{
    return route;
}

void CommSA::setRoute(const char *value)
{
    route = value;
}
