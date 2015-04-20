#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#pragma comment(lib,"ws2_32.lib")

using namespace std;
int main (){
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        cout << "WSAStartup failed.\n";
        system("pause");
        return 1;
    }
    SOCKET Socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    struct hostent *host;
    host = gethostbyname("teste.cacic.cc");
    SOCKADDR_IN SockAddr;
    SockAddr.sin_port=htons(80);
    SockAddr.sin_family=AF_INET;
    SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
    cout << "Connecting...\n";
    if(connect(Socket,(SOCKADDR*)(&SockAddr),sizeof(SockAddr)) != 0){
        cout << "Could not connect";
        system("pause");
        return 1;
    }
    cout << "Connected.\n";
    std::string request;
    request = "POST /ws/neo/config HTTP/1.1\n";
    request.append("Host: teste.cacic.cc \nConnection: close\n");
    request.append("Content-Type: application/json; charset=utf-8\n\n\n");
    request.append("{\"computador\":{\"networkDevices\":[{\"ipv4\":\"10.0.2.15\",\"ipv6\":\"fe80::5164:546f:54bc:9e46%11\",\"mac\":\"08:00:27:54:D7:A4\",\"netmask_ipv4\":\"255.255.255.0\",\"netmask_ipv6\":\"ffff:ffff:ffff:ffff::\",\"nome\":\"Local Area Connection\"},{\"ipv4\":\"2001:0:9d38:6ab8:409:3c41:f5ff:fdf0\",\"ipv6\":\"fe80::409:3c41:f5ff:fdf0%13\",\"mac\":\"00:00:00:00:00:00:00:E0\",\"netmask_ipv4\":\"::\",\"netmask_ipv6\":\"ffff:ffff::\",\"nome\":\"Local Area Connection* 9\"}],\"nmComputador\":\"CACICQT86-PC\",\"operatingSystem\":{\"idOs\":144,\"nomeOs\":\"Microsoft Windows 7 Ultimate\",\"tipo\":\"windows\",\"upTime\":23137},\"usuario\":\"CacicQT86\",\"versaoAgente\":\"3.1.13\",\"versaoGercols\":\"3.1.13\"}}\n");
    cout << request << endl << endl;
    send(Socket, request.c_str(), strlen(request.c_str()),0);
    char buffer[10000];
    int nDataLength;
    while ((nDataLength = recv(Socket,buffer,10000,0)) > 0){
        int i = 0;
        while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r') {
            cout << buffer[i];
            i += 1;
        }
    }
    cout << endl;
    closesocket(Socket);
    WSACleanup();
    system("pause");
    return 0;
}
