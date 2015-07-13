#include "wcomputer.h"

Computer::Computer()
{

}

Computer::~Computer()
{

}

/**
 * @brief Computer::getSo
 *
 * Ajusta e retorna o SO encontrado
 *
 * @return String do SO
 */
std::string Computer::getSo()
{
    if (this->so.empty()) {
        this->so = this->setSo();
    }

    return this->so;
}

/**
 * @brief Computer::getUsuarioSo
 *
 * Ajusta e retorna usuário encontrado
 *
 * @return String do usuário
 */
std::string Computer::getUsuarioSo()
{
    if (this->usuarioSo.empty()) {
        this->usuarioSo = this->setUsuarioSo();
    }

    return this->usuarioSo;
}

int Computer::getNetworkInfo(networkInfo *arrayNetInfo)
{
    int ret = this->setNetworkAdapters(arrayNetInfo);
    for (int i = 0; i<ret; i++){
        memcpy ( &this->arrayNetInfo[i], &arrayNetInfo[i], sizeof(arrayNetInfo[i]) );
    }
    return ret;
}

/**
 * @brief Computer::setNetworkAdapters
 *
 * Salva o IP e SubnetMask de todos os adaptadores de rede ativos
 * dentro de um array de struct networkInfo repassado por parâmetro.
 *
 * OBS: O Array deve ser inicializado antes desse método ser chamado.
 *
 * @return Caso o método seja executado com sucesso, a quantidade de
 * adaptadores ativos encontrados é retornada. Caso dê erro, -1 é retornado.
 */
int Computer::setNetworkAdapters(networkInfo arrayNetInfo[])
{
    int i;

    /* Variables used by GetIpAddrTable */
    PMIB_IPADDRTABLE pIPAddrTable;
    DWORD dwSize = 0;
    DWORD dwRetVal = 0;
    IN_ADDR IPAddr;

    /* Variables used to return error message */
    LPVOID lpMsgBuf;

    // Before calling AddIPAddress we use GetIpAddrTable to get
    // an adapter to which we can add the IP.
    pIPAddrTable = (MIB_IPADDRTABLE *) MALLOC(sizeof (MIB_IPADDRTABLE));

    if (pIPAddrTable) {
        // Make an initial call to GetIpAddrTable to get the
        // necessary size into the dwSize variable
        if (GetIpAddrTable(pIPAddrTable, &dwSize, 0) ==
            ERROR_INSUFFICIENT_BUFFER) {
            FREE(pIPAddrTable);
            pIPAddrTable = (MIB_IPADDRTABLE *) MALLOC(dwSize);

        }
        if (pIPAddrTable == NULL) {
            printf("Memory allocation failed for GetIpAddrTable\n");
            return -1;
        }
    }
    // Make a second call to GetIpAddrTable to get the
    // actual data we want
    if ( (dwRetVal = GetIpAddrTable( pIPAddrTable, &dwSize, 0 )) != NO_ERROR ) {
        printf("GetIpAddrTable failed with error %d\n", dwRetVal);
        if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwRetVal, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),       // Default language
                          (LPTSTR) & lpMsgBuf, 0, NULL)) {
            printf("\tError: %s", lpMsgBuf);
            LocalFree(lpMsgBuf);
        }
        return -1;
    }
    int retorno;
    int numEntries = retorno = (int) pIPAddrTable->dwNumEntries;
    if (numEntries > MAX_NETINFO){
        numEntries = MAX_NETINFO;
    }
//    arrayNetInfo = (struct networkInfo *) malloc(sizeof(struct networkInfo *) * numEntries);
    for (i=0; i < numEntries; i++) {
        if (!(pIPAddrTable->table[i].wType & MIB_IPADDR_DISCONNECTED)){
            IPAddr.S_un.S_addr = (u_long) pIPAddrTable->table[i].dwAddr;
            char* aux = inet_ntoa(IPAddr);
            if (aux == "127.0.0.1"){
                retorno--;
                continue;
            }
            memcpy(arrayNetInfo[i].ip, aux, strlen(aux)+1);
//            printf("\tIP Address[%d]:     \t%s\n", i, netInfo.ip);
            IPAddr.S_un.S_addr = (u_long) pIPAddrTable->table[i].dwMask;
            aux = inet_ntoa(IPAddr);
            memcpy(arrayNetInfo[i].subnetMask, aux, strlen(aux)+1);
//            printf("\tSubnet Mask[%d]:    \t%s\n", i,  netInfo.subnetMask);
        }
        else {
            retorno--;
        }
    }

    if (pIPAddrTable) {
        FREE(pIPAddrTable);
        pIPAddrTable = NULL;
    }
    return numEntries;
}

/**
 * @brief Computer::setSo
 *
 * Executa método GetOSDisplayString para retornar o SO encontrado
 *
 * @return String do SO ou 'Windows Não Indetificado'
 */
