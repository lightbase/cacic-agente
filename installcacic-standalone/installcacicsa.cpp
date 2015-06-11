#include "installcacicsa.h"

InstallCacicSA::InstallCacicSA(const std::string &url, const std::string &user, const std::string &pass)
{
    this->pass = pass;
    this->url  = url;
    this->user = user;
    this->cacicPath = "c:\\cacic";
    this->installDir = this->cacicPath + "\\icsa";
    this->logFile = this->installDir + "\\install.log";
    this->debug = false;

    // Adjust Comm parameters
    this->comm.setHost(this->url.c_str());
}

InstallCacicSA::~InstallCacicSA()
{
    std::string testExceptionFiles[0];

    // Remove diretório
    this->delFolder(this->installDir, testExceptionFiles, 0);
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

bool InstallCacicSA::downloadService(const std::string &path)
{
    // Verifica se o hash remoto está ajustado
    if (this->hashRemoto == "" || this->hashRemoto == "\0") {
        this->getConfig();
    }

    // Força path no binário
    std::string full_path = path;

    // Busca URL para baixar o serviço
    std::string check;
    std::string str_route = std::string(ROUTE_DOWNLOAD) + std::string("/") + this->hashRemoto;
    const char *route = str_route.c_str();
    comm.setHost(this->url.c_str());
    comm.setRoute(route);

    check = comm.sendReq(this->url.c_str(), route, "GET");

    if (check == "" || check == "CONNECTION_ERROR") {
        std::string message = "Binário do serviço não encontrado! Hash: ";
        message += this->hashRemoto;
        this->log(message.c_str());
        return false;
    }

    std::string message = "Baixando serviço na URL: " + check;
    this->log(message.c_str(), "INFO");

    if (this->comm.downloadFile(check.c_str(), full_path.c_str())) {
        // Verifica se o arquivo foi baixado com sucesso
        if (this->fileExists(full_path)) {
            this->setHashLocal(this->getHashFromFile(full_path));
            if (!this->comparaHash()) {
                // O arquivo baixado tem o hash diferente do esperado. Falhar!
                std::string message = "Erro ao baixar o arquivo do serviço! Hash esperado: ";
                message += this->hashRemoto;
                message += " Hash do arquivo baixado: ";
                message += this->hashLocal;
                this->log(message.c_str());

                return false;
            } else {
                return true;
            }
        } else {
            this->log("Erro ao baixar arquivo do serviço! Download falhou!");

            return false;
        }
    } else {
        this->log("Erro ao baixar arquivo do serviço! Não foi possível comunicar com o Gerente!");

        return false;
    }

}

bool InstallCacicSA::downloadMsi(const std::string &path)
{
    // Força path no binário
    std::string full_path = path + std::string("\\") + std::string(CACIC_MSI);

    // Busca URL para baixar o MSI
    std::string check;
    const char *route = ROUTE_DOWNLOAD_MSI;
    comm.setHost(this->url.c_str());
    comm.setRoute(route);

    check = comm.sendReq("");
    if (check == "" || check == "CONNECTION_ERROR") {
        return false;
    }

    return this->comm.downloadFile(check.c_str(),full_path.c_str()) && this->fileExists(full_path);
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

    const char *buffer = "";
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

    // Pega interfaces de rede válidas
    struct networkInfo net[MAX_NETINFO];
    this->getNetworkInfo(net);
    int n = this->getValidNetwork(net);

    if (n <= 0) {
        this->log(11, "", "", "Erro no getConfig: Nenhuma interface de rede válida!", "ERROR");
        this->setHashRemoto(std::string(""));

        return false;
    }
    // Ajusta IP para enviar ao Gerente
    comm.setNetworkInfo(net[n].ip, net[n].subnetMask);

    check = comm.getConfig();
    if (check == "" || check == "CONNECTION_ERROR") {
        std::string message = "Erro de conexão no getConfig!!! URL: " + this->url;
        this->log(message.c_str());
        this->setHashRemoto(std::string(""));

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
/**
 * @brief InstallCacicSA::verificaServico
 *
 * Verificação do serviço observando a seguinte heurística
 *
 *   // 2 - Instalou o MSI mas deu pau
 *   // 2.1 - Serviço não está rodando
 *   // 2.2 - Verifica binário desatualizado
 *   // 2.3 - Pasta de DLL's vazia/corrompida
 *   // 3 - Tudo instalado e serviço fora
 *   // 3.1 - Instala de novo (Baixa o serviço e executa)
 *   // 3.2 - Se der erro no serviço, baixa o MSI
 *   // 4 - Atualização forçada (serviço desatualizado)
 *   // 4.1 - Instala de novo (Baixa o serviço e executa)
 *
 * @return
 */
bool InstallCacicSA::verificaServico()
{

    ServiceController sc(L"CacicDaemon");
    std::string fileService = this->cacicPath + "\\" + std::string(CACIC_SERVICE_BIN);
    std::string fileServiceTemp =this->installDir + "\\" + std::string(CACIC_SERVICE_BIN);
    std::string dirBin = this->cacicPath + "\\bin\\";

    // 2.2 - Verifica binário desatualizado
    if (!this->getConfig()){
        return false;
    }
    if (!this->comparaHash()){
        // 4.1 - Instala de novo (Baixa o serviço e executa)
        if (!this->downloadService(fileServiceTemp)){
            this->informaGerente("Falha ao baixar serviço.");
            return false;
        }
        // 2.1 - Serviço não está rodando
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
                    std::string message("Falha ao mover serviço da pasta:");
                    message += fileServiceTemp;
                    message += " para a pasta: ";
                    message += fileService;
                    this->informaGerente(message);
                    return false;
                }
            }
            if (!sc.start()){
                std::string message("Falha ao iniciar serviço! Mensagem:\n");
                message += sc.getLastError();
                this->informaGerente(message);
            }
        } else {
            if (MoveFileExA(fileServiceTemp.c_str(),
                            fileService.c_str(),
                            MOVEFILE_REPLACE_EXISTING) != 0) {

                this->informaGerente("Falha ao mover serviço da pasta temporária.");
                return false;
            }
            // 3.1 - Instala de novo (Baixa o serviço e executa)
            if (!sc.install(L"C:\\Cacic\\cacic-service.exe")){
                this->informaGerente("Falha ao instalar serviço.");
                return false;
            } else {
                if (!sc.start()){
                    if (!PathIsDirectoryEmptyA(dirBin.c_str())) {
                        this->informaGerente("Não foi possível iniciar o serviço.");
                    } else {
                        // 3.2 - Se der erro no serviço, baixa o MSI
                        this->downloadMsi(this->installDir);
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
                        // 3.2 - Se der erro no serviço, baixa o MSI
                        this->downloadMsi(this->installDir);
                    }
                }
            }
        } else if (this->fileExists(fileService)) {
            if (!sc.install(L"C:\\Cacic\\cacic-service.exe")){
                this->informaGerente("Não foi possível instalar o serviço.");
            }
        }
    }

    return true;
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

/**
 * @brief InstallCacicSA::informaGerente
 *
 * Registra o erro e envia para o Gerente
 *
 * @param error Mensagem de erro
 * @return Verdadeiro ou falso
 */
bool InstallCacicSA::informaGerente(const std::string &error)
{
    return this->log(error.c_str());
}

bool InstallCacicSA::runProgram(const std::string &applicationPath, const std::string &parameters)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    std::string commandLine = applicationPath + " " + parameters;
    std::wstring wAux(commandLine.begin(), commandLine.end());
    std::string message;

    wchar_t *wcAux = new wchar_t[wAux.length() + 1];
    //wcscpy(wcAux, wAux.c_str());
    StringCchCopyW(wcAux, BUFSIZE, wAux.c_str());

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    message = "Executando comando: " + commandLine;
    this->log(message.c_str(), "INFO");

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

bool InstallCacicSA::deleteCacicAntigo()
{
    bool ok = true;
    ServiceController sc(L"cacic");
    this->log("Erro ao desinstalar cacic 2.6", "DEBUG");
    std::cout << "Desinstalando cacic 2.6.\n";
    if (sc.isInstalled()){
        if (sc.isRunning())
            sc.stop();
        if (!sc.uninstall()){
            this->log("Erro ao desinstalar cacic 2.6");
            ok = false;
        }
    }
    std::cout << "Desinstalando cacic 2.8.\n";
    this->log("Desinstalar cacic 2.8", "DEBUG");
    ServiceController sc28(L"CacicSustainService");
    if (sc28.isInstalled()){
        if (sc28.isRunning())
            sc28.stop();
        if (!sc28.uninstall()){
            this->log("Erro ao desinstalar cacic 2.8");
            ok = false;
        }
    }

    remove("C:\\Windows\\chksis.exe");
    remove("C:\\Windows\\cacicsvc.exe");
    remove("C:\\Windows\\cacicservice.exe");
    remove("C:\\Windows\\chksis.ini");

    //Não gostei de ter feito dessa maneira, mas ainda não achei o ideal.
    int numExcept = 13;
    std::string exceptionFiles[numExcept-1];
    exceptionFiles[0] = "deploy";
    exceptionFiles[1] = "coletaDiff.json";
    exceptionFiles[2] = "coleta.json";
    exceptionFiles[3] = "getConfig.json";
    exceptionFiles[4] = "getTest.json";
    exceptionFiles[5] = "cacic-service.exe";
    exceptionFiles[6] = "chksys.exe";
    exceptionFiles[7] = "gercols.exe";
    exceptionFiles[8] = "install-cacic.exe";
    exceptionFiles[9] = "cacicdeploy.exe";
    exceptionFiles[10] = "bin";
    exceptionFiles[11] = "icsa";

    // Não remove também o diretório de instalação
    exceptionFiles[12] = this->installDir;

    this->log("Removendo arquivos de instalações anteriores.", "DEBUG");
    return delFolder("C:\\Cacic\\", exceptionFiles, numExcept) && ok;
}

/**
 * @brief InstallCacicSA::delFolder
 *
 * Deleta o diretório recursivamente, salvando os arquivos de exceções repassados.
 *
 * @param path Diretório que será excluído
 * @param fileException Array com nome de arquivos ou diretórios que não serão excluídos
 * @param numException Quantidade de arquivos nas exceções
 * @param exceptionFound Bool que informa se há exceção no diretório para checagem da própria função.
 * @return Verdadeiro caso termine sem nenhum problema.
 */
bool InstallCacicSA::delFolder(const std::string &path, const std::string *fileException, const int &numException, bool *exceptionFound){
    if (exceptionFound == NULL){
        exceptionFound = (bool *) malloc(sizeof(bool));
    }
    *exceptionFound = false;
    bool ok = true;
    tinydir_dir dir;
    tinydir_open(&dir, path.c_str());

    while (dir.has_next){
        tinydir_file file;
        tinydir_readfile(&dir, &file);
        std::string nameFile(file.name);
        std::cout << nameFile << std::endl;
        if (!(nameFile == ".." || nameFile == ".")){
            //Se for diretório, entra na função novamente para deletar os arquivos
            if (file.is_dir){
                //Verifica se o diretório está nas exceções
                if (fileException != NULL){
                    for(int i = 0; i<numException; i++){
                        *exceptionFound = nameFile == fileException[i];
                        if (*exceptionFound) break;
                    }
                }
                if (*exceptionFound){
                    tinydir_next(&dir);
                    continue;
                } else {
                    ok = ok && this->delFolder(file.path, fileException, numException, exceptionFound);
                }
            } else {
                //verifica se o arquivo está nas exceções
                if (fileException != NULL){
                    for(int i = 0; i<numException; i++){
                        *exceptionFound = nameFile == fileException[i];
                        if (*exceptionFound) break;
                    }
                }
                if (!*exceptionFound){
                    remove(file.path);
                }
            }
        }
        tinydir_next(&dir);
    }
    tinydir_close(&dir);
    if (!*exceptionFound){
        //Se não houver arquivo nas exceções, deleta o path.
        ok = ok && RemoveDirectoryA(path.c_str());

        //Verifica se realmente não foi excluído.
        if (!ok){
            DWORD dwAttrib = GetFileAttributesA(path.c_str());

            ok = (dwAttrib != INVALID_FILE_ATTRIBUTES &&
                    (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
        }
    }
    return ok;
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
            //sprintf(buf, "%c%c", rgbDigits[rgbHash[i] >> 4],
            //        rgbDigits[rgbHash[i] & 0xf]);
            StringCchPrintfA(buf, BUFSIZE, "%c%c", rgbDigits[rgbHash[i] >> 4],
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

std::string InstallCacicSA::getInstallDir()
{
    return this->installDir;
}

void InstallCacicSA::setUrl(const std::string &value)
{
    url = value;
}

/**
 * @brief InstallCacicSA::log
 *
 * Registro de erros de instalação no log
 *
 * @param message Mensagem a ser enviada
 * @return
 */
bool InstallCacicSA::log(const char *message)
{
    double codigo = 99;
    const char *user = "";
    const char *so = "";

    // Executa método com todos os valores vazios
    return this->log(codigo, user, so, message, "ERROR");
}

/**
 * @brief InstallCacicSA::log
 *
 * Registra uma mensagem no Log
 *
 * @param message Mensagem a ser registrada
 * @param level Nível de erro a inserir no log
 * @return Verdadeiro ou falso
 */
bool InstallCacicSA::log(const char *message, const char *level)
{
    double codigo = 99;
    const char *user = "";
    const char *so = "";

    // Registra o log
    return this->log(codigo, user, so, message, level);
}

/**
 * @brief InstallCacicSA::log
 *
 * Registro de erros de instalação no log
 *
 * @param codigo Código de erro (padrão 99)
 * @param user Usuário da máquina
 * @param so SO onde aconteceu o erro
 * @param message Mensagem a ser registrada
 * @return
 */
bool InstallCacicSA::log(double codigo, const char *user, const char *so, const char *message)
{
    // Verifica se arquivo de erros existe
    this->createLogFile();

    // Chama o método de envio com o nível de Erro
    return this->log(codigo, user, so, message, "ERROR");
}

/**
 * @brief InstallCacicSA::log
 *
 * Registro de erros de instalação no log. Só envia para o Gerente se o nível for ERROR
 *
 * @param codigo Código de erro (padrão 99)
 * @param user Usuário da máquina
 * @param so SO onde aconteceu o erro
 * @param message Mensagem a ser registrada
 * @param level Nível de erro
 * @return
 */
bool InstallCacicSA::log(double codigo, const char *user, const char *so, const char *message, const char *level)
{
    // Verifica se arquivo de erros existe
    this->createLogFile();

    // Agora cria arquivo de log
    std::ofstream outfile (this->logFile.c_str(), std::ios::in | std::ios::ate);
    outfile << "[" << this->getStrTime() << "] " << level << ": InstallCacicSA (" << codigo << ") - " << message << std::endl;
    outfile.close();

    // Em modo debug escreve para a saída padrão
    if (this->debug) {
        std::cout << "[" << this->getStrTime() << "] " << level << ": InstallCacicSA (" << codigo << ") - " << message << std::endl;
    }

    if (!user || user == "\0" || user == "") {
        // Pega usuário do SO
        std::string u = this->getUsuarioSo();
        user = u.c_str();
    }

    if (!so || so == "\0" || so == "") {
        // Pega SO
        so = this->getSo().c_str();
    }

    if (level == "ERROR") {
        return this->comm.log(codigo, user, so, message);
    } else {
        return true;
    }

}

/**
 * @brief InstallCacicSA::createInstallDir
 *
 * Verifica se o diretório já existe e cria se não existir
 *
 * @return Caminho completo para o diretório
 */
std::string InstallCacicSA::createInstallDir()
{
    tinydir_dir dir;
    int result;
    BOOL created;

    // Check if dir Exists
    result = tinydir_open(&dir, this->cacicPath.c_str());
    if (result == -1) {
        created = CreateDirectory(this->comm.GetWC(this->cacicPath.c_str()), NULL);
        if (!created) {
            return "";
        }
    }
    tinydir_close(&dir);

    result = tinydir_open(&dir, this->installDir.c_str());
    if (result == -1) {
        created = CreateDirectory(this->comm.GetWC(this->installDir.c_str()), NULL);
        if (!created) {
            return "";
        }
    }
    tinydir_close(&dir);

    return this->installDir;
}

bool InstallCacicSA::removeInstallDir()
{
    const std::string fileException[0];
    return this->delFolder(this->cacicPath, fileException, 0);
}

/**
 * @brief InstallCacicSA::getStrTime
 *
 * Retorna tempo atual como string formatado
 *
 * @return Tempo no formato %d-%m-%Y %H:%M:%S
 */
std::string InstallCacicSA::getStrTime()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,80,"%d-%m-%Y %H:%M:%S",timeinfo);
    std::string str(buffer);

    return str;
}

/**
 * @brief InstallCacicSA::createLogFile
 *
 * Cria log de instalação. Caso já exista, pega o caminho completo para o Log
 *
 * @return String com o caminho completo para o log
 */
std::string InstallCacicSA::createLogFile()
{
    if (this->fileExists(this->logFile)) {
        return this->logFile;
    } else {
        // Primeiro cria diretório se não existir
        this->createInstallDir();

        // Agora cria arquivo de log
        std::ofstream outfile (this->logFile.c_str());
        outfile << "[" << this->getStrTime() << "] InstallCacicSA: Início do log" << std::endl;
        outfile.close();

        // Em modo debug escreve para a saída padrão
        if (this->debug) {
            std::cout << "[" << this->getStrTime() << "] InstallCacicSA: Início do log" << std::endl;
        }

        return this->logFile;
    }
}

bool InstallCacicSA::removeLogFile()
{
    if (!this->fileExists(this->logFile)) {
        return true;
    } else {
        remove(this->logFile.c_str());
    }
}

/**
 * @brief InstallCacicSA::isAdmin
 *
 * Verifica se o Cacic está rodando com permissão de administrador
 *
 * Fonte: https://msdn.microsoft.com/en-us/library/windows/desktop/aa376389%28v=vs.85%29.aspx
 *
 * @return
 */
BOOL InstallCacicSA::isAdmin()
{
    HANDLE hAccessToken;
    UCHAR InfoBuffer[4096];
    PTOKEN_GROUPS ptgGroups = (PTOKEN_GROUPS)InfoBuffer;
    DWORD dwInfoBufferSize;
    PSID psidAdministrators;
    SID_IDENTIFIER_AUTHORITY siaNtAuthority = SECURITY_NT_AUTHORITY;
    UINT x;
    BOOL bSuccess;

    if(!OpenProcessToken(GetCurrentProcess(),TOKEN_READ,&hAccessToken))
        //        return(FALSE);
        return(TRUE);

    bSuccess =GetTokenInformation(hAccessToken,TokenGroups,InfoBuffer,
        sizeof(InfoBuffer), &dwInfoBufferSize);

    CloseHandle(hAccessToken);

    if( !bSuccess )
        //        return FALSE;
        return(TRUE);

    if(!AllocateAndInitializeSid(&siaNtAuthority, 2,
        SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS,
        0, 0, 0, 0, 0, 0,
        &psidAdministrators))
        //    return FALSE;
        return(TRUE);

    // assume that we don't find the admin SID.
    bSuccess = FALSE;

    for(x=0;x<ptgGroups->GroupCount;x++)
    {
        if(EqualSid(psidAdministrators, ptgGroups->Groups[x].Sid) )
        {
            bSuccess = TRUE;
            break;
        }
    }
    FreeSid(psidAdministrators);
    return bSuccess;
}

/**
 * @brief InstallCacicSA::cacicInstalado
 *
 * Verifica se existe entrada de registro para o Cacic
 *
 * @return Verdadeiro ou falso
 */
bool InstallCacicSA::cacicInstalado()
{
    return this->registryExists(HKEY_LOCAL_MACHINE, CACIC_REGISTRY);
}

std::string InstallCacicSA::getSo()
{
    return this->comp.getSo();
}

std::string InstallCacicSA::getUsuarioSo()
{
    return this->comp.getUsuarioSo();
}

/**
 * @brief InstallCacicSA::exec
 *
 * Procedimento completo de instalação do Cacic
 *
 * @return Verdadeiro ou falso se conseguir instalar
 */
bool InstallCacicSA::exec()
{
    // 0 - Verifica comunicação e cria estrutura básica
    this->log("Iniciando instalação...", "INFO");
    this->log("Cria estrutura de diretórios", "INFO");

    if (!this->isAdmin()) {
        this->log(1, "", "", "Erro de permissão. Usuário não é administrador", "ERROR");

        return false;
    }

    if (this->createInstallDir() == "") {
        std::string saida = "Erro ao tentar criar o diretório do Cacic no caminho = " + this->installDir;
        this->log(saida.c_str());

        return false;
    }

    this->log("Testa comunicação...", "DEBUG");

    if (this->ping()) {
        this->log("Comunicação realizado com sucesso! Seguindo...", "INFO");
    } else {
        std::string saida = "Não foi possível realizar comunicação com o Gerente na URL = " + this->url;
        this->log(saida.c_str());

        return false;
    }

    // 1 - Verifica se está instalado
    if (!this->cacicInstalado()) {
        std::string msi_path = this->installDir+"\\" + CACIC_MSI;
        // 1.1 - Baixa e executa o MSI
        this->downloadMsi(this->installDir);
        if (!this->fileExists(msi_path)) {
            std::string saida = "Arquivo MSI não encontrado no caminho = " + msi_path;
            this->log(saida.c_str());

            return false;
        } else {
            if (!this->installCacic(msi_path)) {
                std::string saida = "Falha na instalação do MSI = " + msi_path;
                this->log(saida.c_str());

                return false;
            }
            this->log("MSI instalado com sucesso! Finalizando...", "INFO");

            return true;
        }
    }

    if (!this->verificaServico()) {
        this->log("Não foi possível verificar o serviço!");

        return false;
    } else {

        this->log("Serviço verificado e atualizado com sucesso!", "INFO");
    }

    // TODO: 5 - Remove Cacic 2.6 e 2.8 ainda instalado
    if (!this->deleteCacicAntigo()) {
        this->log("Erro no procedimento de remoção das versões antigas do Cacic");

        return false;
    } else {
        this->log("Fim da remoção de versões antigas do Cacic...", "INFO");
    }

    return true;

}


/**
 * @brief InstallCacicSA::execRemove
 *
 * Procedimento completo de remoção do Cacic
 *
 * @return Verdadeiro ou falso se conseguir remover
 */
bool InstallCacicSA::execRemove()
{
    this->log("Iniciando remoção do Cacic...", "INFO");
    std::string msi_path = this->installDir+"\\" + CACIC_MSI;
    return this->removeCacic(msi_path);
}

int InstallCacicSA::getNetworkInfo(networkInfo *ni)
{
    return this->comp.getNetworkInfo(ni);
}

/**
 * @brief InstallCacicSA::getValidNetwork
 *
 * Retorna somente a estrutrura de rede que for válida
 *
 * @param ni Array com todas as interfaces de rede identificadas
 * @return
 */
int InstallCacicSA::getValidNetwork(struct networkInfo *net)
{
    int n;
    n = this->getNetworkInfo(net);
    this->log("---------------NET INFO-------------\n", "DEBUG");
    for (int i = 0; i<n ; i++){
        std::string message;
        message = std::string("\tIP Address:     \t");
        message += net[i].ip;
        message += "\n";
        this->log(message.c_str(), "DEBUG");
        message = std::string("\tSubnet Mask:     \t");
        message += net[i].subnetMask;
        message += "\n";
        this->log(message.c_str(), "DEBUG");

        // TODO: Melhoras essa regra de validação de IP
        if (std::string(net[i].ip) != std::string("127.0.0.1") &&
                std::string(net[i].ip) != std::string("0.0.0.0")) {
            // Considera esse IP
            message = std::string("Ip Válido Encontrado:    \t");
            message += net[i].ip;
            message += "\n";
            this->log(message.c_str(), "INFO");
            return i;
        }
    }

    // Se chegamos aqui nenhuma interface de rede foi identificada
    this->log("Não foi possível encontrar uma interface de rede válida", "ERROR");
    return -1;
}
