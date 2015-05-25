#include "commsa.h"

CommSA::CommSA()
{
    this->type = "text/plain";
    this->port = 80;
    this->method = "GET";
    this->timeOut = 1000;
}

CommSA::~CommSA()
{

}

std::string CommSA::sendReq(const char* parameters)
{
    return this->sendReq(this->host, this->route, this->method, this->type, this->port, parameters);
}

std::string CommSA::sendReq(const char* host, const char* route, const char* method, const char* type, int port, const char* parameters)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        std::cout << "WSAStartup failed.\n";
        return "";
    }
    SOCKET Socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    struct hostent *shost;
    shost = gethostbyname(host);
    SOCKADDR_IN SockAddr;
    SockAddr.sin_port = htons(port);
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_addr.s_addr = *((unsigned long*)shost->h_addr);
    // Ajusta o timeout para a conexão
    setsockopt(Socket, SOL_SOCKET, SO_SNDTIMEO, (const char *)&this->timeOut, sizeof(this->timeOut));
    setsockopt(Socket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&this->timeOut, sizeof(this->timeOut));
    if(connect(Socket,(SOCKADDR*)(&SockAddr),sizeof(SockAddr)) != 0){
        std::cout << "Could not connect";
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
//    req.append(parameters);

    send(Socket, req.c_str(), strlen(req.c_str()),0);
    char buff[10000];
    int nDataLength;

    //Recebe dados
    nDataLength = recv(Socket,buff,10000,0) > 0;
    closesocket(Socket);
    WSACleanup();

    if (nDataLength != SOCKET_ERROR) {
        // Recupera body da requisição
        std::string body = this->getBody(std::string(buff));
        return body;
    } else {
        return "CONNECTION_ERROR";
    }
}

std::string CommSA::getBody(std::string request) const
{
    /*!
     * \brief results
     *
     * Método que encontra o body da requisição
     *
     */

    std::string start ("{");
    std::string end ("}");

    std::size_t found_s = request.find(start);
    //std::cout << "Índice do começo: " << found_s << std::endl;
    std::size_t found_e = request.find(end);
    //std::cout << "Índice do fim: " << found_e << std::endl;
    std::size_t size = (found_e - found_s);

    if (size <= 1) {
        // String vazia. Retorna
//        std::cout << "String vazia" << std::endl;
        return "";
    }

    // Agora retorna a substring
    std::string body_char = request.substr(found_s, (size+1));
    //std::cout << "Valor encontrado: " << body_char << std::endl;
    std::wstring body_str (body_char.begin(), body_char.end());

    // Tenta associar o a string ao struct do map
    JSONValue *value = JSON::Parse(body_str.c_str());
    if (value == NULL) {
//        std::cout << "Valor nulo" << std::endl;
        return "";
    } else {
        JSONObject root = value->AsObject();
        std::wstring ws;
        if (root.find(L"valor") != root.end() && root[L"valor"]->IsString()){
            ws = root[L"valor"]->AsString();
        } else {
            return "";
        }
        //std::wcout << "Valor encontrado: " << ws << std::endl;
        std::string s(ws.begin(), ws.end());
        return s;
    }
}

bool CommSA::downloadFile(const char *url, const char *filePath)
{
    std::string request; // HTTP Header //
    LPCSTR filename = filePath;
    char buffer[BUFFERSIZE];
    struct sockaddr_in serveraddr;
    int sock;

    WSADATA wsaData;
    int port = 80;

    std::string urlAux(url);
    // Remove's http:// part //,

    if(urlAux.find("http://") != -1){
        std::string temp = urlAux;
        urlAux = temp.substr(urlAux.find("http://") + 7);
    }
    // Split host and file location //
    int dm = urlAux.find("/");
    std::string file = urlAux.substr(dm);
    dm = urlAux.find("/");
    std::string shost = urlAux.substr(0, dm);
    // Generate http header //
    request += "GET " + file + " HTTP/1.0\r\n";
    request += "Host: " + shost + "\r\n";
    request += "\r\n";

    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
        std::cout << "Sem conexão 2!!!" << std::endl;
        return false;
    }

    //Verifica se é possível conexão com a url repassada
    if (!gethostbyaddr(shost.c_str(), shost.size(),AF_INET)){
        // Eduardo: 2015-05-25
        // Não precisa falhar por causa disso. Se for DNS essa função não passa.
        //return false;
    }

    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        std::cout << "Sem conexão 3!!!" << std::endl;
        return false;
    }

    memset(&serveraddr, 0, sizeof(serveraddr));

    // ip address of link //
    hostent *record = gethostbyname(shost.c_str());
    in_addr *address = (in_addr * )record->h_addr;
    std::string ipd = inet_ntoa(* address);
    const char *ipaddr = ipd.c_str();

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(ipaddr);
    serveraddr.sin_port = htons(port);

    if (connect(sock, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) {
        std::cout << "Erro na conexão!!!" << std::endl;
        return false;
    }

    if (send(sock, request.c_str(), request.length(), 0) != request.length()) {
        std::cout << "Erro na conexão!!!" << std::endl;
        return false;
    }

    int nRecv, npos;
    nRecv = recv(sock, (char*)&buffer, BUFFERSIZE, 0);
    // getting end of header //
    std::string str_buff = buffer;

    npos = str_buff.find("\r\n\r\n");

    // open the file in the beginning //
    HANDLE hFile;
    hFile = CreateFileA(filename,
                        GENERIC_WRITE,
                        FILE_SHARE_WRITE,
                        NULL, CREATE_ALWAYS,
                        FILE_ATTRIBUTE_NORMAL, 0);
    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);

    // Download file //
    DWORD ss;
    bool result;
    while((nRecv > 0) && (nRecv != INVALID_SOCKET)){
        if(npos > 0){
            char bf[BUFFERSIZE];
            // copy from end position of header //
            memcpy(bf, buffer + (npos + 4), nRecv - (npos + 4));
            result = WriteFile(hFile, bf,nRecv - (npos + 4), &ss, NULL);
        }else{
            // write normally if end not found //
            result = WriteFile(hFile, buffer, nRecv, &ss, NULL);
        }

        // buffer cleanup  //
        ZeroMemory(&buffer, sizeof(buffer));
        nRecv = recv(sock, (char*)&buffer, BUFFERSIZE, 0);
        str_buff = buffer;
        npos = str_buff.find("\r\n\r\n");
    }

    // Close connection and handle //
    CloseHandle(hFile);
    closesocket(sock);
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
int CommSA::getTimeOut() const
{
    return timeOut;
}

void CommSA::setTimeOut(int value)
{
    timeOut = value;
}
