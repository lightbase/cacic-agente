#include "cacic_hardware.h"
cacic_hardware::cacic_hardware()
{
    logcacic = new LogCacic(LOG_GERCOLS_HARDWARE, Identificadores::ENDERECO_PATCH_CACIC+"/Logs");
}

cacic_hardware::~cacic_hardware()
{
    delete logcacic;
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
        logcacic->escrever(LogCacic::InfoLevel, "lshw nao estava instalado.");
        if(operatingSystem.getIdOs() == OperatingSystem::LINUX_ARCH)
            console("pacman -S --needed --noconfirm lshw");
        else if(operatingSystem.getIdOs() == OperatingSystem::LINUX_DEBIAN ||
                operatingSystem.getIdOs() == OperatingSystem::LINUX_UBUNTU )
            console("apt-get -y install lshw");
    }

    if( console("dmidecode").contains("/bin/sh:") ){
        logcacic->escrever(LogCacic::InfoLevel, "dmidecode nao estava instalado");
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

    HardwareClasses hClass(this->_exceptionsClasses);
    hClass.init();
    foreach (QString className, hClass.classes().keys()) {
        QJsonValue wmiResult;
        wmiResult = wmi::wmiSearch(className, hClass.classes()[className]);
        if (!wmiResult.isNull()){
            if(className == "Win32_OperatingSystem"){
                QJsonObject osJson = wmiResult.toObject();
                QJsonValue osArchitecture = wmi::wmiSearch("Win32_OperatingSystem", QStringList("OSArchitecture"));
                if (!osArchitecture.isNull()) osJson["OSArchitecture"] = osArchitecture.toObject()["OSArchitecture"];
                osJson["InstallDate"] = QJsonValue::fromVariant(QVariant(CCacic::padronizarData(wmiResult.toObject()["InstallDate"].toString())));
                wmiResult = osJson;
            } else if (className == "Win32_Processor") {
                QJsonObject tmp = wmiResult.toObject();
                tmp["Family"] = QJsonValue::fromVariant(QVariant(wmiResult.toObject()["Level"].toString()));
                wmiResult = tmp;
            } else if (className == "Win32_BIOS"){
                QJsonObject tmp = wmiResult.toObject();
                tmp["Version"] = QJsonValue::fromVariant(QVariant(wmiResult.toObject()["SMBIOSBIOSVersion"].toString()));
                tmp["ReleaseDate"] = QJsonValue::fromVariant(QVariant(CCacic::padronizarData(wmiResult.toObject()["ReleaseDate"].toString())));
                wmiResult = tmp;
            } else if (className == "Win32_PortableBattery") {
                QJsonObject notebook;
                notebook["Value"] = QJsonValue::fromVariant(!wmiResult.isNull());
                hardware["IsNotebook"] = notebook;
            }
            hardware[className] = wmiResult;
        }
    }
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


    QJsonObject lshwJson = CCacic::getJsonFromFile("lshwJson.json")["children"].toArray().first().toObject();

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
                                coletaLinuxPci(hardware, pciChildrenObject);
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

    if ( getuid() != 0 ) logcacic->escrever(LogCacic::InfoLevel, "Coleta de Bios e Motherboard requer root.");
//    qDebug() << "OS";
    coletaLinuxOperatingSystem(hardware);
//    qDebug() << "BIOS";
    coletaLinuxBios(hardware);
//    qDebug() << "Motherboard";
    coletaLinuxMotherboard(hardware);
//    qDebug() << "Notebook";
    coletaLinuxIsNotebook(hardware);
//    qDebug() << "Printers";
    coletaLinuxPrinters(hardware);

    if(lshwFile.exists()) {
        lshwFile.remove();
    }
    return hardware;
}

