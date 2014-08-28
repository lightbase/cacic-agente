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
    QJsonObject wmi;
    // Win32_ComputerSystem (Caption, Description, Domain, DNSHostName, Manufacturer,
    //                       Model, Name, PrimaryOwnerName, TotalPhysicalMemory, Workgroup)
    params << "Caption" << "Description" << "Domain" << "DNSHostName" << "Manufacturer" << "Model"
           << "Name" << "PrimaryOwnerName" << "TotalPhysicalMemory" << "Workgroup";
    wmi =  wmiSearch("Win32_ComputerSystem", params);
    if (!wmi.isEmpty())
        hardware["ComputerSystem"] = wmiSearch("Win32_ComputerSystem", params);
    //Win32_PortableBattery
    //  (Verifica se é notebook)
    params.clear();
    hardware["IsNotebook"] = QJsonValue::fromVariant(!wmiSearch("Win32_PortableBattery", params).isEmpty());
    //Win32_Bios
    //  (Manufacturer, SMBIOSBIOSVersion, BIOSVersion, Version, SerialNumber, ReleaseDate)
    params.clear();
    params << "Manufacturer" << "SMBIOSBIOSVersion" << "BIOSVersion" << "Version" << "SerialNumber" << "ReleaseDate";
    wmi = wmiSearch("Win32_Bios", params);
    if (!wmi.isEmpty())
        hardware["Bios"] = wmi;
    //Win32_BaseBoard
    //  (Manufacturer, Model, SerialNumber)
    params.clear();
    params << "Manufacturer" << "Model" << "SerialNumber";
    wmi = wmiSearch("Win32_Baseboard", params);
    if (!wmi.isEmpty())
        hardware["BaseBoard"] = wmi;
    //Win32_SystemEnclosure
    //  (Manufacturer, ChassisTypes, SerialNumber, SMBIOSAssetTag)
    //Win32_FloppyDrive
    //  (Manufacturer, Caption, Description, Name, MediaType, Size)
    //Win32_DiskDrive
    //  (Manufacturer, Caption, Description, Name, MediaType, Size, SerialNumber, Model, FirmwareRevision)
    //Win32_CDROMDrive
    //  (Manufacturer, Caption, Description, Name, MediaType, Size)
    //Win32_FloppyController
    //  (Manufacturer, Caption, Description, Name)
    //Win32_SCSIController
    //  (Manufacturer, Caption, Description, Name, HardwareVersion)
    //Win32_InfraredDevice
    //  (Manufacturer, Caption, Description, Name)
    //Win32_USBController
    //  (Manufacturer, Caption, Description, Name)
    //Win32_PCMCIAController
    //  (Manufacturer, Caption, Description, Name)
    //Win32_VideoController
    //  (Description, VideoProcessor, AdapterRAM, CurrentHorizontalResolution, CurrentVerticalResolution)
    //Win32_DesktopMonitor
    //  (MonitorManufacturer, Caption, Description, MonitorType)
    //Win32_Printer
    //  (Name, DriverName, PortName, ServerName, ShareName, HorizontalResolution, VerticalResolution, Comment, Shared, Network)
    //Win32_PortConnector
    //  (ExternalReferenceDesignator, PortType, ConnectorType)
    //Win32_SerialPort
    //  (Name, Caption, Description, StatusInfo)
    //Win32_Processor
    //  (MaxClockSpeed, Name, Architecture, NumberOfCores, SocketDesignation, Manufacturer, Name, Architecture, NumberOfCores
    //  CurrentClockSpeed, MaxClockSpeed, L2CacheSize, AddressWidth, DataWidth, VoltageCaps, CpuStatus,
    //  ProcessorId || UniqueId, AddressWidth)
    //Win32_OperatingSystem
    //  (Name, Version, CSDVersion, Description, InstallDate, Organization, RegisteredUser, SerialNumber)
    //Win32_SystemSlot
    //  (Name, Description, SlotDesignation, CurrentUsage, Status, Shared)
    //Win32_LogicalDisk
    //  (Caption, DriveType, Filesystem, VolumeName, ProviderName, Filesystem, VolumeName, Size, FreeSpace)
    //Win32_PhysicalMemory
    //  (Caption, Description, BankLabel, DeviceLocator, Capacity, Speed, MemoryTypem, SerialNumber)
    //Win32_Keyboard
    //  (Manufacturer, Caption, Description)
    //Win32_PointingDevice
    //  (Manufacturer, Caption, Description, PointingType, DeviceInterface)
    //Win32_PnPSignedDriver
    //  (Manufacturer, DeviceName, Description, Location, DeviceClass)
    qDebug() << hardware;
    return hardware;
}

