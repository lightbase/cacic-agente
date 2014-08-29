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
#ifdef Q_OS_WIN
QJsonObject cacic_hardware::coletaWin()
{  
    QJsonObject hardware;
    QStringList params;
    QJsonValue wmi;
    // Win32_ComputerSystem (Caption, Description, Domain, DNSHostName, Manufacturer,
    //                       Model, Name, PrimaryOwnerName, TotalPhysicalMemory, Workgroup)
    params << "Caption" << "Description" << "Domain" << "DNSHostName" << "Manufacturer" << "Model"
           << "Name" << "PrimaryOwnerName" << "TotalPhysicalMemory" << "Workgroup";
    wmi =  wmiSearch("Win32_ComputerSystem", params);
    if (!wmi.isNull())
        hardware["ComputerSystem"] = wmi;
    //Win32_PortableBattery
    //  (Verifica se é notebook)
    params.clear();
    hardware["IsNotebook"] = QJsonValue::fromVariant(!wmiSearch("Win32_PortableBattery", params).isNull());
    //Win32_Bios
    //  (Manufacturer, SMBIOSBIOSVersion, BIOSVersion, Version, SerialNumber, ReleaseDate)
    params.clear();
    params << "Manufacturer" << "SMBIOSBIOSVersion" << "BIOSVersion" << "Version" << "SerialNumber" << "ReleaseDate";
    wmi = wmiSearch("Win32_Bios", params);
    if (!wmi.isNull())
        hardware["Bios"] = wmi;
    //Win32_BaseBoard
    //  (Manufacturer, Model, SerialNumber)
    params.clear();
    params << "Manufacturer" << "Model" << "SerialNumber";
    wmi = wmiSearch("Win32_Baseboard", params);
    if (!wmi.isNull())
        hardware["BaseBoard"] = wmi;
    //Win32_SystemEnclosure
    //  (Manufacturer, ChassisTypes, SerialNumber, SMBIOSAssetTag)
    params.clear();
    params << "Manufacturer" << "ChassisTypes" << "SerialNumber" << "SMBIOSAssetTag";
    wmi = wmiSearch("Win32_SystemEnclosure", params);
    if (!wmi.isNull())
        hardware["SystemEnclosure"] = wmi;
    //Win32_FloppyDrive
    //  (Manufacturer, Caption, Description, Name, MediaType, Size)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "Name";
    wmi = wmiSearch("Win32_FloppyDrive", params);
    if (!wmi.isNull())
        hardware["FloppyDrive"] = wmi;
    //Win32_DiskDrive
    //  (Manufacturer, Caption, Description, Name, MediaType, Size, SerialNumber, Model, FirmwareRevision)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "Name" << "MediaType" << "Size" << "SerialNumber"
           << "Model" << "FirmwareRevision";
    wmi = wmiSearch("Win32_DiskDrive", params);
    if (!wmi.isNull())
        hardware["DiskDrive"] = wmi;
    //Win32_CDROMDrive
    //  (Manufacturer, Caption, Description, Name, MediaType, Size)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "MediaType" << "Name" << "Size";
    wmi = wmiSearch("Win32_CDROMDrive", params);
    if (!wmi.isNull())
        hardware["CDROMDrive"] = wmi;
    //Win32_FloppyController
    //  (Manufacturer, Caption, Description, Name)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "Name";
    wmi = wmiSearch("Win32_FloppyController", params);
    if (!wmi.isNull())
        hardware["FloppyController"] = wmi;
    //Win32_SCSIController
    //  (Manufacturer, Caption, Description, Name, HardwareVersion)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "Name" << "HardwareVersion";
    wmi = wmiSearch("Win32_SCSIController", params);
    if (!wmi.isNull())
        hardware["SCSIController"] = wmi;
    //Win32_InfraredDevice
    //  (Manufacturer, Caption, Description, Name)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "Name";
    wmi = wmiSearch("Win32_InfraredDevice", params);
    if (!wmi.isNull())
        hardware["InfraredDevice"] = wmi;
    //Win32_USBController
    //  (Manufacturer, Caption, Description, Name)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "Name";
    wmi = wmiSearch("Win32_USBController", params);
    if (!wmi.isNull())
        hardware["USBController"] = wmi;
    //Win32_PCMCIAController
    //  (Manufacturer, Caption, Description, Name)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "Name";
    wmi = wmiSearch("Win32_PCMCIAController", params);
    if (!wmi.isNull())
        hardware["PCMCIAController"] = wmi;
    //Win32_VideoController
    //  (Description, VideoProcessor, AdapterRAM, CurrentHorizontalResolution, CurrentVerticalResolution, Caption, Name)
    params.clear();
    params << "VideoProcessor" << "AdapterRAM" << "Description" << "Name" << "CurrentHorizontalResolution"
           << "CurrentVerticalResolution" << "Caption" << "AcceleratorCapabilities";
    wmi = wmiSearch("Win32_VideoController", params);
    if (!wmi.isNull())
        hardware["VideoController"] = wmi;
    //Win32_DesktopMonitor
    //  (MonitorManufacturer, Caption, Description, MonitorType, Name)
    params.clear();
    params << "MonitorManufacturer" << "Caption" << "Description" << "MonitorType" << "Name";
    wmi = wmiSearch("Win32_DesktopMonitor", params);
    if (!wmi.isNull())
        hardware["DesktopMonitor"] = wmi;
    //Win32_Printer
    //  (Name, DriverName, PortName, ServerName, ShareName, HorizontalResolution, VerticalResolution, Comment, Shared, Network)
    params.clear();
    params << "Name" << "DriverName" << "PortName" << "ServerName" << "ShareName" << "HorizontalResolution"
           << "VerticalResolution" << "Comment" << "Shared" << "Network";
    wmi = wmiSearch("Win32_Printer", params);
    if (!wmi.isNull())
        hardware["Printer"] = wmi;
    //Win32_PortConnector
    //  (ExternalReferenceDesignator, PortType (verificar), ConnectorType (verificar), Manufacturer, Caption, Name)
    params.clear();
    params << "ExternalReferenceDesignator" << "PortType" << "ConnectorType" << "Name" << "Caption" << "Manufacturer";
    wmi = wmiSearch("Win32_PortConnector", params);
    if (!wmi.isNull())
        hardware["PortConnector"] = wmi;
    //Win32_SerialPort
    //  (Name, Caption, Description, StatusInfo)
    params.clear();
    params << "Name" << "Caption" << "Description" << "StatusInfo";
    wmi = wmiSearch("Win32_SerialPort", params);
    if (!wmi.isNull())
        hardware["SerialPort"] = wmi;
    //Win32_Processor
    //  (MaxClockSpeed, Name, Architecture, NumberOfCores, SocketDesignation, Manufacturer, Architecture, NumberOfCores
    //  CurrentClockSpeed, MaxClockSpeed, L2CacheSize, AddressWidth, DataWidth, VoltageCaps, CpuStatus,
    //  ProcessorId || UniqueId, AddressWidth)
    params.clear();
    params << "MaxClockSpeed" << "Name" << "Architecture" << "NumberOfCores" << "SocketDesignation" << "Manufacturer"
           << "Architecture" << "NumberOfCores" << "CurrentClockSpeed" << "MaxClockSpeed" << "L2CacheSize" << "AddressWidth"
           << "DataWidth" << "VoltageCaps" << "CpuStatus" << "ProcessorId" << "UniqueId" << "AddressWidth";
    wmi = wmiSearch("Win32_Processor", params);
    if (!wmi.isNull())
        hardware["Processor"] = wmi;
    //Win32_OperatingSystem
    //  (Name, Version, CSDVersion, Description, InstallDate, Organization, RegisteredUser, SerialNumber)
    params.clear();
    params << "Name" << "Version" << "CSDVersion" << "Description" << "InstallDate" << "Organization" << "RegisteredUser"
           << "SerialNumber";
    wmi = wmiSearch("Win32_OperatingSystem", params);
    if (!wmi.isNull())
        hardware["OperatingSystem"] = wmi;
    //Win32_SystemSlot
    //  (Name, Description, SlotDesignation, CurrentUsage, Status, Shared)
    params.clear();
    params << "Name" << "Description" << "SlotDesignation" << "CurrentUsage" << "Status" << "Shared";
    wmi = wmiSearch("Win32_SystemSlot", params);
    if (!wmi.isNull())
        hardware["SystemSlot"] = wmi;
    //Win32_LogicalDisk
    //  (Caption, DriveType, Filesystem, VolumeName, ProviderName, Filesystem, VolumeName, Size, FreeSpace)
    params.clear();
    params << "Caption" << "DriveType" << "Filesystem" << "VolumeName" << "ProviderName" << "Filesystem" << "VolumeName"
           << "Size" << "FreeSpace";
    wmi = wmiSearch("Win32_LogicalDisk", params);
    if (!wmi.isNull())
        hardware["LogicalDisk"] = wmi;
    //Win32_PhysicalMemory
    //  (Caption, Description, BankLabel, DeviceLocator, Capacity, Speed, MemoryType, SerialNumber)
    params.clear();
    params << "Caption" << "Description" << "BankLabel" << "DeviceLocator" << "Capacity" << "Speed" << "MemoryType";
    wmi = wmiSearch("Win32_PhysicalMemory", params);
    if (!wmi.isNull())
        hardware["PhysicalMemory"] = wmi;
    //Win32_Keyboard
    //  (Caption, Description, Name)
    params.clear();
    params << "Caption" << "Description" << "Name" << "Layout";
    wmi = wmiSearch("Win32_Keyboard", params);
    if (!wmi.isNull())
        hardware["Keyboard"] = wmi;
    //Win32_PointingDevice
    //  (Manufacturer, Caption, Description, PointingType, DeviceInterface)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "PointingType" << "DeviceInterface";
    wmi = wmiSearch("Win32_PointingDevice", params);
    if (!wmi.isNull())
        hardware["PointingDevice"] = wmi;
    //Win32_PnPSignedDriver
    //  (Manufacturer, DeviceName, Description, Location, DeviceClass)
    params.clear();
    params << "Manufacturer" << "DeviceName" << "Description" << "Location" << "DeviceClass";
    wmi = wmiSearch("Win32_PnPSignedDriver", params);
    if (!wmi.isNull())
        hardware["PnPSignedDriver"] = wmi;
//    qDebug() << hardware;
    return hardware;
}