std::string Computer::setSo()
{
    wchar_t OSInfo[BUFSIZE];
    BOOL found = this->GetOsDisplayString(OSInfo);
    if (found) {
        std::wstring wso(OSInfo);
        std::string so(wso.begin(), wso.end());
        return so;
    } else {
        std::cout << "SO não identificado" << std::endl;
        std::string so = "";
        return so;
    }
}

/**
 * @brief Computer::setUsuarioSo
 *
 * Utiliza a API nativa do Windows para identificar o usuário do Sistema Operacional
 *
 * @return String com o nome do usuário
 */
std::string Computer::setUsuarioSo()
{
    TCHAR username[BUFSIZE];
    DWORD size = BUFSIZE;
    BOOL result = GetUserName((TCHAR*)username, &size);
    if (result) {
        std::wstring wuser(username);
        std::string user(wuser.begin(), wuser.end());
        return user;
    } else {
        std::cout << "Usuário não identificado" << std::endl;
        std::string user = "Não Identificado";
        return user;
    }
}

/**
 * @brief Computer::GetOsDisplayString
 *
 * Recupera string do Sistema Operacional
 * Fonte: http://forum.codecall.net/topic/60159-how-to-determine-windows-operating-system-version-information-using-visual-c/
 *
 * @param pszOS
 * @return Verdadeiro se identificou ou falso se não conseguiu
 */
