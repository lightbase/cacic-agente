#include "cacic_hardware.h"

cacic_hardware::cacic_hardware()
{
}

void cacic_hardware::iniciaColeta()
{
#ifdef Q_OS_WIN
    this->coletaHardware = coletaWin();
#elif defined(Q_OS_LINUX)
    this->coletaHardware = coletaLinux();
#endif
}

QJsonObject cacic_hardware::coletaWin()
{
#if defined(Q_OS_WIN)
    // Implementação seguindo as definições na documentação WMI.
#endif
    return QJsonObject();
}

QJsonObject cacic_hardware::coletaLinux()
{

    OperatingSystem operatingSystem;
    ConsoleObject console;

    QJsonObject hardware;

    QFile lshwFile("lshwJson.json");
    if( lshwFile.exists() )
        lshwFile.remove();

    console("lshw -json >> lshwJson.json");


    QJsonObject lshwJson = oCacic.getJsonFromFile("lshwJson.json")["children"].toArray().first().toObject();

    if( lshwJson.contains("id") && lshwJson["id"] == "core") {
        if ( lshwJson["children"].isArray() ){
            qDebug() << "IS ARRAY!!";
            QJsonArray componentsArray =  lshwJson["children"].toArray();

            foreach(QJsonValue componentValue, componentsArray ) {
                QJsonObject component = componentValue.toObject();

                /* TODO:
                 * - Formatar direito as quantidades (memória,clock do cpu)
                 * com unidades mais amigáveis para humanos em todos métodos.
                 *
                 * coletaLinuxMem
                 * coletaLinuxCpu
                 * coletaLinuxPci - a fazer
                 */

                if( component["id"] == "memory" ) {
                    coletaLinuxMem(hardware,component);
                } else if ( component["id"] == "cpu" ) {
                    coletaLinuxCpu(hardware,component);
                } else if ( component["id"] == "pci" ) {
                    QJsonArray pciArray = component["children"].toArray();

                    foreach(QJsonValue pciValue, pciArray){
                        QJsonObject pciObject = pciValue.toObject();

                        coletaLinuxPci(hardware, pciObject);
                    }

                }

            }
        }

    }

    return hardware;
}

int cacic_hardware::wmi()
{
//    HRESULT hres;

//    // Step 1: --------------------------------------------------
//    // Initialize COM. ------------------------------------------

//    hres =  CoInitializeEx(0, COINIT_MULTITHREADED);
//    if (FAILED(hres))
//    {
//        cout << "Failed to initialize COM library. Error code = 0x"
//             << hex << hres << endl;
//        return 1;                  // Program has failed.
//    }

//    // Step 2: --------------------------------------------------
//    // Set general COM security levels --------------------------

//    hres =  CoInitializeSecurity(
//        NULL,
//        -1,                          // COM authentication
//        NULL,                        // Authentication services
//        NULL,                        // Reserved
//        RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
//        RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
//        NULL,                        // Authentication info
//        EOAC_NONE,                   // Additional capabilities
//        NULL                         // Reserved
//        );


//    if (FAILED(hres))
//    {
//        cout << "Failed to initialize security. Error code = 0x"
//            << hex << hres << endl;
//        CoUninitialize();
//        return 1;                    // Program has failed.
//    }

//    // Step 3: ---------------------------------------------------
//    // Obtain the initial locator to WMI -------------------------

//    IWbemLocator *pLoc = NULL;

//    hres = CoCreateInstance(
//        CLSID_WbemLocator,
//        0,
//        CLSCTX_INPROC_SERVER,
//        IID_IWbemLocator, (LPVOID *) &pLoc);

//    if (FAILED(hres))
//    {
//        cout << "Failed to create IWbemLocator object."
//            << " Err code = 0x"
//            << hex << hres << endl;
//        CoUninitialize();
//        return 1;                 // Program has failed.
//    }

//    // Step 4: -----------------------------------------------------
//    // Connect to WMI through the IWbemLocator::ConnectServer method

//    IWbemServices *pSvc = NULL;

//    // Connect to the local root\cimv2 namespace
//    // and obtain pointer pSvc to make IWbemServices calls.
//    hres = pLoc->ConnectServer(
//                    _bstr_t(L"ROOT\\CIMV2"),
//                    NULL,
//                    NULL,
//                    0,
//                    NULL,
//                    0,
//                    0,
//                    &pSvc
//                    );

//    if (FAILED(hres))
//    {
//        cout << "Could not connect. Error code = 0x"
//             << hex << hres << endl;
//        pLoc->Release();
//        CoUninitialize();
//        return 1;                // Program has failed.
//    }

//    cout << "Connected to ROOT\\CIMV2 WMI namespace" << endl;


//    // Step 5: --------------------------------------------------
//    // Set security levels on the proxy -------------------------

//    hres = CoSetProxyBlanket(
//       pSvc,                        // Indicates the proxy to set
//       RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
//       RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
//       NULL,                        // Server principal name
//       RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
//       RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
//       NULL,                        // client identity
//       EOAC_NONE                    // proxy capabilities
//    );

//    if (FAILED(hres))
//    {
//        cout << "Could not set proxy blanket. Error code = 0x"
//            << hex << hres << endl;
//        pSvc->Release();
//        pLoc->Release();
//        CoUninitialize();
//        return 1;               // Program has failed.
//    }

//    // Step 6: --------------------------------------------------
//    // Use the IWbemServices pointer to make requests of WMI ----

//    // For example, get the name of the operating system.
//    // The IWbemService::ExecQueryAsync method will call
//    // the QuerySink::Indicate method when it receives a result
//    // and the QuerySink::Indicate method will display the OS name
//    QuerySink* pResponseSink = new QuerySink();
//    hres = pSvc->ExecQueryAsync(
//        bstr_t("WQL"),
//        bstr_t("SELECT * FROM Win32_OperatingSystem"),
//        WBEM_FLAG_BIDIRECTIONAL,
//        NULL,
//        pResponseSink);

//    if (FAILED(hres))
//    {
//        cout << "Query for operating system name failed."
//            << " Error code = 0x"
//            << hex << hres << endl;
//        pSvc->Release();
//        pLoc->Release();
//        pResponseSink->Release();
//        CoUninitialize();
//        return 1;               // Program has failed.
//    }

//    // Step 7: -------------------------------------------------
//    // Wait to get the data from the query in step 6 -----------
//    Sleep(500);
//    pSvc->CancelAsyncCall(pResponseSink);

//    // Cleanup
//    // ========
//    pSvc->Release();
//    pLoc->Release();
//    CoUninitialize();

    return 0;   // Program successfully completed.

}

void cacic_hardware::coletaLinuxMem(QJsonObject &hardware, const QJsonObject &component)
{
    QJsonObject memory;

    memory["size"] = QJsonValue::fromVariant(oCacic.convertDouble(component["size"].toDouble(),0) + " bytes");

    hardware["memory"] = memory;
}

void cacic_hardware::coletaLinuxCpu(QJsonObject &hardware, const QJsonObject &component)
{
    QJsonObject cpu;

    cpu["name"] = component["product"];
    cpu["vendor"] = component["vendor"];
    cpu["clock"] = QJsonValue::fromVariant(oCacic.convertDouble(component["capacity"].toDouble(),0) + " Hz");

    hardware["cpu"] = cpu;
}

void cacic_hardware::coletaLinuxPci(QJsonObject &hardware, const QJsonObject &pciJson)
{

}

QJsonObject cacic_hardware::toJsonObject() {
    return coletaHardware;
}