void cacic_hardware::coletaLinuxOperatingSystem(QJsonObject &hardware){
    QJsonObject so;
    OperatingSystem op;
    so["Caption"] = op.getNomeOs();
    so["Version"] = op.coletaVersaoOsEmString();
    QStringList auxList = console("df").split("\n");
    QString particao;
    if (auxList.size() > 0) {
        particao = auxList.at(1);
        auxList = particao.split(" ");
        if (auxList.size() > 0){
            particao = auxList.takeFirst();
        }else
            particao = "";
    }
    if (!particao.isEmpty() && !particao.contains("/cow")){
        QString data = console("tune2fs -l "+ particao +" | grep 'Filesystem created:'");
        QStringList auxList = data.split("\n");
        if (auxList.size() > 0)
            data = auxList.first();
        so["InstallDate"] = CCacic::padronizarData(data);
    } else
        so["InstallDate"] = QString("00/00/0000");
    hardware["OperatingSystem"] = so;
}

void cacic_hardware::coletaLinuxMem(QJsonObject &hardware, const QJsonObject &component)
{
    if(!_exceptionsClasses.contains("Win32_PhysicalMemory")
        || !(_exceptionsClasses.contains("Win32_PhysicalMemory")
            && _exceptionsClasses["Win32_PhysicalMemory"].isEmpty())){
        QJsonObject memory;

        if (!(_exceptionsClasses.contains("Win32_PhysicalMemory")
              && _exceptionsClasses["Win32_PhysicalMemory"].contains("Caption")))
            memory["Caption"] = component["description"];
        if (!(_exceptionsClasses.contains("Win32_PhysicalMemory")
              && _exceptionsClasses["Win32_PhysicalMemory"].contains("DeviceLocator")))
            memory["DeviceLocator"] = component["physid"];
        if (!(_exceptionsClasses.contains("Win32_PhysicalMemory")
              && _exceptionsClasses["Win32_PhysicalMemory"].contains("Capacity")))
            memory["Capacity"] = QJsonValue::fromVariant(CCacic::convertDouble(component["size"].toDouble(),0));

        if (!(_exceptionsClasses.contains("Win32_PhysicalMemory")
              && _exceptionsClasses["Win32_PhysicalMemory"].contains("MemoryType"))){
            QStringList consoleOutput;
            consoleOutput = console("dmidecode --type 17").split("\n", QString::SkipEmptyParts);
            foreach(QString line, consoleOutput){
                if(line.contains("Type:")){

                    // Mapa criado de acordo com a documentacao MSDN para
                    // Win32_PhysicalMemory. Pode ser que o nome dado pelo
                    // dmidecode difere dos declarados aqui.
                    QMap<QString, int> memMap;
                    memMap.insert("DRAM", 2);
                    memMap.insert("Syncronous DRAM", 3);
                    memMap.insert("Cache DRAM", 4);
                    memMap.insert("EDO", 5);
                    memMap.insert("EDRAM", 6);
                    memMap.insert("VRAM", 7);
                    memMap.insert("SRAM", 8);
                    memMap.insert("RAM", 9);
                    memMap.insert("ROM", 10);
                    memMap.insert("Flash", 11);
                    memMap.insert("EEPROM", 12);
                    memMap.insert("FEPROM", 13);
                    memMap.insert("EPROM", 14);
                    memMap.insert("CDRAM", 15);
                    memMap.insert("3DRAM", 16);
                    memMap.insert("SDRAM", 17);
                    memMap.insert("SGRAM", 18);
                    memMap.insert("RDRAM", 19);
                    memMap.insert("DDR", 20);
                    memMap.insert("DDR2", 21);
                    memMap.insert("DDR3", 22);

                    QString memoryTypeS = QString(line.split("Type: ", QString::SkipEmptyParts).takeLast());

                    if (memMap.contains(memoryTypeS))
                        memory["MemoryType"] = memMap.value(memoryTypeS);
                    else
                        memory["MemoryType"] = 0;

                    break;
                }
            }
        }

        hardware["Win32_PhysicalMemory"] = memory;
    } else {
        qDebug() << "Coleta PhysicalMemory desativada";
    }
}

