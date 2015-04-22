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

bool CommSA::sendReq(char* buffer, const char* parameters)
{
    return this->sendReq(buffer, this->host, this->route, this->method, this->type, this->port, parameters);
}

bool CommSA::sendReq(char* buffer, const char* host, const char* route, const char* method, const char* type, int port, const char* parameters)
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

    if(connect(Socket,(SOCKADDR*)(&SockAddr),sizeof(SockAddr)) != 0){
//        cout << "Could not connect";
        return false;
    }

    std::string request;
    request.append(method);
    request.append(" ");
    request.append(route);
    request.append(" HTTP/1.1\n");
    request.append("Host: ");
    request.append(host);
    request.append(" \nConnection: close\n");
    request.append("Content-Type: ");
    request.append(type);
    request.append("; charset=utf-8\n\n\n");
    request.append(parameters);

    send(Socket, request.c_str(), strlen(request.c_str()),0);
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
    return true;
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






