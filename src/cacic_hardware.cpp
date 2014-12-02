#include "cacic_hardware.h"
cacic_hardware::cacic_hardware()
{
    QDir dir;
    logManager = QLogger::QLoggerManager::getInstance();
    logManager->addDestination(dir.currentPath() + "/Logs/cacic.log","Gercols (hardware)",QLogger::InfoLevel);
    logManager->addDestination(dir.currentPath() + "/Logs/cacic.log","Gercols (hardware)",QLogger::ErrorLevel);
}

cacic_hardware::~cacic_hardware()
{
    logManager->closeLogger();
    delete logManager;
}

void cacic_hardware::iniciaColeta()
{
#ifdef Q_OS_WIN
    this->coletaHardware = coletaWin();
#elif defined(Q_OS_LINUX)
    OperatingSystem operatingSystem;


    // Como criaremos pacotes agora, todas essas verificações podiam só
    // ser incluídas como dependências.

    // se o shell retorna erro ao tentar utilizar o lshw ou o dmidecode, instala o mesmo
    if( console("lshw").contains("/bin/sh:") ){
        QLogger::QLog_Info("Gercols (hardware)", "lshw nao estava instalado.");
        if(operatingSystem.getIdOs() == OperatingSystem::LINUX_ARCH)
            console("pacman -S --needed --noconfirm lshw");
        else if(operatingSystem.getIdOs() == OperatingSystem::LINUX_DEBIAN ||
                operatingSystem.getIdOs() == OperatingSystem::LINUX_UBUNTU )
            console("apt-get -y install lshw");
    }

    if( console("dmidecode").contains("/bin/sh:") ){
        QLogger::QLog_Info("Gercols (hardware)", "dmidecode nao estava instalado");
        if(operatingSystem.getIdOs() == OperatingSystem::LINUX_ARCH)
            console("pacman -S --needed --noconfirm dmidecode");
        else if(operatingSystem.getIdOs() == OperatingSystem::LINUX_DEBIAN ||
                operatingSystem.getIdOs() == OperatingSystem::LINUX_UBUNTU )
            console("apt-get -y install dmidecode");
    }

    this->coletaHardware = coletaLinux();
#endif
}
#ifdef Q_OS_WIN
/**********************************************************************
 * Faz a coleta de hardware por meio de WMI armazenando em formato json
 **********************************************************************/