void cacic_hardware::coletaLinuxCpu(QJsonObject &hardware, const QJsonObject &component)
{
    if(!_exceptionsClasses.contains("Win32_Processor")
        || !(_exceptionsClasses.contains("Win32_Processor")
            && _exceptionsClasses["Win32_Processor"].isEmpty())){
        QJsonObject cpu;
        QStringList consoleOutput;

        if (!(_exceptionsClasses.contains("Win32_Processor")
              && _exceptionsClasses["Win32_Processor"].contains("Family")))
            cpu["Family"] = component["product"];
        if (!(_exceptionsClasses.contains("Win32_Processor")
              && _exceptionsClasses["Win32_Processor"].contains("Manufacturer")))
            cpu["Manufacturer"] = component["vendor"];

        consoleOutput = console("dmidecode -t processor").split("\n", QString::SkipEmptyParts);
        foreach(QString line, consoleOutput){
            if (!line.isEmpty()) {
                QString aux;
                QStringList auxList;

                if(line.contains("Max Speed") || line.contains("Current Speed")) {

                    auxList = line.split(" ");
                    if (auxList.size() >= 2) {
                        aux = auxList.at(2);
                        if( auxList.size() >=3 ){
                            aux.append(" ");
                            aux.append(auxList.at(3));
                        }
                    } else
                        aux = "";

                    if(line.contains("Current Speed")
                        && !(_exceptionsClasses.contains("Win32_Processor")
                            && _exceptionsClasses["Win32_Processor"].contains("CurrentClockSpeed")))
                        cpu["CurrentClockSpeed"] = QJsonValue::fromVariant(aux);
                    else if(line.contains("Max Speed")
                            && !(_exceptionsClasses.contains("Win32_Processor")
                                && _exceptionsClasses["Win32_Processor"].contains("MaxClockSpeed")))
                        cpu["MaxClockSpeed"] = QJsonValue::fromVariant(aux);
                } else if (line.contains("Version")
                           && !(_exceptionsClasses.contains("Win32_Processor")
                               && _exceptionsClasses["Win32_Processor"].contains("Name"))){
                    auxList = line.split(":");

                    if(auxList.size() >= 2)
                        aux = auxList.at(1).trimmed();
                    else
                        aux = "";

                    cpu["Name"] = QJsonValue::fromVariant(aux);
                }
            }
        }

        if (!(_exceptionsClasses.contains("Win32_Processor")
              && _exceptionsClasses["Win32_Processor"].contains("NumberOfCores")))
        {
            consoleOutput = console("lscpu").split("\n", QString::SkipEmptyParts);
            foreach(QString line, consoleOutput){
                if(line.contains("CPU(s):")){
                    cpu["NumberOfCores"] = QJsonValue::fromVariant(QString(line.split(" ").takeLast()));
                    break;
                }
            }
        }

    //    consoleOutput = console("cat /proc/cpuinfo").split("\n", QString::SkipEmptyParts);
    //    foreach(QString line, consoleOutput){
    //        if(line.contains("family") ){
    //            cpu["Family"] = QJsonValue::fromVariant(QString(line.split(" ").takeLast()));
    //            break;
    //        }
    //    }
        hardware["Win32_Processor"] = cpu;
    } else {
        qDebug() << "Coleta Processor desativada";
    }
}

