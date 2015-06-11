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

/**
 * @brief CommSA::sendReq
 *
 * Me´todo que envia a requisição com parâmetros padrão
 *
 * @param parameters Dado que vai no corpo da requisição
 * @return String com o parsing do JSON da resposta
 */
std::string CommSA::sendReq(const char* parameters)
{
    return this->sendReq(this->host, this->route, this->method, this->type, this->port, parameters);
}

std::string CommSA::sendReq(const char* host, const char* route, const char* method)
{
    return this->sendReq(host, route, method, this->type, this->port, "");
}

/**
 * @brief CommSA::sendReq
 *
 * Envia a requisição
 *
 * @param host
 * @param route
 * @param method
 * @param type
 * @param port
 * @param parameters
 * @return Resposta da requisição
 */
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
    if (shost == NULL){
        std::cout << "Erro no gethostbyname" << std::endl;
        return "CONNECTION_ERROR";
    }
    SOCKADDR_IN SockAddr;
    SockAddr.sin_port = htons(port);
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_addr.s_addr = *((unsigned long*)shost->h_addr);

    // Ajusta o timeout para a conexão
    setsockopt(Socket, SOL_SOCKET, SO_SNDTIMEO, (const char *)&this->timeOut, sizeof(this->timeOut));
    setsockopt(Socket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&this->timeOut, sizeof(this->timeOut));
    if(connect(Socket,(SOCKADDR*)(&SockAddr),sizeof(SockAddr)) != 0){
        std::cout << "Could not connect" << std::endl;
        // Throw exception if it was not possible to connect
        return "CONNECTION_ERROR";
    }
    //Ponteiro da struct está dando erro de permissão, ainda não descobri o motivo.
//    networkInfo *netInfo;
//    for(int i = 0; i< this->getNetworkInfo(netInfo); i++){
//        printf("----------NETWORK----------\n");
//        printf("Ip:         ");
//        std::string aux = netInfo[i].ip;
//        printf("\t%s\n", aux.c_str());
//        printf("SubnetMask: ");
//        aux = netInfo[i].subnetMask;
//        printf("\t%s\n", aux.c_str());
//    }

    std::ostringstream FormBuffer;
    // header
    if (method) {
        FormBuffer << method;
    } else {
        FormBuffer << "GET";
    }
    FormBuffer << " " << route << " HTTP/1.1\r\n";
    FormBuffer << "Content-Type: " << type << "\r\n";
    FormBuffer << "Host: " << host << "\r\n";
    FormBuffer << "User-Agent: " << "CACIC-Installer/" << CACIC_VERSION << "\r\n";

    // actual content
    if (parameters && parameters != "") {
        FormBuffer << "Content-Length: " << strlen(parameters) << "\n\n";
        FormBuffer << parameters;
    }

    std::string str = FormBuffer.str();

    std::cout << str << std::endl;

    send(Socket, str.data(), strlen(str.c_str()), NULL);


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

const wchar_t *CommSA::GetWC(const char *c)
{
    const size_t cSize = strlen(c)+1;
    wchar_t* wc = new wchar_t[cSize];
    mbstowcs (wc, c, cSize);

    return wc;
}

/**
 * @brief CommSA::setNetworkInfo
 *
 * Ajusta valores referentes às interfaces de rede
 *
 * @param ip
 * @param subnetMask
 * @return
 */
bool CommSA::setNetworkInfo(const char *ip, const char *subnetMask)
{
    //std::cout << "IP: " << ip << " Mask: " << subnetMask << std::endl;
    //memcpy ( this->ip, &ip, sizeof(ip) );
    //memcpy ( this->subnetMask, &subnetMask, sizeof(subnetMask) );
    this->ip = ip;
    this->subnetMask = subnetMask;

    return true;
}