BOOL Computer::GetOsDisplayString(LPTSTR pszOS)
{
    OSVERSIONINFOEX osvi;
    SYSTEM_INFO si;
    PGETSYSTEMINFO pGetSystemInfo;
    PGETPRODUCTINFO pGetProductInfo;
    BOOL bOsVersionInfoEx;
    DWORD dwType;

    ZeroMemory(&si, sizeof(SYSTEM_INFO));
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
       return 1;

    // Call GetNativeSystemInfo if supported or GetSystemInfo otherwise.

    pGetSystemInfo = (PGETSYSTEMINFO ) GetProcAddress(
       GetModuleHandle(TEXT("kernel32.dll")),
       "GetNativeSystemInfo");
    if(NULL != pGetSystemInfo)
       pGetSystemInfo(&si);
    else GetSystemInfo(&si);

    if ( VER_PLATFORM_WIN32_NT==osvi.dwPlatformId &&
         osvi.dwMajorVersion > 4 )
    {
       StringCchCopy(pszOS, BUFSIZE, TEXT("Microsoft "));

       // Test for the specific product.

       if ( osvi.dwMajorVersion == 6 )
       {
          if( osvi.dwMinorVersion == 0 )
          {
             if( osvi.wProductType == VER_NT_WORKSTATION )
                 StringCchCat(pszOS, BUFSIZE, TEXT("Windows Vista "));
             else StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2008 " ));
          }

          if ( osvi.dwMinorVersion == 1 )
          {
             if( osvi.wProductType == VER_NT_WORKSTATION )
                 StringCchCat(pszOS, BUFSIZE, TEXT("Windows 7 "));
             else StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2008 R2 " ));
          }

          pGetProductInfo = (PGETPRODUCTINFO) GetProcAddress(
             GetModuleHandle(TEXT("kernel32.dll")),
             "GetProductInfo");

          pGetProductInfo( osvi.dwMajorVersion, osvi.dwMinorVersion, 0, 0, &dwType);

          switch( dwType )
          {
             case PRODUCT_ULTIMATE:
                StringCchCat(pszOS, BUFSIZE, TEXT("Ultimate" ));
                break;
             case 0x00000030:
                StringCchCat(pszOS, BUFSIZE, TEXT("Professional" ));
                break;
             case PRODUCT_HOME_PREMIUM:
                StringCchCat(pszOS, BUFSIZE, TEXT("Home Premium" ));
                break;
             case PRODUCT_HOME_BASIC:
                StringCchCat(pszOS, BUFSIZE, TEXT("Home Basic" ));
                break;
             case PRODUCT_ENTERPRISE:
                StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise" ));
                break;
             case PRODUCT_BUSINESS:
                StringCchCat(pszOS, BUFSIZE, TEXT("Business" ));
                break;
             case PRODUCT_STARTER:
                StringCchCat(pszOS, BUFSIZE, TEXT("Starter" ));
                break;
             case PRODUCT_CLUSTER_SERVER:
                StringCchCat(pszOS, BUFSIZE, TEXT("Cluster Server" ));
                break;
             case PRODUCT_DATACENTER_SERVER:
                StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter" ));
                break;
             case PRODUCT_DATACENTER_SERVER_CORE:
                StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter(core installation)" ));
                break;
             case PRODUCT_ENTERPRISE_SERVER:
                StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise" ));
                break;
             case PRODUCT_ENTERPRISE_SERVER_CORE:
                StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise (core installation)" ));
                break;
             case PRODUCT_ENTERPRISE_SERVER_IA64:
                StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise for Itanium-based Systems" ));
                break;
             case PRODUCT_SMALLBUSINESS_SERVER:
                StringCchCat(pszOS, BUFSIZE, TEXT("Small Business Server" ));
                break;
             case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
                StringCchCat(pszOS, BUFSIZE, TEXT("Small Business Server Premium" ));
                break;
             case PRODUCT_STANDARD_SERVER:
                StringCchCat(pszOS, BUFSIZE, TEXT("Standard" ));
                break;
             case PRODUCT_STANDARD_SERVER_CORE:
                StringCchCat(pszOS, BUFSIZE, TEXT("Standard (core installation)" ));
                break;
             case PRODUCT_WEB_SERVER:
                StringCchCat(pszOS, BUFSIZE, TEXT("Web Server" ));
                break;
          }
       }

       if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
       {
          if( GetSystemMetrics(SM_SERVERR2) )
             StringCchCat(pszOS, BUFSIZE, TEXT( "Windows Server 2003 R2, "));
          else if ( osvi.wSuiteMask & VER_SUITE_STORAGE_SERVER )
             StringCchCat(pszOS, BUFSIZE, TEXT( "Windows Storage Server 2003"));
          else if ( osvi.wSuiteMask & 0x00008000  )
             StringCchCat(pszOS, BUFSIZE, TEXT( "Windows Home Server"));
          else if( osvi.wProductType == VER_NT_WORKSTATION &&
                   si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
          {
             StringCchCat(pszOS, BUFSIZE, TEXT( "Windows XP Professional x64 Edition"));
          }
          else StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2003, "));

          // Test for the server type.
          if ( osvi.wProductType != VER_NT_WORKSTATION )
          {
             if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_IA64 )
             {
                 if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                    StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter Edition for Itanium-based Systems" ));
                 else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                    StringCchCat(pszOS, BUFSIZE, TEXT( "Enterprise Edition for Itanium-based Systems" ));
             }

             else if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
             {
                 if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                    StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter x64 Edition" ));
                 else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                    StringCchCat(pszOS, BUFSIZE, TEXT( "Enterprise x64 Edition" ));
                 else StringCchCat(pszOS, BUFSIZE, TEXT( "Standard x64 Edition" ));
             }

             else
             {
                 if ( osvi.wSuiteMask & VER_SUITE_COMPUTE_SERVER )
                    StringCchCat(pszOS, BUFSIZE, TEXT( "Compute Cluster Edition" ));
                 else if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                    StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter Edition" ));
                 else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                    StringCchCat(pszOS, BUFSIZE, TEXT( "Enterprise Edition" ));
                 else if ( osvi.wSuiteMask & VER_SUITE_BLADE )
                    StringCchCat(pszOS, BUFSIZE, TEXT( "Web Edition" ));
                 else StringCchCat(pszOS, BUFSIZE, TEXT( "Standard Edition" ));
             }
          }
       }

       if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
       {
          StringCchCat(pszOS, BUFSIZE, TEXT("Windows XP "));
          if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
             StringCchCat(pszOS, BUFSIZE, TEXT( "Home Edition" ));
          else StringCchCat(pszOS, BUFSIZE, TEXT( "Professional" ));
       }

       if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
       {
          StringCchCat(pszOS, BUFSIZE, TEXT("Windows 2000 "));

          if ( osvi.wProductType == VER_NT_WORKSTATION )
          {
             StringCchCat(pszOS, BUFSIZE, TEXT( "Professional" ));
          }
          else
          {
             if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter Server" ));
             else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                StringCchCat(pszOS, BUFSIZE, TEXT( "Advanced Server" ));
             else StringCchCat(pszOS, BUFSIZE, TEXT( "Server" ));
          }
       }

       // Eduardo: 2015-05-29
       // Remove essas informações que são desnecessárias
       return TRUE;

       // Include service pack (if any) and build number.

       //if( _tcslen(osvi.szCSDVersion) > 0 )
       if( size_t(wcslen(osvi.szCSDVersion)) > 0 )
       {
           StringCchCat(pszOS, BUFSIZE, TEXT(" ") );
           StringCchCat(pszOS, BUFSIZE, osvi.szCSDVersion);
       }

       TCHAR buf[80];

       StringCchPrintf( buf, 80, TEXT(" (build %d)"), osvi.dwBuildNumber);
       StringCchCat(pszOS, BUFSIZE, buf);

       if ( osvi.dwMajorVersion >= 6 )
       {
          if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
             StringCchCat(pszOS, BUFSIZE, TEXT( ", 64-bit" ));
          else if (si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_INTEL )
             StringCchCat(pszOS, BUFSIZE, TEXT(", 32-bit"));
       }

       _tprintf(_T("OS Version Information %s\n"), pszOS);
       return TRUE;
    }

    else
    {
       printf( "This sample does not support this version of Windows.\n");
       return FALSE;
    }
}