void cacic_hardware::coletaLinuxPci(QJsonObject &hardware, const QJsonObject &pciJson)
{

    QJsonObject pciMember;
    QJsonArray pciNetwork;
    if (!hardware["NetworkAdapterConfiguration"].isNull()){
        pciNetwork = hardware["NetworkAdapterConfiguration"].toArray();
    }

    if ( pciJson["id"] == QJsonValue::fromVariant(QString("multimedia"))
         && (!_exceptionsClasses.contains("Multimedia")
             || !(_exceptionsClasses.contains("Multimedia")
                 && _exceptionsClasses["Multimedia"].isEmpty()))){
        if (!(_exceptionsClasses.contains("Multimedia")
              && _exceptionsClasses["Multimedia"].contains("Description")))
            pciMember["Description"] = pciJson["description"];
        if (!(_exceptionsClasses.contains("Multimedia")
              && _exceptionsClasses["Multimedia"].contains("Product")))
            pciMember["Product"] = pciJson["product"];
        if (!(_exceptionsClasses.contains("Multimedia")
              && _exceptionsClasses["Multimedia"].contains("Manufacturer")))
            pciMember["Manufacturer"] = pciJson["vendor"];

        hardware["Multimedia"] = pciMember;
    } else if( pciJson["id"] == QJsonValue::fromVariant(QString("network")) &&
               ( pciJson["description"].toString().contains("Wireless") ||
                 pciJson["product"].toString().contains("Wireless") )) {
        pciMember["Description"] = pciJson["description"];
        pciMember["Product"] = pciJson["product"];
        pciMember["Manufacturer"] = pciJson["vendor"];
        pciMember["Logicalname"] = pciJson["logicalname"];
        pciMember["Serial"] = pciJson["serial"];
        pciMember["Firmware"] = pciJson["configuration"].toObject()["firmware"];

        pciNetwork.append(pciMember);
        //        hardware["wireless_card"] = pciMember;
    } else if( pciJson["id"] == QJsonValue::fromVariant(QString("network")) ) {
        pciMember["Description"] = pciJson["description"];
        pciMember["Product"] = pciJson["product"];
        pciMember["Manufacturer"] = pciJson["vendor"];
        pciMember["LogicalName"] = pciJson["logicalname"];
        pciMember["Serial"] = pciJson["serial"];
        pciMember["Capacity"] = QJsonValue::fromVariant(
                    CCacic::convertDouble(pciJson["capacity"].toDouble(), 0));

        //        hardware["ethernet_card"] = pciMember;
        pciNetwork.append(pciMember);
    } else if( pciJson["id"] == QJsonValue::fromVariant(QString("display"))
               && (!_exceptionsClasses.contains("Win32_PCMCIAController")
                   || !(_exceptionsClasses.contains("Win32_PCMCIAController")
                       && _exceptionsClasses["Win32_PCMCIAController"].isEmpty()))){
        if (!(_exceptionsClasses.contains("Win32_PCMCIAController")
              && _exceptionsClasses["Win32_PCMCIAController"].contains("Description")))
            pciMember["Description"] = pciJson["description"];
        if (!(_exceptionsClasses.contains("Win32_PCMCIAController")
              && _exceptionsClasses["Win32_PCMCIAController"].contains("Product")))
            pciMember["Product"] = pciJson["product"];
        if (!(_exceptionsClasses.contains("Win32_PCMCIAController")
              && _exceptionsClasses["Win32_PCMCIAController"].contains("Manufacturer")))
            pciMember["Manufacturer"] = pciJson["vendor"];
        if (!(_exceptionsClasses.contains("Win32_PCMCIAController")
              && _exceptionsClasses["Win32_PCMCIAController"].contains("Width")))
            pciMember["Width"] = QJsonValue::fromVariant(CCacic::convertDouble(pciJson["width"].toDouble(),0) );
        if (!(_exceptionsClasses.contains("Win32_PCMCIAController")
              && _exceptionsClasses["Win32_PCMCIAController"].contains("Clock")))
            pciMember["Clock"] = QJsonValue::fromVariant(CCacic::convertDouble(pciJson["clock"].toDouble(),0) );


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
        if (!(_exceptionsClasses.contains("Win32_DiskDrive")
              && _exceptionsClasses["Win32_DiskDrive"].contains("Caption")))
            dispositivo["Caption"] = ioJson["description"];
        if (!(_exceptionsClasses.contains("Win32_DiskDrive")
              && _exceptionsClasses["Win32_DiskDrive"].contains("Name")))
            dispositivo["Name"] = ioJson["logicalname"];
    } else if ( ioJson["id"] == QJsonValue::fromVariant(QString("disk")) ) {
        if (!(_exceptionsClasses.contains("Win32_DiskDrive")
              && _exceptionsClasses["Win32_DiskDrive"].contains("Caption")))
            dispositivo["Caption"] = ioJson["description"];
        if (!(_exceptionsClasses.contains("Win32_DiskDrive")
              && _exceptionsClasses["Win32_DiskDrive"].contains("Model")))
            dispositivo["Model"] = ioJson["product"];
        if (!(_exceptionsClasses.contains("Win32_DiskDrive")
              && _exceptionsClasses["Win32_DiskDrive"].contains("Name")))
            dispositivo["Name"] = ioJson["logicalname"];
        //        dispositivo["serial"] = ioJson["serial"];
        if (!(_exceptionsClasses.contains("Win32_DiskDrive")
              && _exceptionsClasses["Win32_DiskDrive"].contains("Size")))
            dispositivo["Size"] = QJsonValue::fromVariant(CCacic::convertDouble(ioJson["size"].toDouble(),0));

        if(!_exceptionsClasses.contains("Win32_LogicalDisk")
            || !(_exceptionsClasses.contains("Win32_LogicalDisk")
                && _exceptionsClasses["Win32_LogicalDisk"].isEmpty())){
            QJsonArray partitionsList;
            if( !hardware["Win32_LogicalDisk"].isNull() ) {
                partitionsList = hardware["Win32_LogicalDisk"].toArray();
            }

            foreach(QJsonValue partitionValue, ioJson["children"].toArray() ) {
                QJsonObject partitionObject = partitionValue.toObject();
                QJsonObject newPartition;
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
                        newPartition["Children"] = extendedList;
                    }

                } else {
                    if ( !partitionObject["configuration"].toObject()["filesystem"].isNull()
                         && !(_exceptionsClasses.contains("Win32_LogicalDisk")
                             && _exceptionsClasses["Win32_LogicalDisk"].contains("Filesystem")))
                        newPartition["Filesystem"] = partitionObject["configuration"].toObject()["filesystem"];
                    if ( !partitionObject["configuration"].toObject()["created"].isNull()
                         && !(_exceptionsClasses.contains("Win32_LogicalDisk")
                             && _exceptionsClasses["Win32_LogicalDisk"].contains("Created")))
                        newPartition["Created"] = partitionObject["configuration"].toObject()["created"];
                    if ( !partitionObject["configuration"].toObject()["mount.options"].isNull()
                         && !(_exceptionsClasses.contains("Win32_LogicalDisk")
                             && _exceptionsClasses["Win32_LogicalDisk"].contains("MountOptions")))
                        newPartition["MountOptions"] = partitionObject["configuration"].toObject()["mount.options"];
                    if (!(_exceptionsClasses.contains("Win32_LogicalDisk")
                          && _exceptionsClasses["Win32_LogicalDisk"].contains("MediaType"))){
                        if ( !partitionObject["configuration"].toObject()["label"].isNull()){
                            newPartition["MediaType"] = partitionObject["configuration"].toObject()["label"];
                        } else {
                            newPartition["MediaType"] = ioJson["description"];
                        }
                    }
                }

                partitionsList.append(newPartition);
            }
            hardware["Win32_LogicalDisk"] = partitionsList;
        } else {
            qDebug() << "Coleta LogicalDisk desativada";
        }

    }

    if(!_exceptionsClasses.contains("Win32_DiskDrive")
        || !(_exceptionsClasses.contains("Win32_DiskDrive")
            && _exceptionsClasses["Win32_DiskDrive"].isEmpty())){
        physicalArray.append(dispositivo);
        hardware["Win32_DiskDrive"] = physicalArray;
    } else {
        qDebug() << "Coleta DiskDrive desativada." << _exceptionsClasses << "Empty? " << _exceptionsClasses["Win32_DiskDrive"].isEmpty();
    }
}