bool CommSA::downloadFile(const char *url, const char *filePath)
{
    LPCSTR filename = filePath;
    char buffer[BUFFERSIZE];
    struct sockaddr_in serveraddr;
    int sock;

    WSADATA wsaData;
    int port = 80;

//    std::cout << "Baixando arquivo: " << url << " para o caminho: " << filePath << std::endl;

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

    std::ostringstream FormBuffer;
    // header
    FormBuffer << "GET " << file << " HTTP/1.1\r\n";
    FormBuffer << "Host: " << shost << "\r\n";
    FormBuffer << "User-Agent: " << "CACIC-Installer/" << CACIC_VERSION << "\r\n";

    std::string request = FormBuffer.str();

    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
        return false;
    }

    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        return false;
    }

    memset(&serveraddr, 0, sizeof(serveraddr));

    //Verifica se é possível conexão com a url repassada
    if (!gethostbyaddr(url, strlen(url),AF_INET)){
        //Com a url completa funciona
//        std::cout << "Fail gethostbyaddr: " << url << std::endl;
        return false;
    }

    // ip address of link //
    hostent *record = gethostbyname(shost.c_str());
    in_addr *address = (in_addr * )record->h_addr;
    std::string ipd = inet_ntoa(* address);
    const char *ipaddr = ipd.c_str();

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(ipaddr);
    serveraddr.sin_port = htons(port);

    if (connect(sock, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) {
//        std::cout << "Erro na conexão!!!" << std::endl;
        return false;
    }

    if (send(sock, request.data(), request.length(), 0) != request.length()) {
//        std::cout << "Erro na conexão!!!" << std::endl;
        return false;
    }

    int nRecv, npos;
    nRecv = recv(sock, (char*)&buffer, BUFFERSIZE, 0);
    // getting end of header //
    std::string str_buff = buffer;

    // Checa se existe a possibildiade do arquivo não estar lá
    npos = str_buff.find("404 Not Found");
    if (npos != std::string::npos) {
//        std::cout << "Arquivo não existe no servidor!!!" << std::endl;
        return false;
    }

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

bool CommSA::log(const char *message)
{
    return this->log(99, "", "", message);
}

bool CommSA::log(double codigo, const char *user, const char *so, const char *message)
{
    JSONObject root;

    // Add JSON values
    if (user != "") {
        const wchar_t *u = this->GetWC(user);
        root[L"user"] = new JSONValue(u);
    }
    if (so != "") {
        const wchar_t *s = this->GetWC(so);
        root[L"so"] = new JSONValue(s);
    }
    root[L"codigo"] = new JSONValue(codigo);

    // Convert
    const wchar_t *m = this->GetWC(message);
    root[L"message"] = new JSONValue(m);

    // Convert JSON to text
    JSONValue *value = new JSONValue(root);

    // Convert JSON Values
    this->setRoute("/ws/instala/erro");
    this->setMethod("POST");
    this->setType("application/json");
    std::wstring params = value->Stringify();
    std::string par (params.begin(), params.end());

//    std::cout << "Arquivo JSON Enviado: " << std::endl;
//    std::wcout << par.c_str() << std::endl;

    // Send JSON request to Server
    std::string response =  this->sendReq(this->host, this->route, this->method, this->type, this->port, par.c_str());

    if (response == "CONNECTION_ERROR") {
        return false;
    } else {
        return true;
    }
}

std::string CommSA::getConfig()
{
    JSONObject root;

    // Convert
    const wchar_t *m = this->GetWC(this->ip);
    root[L"ip_address"] = new JSONValue(m);

    const wchar_t *s = this->GetWC(this->subnetMask);
    root[L"netmask"] = new JSONValue(s);

    // Convert JSON to text
    JSONValue *value = new JSONValue(root);

    // Convert JSON Values
    this->setRoute(ROUTE_HASH);
    this->setMethod("POST");
    this->setType("application/json");
    std::wstring params = value->Stringify();
    std::string par (params.begin(), params.end());

    // std::cout << "Arquivo JSON Enviado: " << std::endl;
    // std::wcout << par.c_str() << std::endl;

    // Send JSON request to Server
    std::string response =  this->sendReq(this->host, this->route, this->method, this->type, this->port, par.c_str());

    return response;
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
