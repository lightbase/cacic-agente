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
    return this->hashLocal == this->hashRemoto;
}

bool InstallCacicSA::verificaServico()
{
    ServiceController sc(L"CacicDaemon");
    //Verifica se o serviço está rodando;
    if (!this->comparaHash()){
        //baixa novo serviço
        if (sc.isInstalled()){
            if (sc.isRunning()){
                sc.stop();
                //move da pasta temporária para a do cacic
            }
            if (!sc.start()){
                //envia falha pro gerente
            }
        } else {
            //move da pasta temporária para a do cacic
            if (!sc.install(L"C:\\Cacic\\cacic-service.exe")){
                //envia falha pro gerente
                return false;
            } else {
                if (!sc.start()){
                    if (/*verifica se a pasta /bin existe e tem arquivos*/ true) {
                    //envia falha pro gerente
                    } else {
                        //instala o MSI do cacic de novo
                    }
                }
            }
        }
    } else {
        if (sc.isInstalled()) {
            if (!sc.isRunning()){
                if (!sc.start()){
                    if (/*verifica se a pasta /bin existe e tem arquivos*/ true) {
                    //envia falha pro gerente
                    } else {
                        //instala o MSI do cacic de novo
                    }
                }
            }
        } else if (/*cacic-service existir*/ true) {
            if (!sc.install(L"C:\\Cacic\\cacic-service.exe")){
                //envia falha ao gerente
            }
        }
    }

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
    DWORD dwStatus = 0;
    BOOL bResult = FALSE;
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    HANDLE hFile = NULL;
    BYTE rgbFile[BUFSIZE];
    DWORD cbRead = 0;
    BYTE rgbHash[MD5LEN];
    DWORD cbHash = 0;
    CHAR rgbDigits[] = "0123456789abcdef";
    std::string retorno;

    //
    std::wstring wc( filePath.size(), L'#' );
    mbstowcs( &wc[0], filePath.c_str(), filePath.size() );

    LPCWSTR filename = wc.c_str();
    // Logic to check usage goes here.

    hFile = CreateFile(filename,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_SEQUENTIAL_SCAN,
        NULL);

    if (INVALID_HANDLE_VALUE == hFile)
    {
        dwStatus = GetLastError();
//        printf("Error opening file %s\nError: %d\n", filename,
//            dwStatus);
        return "ERROR";
    }

    // Get handle to the crypto provider
    if (!CryptAcquireContext(&hProv,
        NULL,
        NULL,
        PROV_RSA_FULL,
        CRYPT_VERIFYCONTEXT))
    {
        dwStatus = GetLastError();
//        printf("CryptAcquireContext failed: %d\n", dwStatus);
        CloseHandle(hFile);
        return "ERROR";
    }

    if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
    {
        dwStatus = GetLastError();
//        printf("CryptAcquireContext failed: %d\n", dwStatus);
        CloseHandle(hFile);
        CryptReleaseContext(hProv, 0);
        return "ERROR";
    }

    while (bResult = ReadFile(hFile, rgbFile, BUFSIZE,
        &cbRead, NULL))
    {
        if (0 == cbRead)
        {
            break;
        }

        if (!CryptHashData(hHash, rgbFile, cbRead, 0))
        {
            dwStatus = GetLastError();
//            printf("CryptHashData failed: %d\n", dwStatus);
            CryptReleaseContext(hProv, 0);
            CryptDestroyHash(hHash);
            CloseHandle(hFile);
            return "ERROR";
        }
    }

    if (!bResult)
    {
        dwStatus = GetLastError();
//        printf("ReadFile failed: %d\n", dwStatus);
        CryptReleaseContext(hProv, 0);
        CryptDestroyHash(hHash);
        CloseHandle(hFile);
        return "ERROR";
    }

    cbHash = MD5LEN;
    char buf[32];
    if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0))
    {
//        printf("MD5 hash of file %s is: ", filename);
        for (DWORD i = 0; i < cbHash; i++)
        {
//            printf("%c%c", rgbDigits[rgbHash[i] >> 4],
//                rgbDigits[rgbHash[i] & 0xf]);
            sprintf(buf, "%c%c", rgbDigits[rgbHash[i] >> 4],
                    rgbDigits[rgbHash[i] & 0xf]);
            retorno.append( buf );
        }
//        printf("\n");
    }
    else
    {
        dwStatus = GetLastError();
//        printf("CryptGetHashParam failed: %d\n", dwStatus);
    }

    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
    CloseHandle(hFile);

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

