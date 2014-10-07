#include "cacic_hardware.h"
cacic_hardware::cacic_hardware()
{
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
    if( console("lshw").contains("/bin/sh:") ){ qDebug() << "lshw nao instalado.";
        if(operatingSystem.getIdOs() == OperatingSystem::LINUX_ARCH)
            console("pacman -S --needed --noconfirm lshw");
        else if(operatingSystem.getIdOs() == OperatingSystem::LINUX_DEBIAN ||
               operatingSystem.getIdOs() == OperatingSystem::LINUX_UBUNTU )
            console("apt-get -y install lshw");
    }

    if( console("dmidecode").contains("/bin/sh:") ){ qDebug() << "dmidecode nao instalado.";
        if(operatingSystem.getIdOs() == OperatingSystem::LINUX_ARCH)
            qDebug() << console("pacman -S --needed --noconfirm dmidecode");
        else if(operatingSystem.getIdOs() == OperatingSystem::LINUX_DEBIAN ||
               operatingSystem.getIdOs() == OperatingSystem::LINUX_UBUNTU )
            console("apt-get -y install dmidecode");
    }

    this->coletaHardware = coletaLinux();
#endif
}
#ifdef Q_OS_WIN
QJsonObject cacic_hardware::coletaWin()
{  
    QJsonObject hardware;
    QStringList params;
    QJsonValue wmiResult;
    // Win32_ComputerSystem (Caption, Description, Domain, DNSHostName, Manufacturer,
    //                       Model, Name, PrimaryOwnerName, TotalPhysicalMemory, Workgroup)
    params << "Caption" << "Description" << "Domain" << "DNSHostName" << "Manufacturer" << "Model"
           << "Name" << "PrimaryOwnerName" << "TotalPhysicalMemory" << "Workgroup";
    wmiResult = wmi::wmiSearch("Win32_ComputerSystem", params);
    if (!wmiResult.isNull())
        hardware["ComputerSystem"] = wmiResult;
    //Win32_PortableBattery
    //  (Verifica se é notebook)
    params.clear();
    wmiResult = wmi::wmiSearch("Win32_PortableBattery", params);
    hardware["IsNotebook"] = QJsonValue::fromVariant(!wmiResult.isNull());
    if (!wmiResult.isNull()){
        hardware["PortableBattery"] = wmiResult;
    }
    //Win32_Bios
    //  (Manufacturer, SMBIOSBIOSVersion, BIOSVersion, Version, SerialNumber, ReleaseDate)
    params.clear();
    params << "Manufacturer" << "SMBIOSBIOSVersion" << "BIOSVersion" << "Version" << "SerialNumber" << "ReleaseDate";
    wmiResult = wmi::wmiSearch("Win32_Bios", params);
    if (!wmiResult.isNull())
        hardware["Bios"] = wmiResult;
    //Win32_BaseBoard
    //  (Manufacturer, Model, SerialNumber)
    params.clear();
    params << "Manufacturer" << "Model" << "SerialNumber";
    wmiResult = wmi::wmiSearch("Win32_Baseboard", params);
    if (!wmiResult.isNull())
        hardware["BaseBoard"] = wmiResult;
    //Win32_SystemEnclosure
    //  (Manufacturer, ChassisTypes, SerialNumber, SMBIOSAssetTag)
    params.clear();
    params << "Manufacturer" << "ChassisTypes" << "SerialNumber" << "SMBIOSAssetTag";
    wmiResult = wmi::wmiSearch("Win32_SystemEnclosure", params);
    if (!wmiResult.isNull())
        hardware["SystemEnclosure"] = wmiResult;
    //Win32_FloppyDrive
    //  (Manufacturer, Caption, Description, Name, MediaType, Size)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "Name";
    wmiResult = wmi::wmiSearch("Win32_FloppyDrive", params);
    if (!wmiResult.isNull())
        hardware["FloppyDrive"] = wmiResult;
    //Win32_DiskDrive
    //  (Manufacturer, Caption, Description, Name, MediaType, Size, SerialNumber, Model, FirmwareRevision)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "Name" << "MediaType" << "Size" << "SerialNumber"
           << "Model" << "FirmwareRevision";
    wmiResult = wmi::wmiSearch("Win32_DiskDrive", params);
    if (!wmiResult.isNull())
        hardware["DiskDrive"] = wmiResult;
    //Win32_CDROMDrive
    //  (Manufacturer, Caption, Description, Name, MediaType, Size)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "MediaType" << "Name" << "Size";
    wmiResult = wmi::wmiSearch("Win32_CDROMDrive", params);
    if (!wmiResult.isNull())
        hardware["CDROMDrive"] = wmiResult;
    //Win32_FloppyController
    //  (Manufacturer, Caption, Description, Name)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "Name";
    wmiResult = wmi::wmiSearch("Win32_FloppyController", params);
    if (!wmiResult.isNull())
        hardware["FloppyController"] = wmiResult;
    //Win32_SCSIController
    //  (Manufacturer, Caption, Description, Name, HardwareVersion)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "Name" << "HardwareVersion";
    wmiResult = wmi::wmiSearch("Win32_SCSIController", params);
    if (!wmiResult.isNull())
        hardware["SCSIController"] = wmiResult;
    //Win32_InfraredDevice
    //  (Manufacturer, Caption, Description, Name)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "Name";
    wmiResult = wmi::wmiSearch("Win32_InfraredDevice", params);
    if (!wmiResult.isNull())
        hardware["InfraredDevice"] = wmiResult;
    //Win32_USBController
    //  (Manufacturer, Caption, Description, Name)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "Name";
    wmiResult = wmi::wmiSearch("Win32_USBController", params);
    if (!wmiResult.isNull())
        hardware["USBController"] = wmiResult;
    //Win32_PCMCIAController
    //  (Manufacturer, Caption, Description, Name)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "Name";
    wmiResult = wmi::wmiSearch("Win32_PCMCIAController", params);
    if (!wmiResult.isNull())
        hardware["PCMCIAController"] = wmiResult;
    //Win32_VideoController
    //  (Description, VideoProcessor, AdapterRAM, CurrentHorizontalResolution, CurrentVerticalResolution, Caption, Name)
    params.clear();
    params << "VideoProcessor" << "AdapterRAM" << "Description" << "Name" << "CurrentHorizontalResolution"
           << "CurrentVerticalResolution" << "Caption" << "AcceleratorCapabilities";
    wmiResult = wmi::wmiSearch("Win32_VideoController", params);
    if (!wmiResult.isNull())
        hardware["VideoController"] = wmiResult;
    //Win32_DesktopMonitor
    //  (MonitorManufacturer, Caption, Description, MonitorType, Name)
    params.clear();
    params << "MonitorManufacturer" << "Caption" << "Description" << "MonitorType" << "Name";
    wmiResult = wmi::wmiSearch("Win32_DesktopMonitor", params);
    if (!wmiResult.isNull())
        hardware["DesktopMonitor"] = wmiResult;
    //Win32_Printer
    //  (Name, DriverName, PortName, ServerName, ShareName, HorizontalResolution, VerticalResolution, Comment, Shared, Network)
    params.clear();
    params << "Name" << "DriverName" << "PortName" << "ServerName" << "ShareName" << "HorizontalResolution"
           << "VerticalResolution" << "Comment" << "Shared" << "Network";
    wmiResult = wmi::wmiSearch("Win32_Printer", params);
    if (!wmiResult.isNull())
        hardware["Printer"] = wmiResult;
    //Win32_PortConnector
    //  (ExternalReferenceDesignator, PortType (verificar), ConnectorType (verificar), Manufacturer, Caption, Name)
    params.clear();
    params << "ExternalReferenceDesignator" << "PortType" << "ConnectorType" << "Name" << "Caption" << "Manufacturer";
    wmiResult = wmi::wmiSearch("Win32_PortConnector", params);
    if (!wmiResult.isNull())
        hardware["PortConnector"] = wmiResult;
    //Win32_SerialPort
    //  (Name, Caption, Description, StatusInfo)
    params.clear();
    params << "Name" << "Caption" << "Description" << "StatusInfo";
    wmiResult = wmi::wmiSearch("Win32_SerialPort", params);
    if (!wmiResult.isNull())
        hardware["SerialPort"] = wmiResult;
    //Win32_Processor
    //  (MaxClockSpeed, Name, Architecture, NumberOfCores, SocketDesignation, Manufacturer, Architecture, NumberOfCores
    //  CurrentClockSpeed, MaxClockSpeed, L2CacheSize, AddressWidth, DataWidth, VoltageCaps, CpuStatus,
    //  ProcessorId || UniqueId, AddressWidth)
    params.clear();
    params << "MaxClockSpeed" << "Name" << "Architecture" << "NumberOfCores" << "SocketDesignation" << "Manufacturer"
           << "Architecture" << "NumberOfCores" << "CurrentClockSpeed" << "MaxClockSpeed" << "L2CacheSize" << "AddressWidth"
           << "DataWidth" << "VoltageCaps" << "CpuStatus" << "ProcessorId" << "UniqueId" << "AddressWidth";
    wmiResult = wmi::wmiSearch("Win32_Processor", params);
    if (!wmiResult.isNull())
        hardware["Processor"] = wmiResult;
    //Win32_OperatingSystem
    //  (Name, Version, CSDVersion, Description, InstallDate, Organization, RegisteredUser, SerialNumber)
    params.clear();
    params << "Name" << "Version" << "CSDVersion" << "Description" << "InstallDate" << "Organization" << "RegisteredUser"
           << "SerialNumber";
    wmiResult = wmi::wmiSearch("Win32_OperatingSystem", params);
    if (!wmiResult.isNull())
        hardware["OperatingSystem"] = wmiResult;
    //Win32_SystemSlot
    //  (Name, Description, SlotDesignation, CurrentUsage, Status, Shared)
    params.clear();
    params << "Name" << "Description" << "SlotDesignation" << "CurrentUsage" << "Status" << "Shared";
    wmiResult = wmi::wmiSearch("Win32_SystemSlot", params);
    if (!wmiResult.isNull())
        hardware["SystemSlot"] = wmiResult;
    //Win32_LogicalDisk
    //  (Caption, DriveType, Filesystem, VolumeName, ProviderName, Filesystem, VolumeName, Size, FreeSpace)
    params.clear();
    params << "Caption" << "DriveType" << "Filesystem" << "VolumeName" << "ProviderName" << "Filesystem" << "VolumeName"
           << "Size" << "FreeSpace";
    wmiResult = wmi::wmiSearch("Win32_LogicalDisk", params);
    if (!wmiResult.isNull())
        hardware["LogicalDisk"] = wmiResult;
    //Win32_PhysicalMemory
    //  (Caption, Description, BankLabel, DeviceLocator, Capacity, Speed, MemoryType, SerialNumber)
    params.clear();
    params << "Caption" << "Description" << "BankLabel" << "DeviceLocator" << "Capacity" << "Speed" << "MemoryType";
    wmiResult = wmi::wmiSearch("Win32_PhysicalMemory", params);
    if (!wmiResult.isNull())
        hardware["PhysicalMemory"] = wmiResult;
    //Win32_Keyboard
    //  (Caption, Description, Name)
    params.clear();
    params << "Caption" << "Description" << "Name" << "Layout";
    wmiResult = wmi::wmiSearch("Win32_Keyboard", params);
    if (!wmiResult.isNull())
        hardware["Keyboard"] = wmiResult;
    //Win32_PointingDevice
    //  (Manufacturer, Caption, Description, PointingType, DeviceInterface)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "PointingType" << "DeviceInterface";
    wmiResult = wmi::wmiSearch("Win32_PointingDevice", params);
    if (!wmiResult.isNull())
        hardware["PointingDevice"] = wmiResult;
    //Win32_PnPSignedDriver
    //  (Manufacturer, DeviceName, Description, Location, DeviceClass)
    params.clear();
    params << "Manufacturer" << "DeviceName" << "Description" << "Location" << "DeviceClass";
    wmiResult = wmi::wmiSearch("Win32_PnPSignedDriver", params);
    if (!wmiResult.isNull())
        hardware["PnPSignedDriver"] = wmiResult;
//    qDebug() << hardware;
    return hardware;
}