QJsonObject cacic_hardware::coletaWin()
{  
    QJsonObject hardware;
    QStringList params;
    QJsonValue wmiResult;
    // Win32_ComputerSystem (Caption, Description, Domain, Manufacturer,
    //                       Model, Name, PrimaryOwnerName, TotalPhysicalMemory, Workgroup)
    params << "Caption" << "Description" << "Domain" << "Manufacturer" << "Model"
           << "Name" << "PrimaryOwnerName" << "TotalPhysicalMemory" << "Workgroup";
    wmiResult = wmi::wmiSearch("Win32_ComputerSystem", params);
    if (!wmiResult.isNull())
        hardware["Win32_ComputerSystem"] = wmiResult;
    //Win32_PortableBattery
    //  (Verifica se é notebook)
    params.clear();
    params << "Caption" << "Description" << "DeviceID" << "EstimatedChargeRemaining" << "ExpectedLife"
           << "FullChargeCapacity" << "Name" << "Status" << "Location";
    wmiResult = wmi::wmiSearch("Win32_PortableBattery", params);
    QJsonObject notebook;
    notebook["Value"] = QJsonValue::fromVariant(!wmiResult.isNull());
    hardware["IsNotebook"] = notebook;
    if (!wmiResult.isNull()){
        hardware["Win32_PortableBattery"] = wmiResult;
    }
    //Win32_Bios
    //  (Manufacturer, SMBIOSBIOSVersion, BIOSVersion, Version, SerialNumber, ReleaseDate)
    params.clear();
    params << "Manufacturer" << "SMBIOSBIOSVersion" << "BIOSVersion" << "Version" << "SerialNumber" << "ReleaseDate";
    wmiResult = wmi::wmiSearch("Win32_Bios", params);
    if (!wmiResult.isNull())
        hardware["Win32_BIOS"] = wmiResult;
    //Win32_BaseBoard
    //  (Manufacturer, Model, SerialNumber)
    params.clear();
    params << "Manufacturer" << "Model" << "SerialNumber";
    wmiResult = wmi::wmiSearch("Win32_Baseboard", params);
    if (!wmiResult.isNull())
        hardware["Win32_BaseBoard"] = wmiResult;
    //Win32_SystemEnclosure
    //  (Manufacturer, ChassisTypes, SerialNumber, SMBIOSAssetTag)
    params.clear();
    params << "Manufacturer" << "ChassisTypes" << "SerialNumber" << "SMBIOSAssetTag";
    wmiResult = wmi::wmiSearch("Win32_SystemEnclosure", params);
    if (!wmiResult.isNull())
        hardware["Win32_SystemEnclosure"] = wmiResult;
    //Win32_FloppyDrive
    //  (Manufacturer, Caption, Description, Name, MediaType, Size)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "Name";
    wmiResult = wmi::wmiSearch("Win32_FloppyDrive", params);
    if (!wmiResult.isNull())
        hardware["Win32_FloppyDrive"] = wmiResult;
    //Win32_DiskDrive
    //  (Manufacturer, Caption, Description, MediaType, Name, Size, Model)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "Name" << "MediaType" << "Size" << "Model";
    wmiResult = wmi::wmiSearch("Win32_DiskDrive", params);
    if (!wmiResult.isNull())
        hardware["Win32_DiskDrive"] = wmiResult;
    //Win32_CDROMDrive
    //  (Manufacturer, Caption, Description, Name, MediaType, Size)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "MediaType" << "Name" << "Size";
    wmiResult = wmi::wmiSearch("Win32_CDROMDrive", params);
    if (!wmiResult.isNull())
        hardware["Win32_CDROMDrive"] = wmiResult;
    //Win32_FloppyController
    //  (Manufacturer, Caption, Description, Name)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "Name";
    wmiResult = wmi::wmiSearch("Win32_FloppyController", params);
    if (!wmiResult.isNull())
        hardware["Win32_FloppyController"] = wmiResult;
    //Win32_MemoryDevice
    //  (Caption, Description, Name, BlockSize, DeviceID, NumberOfBlocks, Status)
    params.clear();
    params << "Caption" << "Description" << "Name" << "BlockSize" << "DeviceID" << "NumberOfBlocks" << "Status";
    wmiResult = wmi::wmiSearch("Win32_MemoryDevice", params);
    if (!wmiResult.isNull())
        hardware["Win32_MemoryDevice"] = wmiResult;
    //Win32_SMBIOSMemory
    //  (BlockSize, Caption, Description, DeviceID, Name, NumberOfBlocks, Status)
    params.clear();
    params << "Caption" << "Description" << "Name" << "BlockSize" << "DeviceID" << "NumberOfBlocks" << "Status";
    wmiResult = wmi::wmiSearch("Win32_SMBIOSMemory", params);
    if (!wmiResult.isNull())
        hardware["Win32_SMBIOSMemory"] = wmiResult;
    //Win32_SCSIController
    //  (Manufacturer, Caption, Description, Name, HardwareVersion)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "Name" << "HardwareVersion";
    wmiResult = wmi::wmiSearch("Win32_SCSIController", params);
    if (!wmiResult.isNull())
        hardware["Win32_SCSIController"] = wmiResult;
    //Win32_InfraredDevice
    //  (Manufacturer, Caption, Description, Name)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "Name";
    wmiResult = wmi::wmiSearch("Win32_InfraredDevice", params);
    if (!wmiResult.isNull())
        hardware["Win32_InfraredDevice"] = wmiResult;
    //Win32_USBController
    //  (Manufacturer, Caption, Description, Name)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "Name";
    wmiResult = wmi::wmiSearch("Win32_USBController", params);
    if (!wmiResult.isNull())
        hardware["Win32_USBController"] = wmiResult;
    //Win32_PCMCIAController
    //  (Manufacturer, Caption, Description, Name)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "Name";
    wmiResult = wmi::wmiSearch("Win32_PCMCIAController", params);
    if (!wmiResult.isNull())
        hardware["Win32_PCMCIAController"] = wmiResult;
    //Win32_VideoController
    //  (Description, VideoProcessor, AdapterRAM, CurrentHorizontalResolution, CurrentVerticalResolution, Caption, Name)
    params.clear();
    params << "VideoProcessor" << "AdapterRAM" << "Description" << "Name" << "CurrentHorizontalResolution"
           << "CurrentVerticalResolution" << "Caption" << "AcceleratorCapabilities";
    wmiResult = wmi::wmiSearch("Win32_VideoController", params);
    if (!wmiResult.isNull())
        hardware["Win32_VideoController"] = wmiResult;
    //Win32_DesktopMonitor
    //  (MonitorManufacturer, Caption, Description, MonitorType, Name)
    params.clear();
    params << "MonitorManufacturer" << "Caption" << "Description" << "MonitorType" << "Name";
    wmiResult = wmi::wmiSearch("Win32_DesktopMonitor", params);
    if (!wmiResult.isNull())
        hardware["Win32_DesktopMonitor"] = wmiResult;
    //Win32_Printer
    //  (Name, DriverName, PortName, ServerName, ShareName, HorizontalResolution, VerticalResolution, Comment, Shared, Network)
    params.clear();
    params << "Name" << "DriverName" << "PortName" << "ServerName" << "ShareName" << "HorizontalResolution"
           << "VerticalResolution" << "Comment" << "Shared" << "Network";
    wmiResult = wmi::wmiSearch("Win32_Printer", params);
    if (!wmiResult.isNull())
        hardware["Win32_Printer"] = wmiResult;
    //Win32_PortConnector
    //  (ExternalReferenceDesignator, PortType (verificar), ConnectorType (verificar), Manufacturer, Caption, Name)
    params.clear();
    params << "ExternalReferenceDesignator" << "PortType" << "ConnectorType" << "Name" << "Caption" << "Manufacturer";
    wmiResult = wmi::wmiSearch("Win32_PortConnector", params);
    if (!wmiResult.isNull())
        hardware["Win32_PortConnector"] = wmiResult;
    //Win32_SerialPort
    //  (Name, Caption, Description, StatusInfo)
    params.clear();
    params << "Name" << "Caption" << "Description" << "StatusInfo";
    wmiResult = wmi::wmiSearch("Win32_SerialPort", params);
    if (!wmiResult.isNull())
        hardware["Win32_SerialPort"] = wmiResult;
    //Win32_Processor
    //  (MaxClockSpeed, Name, Architecture, NumberOfCores, SocketDesignation, Manufacturer, Architecture, NumberOfCores
    //  CurrentClockSpeed, MaxClockSpeed, L2CacheSize, AddressWidth, DataWidth, VoltageCaps, CpuStatus,
    //  ProcessorId || UniqueId, AddressWidth)
    params.clear();
    params << "MaxClockSpeed" << "Name" << "Architecture" << "NumberOfCores" << "SocketDesignation" << "Manufacturer"
           << "Architecture" << "NumberOfCores" << "NumberOfLogicalProcessors" << "CurrentClockSpeed" << "MaxClockSpeed" << "L2CacheSize" << "AddressWidth"
           << "DataWidth" << "VoltageCaps" << "CpuStatus" << "ProcessorId" << "UniqueId" << "AddressWidth" << "Caption" << "Family";
    wmiResult = wmi::wmiSearch("Win32_Processor", params);
    if (!wmiResult.isNull())
        hardware["Win32_Processor"] = wmiResult;
    //Win32_OperatingSystem
    //  (Name, Version, CSDVersion, Description, InstallDate, Organization, RegisteredUser, SerialNumber)
    params.clear();
    params << "Name" << "Version" << "CSDVersion" << "Description" << "InstallDate" << "Organization" << "RegisteredUser"
           << "SerialNumber" << "Caption";
    wmiResult = wmi::wmiSearch("Win32_OperatingSystem", params);
    if (!wmiResult.isNull())
        hardware["Win32_OperatingSystem"] = wmiResult;
    //Win32_SystemSlot
    //  (Name, Description, SlotDesignation, CurrentUsage, Status, Shared)
    params.clear();
    params << "Name" << "Description" << "SlotDesignation" << "CurrentUsage" << "Status" << "Shared";
    wmiResult = wmi::wmiSearch("Win32_SystemSlot", params);
    if (!wmiResult.isNull())
        hardware["Win32_SystemSlot"] = wmiResult;
    //Win32_LogicalDisk
    //  (Caption, DriveType, Filesystem, VolumeName, ProviderName, Filesystem, VolumeName, Size, FreeSpace)
    params.clear();
    params << "Caption" << "DriveType" << "Filesystem" << "VolumeName" << "ProviderName" << "Filesystem" << "VolumeName"
           << "Size" << "FreeSpace" << "MediaType";
    wmiResult = wmi::wmiSearch("Win32_LogicalDisk", params);
    if (!wmiResult.isNull())
        hardware["Win32_LogicalDisk"] = wmiResult;
    //Win32_PhysicalMemory
    //  (Caption, Description, BankLabel, DeviceLocator, Capacity, Speed, MemoryType, SerialNumber)
    params.clear();
    params << "Caption" << "Description" << "BankLabel" << "DeviceLocator" << "Capacity" << "Speed" << "MemoryType";
    wmiResult = wmi::wmiSearch("Win32_PhysicalMemory", params);
    if (!wmiResult.isNull())
        hardware["Win32_PhysicalMemory"] = wmiResult;
    //Win32_Keyboard
    //  (Caption, Description, Name)
    params.clear();
    params << "Caption" << "Description" << "Name" << "Layout";
    wmiResult = wmi::wmiSearch("Win32_Keyboard", params);
    if (!wmiResult.isNull())
        hardware["Win32_Keyboard"] = wmiResult;
    //Win32_PointingDevice
    //  (Manufacturer, Caption, Description, PointingType, DeviceInterface)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "PointingType" << "DeviceInterface";
    wmiResult = wmi::wmiSearch("Win32_PointingDevice", params);
    if (!wmiResult.isNull())
        hardware["Win32_PointingDevice"] = wmiResult;
    //Win32_PnPSignedDriver (Muito grande, é necessário?)
    //  (Manufacturer, DeviceName, Description, Location, DeviceClass)
    //    params.clear();
    //    params << "Manufacturer" << "DeviceName" << "Description" << "Location" << "DeviceClass";
    //    wmiResult = wmi::wmiSearch("Win32_PnPSignedDriver", params);
    //    if (!wmiResult.isNull())
    //        hardware["Win32_PnPSignedDriver"] = wmiResult;

    return hardware;
}