void cacic_hardware::coletaGenericPartitionInfo(QJsonObject &newPartition, const QJsonObject &partitionObject)
{
    newPartition["Description"] = partitionObject["description"];

    if( !partitionObject["size"].isNull() )
        newPartition["Size"] = QJsonValue::fromVariant(CCacic::convertDouble(partitionObject["size"].toDouble(),0));
    else
        newPartition["Size"] = QJsonValue::fromVariant(CCacic::convertDouble(partitionObject["capacity"].toDouble(),0)
                + " " + partitionObject["units"].toString());

    //    if ( !partitionObject["capabilities"].toObject()["primary"].isNull() )
    //        newPartition["primary"] = partitionObject["capabilities"].toObject()["primary"];
    //    if ( !partitionObject["capabilities"].toObject()["bootable"].isNull() )
    //        newPartition["bootable"] = partitionObject["capabilities"].toObject()["bootable"];
    //    if ( !partitionObject["capabilities"].toObject()["journaled"].isNull() )
    //        newPartition["journaled"] = partitionObject["capabilities"].toObject()["journaled"];
    if (partitionObject.contains("logicalname")) {
        if( partitionObject["logicalname"].isArray() ) {
            newPartition["Caption"] = partitionObject["logicalname"].toArray().first();
        } else {
            newPartition["Caption"] = partitionObject["logicalname"];
        }

        QStringList dfOutput = console(QString("df -h")).split("\n");
        foreach(QString dfLine, dfOutput){
            if(dfLine.split(" ").at(0) == newPartition["Caption"].toString() ) {
                QStringList splitLine = dfLine.split(" ");
                splitLine.removeAll(" ");
                newPartition["FreeSpace"] = QJsonValue::fromVariant(splitLine.at(3));
            }
        }
    }
}