#elif defined(Q_OS_LINUX)
QJsonObject cacic_hardware::coletaLinux()
{

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

    if ( getuid() != 0 ) qDebug() << "Coleta de Bios e Motherboard requer root.";
    coletaLinuxBios(hardware);
    coletaLinuxMotherboard(hardware);
    coletaLinuxIsNotebook(hardware);
    coletaLinuxPrinters(hardware);

    lshwFile.remove();
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

void cacic_hardware::coletaLinuxBios(QJsonObject &hardware)
{

    QJsonObject bios;
    QStringList consoleOutput;

    consoleOutput = console("dmidecode -t bios").split("\n");
    foreach(QString line, consoleOutput){
        if(line.contains("Vendor:") ){
                bios["vendor"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
        } else if(line.contains("Version:")){
              bios["version"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
        } else if(line.contains("Release Date:")){
              bios["releaseDate"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
        } else if(line.contains("Runtime Size:")){
              bios["runtimeSize"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
        } else if(line.contains("ROM Size:")){
              bios["romSize"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
        } else if(line.contains("BIOS Revision:")){
              bios["revision"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
        }
    }
    hardware["bios"] = bios;
}

void cacic_hardware::coletaLinuxMotherboard(QJsonObject &hardware)
{

    QJsonObject motherboard;
    QStringList consoleOutput;

    consoleOutput= console("dmidecode -t 2").split("\n");
//    qDebug() << consoleOutput;
    foreach(QString line, consoleOutput){
        if(line.contains("Manufacturer:") ){
                motherboard["manufacturer"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
        } else if(line.contains("Product Name:")){
              motherboard["productName"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
        } else if(line.contains("Version:")){
              motherboard["version"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
        } else if(line.contains("Asset Tag:")){
            motherboard["assetTag"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
        } else if(line.contains("Serial Number:")){
            motherboard["serialNumber"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
        }
    }

    consoleOutput= console("dmidecode -t 10").split("\n");

    QVariantList onboardCapabilities;
    foreach(QString line, consoleOutput){

        if(line.contains("Type:") )
            onboardCapabilities.append( QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) ) );

    }

    motherboard["onboardCapabilities"] = QJsonValue::fromVariant(onboardCapabilities);

    hardware["motherboard"] = motherboard;
}

void cacic_hardware::coletaLinuxIsNotebook(QJsonObject &hardware)
{

    QStringList consoleOutput;

    consoleOutput= console("dmidecode -t 3").split("\n");
//    qDebug() << consoleOutput;
    foreach(QString line, consoleOutput){
        if(line.contains("Type:")
                && (line.contains("Notebook") || line.contains("Portable")) ){
                QJsonObject notebook;
                notebook["value"] = QJsonValue::fromVariant(true);
                hardware["isNotebook"] = notebook;
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
        hardware["printers"] = printersList;
    }

}

#endif
QJsonObject cacic_hardware::toJsonObject() {
    return coletaHardware;
}