#elif defined(Q_OS_LINUX)
/******************************************************************
 * Realiza coleta de hardware de acordo com o json gerado pelo lshw,
 * pegando os valores 'úteis' e armazenando em formato json
 ******************************************************************/
QJsonObject cacic_hardware::coletaLinux()
{
    /*Aumentar coleta de Hardware. jsonvalue-> hardware["Win32_Keyboard"] = keyboard (se possível)
 *                             jsonvalue-> hardware["Win32_PointingDevice"] = mouse (se possível)
 *                             Fora essas, detalhar mais as outras que já existem. Pegar todos os nomes, versão, vendor, id, etc..
 */
    QJsonObject hardware;

    QFile lshwFile("lshwJson.json");
    if( lshwFile.exists() )
        lshwFile.remove();

    console("lshw -json >> lshwJson.json");


    QJsonObject lshwJson = oCacic.getJsonFromFile("lshwJson.json")["children"].toArray().first().toObject();

    if( lshwJson.contains("id") && lshwJson["id"] == QJsonValue::fromVariant(QString("core")) ) {
        if ( lshwJson["children"].isArray() ){

            QJsonArray componentsArray =  lshwJson["children"].toArray();

            foreach(QJsonValue componentValue, componentsArray ) {
                QJsonObject component = componentValue.toObject();

                if( component["id"] == QJsonValue::fromVariant(QString("memory")) ) {
                    coletaLinuxMem(hardware, component);
                } else if ( component["id"] == QJsonValue::fromVariant(QString("cpu")) ) {
                    coletaLinuxCpu(hardware, component);
                } else if ( component["id"].toString().startsWith("pci") ) {
                    QJsonArray pciArray = component["children"].toArray();

                    foreach(QJsonValue pciValue, pciArray){
                        QJsonObject pciObject = pciValue.toObject();

                        if( pciObject["id"].toString().startsWith("pci") ) {
                            QJsonArray pciChildrenArray = pciObject["children"].toArray();

                            foreach(QJsonValue pciChildrenValue, pciChildrenArray) {
                                QJsonObject pciChildrenObject = pciChildrenValue.toObject();
                                coletaLinuxPci(hardware, pciChildrenObject); //Aqui não seria picChuldrenObject?!
                            }

                        } else if (pciObject["id"] == QJsonValue::fromVariant(QString("storage")) ) {
                            QJsonArray ioArray = pciObject["children"].toArray();

                            foreach(QJsonValue ioValue, ioArray) {
                                QJsonObject ioObject = ioValue.toObject();
                                coletaLinuxIO(hardware, ioObject);
                            }

                        } else {
                            coletaLinuxPci(hardware, pciObject);
                        }
                    }

                } else if ( component["id"].toString().startsWith("scsi") ) {
                    QJsonArray ioArray = component["children"].toArray();

                    foreach(QJsonValue ioValue, ioArray){
                        QJsonObject ioObject = ioValue.toObject();

                        coletaLinuxIO(hardware, ioObject);
                    }
                }

            }
        }

    }

    if ( getuid() != 0 ) qDebug() << "Coleta de Bios e Motherboard requer root.";
    coletaLinuxOperatingSystem(hardware);
    coletaLinuxBios(hardware);
    coletaLinuxMotherboard(hardware);
    coletaLinuxIsNotebook(hardware);
    coletaLinuxPrinters(hardware);

    lshwFile.remove();
    return hardware;
}

