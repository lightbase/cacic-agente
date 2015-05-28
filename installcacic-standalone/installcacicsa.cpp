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
    std::string full_path = path + std::string("\\") + std::string(CACIC_SERVICE_BIN);

    return this->comm.downloadFile(rota.c_str(),full_path.c_str()) && this->fileExists(full_path);
}

bool InstallCacicSA::downloadMsi(const std::string &rota, const std::string &path)
{
    // Força path no binário
    std::string full_path = path + std::string("\\") + std::string(CACIC_MSI);

    return this->comm.downloadFile(rota.c_str(),full_path.c_str()) && this->fileExists(full_path);
}

bool InstallCacicSA::installService(const std::string &serviceName, const std::string &serviceBinPath)
{
    std::wstring wServiceName(serviceName.begin(), serviceName.end());
    std::wstring wServiceBinPath(serviceBinPath.begin(), serviceBinPath.end());
    ServiceController service(wServiceName);
    if (!service.install(wServiceBinPath)){
        this->informaGerente("Falha ao instalar serviço");
        return false;
    } else {
        if (!service.start()){
            this->informaGerente("Falha ao iniciar serviço.");
            return false;
        } else {
            return true;
        }
    }
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

    if (!this->getConfig()){
        return false;
    }
    if (!this->comparaHash()){
        if (!this->downloadService(this->hashRemoto, fileService)){
            this->informaGerente("Falha ao baixar serviço.");
            return false;
        }
        //Verifica se o serviço está rodando;
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
        } else if (this->fileExists(fileService)) {
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

bool InstallCacicSA::runProgram(const std::string &applicationPath, const std::string &parameters)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    std::string commandLine = applicationPath + " " + parameters;
    std::wstring wAux(commandLine.begin(), commandLine.end());

    wchar_t *wcAux = new wchar_t[wAux.length() + 1];
    wcscpy(wcAux, wAux.c_str());

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    std::cout << "Executando comando: " << commandLine << std::endl;

    // Start the child process.
    if( !CreateProcessW(NULL,   // No module name (use command line)
                        wcAux,        // Command line
                        NULL,           // Process handle not inheritable
                        NULL,           // Thread handle not inheritable
                        FALSE,          // Set handle inheritance to FALSE
                        0,              // No creation flags
                        NULL,           // Use parent's environment block
                        NULL,           // Use parent's starting directory
                        &si,            // Pointer to STARTUPINFO structure
                        &pi )           // Pointer to PROCESS_INFORMATION structure
       )
    {
        printf("Falha ao criar processo... %i \n", GetLastError());
        this->informaGerente("Falha ao criar processo utilizando: " + applicationPath + " " + parameters);
        return false;
    }

    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles.
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
}

bool InstallCacicSA::installCacic(const std::string &msiPath)
{
    std::cout << "Executando módulo no caminho: " << msiPath << std::endl;
    return this->runProgram("msiexec /i \"" + msiPath + "\"", " /quiet HOST=" + this->url +" USER=cacic PASS=cacic123");
}

bool InstallCacicSA::removeCacic(const std::string &msiPath)
{
    return this->runProgram("msiexec /x \"" + msiPath + "\"", " /quiet");
}

bool InstallCacicSA::deleteCacic26()
{
    ServiceController sc(L"cacic");
    if (sc.isInstalled()){
        if (sc.isRunning())
            sc.stop();
        if (!sc.uninstall()){
            //enviar erro(?)
        }
    }

    tinydir_dir dir;
    tinydir_open(&dir, "C:\\cacic\\");
    while (dir.has_next){
        tinydir_file file;
        tinydir_readfile(&dir, &file);
        std::string nameFile(file.name);
        if (!(nameFile == ".." || nameFile == ".")){
            printf("%s", nameFile.c_str());
            if (file.is_dir)
            {
                printf("/");
            }
            printf("\n");
        }

        tinydir_next(&dir);
    }
    return false;
}

bool InstallCacicSA::delFolder(const std::string &path, const std::string *fileException, const int &numException, bool *exceptionFound){
    if (exceptionFound == NULL){
        exceptionFound = (bool *) malloc(sizeof(bool));
    }
    *exceptionFound = false;
    bool ok = true;
    tinydir_dir dir;
    tinydir_open(&dir, path.c_str());
    std::cout << path << std::endl;
    while (dir.has_next){
        tinydir_file file;
        tinydir_readfile(&dir, &file);
        std::string nameFile(file.name);
        if (!(nameFile == ".." || nameFile == ".")){
            //Se for diretório, entra na função novamente para deletar os arquivos
            if (file.is_dir){
                printf("|--%s\\ ", nameFile.c_str());
                if (this->delFolder(file.path, fileException, numException, exceptionFound) && !*exceptionFound){
                    //Apenas se não houver um arquivo nas exceções, o diretório é excluído
                    if (!*exceptionFound){
                        ok = ok && RemoveDirectoryA(file.path);
                        if (ok){
                            printf("(diretorio excluido)");
                        }
                    }
                } else {
                    printf("(falha ao entrar an recursividade)");
                }
                printf("\n   ");
            } else {
                printf("    |---%s", nameFile.c_str());
                //verifica se o arquivo está nas exceções
                for(int i = 0; i<numException; i++){
                    if (nameFile == fileException[i]){
                        *exceptionFound = true;
                        printf (" (excecao)");
                        break;
                    }
                }
                if (!*exceptionFound){
                    ok = (remove(file.path) == 0) && ok;
                    if (ok)
                        printf(" (excluido)");
                }
                printf("\n");
            }
        }
        tinydir_next(&dir);
    }
    return ok;
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