void cacic_hardware::coletaLinuxBios(QJsonObject &hardware)
{
    if(!_exceptionsClasses.contains("Win32_BIOS")
        || !(_exceptionsClasses.contains("Win32_BIOS")
            && _exceptionsClasses["Win32_BIOS"].isEmpty())){
        QJsonObject bios;
        QStringList consoleOutput;

        consoleOutput = console("dmidecode -t bios").split("\n");
        foreach(QString line, consoleOutput){
            if(line.contains("Vendor:")
                    && !(_exceptionsClasses.contains("Win32_BIOS")
                        && _exceptionsClasses["Win32_BIOS"].contains("Manufacturer"))){
                bios["Manufacturer"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
            } else if(line.contains("Version:")
                      && !(_exceptionsClasses.contains("Win32_BIOS")
                          && _exceptionsClasses["Win32_BIOS"].contains("Version"))){
                bios["Version"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
            } else if(line.contains("Release Date:")
                      && !(_exceptionsClasses.contains("Win32_BIOS")
                          && _exceptionsClasses["Win32_BIOS"].contains("ReleaseDate"))){
                bios["ReleaseDate"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
            } else if(line.contains("Runtime Size:")
                      && !(_exceptionsClasses.contains("Win32_BIOS")
                          && _exceptionsClasses["Win32_BIOS"].contains("RuntimeSize"))){
                bios["RuntimeSize"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
            } else if(line.contains("ROM Size:")
                      && !(_exceptionsClasses.contains("Win32_BIOS")
                          && _exceptionsClasses["Win32_BIOS"].contains("RomSize"))){
                bios["RomSize"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
            } else if(line.contains("BIOS Revision:")
                      && !(_exceptionsClasses.contains("Win32_BIOS")
                          && _exceptionsClasses["Win32_BIOS"].contains("Revision"))){
                bios["Revision"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
            }
        }
        hardware["Win32_BIOS"] = bios;
    } else {
        qDebug () << "Coleta BIOS desativada.";
    }
}

void cacic_hardware::coletaLinuxMotherboard(QJsonObject &hardware)
{
    if(!_exceptionsClasses.contains("Win32_BaseBoard")
        || !(_exceptionsClasses.contains("Win32_BaseBoard")
            && _exceptionsClasses["Win32_BaseBoard"].isEmpty())){
        QJsonObject motherboard;
        QStringList consoleOutput;

        consoleOutput= console("dmidecode -t 2").split("\n");

        foreach(QString line, consoleOutput){
            if(line.contains("Manufacturer:")
                && !(_exceptionsClasses.contains("Win32_BaseBoard")
                    && _exceptionsClasses["Win32_BaseBoard"].contains("Manufacturer"))){
                motherboard["Manufacturer"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
            } else if(line.contains("Product Name:")
                      && !(_exceptionsClasses.contains("Win32_BaseBoard")
                          && _exceptionsClasses["Win32_BaseBoard"].contains("ProductName"))){
                motherboard["ProductName"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
            } else if(line.contains("Version:")
                      && !(_exceptionsClasses.contains("Win32_BaseBoard")
                          && _exceptionsClasses["Win32_BaseBoard"].contains("Version"))){
                motherboard["Version"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
            } else if(line.contains("Asset Tag:")
                      && !(_exceptionsClasses.contains("Win32_BaseBoard")
                          && _exceptionsClasses["Win32_BaseBoard"].contains("Tag"))){
                motherboard["Tag"] = QJsonValue::fromVariant( QString(line.split(":")[1].mid(1)) );
            } else if(line.contains("Serial Number:")
                      && !(_exceptionsClasses.contains("Win32_BaseBoard")
                          && _exceptionsClasses["Win32_BaseBoard"].contains("SerialNumber"))){
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

        if (!onboardCapabilities.isEmpty()
                && !(_exceptionsClasses.contains("Win32_BaseBoard")
                    && _exceptionsClasses["Win32_BaseBoard"].contains("onboardCapabilities")))
            motherboard["onboardCapabilities"] = QJsonValue::fromVariant(onboardCapabilities);

        hardware["Win32_BaseBoard"] = motherboard;
    } else {
        qDebug() << "Coleta BaseBoard desativada.";
    }
}

void cacic_hardware::coletaLinuxIsNotebook(QJsonObject &hardware)
{    
    if(!_exceptionsClasses.contains("IsNotebook")){
        QStringList consoleOutput;
        QJsonObject notebook;
        consoleOutput= console("dmidecode -t 3").split("\n");
        foreach(QString line, consoleOutput){
            if(line.contains("Type:") && (line.contains("Notebook") ||
                                          line.contains("Portable") ||
                                          line.contains("Laptop")   ||
                                          line.contains("Sub Notebook")) ) {
                notebook["Value"] = QJsonValue::fromVariant(true);
                hardware["IsNotebook"] = notebook;

                return;
            }
        }
        notebook["Value"] = QJsonValue::fromVariant(false);
        hardware["IsNotebook"] = notebook;
    } else {
        qDebug() << "Coleta IsNotebook desativada.";
    }
}

void cacic_hardware::coletaLinuxPrinters(QJsonObject &hardware)
{
    if(!_exceptionsClasses.contains("Win32_Printer")){
        QStringList consoleOutput;

        if( console("lpstat").contains("/bin/sh:") ) { // Cups não instalado
            logcacic->escrever(LogCacic::ErrorLevel, "lpstat não instalado para verificação de impressoras.");
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
                    QJsonObject printer;
                    printer["Name"] = QJsonValue::fromVariant(printerName);
                    printersList.append(printer);
                }
            }
            hardware["Win32_Printer"] = printersList;
        }
    } else {
        qDebug() << "Coleta de impressora desativada.";
    }

}

#endif
QJsonObject cacic_hardware::toJsonObject() {
    return coletaHardware;
}

void cacic_hardware::setExceptionClasses(const QHash<QString, QStringList> &value)
{
    this->_exceptionsClasses = value;
}