QJsonObject cacic_hardware::wmiSearch(QString classe, QStringList params)
{
    QJsonObject wmiReturn;
    QString paramsString;
    if (!params.empty()){
        for(int i = 0; i!=params.size();i++)
            paramsString.append(params.at(i) + ((i != params.size() - 1) ? "," : ""));
    }
    QAxObject *objIWbemLocator = new QAxObject("WbemScripting.SWbemLocator");
    QAxObject *objWMIService = objIWbemLocator->querySubObject("ConnectServer(QString&,QString&)",
                                                                    QString("."),
                                                                    QString("root\\CIMV2")
                                                               );
    QAxObject* returnList = objWMIService->querySubObject("ExecQuery(QString&)",
                                                                QString("SELECT " + ((!paramsString.isEmpty()) ?
                                                                                     paramsString : "*") +
                                                                        " FROM " + classe)
                                                          );
    QAxObject *enum1 = returnList->querySubObject("_NewEnum");
    //ui->textBrowser_4->setHtml(enum1->generateDocumentation());
    IEnumVARIANT* enumInterface = 0;
    enum1->queryInterface(IID_IEnumVARIANT, (void**)&enumInterface);

    //ui->textBrowser_4->setHtml(enumInterface->);
    enumInterface->Reset();
    // QAxObject *item = 0;

    //qDebug()<<"the count of objinterlist is "<<QString::number(objInterList->dynamicCall("Count").toInt());
    for (int i = 0; i < returnList->dynamicCall("Count").toInt(); i++) {
        VARIANT *theItem = (VARIANT*)malloc(sizeof(VARIANT));
        if (enumInterface->Next(1,theItem,NULL) != S_FALSE){
            QAxObject *item = new QAxObject((IUnknown *)theItem->punkVal);
            if(item){
                //pega o retorno da query
                QString instance = item->dynamicCall("GetObjectText_(int)", QVariant(0)).toString();
                QStringList instanceList = instance.split("\n\t");
                //Pra cada linha grava no json os valores com cada tag
                foreach(QString value, instanceList){
                    qDebug() << value;
                    QStringList valueList = value.split("=");
                    if (valueList.size() > 1){
                        QString tag = valueList.at(0).trimmed();
                        QString aux = valueList.at(1).trimmed();
                        aux.remove("\"");
                        aux.remove(";");
                        aux.remove("\n");
                        //verifica se é lista
                        qDebug() << aux;
                        if(aux.contains("{") && aux.contains("}")){
                            QStringList auxList = aux.split(",");
                            QJsonArray jList;
                            foreach(QString valueList, auxList){
                                if (valueList.contains("{"))
                                    valueList.remove("{");
                                else if (valueList.contains("}"))
                                    valueList.remove("}");
                                jList.append(valueList.trimmed());
                            }
                            wmiReturn[tag] = jList;
                        } else {
                            //O último valor sempre volta com "}" no final.
                            if (aux.contains("}"))
                                aux.remove("}");
                            wmiReturn[tag] = QJsonValue::fromVariant(aux.trimmed());
                        }
                    }
                }
            }
        }
    }
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
