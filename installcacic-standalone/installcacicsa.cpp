#include "installcacicsa.h"

InstallCacicSA::InstallCacicSA(const std::string &url, const std::string &user, const std::string &pass)
{
    this->pass = pass;
    this->url  = url;
    this->user = user;
}

InstallCacicSA::~InstallCacicSA()
{

}

bool InstallCacicSA::registryExists(HKEY RootKey,LPCTSTR SubKey)
{
    LONG lResult;
    HKEY mTempKey;
    lResult = RegOpenKeyEx(RootKey,SubKey,0,KEY_ALL_ACCESS,&mTempKey);

    switch(lResult)
    {
        case ERROR_SUCCESS: //ok
            RegCloseKey(mTempKey);
            return true;
            break;
        default: //an error code from windows
            return false;
            break;
    }
}

bool InstallCacicSA::downloadService(const std::string &rota, const std::string &path)
{
    return false;
}

bool InstallCacicSA::installService()
{
    return false;
}

bool InstallCacicSA::ping()
{
    // Envia requisição para testar se o servidor está no ar
    std::string check;
    const char *route = "/";
    comm.setHost(this->url.c_str());
    comm.setRoute(route);

    char *buffer;
    check = comm.sendReq(buffer);
    if (check == "CONNECTION_ERROR") {
        return false;
    }
    return true;
}

bool InstallCacicSA::getConfig()
{
    std::string check;
    const char *route = ROUTE_HASH;
    comm.setHost(this->url.c_str());
    comm.setRoute(route);

    check = comm.sendReq("teste");
    if (check == "") {
        return false;
    } else {
    // Ajusta hash remoto recebido do servidor
        this->setHashRemoto(check);
        return true;
    }
}

bool InstallCacicSA::comparaHash()
{
    if(this->hashLocal != this->hashRemoto) {
        return false;
    } else {
        return true;
    }
}

bool InstallCacicSA::verificaServico()
{
    //Verifica se o serviço está rodando;

    //Verifica se o serviço está atualizado, senão baixa um novo;

    //Confere se a pasta bin existe, senão executa msi novamente.
    return false;
}

bool InstallCacicSA::installCacic()
{
    return false;
}

bool InstallCacicSA::deleteCacic26()
{
    //verifica se há algum processo do cacic rodando e os mata.
    //Deleta arquivos da versão 2.6 da pasta do cacic.
    return false;
}

bool InstallCacicSA::deleteCacic28()
{
    //verifica se há algum processo do cacic rodando e os mata.
    //Deleta arquivos da versão 2.8 da pasta do cacic.
    return false;
}

bool InstallCacicSA::setHashLocal(const std::string &hash)
{
    // Ajusta na classe o parâmetro hash para o módulo installcacic
    this->hashLocal = hash;

    return true;
}

bool InstallCacicSA::setHashRemoto(const std::string &hash)
{
    // Ajusta hash do servidor remoto
    this->hashRemoto = hash;

    return true;
}

bool InstallCacicSA::setPort(int port)
{
    this->comm.setPort(port);

    return true;
}

std::string InstallCacicSA::getHashFromFile(const std::string &filePath)
{
    unsigned char c[MD5_DIGEST_LENGTH];
    int i;
    FILE *inFile = fopen (filePath.c_str(), "rb");
    MD5_CTX mdContext;
    int bytes;
    unsigned char data[1024];
    std::string retorno;

    if (inFile == NULL) {
        return "";
    }

    MD5_Init (&mdContext);
    while ((bytes = fread (data, 1, 1024, inFile)) != 0)
        MD5_Update (&mdContext, data, bytes);
    MD5_Final (c,&mdContext);
    char buf[32];
    for(i = 0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf(buf, "%02x", c[i]);
        retorno.append( buf );
    }

    fclose (inFile);
    return retorno;
}
std::string InstallCacicSA::getHashLocal()
{
    return this->hashLocal;
}

std::string InstallCacicSA::getHashRemoto()
{
    return this->hashRemoto;
}