void cacic_hardware::coletaLinuxOperatingSystem(QJsonObject &hardware){
    QJsonObject so;
    OperatingSystem op;
    so["Caption"] = op.coletaVersaoOsEmString();
    so["Version"] = op.getNomeOs();
    so["InstallDate"] = console("ls -alct /|tail -1|awk '{print $6, $7, $8}'").split("\n").takeFirst();
    hardware["Win32_OperatingSystem"] = so;
}

void cacic_hardware::coletaLinuxMem(QJsonObject &hardware, const QJsonObject &component)
{
    QJsonObject memory;

    memory["Caption"] = component["description"];
    memory["DeviceLocator"] = component["physid"];
    memory["Capacity"] = QJsonValue::fromVariant(oCacic.convertDouble(component["size"].toDouble(),0) + " bytes");
    QStringList consoleOutput;
    consoleOutput = console("dmidecode --type 17").split("\n", QString::SkipEmptyParts);
    foreach(QString line, consoleOutput){
        if(line.contains("Type:")){
            memory["MemoryType"] = QJsonValue::fromVariant(QString(line.split("Type: ", QString::SkipEmptyParts).takeLast()));
            break;
        }
    }

    //memory["MemoryType"] = QJsonValue::fromVariant(component["children"].toObject()["slot"].toString());

    hardware["Win32_PhysicalMemory"] = memory;
}

