#include "hardwareclasses.h"

HardwareClasses::HardwareClasses()
{
    this->_initialized = false;
}

HardwareClasses::HardwareClasses(const QHash<QString, QStringList> &exceptions)
{
    this->_exceptionClasses = exceptions;
}

bool HardwareClasses::init()
{
    QStringList params;
    // Win32_ComputerSystem (Caption, Description, Domain, Manufacturer,
    //                       Model, Name, PrimaryOwnerName, TotalPhysicalMemory, Workgroup)
    params << "Caption" << "Description" << "Domain" << "Manufacturer" << "Model"
           << "Name" << "PrimaryOwnerName" << "TotalPhysicalMemory" << "Workgroup";
    this->addClass("Win32_ComputerSystem", params);

    //Win32_PortableBattery
    //  (Verifica se é notebook)
    params.clear();
    params << "Caption" << "Description" << "DeviceID" << "EstimatedChargeRemaining" << "ExpectedLife"
           << "FullChargeCapacity" << "Name" << "Status" << "Location";
    this->addClass("Win32_PortableBattery", params);

    //Win32_Bios
    //  (Manufacturer, SMBIOSBIOSVersion, BIOSVersion, Version, SerialNumber, ReleaseDate)
    params.clear();
    params << "Manufacturer" << "SMBIOSBIOSVersion" << "BIOSVersion" << "Version" << "SerialNumber" << "ReleaseDate";
    this->addClass("Win32_Bios", params);

    //Win32_BaseBoard
    //  (Manufacturer, Model, SerialNumber)
    params.clear();
    params << "Manufacturer" << "Model" << "SerialNumber";
    this->addClass("Win32_Baseboard", params);

    //Win32_SystemEnclosure
    //  (Manufacturer, ChassisTypes, SerialNumber, SMBIOSAssetTag)
    params.clear();
    params << "Manufacturer" << "ChassisTypes" << "SerialNumber" << "SMBIOSAssetTag";
    this->addClass("Win32_SystemEnclosure", params);

    //Win32_FloppyDrive
    //  (Manufacturer, Caption, Description, Name, MediaType, Size)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "Name";
    this->addClass("Win32_FloppyDrive", params);

    //Win32_DiskDrive
    //  (Manufacturer, Caption, Description, MediaType, Name, Size, Model)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "Name" << "MediaType" << "Size" << "Model";
    this->addClass("Win32_DiskDrive", params);

    //Win32_CDROMDrive
    //  (Manufacturer, Caption, Description, Name, MediaType, Size)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "MediaType" << "Name" << "Size";
    this->addClass("Win32_CDROMDrive", params);

    //Win32_FloppyController
    //  (Manufacturer, Caption, Description, Name)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "Name";
    this->addClass("Win32_FloppyController", params);

    //Win32_MemoryDevice
    //  (Caption, Description, Name, BlockSize, DeviceID, NumberOfBlocks, Status)
    params.clear();
    params << "Caption" << "Description" << "Name" << "BlockSize" << "DeviceID" << "NumberOfBlocks" << "Status";
    this->addClass("Win32_MemoryDevice", params);

    //Win32_SMBIOSMemory
    //  (BlockSize, Caption, Description, DeviceID, Name, NumberOfBlocks, Status)
    params.clear();
    params << "Caption" << "Description" << "Name" << "BlockSize" << "DeviceID" << "NumberOfBlocks" << "Status";
    this->addClass("Win32_SMBIOSMemory", params);

    //Win32_SCSIController
    //  (Manufacturer, Caption, Description, Name, HardwareVersion)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "Name" << "HardwareVersion";
    this->addClass("Win32_SCSIController", params);

    //Win32_InfraredDevice
    //  (Manufacturer, Caption, Description, Name)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "Name";
    this->addClass("Win32_InfraredDevice", params);

    //Win32_USBController
    //  (Manufacturer, Caption, Description, Name)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "Name";
    this->addClass("Win32_USBController", params);

    //Win32_PCMCIAController
    //  (Manufacturer, Caption, Description, Name)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "Name";
    this->addClass("Win32_PCMCIAController", params);

    //Win32_VideoController
    //  (Description, VideoProcessor, AdapterRAM, CurrentHorizontalResolution, CurrentVerticalResolution, Caption, Name)
    params.clear();
    params << "VideoProcessor" << "AdapterRAM" << "Description" << "Name" << "CurrentHorizontalResolution"
           << "CurrentVerticalResolution" << "Caption" << "AcceleratorCapabilities";
    this->addClass("Win32_VideoController", params);

    //Win32_DesktopMonitor
    //  (MonitorManufacturer, Caption, Description, MonitorType, Name)
    params.clear();
    params << "MonitorManufacturer" << "Caption" << "Description" << "MonitorType" << "Name";
    this->addClass("Win32_DesktopMonitor", params);

    //Win32_Printer
    //  (Name, DriverName, PortName, ServerName, ShareName, HorizontalResolution, VerticalResolution, Comment, Shared, Network)
    params.clear();
    params << "Name" << "DriverName" << "PortName" << "ServerName" << "ShareName" << "HorizontalResolution"
           << "VerticalResolution" << "Comment" << "Shared" << "Network";
    this->addClass("Win32_Printer", params);

    //Win32_PortConnector
    //  (ExternalReferenceDesignator, PortType (verificar), ConnectorType (verificar), Manufacturer, Caption, Name)
    params.clear();
    params << "ExternalReferenceDesignator" << "PortType" << "ConnectorType" << "Name" << "Caption" << "Manufacturer";
    this->addClass("Win32_PortConnector", params);

    //Win32_SerialPort
    //  (Name, Caption, Description, StatusInfo)
    params.clear();
    params << "Name" << "Caption" << "Description" << "StatusInfo";
    this->addClass("Win32_SerialPort", params);

    //Win32_Processor
    //  (MaxClockSpeed, Name, Architecture, NumberOfCores, SocketDesignation, Manufacturer, Architecture, NumberOfCores
    //  CurrentClockSpeed, MaxClockSpeed, L2CacheSize, AddressWidth, DataWidth, VoltageCaps, CpuStatus,
    //  ProcessorId || UniqueId, AddressWidth)
    params.clear();
    params << "MaxClockSpeed" << "Name" << "Architecture" << "NumberOfCores" << "SocketDesignation" << "Manufacturer"
           << "Architecture" << "NumberOfCores" << "NumberOfLogicalProcessors" << "CurrentClockSpeed" << "MaxClockSpeed" << "L2CacheSize" << "AddressWidth"
           << "DataWidth" << "VoltageCaps" << "CpuStatus" << "ProcessorId" << "UniqueId" << "AddressWidth" << "Caption" << "Family" << "Level";
    this->addClass("Win32_Processor", params);

    //Win32_OperatingSystem
    //  (Name, Version, CSDVersion, Description, InstallDate, Organization, RegisteredUser, SerialNumber)
    params.clear();
    params << "Name" << "Version" << "CSDVersion" << "Description" << "InstallDate" << "Organization" << "RegisteredUser"
           << "SerialNumber" << "Caption";
    this->addClass("Win32_OperatingSystem", params);

    //Win32_SystemSlot
    //  (Name, Description, SlotDesignation, CurrentUsage, Status, Shared)
    params.clear();
    params << "Name" << "Description" << "SlotDesignation" << "CurrentUsage" << "Status" << "Shared";
    this->addClass("Win32_SystemSlot", params);

    //Win32_LogicalDisk
    //  (Caption, DriveType, Filesystem, VolumeName, ProviderName, Filesystem, VolumeName, Size, FreeSpace)
    params.clear();
    params << "Caption" << "DriveType" << "Filesystem" << "VolumeName" << "ProviderName" << "Filesystem" << "VolumeName"
           << "Size" << "FreeSpace" << "MediaType";
    this->addClass("Win32_LogicalDisk", params);

    //Win32_PhysicalMemory
    //  (Caption, Description, BankLabel, DeviceLocator, Capacity, Speed, MemoryType, SerialNumber)
    params.clear();
    params << "Caption" << "Description" << "BankLabel" << "DeviceLocator" << "Capacity" << "Speed" << "MemoryType";
    this->addClass("Win32_PhysicalMemory", params);

    //Win32_Keyboard
    //  (Caption, Description, Name)
    params.clear();
    params << "Caption" << "Description" << "Name" << "Layout";
    this->addClass("Win32_Keyboard", params);

    //Win32_PointingDevice
    //  (Manufacturer, Caption, Description, PointingType, DeviceInterface)
    params.clear();
    params << "Manufacturer" << "Caption" << "Description" << "PointingType" << "DeviceInterface";
    this->addClass("Win32_PointingDevice", params);

    // Remove all exception classes from classes hash
    foreach(QString exptClass, this->_exceptionClasses.keys()){
        if(this->_exceptionClasses[exptClass].isEmpty()){
            this->_classes.remove(exptClass);
        } else {
            foreach(QString attr, this->_classes[exptClass]){
                this->_classes[exptClass].removeOne(attr);
            }
        }
    }

    this->_initialized = true;
    return true;
}

void HardwareClasses::addClass(const QString &name, const QStringList &atr)
{
    this->_classes[name] = atr;
}

QHash<QString, QStringList> HardwareClasses::classes() const
{
    return this->_classes;
}

