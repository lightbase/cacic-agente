#include "installcacicsa.h"

InstallCacicSA::InstallCacicSA(const std::string &url, const std::string &user, const std::string &pass)
{
    this->pass = pass;
    this->url  = url;
    this->user = user;
    this->cacicPath = "c:\\cacic";
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
    // Força path no binário
    std::string full_path = path + std::string("/") + std::string(CACIC_SERVICE_BIN);

    std::cout << "Baixando arquivo da URL: " << rota << std::endl;
    std::cout << "Path para procurar o arquivo: " << full_path << std::endl;

    // Resultados
    bool downloaded = this->comm.downloadFile(rota.c_str(), path.c_str());
    std::cout << "Resultado do Download: " << downloaded << std::endl;

    bool exists = this->fileExists(full_path);
    std::cout << "Resultado do exists: " << exists << std::endl;

    return downloaded && exists;
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
    if (check == "" || check == "CONNECTION_ERROR") {
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
    std::string fileService = this->cacicPath + "\\" + std::string(CACIC_SERVICE_BIN);
    std::string fileServiceTemp =this->cacicPath + "\\temp\\" + std::string(CACIC_SERVICE_BIN);
    std::string dirBin = this->cacicPath + "\\bin\\";
    //Verifica se o serviço está rodando;
    if (!this->comparaHash()){
        if (!this->getConfig()){
            return false;
        }
        if (!this->downloadService(this->hashRemoto, fileService)){
            this->informaGerente("Falha ao baixar serviço.");
            return false;
        }
        if (sc.isInstalled()){
            if (sc.isRunning()){
                sc.stop();
                if(!this->fileExists(fileService)){
                    BOOL ok = DeleteFileA(fileService.c_str());
                    if(ok != ERROR_FILE_NOT_FOUND){
                        //Aguarda pra ter certeza de que o serviço não esteja rodando mais e tenta de novo
                        Sleep(3000);
                        ok = DeleteFileA(fileService.c_str());
                        if (ok != ERROR_FILE_NOT_FOUND){
                            this->informaGerente("Falha ao tentar excluir serviço antigo.");
                        }
                    }
                }
                if (MoveFileExA(fileServiceTemp.c_str(),
                                fileService.c_str(),
                                MOVEFILE_REPLACE_EXISTING) != 0) {

                    this->informaGerente("Falha ao mover serviço da pasta temporária.");
                    return false;
                }
            }
            if (!sc.start()){
                this->informaGerente("Falha ao iniciar serviço.");
            }
        } else {
            if (MoveFileExA(fileServiceTemp.c_str(),
                            fileService.c_str(),
                            MOVEFILE_REPLACE_EXISTING) != 0) {

                this->informaGerente("Falha ao mover serviço da pasta temporária.");
                return false;
            }
            if (!sc.install(L"C:\\Cacic\\cacic-service.exe")){
                this->informaGerente("Falha ao instalar serviço.");
                return false;
            } else {
                if (!sc.start()){
                    if (!PathIsDirectoryEmptyA(dirBin.c_str())) {
                        this->informaGerente("Não foi possível iniciar o serviço.");
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
                    if (!PathIsDirectoryEmptyA(dirBin.c_str())) {
                        this->informaGerente("Não foi possível iniciar o serviço.");
                    } else {
                        //instala o MSI do cacic de novo
                    }
                }
            }
        } else if (/*cacic-service existir*/ true) {
            if (!sc.install(L"C:\\Cacic\\cacic-service.exe")){
                this->informaGerente("Não foi possível instalar o serviço.");
            }
        }
    }

    return false;
}

bool InstallCacicSA::fileExists(const std::string &filePath)
{
    if (FILE *file = fopen(filePath.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

bool InstallCacicSA::informaGerente(const std::string &error)
{
    const char *route = ROUTE_ERRO;
    comm.setHost(this->url.c_str());
    comm.setRoute(route);

    std::string check = comm.sendReq(error.c_str());
    if (check == "" || check == "CONNECTION_ERROR") {
        return false;
    } else {
        return true;
    }
}

bool InstallCacicSA::installCacic()
{
    return false;
}

bool InstallCacicSA::deleteCacic26()
{

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
std::string InstallCacicSA::getUrl() const
{
    return url;
}

void InstallCacicSA::setUrl(const std::string &value)
{
    url = value;
}