void cacic_hardware::coletaLinuxCpu(QJsonObject &hardware, const QJsonObject &component)
{
    QJsonObject cpu;

    cpu["Caption"] = component["product"];
    cpu["Manufacturer"] = component["vendor"];
    cpu["MaxClockSpeed"] = QJsonValue::fromVariant(oCacic.convertDouble(component["capacity"].toDouble(),0) + " Hz");

    QStringList consoleOutput;
    consoleOutput = console("lscpu").split("\n", QString::SkipEmptyParts);
    foreach(QString line, consoleOutput){
        if(line.contains("CPU(s):")){
            cpu["NumberOfLogicalProcessors"] = QJsonValue::fromVariant(QString(line.split(" ").takeLast()));
            break;
        }
    }
    consoleOutput = console("cat /proc/cpuinfo").split("\n", QString::SkipEmptyParts);
    foreach(QString line, consoleOutput){
        if(line.contains("family") ){
            cpu["Family"] = QJsonValue::fromVariant(QString(line.split(" ").takeLast()));
            break;
        }
    }
    hardware["Win32_Processor"] = cpu;
}

void cacic_hardware::coletaLinuxPci(QJsonObject &hardware, const QJsonObject &pciJson)
{

    QJsonObject pciMember;
    QJsonArray pciNetwork;
    if (!hardware["NetworkAdapterConfiguration"].isNull()){
        pciNetwork = hardware["NetworkAdapterConfiguration"].toArray();
    }

    if ( pciJson["id"] == QJsonValue::fromVariant(QString("multimedia")) ) {
        pciMember["description"] = pciJson["description"];
        pciMember["product"] = pciJson["product"];
        pciMember["vendor"] = pciJson["vendor"];

        hardware["multimedia"] = pciMember;
    } else if( pciJson["id"] == QJsonValue::fromVariant(QString("network")) &&
               ( pciJson["description"].toString().contains("Wireless") ||
                 pciJson["product"].toString().contains("Wireless") )) {
        pciMember["description"] = pciJson["description"];
        pciMember["product"] = pciJson["product"];
        pciMember["vendor"] = pciJson["vendor"];
        pciMember["logicalname"] = pciJson["logicalname"];
        pciMember["serial"] = pciJson["serial"];
        pciMember["firmware"] = pciJson["configuration"].toObject()["firmware"];

        pciNetwork.append(pciMember);
        //        hardware["wireless_card"] = pciMember;
    } else if( pciJson["id"] == QJsonValue::fromVariant(QString("network")) ) {
        pciMember["description"] = pciJson["description"];
        pciMember["product"] = pciJson["product"];
        pciMember["vendor"] = pciJson["vendor"];
        pciMember["logicalname"] = pciJson["logicalname"];
        pciMember["serial"] = pciJson["serial"];
        pciMember["capacity"] = QJsonValue::fromVariant(
                    oCacic.convertDouble(pciJson["capacity"].toDouble(), 0) +
                " bits/s" );

        //        hardware["ethernet_card"] = pciMember;
        pciNetwork.append(pciMember);
    } else if( pciJson["id"] == QJsonValue::fromVariant(QString("display")) ) {
        pciMember["Description"] = pciJson["description"];
        pciMember["Product"] = pciJson["product"];
        pciMember["Vendor"] = pciJson["vendor"];
        pciMember["Width"] = QJsonValue::fromVariant(oCacic.convertDouble(pciJson["width"].toDouble(),0) );
        pciMember["Clock"] = QJsonValue::fromVariant(oCacic.convertDouble(pciJson["clock"].toDouble(),0) );


        hardware["Win32_PCMCIAController"] = pciMember;
    }

    if (!pciNetwork.isEmpty())
        hardware["NetworkAdapterConfiguration"] = pciNetwork;
}