QJsonValue cacic_hardware::wmiSearch(QString classe, QStringList params)
{
    QJsonValue wmiReturn;
    QJsonObject itemWmi;
    QJsonArray arrayItens;
    QString paramsString;
    if (!params.empty()){
        for(int i = 0; i!=params.size();i++)
            paramsString.append(params.at(i) + ((i != params.size() - 1) ? "," : ""));
    }
    QString query = "SELECT " + ((!paramsString.isEmpty()) ? paramsString : "*") + " FROM " + classe;
    //conecta ao script (Como já estamos usando QObject, o WbemScripting... não funciona chamando normal,
    // por isso deve ser usado o CoInitializeEx)
    ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
    QAxObject *objIWbemLocator = new QAxObject();
    objIWbemLocator->setControl("WbemScripting.SWbemLocator");
    if (objIWbemLocator->isNull())
        return QJsonValue();
    //conecta ao wmi
    QAxObject *objWMIService = objIWbemLocator->querySubObject("ConnectServer(QString&,QString&)",
                                                                    QString("."),
                                                                    QString("root\\CIMV2")
                                                               );
//    qDebug() << ">>>>>>>>>" << objIWbemLocator->isNull() << objWMIService->isNull();
    //Faz a pesquisa no wmi
    QAxObject* returnList = objWMIService->querySubObject("ExecQuery(QString&)", query);
//        qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<";
    QAxObject *enum1 = returnList->querySubObject("_NewEnum");

    IEnumVARIANT* enumInterface = 0;

    enum1->queryInterface(IID_IEnumVARIANT, (void**)&enumInterface);

    enumInterface->Reset();
    int countReturnListObj = returnList->dynamicCall("Count").toInt();
    for (int i = 0; i < countReturnListObj; i++) {
        VARIANT *theItem = (VARIANT*)malloc(sizeof(VARIANT));
        if (enumInterface->Next(1,theItem,NULL) != S_FALSE){
            QAxObject *item = new QAxObject((IUnknown *)theItem->punkVal);
            if(item){
                //pega o retorno da query
                QString instance = item->dynamicCall("GetObjectText_(int)", QVariant(0)).toString();
                QStringList instanceList = instance.split("\n\t");
                //Pra cada linha grava no json os valores com cada tag
                foreach(QString value, instanceList){
                    QStringList valueList = value.split("=");
                    if (valueList.size() > 1){
                        QString tag = valueList.at(0).trimmed();
                        QString aux = valueList.at(1).trimmed();
                        aux.remove("\"");
                        aux.remove(";");
                        aux.remove("\n");
                        //verifica se é lista
                        if(aux.startsWith("{") && aux.endsWith("}")){
                            QStringList auxList = aux.split(",");
                            QJsonArray jList;
                            foreach(QString valueList, auxList){
                                if (valueList.contains("{"))
                                    valueList.remove("{");
                                if (valueList.contains("}"))
                                    valueList.remove("}");
                                jList.append(valueList.trimmed());
                            }
                            itemWmi[tag] = jList;
                        } else {
                            //O último valor sempre volta com "}" no final.
                            if (aux.contains("}"))
                                aux.remove("}");
                            itemWmi[tag] = QJsonValue::fromVariant(aux.trimmed());
                        }
                    }
                } // foreach(QString value, instanceList)
            } // if(item)
        } // if (enumInterface->Next(1,theItem,NULL) != S_FALSE)
        //Se houver mais de um objeto, grava em uma lista
        if (countReturnListObj > 1){
            arrayItens.append(QJsonValue(itemWmi));
        }
    }
    objIWbemLocator->clear();
    ::CoUninitialize();
    //se for mais de um objeto, retorna array.
    if (!arrayItens.isEmpty())
        wmiReturn = QJsonValue(arrayItens);
    //senão, se o objeto não for vazio (se tiver coletado algo)
    else if (!itemWmi.isEmpty())
        wmiReturn = QJsonValue(itemWmi);

    return wmiReturn;
}
#elif defined(Q_OS_LINUX)
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

    if( lshwJson.contains("id") && lshwJson["id"] == QJsonValue::fromVariant(QString("core")) ) {
        if ( lshwJson["children"].isArray() ){
//            qDebug() << "IS ARRAY!!";
            QJsonArray componentsArray =  lshwJson["children"].toArray();

            foreach(QJsonValue componentValue, componentsArray ) {
                QJsonObject component = componentValue.toObject();

                /* TODO:
                 * - Formatar direito as quantidades (memória,clock do cpu)
                 * com unidades mais amigáveis para humanos em todos métodos.
                 *
                 * coletaLinuxMem
                 * coletaLinuxCpu
                 * coletaLinuxPci
                 */

                if( component["id"] == QJsonValue::fromVariant(QString("memory")) ) {
                    coletaLinuxMem(hardware,component);
                } else if ( component["id"] == QJsonValue::fromVariant(QString("cpu")) ) {
                    coletaLinuxCpu(hardware,component);
                } else if ( component["id"] == QJsonValue::fromVariant(QString("pci")) ) {
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

    QJsonObject pciMember;

    if ( pciJson["id"] == QJsonValue::fromVariant(QString("multimedia")) ) {
        pciMember["description"] = pciJson["description"];
        pciMember["product"] = pciJson["product"];
        pciMember["vendor"] = pciJson["vendor"];

        hardware["multimedia"] = pciMember;
    } else if (pciJson["id"].toString().contains("pci:") ) {
        QJsonArray pciChildren = pciJson["children"].toArray();

        foreach( QJsonValue pciChild, pciChildren ) {
            QJsonObject pciChildJson = pciChild.toObject();

            if( pciChildJson["id"] == QJsonValue::fromVariant(QString("network")) &&
                   ( pciChildJson["description"].toString().contains("Wireless") ||
                     pciChildJson["product"].toString().contains("Wireless") )) {
                pciMember["description"] = pciChildJson["description"];
                pciMember["product"] = pciChildJson["product"];
                pciMember["vendor"] = pciChildJson["vendor"];
                pciMember["logicalname"] = pciChildJson["logicalname"];
                pciMember["serial"] = pciChildJson["serial"];
                pciMember["firmware"] = pciChildJson["configuration"].toObject()["firmware"];

                hardware["wireless_card"] = pciMember;
            } else if( pciChildJson["id"] == QJsonValue::fromVariant(QString("network")) ) {
                pciMember["description"] = pciChildJson["description"];
                pciMember["product"] = pciChildJson["product"];
                pciMember["vendor"] = pciChildJson["vendor"];
                pciMember["logicalname"] = pciChildJson["logicalname"];
                pciMember["serial"] = pciChildJson["serial"];
                pciMember["capacity"] = QJsonValue::fromVariant(
                                        oCacic.convertDouble(pciChildJson["capacity"].toDouble(), 0) +
                                        " bits/s" );

                hardware["ethernet_card"] = pciMember;
            }

        }
    }
}
#endif
QJsonObject cacic_hardware::toJsonObject() {
    return coletaHardware;
}