void cacic_hardware::coletaLinuxIO(QJsonObject &hardware, const QJsonObject &ioJson)
{
    QJsonArray physicalArray;
    QJsonObject dispositivo;

    if ( !hardware["Win32_DiskDrive"].isNull() ) {
        physicalArray = hardware["Win32_DiskDrive"].toArray();
    }

    if ( ioJson["id"] == QJsonValue::fromVariant(QString("cdrom")) ) {

        dispositivo["Caption"] = ioJson["description"];
        dispositivo["Name"] = ioJson["logicalname"];

    } else if ( ioJson["id"] == QJsonValue::fromVariant(QString("disk")) ) {

        dispositivo["Caption"] = ioJson["description"];
        dispositivo["Model"] = ioJson["product"];
        dispositivo["Name"] = ioJson["logicalname"];
        //        dispositivo["serial"] = ioJson["serial"];
        dispositivo["Size"] = QJsonValue::fromVariant(oCacic.convertDouble(ioJson["size"].toDouble(),0)
                + " " + ioJson["units"].toString());

        foreach(QJsonValue partitionValue, ioJson["children"].toArray() ) {
            QJsonObject partitionObject = partitionValue.toObject();
            QJsonObject newPartition;
            QJsonArray partitionsList;

            if( !hardware["Win32_LogicalDisk"].isNull() ) {
                partitionsList = hardware["Win32_LogicalDisk"].toArray();
            }

            coletaGenericPartitionInfo(newPartition, partitionObject);

            if( partitionObject["description"] == QJsonValue::fromVariant(QString("Extended partition")) ) {

                foreach(QJsonValue extendedValue, partitionObject["children"].toArray()){
                    QJsonObject extendedObject = extendedValue.toObject();
                    QJsonArray extendedList;

                    if( !newPartition["children"].isNull() ) {
                        extendedList = newPartition["children"].toArray();
                    }

                    QString newExtended;

                    if ( extendedObject["logicalname"].isArray() ) {
                        newExtended = extendedObject["description"].toString() +  " " + extendedObject["logicalname"].toArray().at(0).toString();
                    } else {
                        newExtended = extendedObject["description"].toString() +  " " + extendedObject["logicalname"].toString();
                    }

                    extendedList.append(QJsonValue::fromVariant(newExtended));
                    newPartition["children"] = extendedList;
                }

            } else {
                if ( !partitionObject["configuration"].toObject()["filesystem"].isNull() )
                    newPartition["Filesystem"] = partitionObject["configuration"].toObject()["filesystem"];
                if ( !partitionObject["configuration"].toObject()["created"].isNull() )
                    newPartition["Created"] = partitionObject["configuration"].toObject()["created"];
                if ( !partitionObject["configuration"].toObject()["mount.options"].isNull() )
                    newPartition["MountOptions"] = partitionObject["configuration"].toObject()["mount.options"];
                if ( !partitionObject["configuration"].toObject()["label"].isNull() ){
                    newPartition["MediaType"] = partitionObject["configuration"].toObject()["label"];
                }else{
                    newPartition["MediaType"] = ioJson["description"];
                }
            }

            partitionsList.append(newPartition);
            hardware["Win32_LogicalDisk"] = partitionsList;
        }

    }

    physicalArray.append(dispositivo);
    hardware["Win32_DiskDrive"] = physicalArray;
}

void cacic_hardware::coletaGenericPartitionInfo(QJsonObject &newPartition, const QJsonObject &partitionObject)
{
    newPartition["Description"] = partitionObject["description"];

    if( !partitionObject["size"].isNull() )
        newPartition["Size"] = QJsonValue::fromVariant(oCacic.convertDouble(partitionObject["size"].toDouble(),0)
                + " " + partitionObject["units"].toString());
    else
        newPartition["Size"] = QJsonValue::fromVariant(oCacic.convertDouble(partitionObject["capacity"].toDouble(),0)
                + " " + partitionObject["units"].toString());

    //    if ( !partitionObject["capabilities"].toObject()["primary"].isNull() )
    //        newPartition["primary"] = partitionObject["capabilities"].toObject()["primary"];
    //    if ( !partitionObject["capabilities"].toObject()["bootable"].isNull() )
    //        newPartition["bootable"] = partitionObject["capabilities"].toObject()["bootable"];
    //    if ( !partitionObject["capabilities"].toObject()["journaled"].isNull() )
    //        newPartition["journaled"] = partitionObject["capabilities"].toObject()["journaled"];

    if( partitionObject["logicalname"].isArray() ) {
        newPartition["Caption"] = partitionObject["logicalname"].toArray().first();
    } else {
        newPartition["Caption"] = partitionObject["logicalname"];
    }

    QStringList dfOutput = console(QString("df -h")).split("\n");
    foreach(QString dfLine, dfOutput){
        if(dfLine.split(" ").at(0) == newPartition["Caption"].toString() ) {
            QStringList splitLine = dfLine.split(" ");
            splitLine.removeAll("");

            newPartition["FreeSpace"] = QJsonValue::fromVariant(splitLine.at(3));
        }
    }
}

void cacic_hardware::coletaLinuxBios(QJsonObject &hardware)
{

    QJsonObject bios;
    QStringList consoleOutput;

    consoleOutput = console("dmidecode -t bios").split("\n");
    foreach(QString line, consoleOutput){
        if(line.contains("Vendor:") ){
            bios["Manufacturer"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
        } else if(line.contains("Version:")){
            bios["Version"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
        } else if(line.contains("Release Date:")){
            bios["ReleaseDate"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
        } else if(line.contains("Runtime Size:")){
            bios["runtimeSize"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
        } else if(line.contains("ROM Size:")){
            bios["romSize"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
        } else if(line.contains("BIOS Revision:")){
            bios["revision"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
        }
    }
    hardware["Win32_BIOS"] = bios;
}

void cacic_hardware::coletaLinuxMotherboard(QJsonObject &hardware)
{

    QJsonObject motherboard;
    QStringList consoleOutput;

    consoleOutput= console("dmidecode -t 2").split("\n");

    foreach(QString line, consoleOutput){
        if(line.contains("Manufacturer:") ){
            motherboard["Manufacturer"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
        } else if(line.contains("Product Name:")){
            motherboard["ProductName"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
        } else if(line.contains("Version:")){
            motherboard["Version"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
        } else if(line.contains("Asset Tag:")){
            motherboard["Tag"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
        } else if(line.contains("Serial Number:")){
            motherboard["SerialNumber"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
        }
    }

    consoleOutput= console("dmidecode -t 10").split("\n");

    QStringList onboardCapabilities;
    foreach(QString line, consoleOutput){
        QString value;
        if(line.contains("Type:") ){
            value = line.split(":")[1].mid(1);
            if (!value.isNull() && !value.isEmpty())
                onboardCapabilities.push_back( value );
        }
    }

    if (!onboardCapabilities.isEmpty())
        motherboard["onboardCapabilities"] = QJsonValue::fromVariant(onboardCapabilities);

    hardware["Win32_BaseBoard"] = motherboard;
}

void cacic_hardware::coletaLinuxIsNotebook(QJsonObject &hardware)
{

    QStringList consoleOutput;

    consoleOutput= console("dmidecode -t 3").split("\n");

    foreach(QString line, consoleOutput){
        if(line.contains("Type:")
                && (line.contains("Notebook") || line.contains("Portable")) ){
            QJsonObject notebook;
            notebook["Value"] = QJsonValue::fromVariant(true);
            hardware["IsNotebook"] = notebook;
        }
    }

}

void cacic_hardware::coletaLinuxPrinters(QJsonObject &hardware)
{
    QStringList consoleOutput;

    if( console("lpstat").contains("/bin/sh:") ) { // Cups não instalado
        return;
    } else {

        consoleOutput = console("lpstat -a").split("\n");
        consoleOutput.removeLast(); // remover o último elemento que é somente vazio

        if( consoleOutput[0].contains("No destination") )
            return;

        QJsonArray printersList;
        foreach(QString line, consoleOutput ) {

            if ( line.split(" ")[1] == QString("accepting") ) {
                QString printerName = line.split(" ")[0];
                printersList.append(QJsonValue::fromVariant(printerName));
            }
        }
        hardware["Win32_Printer"] = printersList;
    }

}

#endif
QJsonObject cacic_hardware::toJsonObject() {
    return coletaHardware;
}
