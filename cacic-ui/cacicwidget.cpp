#include "cacicwidget.h"
#include "ui_widget.h"

CacicWidget::CacicWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    QString folder = CCacic::getValueFromRegistry("Lightbase", "Cacic", "mainFolder").toString();
    cacicMainFolder = !folder.isEmpty() && !folder.isNull() ? folder : Identificadores::ENDERECO_PATCH_CACIC;

    logcacic = new LogCacic(LOG_CACICUI, cacicMainFolder+"/Logs");

    windowOpen = false;

    cliente = new UiClient(cacicMainFolder,this);
    cacicSysTray = new CacicSysTray(cacicMainFolder,this);

    connect(cacicSysTray,&CacicSysTray::infosClicked,this,&CacicWidget::on_infosClicked,Qt::UniqueConnection);
    connect(cacicSysTray,&CacicSysTray::forcarClicked,cliente,&UiClient::on_forcarClicked,Qt::UniqueConnection);
    connect(cacicSysTray,&CacicSysTray::finalizarClicked,cliente,&UiClient::on_finalizarClicked,Qt::UniqueConnection);
    connect(cliente,&UiClient::finalizar,this,&CacicWidget::on_finalizar,Qt::QueuedConnection);

    cacicSysTray->iniciarSysTray();

    ui->setupUi(this);
}

CacicWidget::~CacicWidget()
{
    delete ui;
    delete cacicSysTray;
}

void CacicWidget::closeEvent(QCloseEvent *event)
{
        event->ignore();
        this->hide();
        windowOpen = false;
}

void CacicWidget::hardwareItemPressed(QListWidgetItem *item)
{
    QJsonObject hardware = CCacic::getJsonFromFile(cacicMainFolder + "/coleta.json")["hardware"].toObject();
    qDeleteAll(ui->hardwareInfoWidget->children());

    if(item->text() == "É Notebook?") {
        if(hardware.contains("IsNotebook") && hardware["IsNotebook"].isObject()) {
            QJsonObject isNotebook = hardware["IsNotebook"].toObject();
            QFormLayout *formLayout = new QFormLayout;

            QLineEdit *lineValue = new QLineEdit;
            if(isNotebook.contains("Value") && isNotebook["Value"].isBool()) {
                QString aux;
                isNotebook["Value"].toBool() ? aux = "verdadeiro" : aux = "falso";
                lineValue->setText(aux);
            }
            lineValue->setReadOnly(true);
            lineValue->setAlignment(Qt::AlignRight);
            formLayout->addRow("Valor",lineValue);

            ui->hardwareInfoWidget->setLayout(formLayout);
        } else {
            QLabel *newLabel = new QLabel("Não foi possível recuperar a informação da coleta.");
            QLayout *layout = new QVBoxLayout();
            layout->addWidget(newLabel);
            ui->hardwareInfoWidget->setLayout(layout);
        }
    } else if(item->text() == "Configuração do Adaptador de Rede") {
        if(hardware.contains("NetworkAdapterConfiguration") && hardware["NetworkAdapterConfiguration"].isArray()) {
            QJsonArray netAdapter = hardware["NetworkAdapterConfiguration"].toArray();

            QVBoxLayout *layout = new QVBoxLayout(this);
            QTabWidget *tabWidget = new QTabWidget(this);

            if( !netAdapter.isEmpty() ) {
                for(int i = 0; i < netAdapter.size(); i++) {
                    if(netAdapter.at(i).isObject() && !netAdapter.at(i).toObject().isEmpty()) {
                        QJsonObject netJsonObj = netAdapter.at(i).toObject();

                        NetAdapterConfigTab *newTab = new NetAdapterConfigTab(netJsonObj,tabWidget);

                        tabWidget->addTab(newTab,"Dispositivo" + QString::number(i+1));
                        tabWidget->setCurrentIndex(0);
                    }
                }
            }

            layout->addWidget(tabWidget);
            ui->hardwareInfoWidget->setLayout(layout);

        } else {
            QLabel *newLabel = new QLabel("Não foi possível recuperar a informação da coleta.");
            QLayout *layout = new QVBoxLayout();
            layout->addWidget(newLabel);
            ui->hardwareInfoWidget->setLayout(layout);
        }
    } else if(item->text() == "Sistema Operacional") {
        if(hardware.contains("OperatingSystem") && hardware["OperatingSystem"].isObject()) {
            QJsonObject operatingSystem = hardware["OperatingSystem"].toObject();
            QFormLayout *formLayout = new QFormLayout;

            QJsonObject::const_iterator it;
            for(it = operatingSystem.begin(); it != operatingSystem.end(); it++) {
                QLineEdit *lineEdit = new QLineEdit;
                if( it.value().isString() )
                    lineEdit->setText(it.value().toString());
                lineEdit->setReadOnly(true);
                lineEdit->setAlignment(Qt::AlignRight);

                if(it.key() == "CSDVersion")
                    formLayout->addRow("Versão CSF",lineEdit);
                else if(it.key() == "Caption")
                    formLayout->addRow("Subtítulo",lineEdit);
                else if(it.key() == "InstallDate")
                    formLayout->addRow("Data de Instalação",lineEdit);
                else if(it.key() == "Name")
                    formLayout->addRow("Nome",lineEdit);
                else if(it.key() == "OSArchitecture")
                    formLayout->addRow("Arquitetura",lineEdit);
                else if(it.key() == "RegisteredUser")
                    formLayout->addRow("Usuário registrado",lineEdit);
                else if(it.key() == "SerialNumber")
                    formLayout->addRow("Número de série",lineEdit);
                else if(it.key() == "Version")
                    formLayout->addRow("Versão",lineEdit);
            }

            ui->hardwareInfoWidget->setLayout(formLayout);

        } else {
            QLabel *newLabel = new QLabel("Não foi possível recuperar a informação da coleta.");
            QLayout *layout = new QVBoxLayout();
            layout->addWidget(newLabel);
            ui->hardwareInfoWidget->setLayout(layout);
        }
    } else if(item->text() == "BIOS") {
        if(hardware.contains("Win32_BIOS") && hardware["Win32_BIOS"].isObject()) {
            QJsonObject bios = hardware["Win32_BIOS"].toObject();
            QFormLayout *formLayout = new QFormLayout;

            QJsonObject::const_iterator it;
            for(it = bios.begin(); it != bios.end(); it++) {
                if( it.value().isString() ) {
                    QLineEdit *lineEdit = new QLineEdit;

                    lineEdit->setText(it.value().toString());
                    lineEdit->setReadOnly(true);
                    lineEdit->setAlignment(Qt::AlignRight);

                    if(it.key() == "Manufacturer")
                        formLayout->addRow("Fabricante",lineEdit);
                    else if(it.key() == "Name")
                        formLayout->addRow("Nome",lineEdit);
                    else if(it.key() == "ReleaseDate")
                        formLayout->addRow("Data de lançamento",lineEdit);
                    else if(it.key() == "SMBIOSBIOSVersion")
                        formLayout->addRow("Versão SMBIOS",lineEdit);
                    else if(it.key() == "SerialNumber")
                        formLayout->addRow("Número de série",lineEdit);
                    else if(it.key() == "SoftwareElementID")
                        formLayout->addRow("ID de Software",lineEdit);
                    else if(it.key() == "SoftwareElementState")
                        formLayout->addRow("Estado de Software",lineEdit);
                    else if(it.key() == "TargetOperatingSystem")
                        formLayout->addRow("Sistema Operacional Alvo",lineEdit);
                    else if(it.key() == "Version")
                        formLayout->addRow("Versão",lineEdit);
                } else if (it.value().isArray()) {
                    QJsonArray jsonArray = it.value().toArray();

                    QString aux;
                    for(int i = 0; i < jsonArray.size(); i++) {
                        if(jsonArray.at(i).isString()) {
                            aux.append(jsonArray.at(i).toString());
                            if(i < jsonArray.size() - 1)
                                aux.append(", ");
                        }
                    }

                    QLineEdit *lineEdit = new QLineEdit;

                    lineEdit->setText(aux);
                    lineEdit->setReadOnly(true);
                    lineEdit->setAlignment(Qt::AlignRight);

                    if(it.key() == "BIOSVersion")
                        formLayout->addRow("Versão BIOS",lineEdit);
                }
            }

            ui->hardwareInfoWidget->setLayout(formLayout);

        } else {
            QLabel *newLabel = new QLabel("Não foi possível recuperar a informação da coleta.");
            QLayout *layout = new QVBoxLayout();
            layout->addWidget(newLabel);
            ui->hardwareInfoWidget->setLayout(layout);
        }
    } else if(item->text() == "Placa base") {
        if(hardware.contains("Win32_BaseBoard") && hardware["Win32_BaseBoard"].isObject()) {
            QJsonObject baseBoard = hardware["Win32_BaseBoard"].toObject();
            QFormLayout *formLayout = new QFormLayout;

            QJsonObject::const_iterator it;
            for(it = baseBoard.begin(); it != baseBoard.end(); it++) {
                if( it.value().isString() ) {
                    QLineEdit *lineEdit = new QLineEdit;

                    lineEdit->setText(it.value().toString());
                    lineEdit->setReadOnly(true);
                    lineEdit->setAlignment(Qt::AlignRight);

                    if(it.key() == "Manufacturer")
                        formLayout->addRow("Fabricante",lineEdit);
                    else if(it.key() == "SerialNumber")
                        formLayout->addRow("Número de série",lineEdit);
                    else if(it.key() == "Tag")
                        formLayout->addRow("Tag",lineEdit);
                }
            }

            ui->hardwareInfoWidget->setLayout(formLayout);

        } else {
            QLabel *newLabel = new QLabel("Não foi possível recuperar a informação da coleta.");
            QLayout *layout = new QVBoxLayout();
            layout->addWidget(newLabel);
            ui->hardwareInfoWidget->setLayout(layout);
        }
    } else if(item->text() == "Drive CDROM") {
        if(hardware.contains("Win32_CDROMDrive") && hardware["Win32_CDROMDrive"].isArray()) {
            QJsonArray cdDrives = hardware["Win32_CDROMDrive"].toArray();

            QVBoxLayout *layout = new QVBoxLayout(this);
            QTabWidget *tabWidget = new QTabWidget(this);

            if( !cdDrives.isEmpty() ) {
                for(int i = 0; i < cdDrives.size(); i++) {
                    if(cdDrives.at(i).isObject() && !cdDrives.at(i).toObject().isEmpty()) {
                        QJsonObject jsonObj = cdDrives.at(i).toObject();

                        CdDriveTab *newTab = new CdDriveTab(jsonObj,tabWidget);

                        tabWidget->addTab(newTab,"Dispositivo" + QString::number(i+1));
                        tabWidget->setCurrentIndex(0);
                    }
                }
            }

            layout->addWidget(tabWidget);
            ui->hardwareInfoWidget->setLayout(layout);

        } else {
            QLabel *newLabel = new QLabel("Não foi possível recuperar a informação da coleta.");
            QLayout *layout = new QVBoxLayout();
            layout->addWidget(newLabel);
            ui->hardwareInfoWidget->setLayout(layout);
        }
    } else if(item->text() == "Sistema de Computador") {
        if(hardware.contains("Win32_ComputerSystem") && hardware["Win32_ComputerSystem"].isObject()) {
            QJsonObject computerSystem = hardware["Win32_ComputerSystem"].toObject();
            QFormLayout *formLayout = new QFormLayout;

            QJsonObject::const_iterator it;
            for(it = computerSystem.begin(); it != computerSystem.end(); it++) {
                if( it.value().isString() ) {
                    QLineEdit *lineEdit = new QLineEdit;

                    lineEdit->setText(it.value().toString());
                    lineEdit->setReadOnly(true);
                    lineEdit->setAlignment(Qt::AlignRight);

                    if(it.key() == "Caption")
                        formLayout->addRow("Subtítulo",lineEdit);
                    else if(it.key() == "Description")
                        formLayout->addRow("Descrição",lineEdit);
                    else if(it.key() == "Domain")
                        formLayout->addRow("Domínio",lineEdit);
                    else if(it.key() == "Manufacturer")
                        formLayout->addRow("Fabricante",lineEdit);
                    else if(it.key() == "Model")
                        formLayout->addRow("Modelo",lineEdit);
                    else if(it.key() == "Name")
                        formLayout->addRow("Nome",lineEdit);
                    else if(it.key() == "PrimaryOwnerName")
                        formLayout->addRow("Nome do Dono primário",lineEdit);
                    else if(it.key() == "TotalPhysicalMemory") {
                        double sizeBytes = it.value().toString().toDouble();

                        double sizeGb = (sizeBytes/pow(10.0,9.0));
                        double sizeMb = (sizeBytes/pow(10.0,6.0));
                        double sizeKb = (sizeBytes/pow(10.0,3.0));

                        if ( floor(sizeGb) != 0 )
                            lineEdit->setText(QString::number(sizeGb) + " GB");
                        else if ( floor(sizeMb) != 0 )
                            lineEdit->setText(QString::number(sizeMb) + " MB");
                        else if ( floor(sizeKb) != 0 )
                            lineEdit->setText(QString::number(sizeKb) + " KB");
                        else
                            lineEdit->setText(it.value().toString() + " B");

                        formLayout->addRow("Memória Física",lineEdit);
                    }
                    else if(it.key() == "Workgroup")
                        formLayout->addRow("Grupo de trabalho",lineEdit);
                }
            }

            ui->hardwareInfoWidget->setLayout(formLayout);
        } else {
            QLabel *newLabel = new QLabel("Não foi possível recuperar a informação da coleta.");
            QLayout *layout = new QVBoxLayout();
            layout->addWidget(newLabel);
            ui->hardwareInfoWidget->setLayout(layout);
        }
    } else if(item->text() == "Monitor Desktop") {
        if(hardware.contains("Win32_DesktopMonitor") && hardware["Win32_DesktopMonitor"].isObject()) {
            QJsonObject deskMonitor = hardware["Win32_DesktopMonitor"].toObject();
            QFormLayout *formLayout = new QFormLayout;

            QJsonObject::const_iterator it;
            for(it = deskMonitor.begin(); it != deskMonitor.end(); it++) {
                if( it.value().isString() ) {
                    QLineEdit *lineEdit = new QLineEdit;

                    lineEdit->setText(it.value().toString());
                    lineEdit->setReadOnly(true);
                    lineEdit->setAlignment(Qt::AlignRight);

                    if(it.key() == "Caption")
                        formLayout->addRow("Subtítulo",lineEdit);
                    else if(it.key() == "Description")
                        formLayout->addRow("Descrição",lineEdit);
                    else if(it.key() == "DeviceID")
                        formLayout->addRow("ID do dispositivo",lineEdit);
                    else if(it.key() == "MonitorType")
                        formLayout->addRow("Tipo de monitor",lineEdit);
                    else if(it.key() == "Name")
                        formLayout->addRow("Nome",lineEdit);
                }
            }

            ui->hardwareInfoWidget->setLayout(formLayout);
        } else {
            QLabel *newLabel = new QLabel("Não foi possível recuperar a informação da coleta.");
            QLayout *layout = new QVBoxLayout();
            layout->addWidget(newLabel);
            ui->hardwareInfoWidget->setLayout(layout);
        }
    } else if(item->text() == "Drive de disco") {
        if(hardware.contains("Win32_DiskDrive") && hardware["Win32_DiskDrive"].isObject()) {
            QJsonObject diskDrive = hardware["Win32_DiskDrive"].toObject();
            QFormLayout *formLayout = new QFormLayout;

            QJsonObject::const_iterator it;
            for(it = diskDrive.begin(); it != diskDrive.end(); it++) {
                if( it.value().isString() ) {
                    QLineEdit *lineEdit = new QLineEdit;

                    lineEdit->setText(it.value().toString());
                    lineEdit->setReadOnly(true);
                    lineEdit->setAlignment(Qt::AlignRight);

                    if(it.key() == "Caption")
                        formLayout->addRow("Subtítulo",lineEdit);
                    else if(it.key() == "Description")
                        formLayout->addRow("Descrição",lineEdit);
                    else if(it.key() == "DeviceID")
                        formLayout->addRow("ID do dispositivo",lineEdit);
                    else if(it.key() == "Manufacturer")
                        formLayout->addRow("Fabricante",lineEdit);
                    else if(it.key() == "MediaType")
                        formLayout->addRow("Tipo de Mídia",lineEdit);
                    else if(it.key() == "Model")
                        formLayout->addRow("Modelo",lineEdit);
                    else if(it.key() == "Name")
                        formLayout->addRow("Nome",lineEdit);
                    else if(it.key() == "Size") {
                        double sizeBytes = it.value().toString().toDouble();

                        double sizeGb = (sizeBytes/pow(10.0,9.0));
                        double sizeMb = (sizeBytes/pow(10.0,6.0));
                        double sizeKb = (sizeBytes/pow(10.0,3.0));

                        if ( floor(sizeGb) != 0 )
                            lineEdit->setText(QString::number(sizeGb) + " GB");
                        else if ( floor(sizeMb) != 0 )
                            lineEdit->setText(QString::number(sizeMb) + " MB");
                        else if ( floor(sizeKb) != 0 )
                            lineEdit->setText(QString::number(sizeKb) + " KB");
                        else
                            lineEdit->setText(it.value().toString() + " B");

                        formLayout->addRow("Tamanho",lineEdit);
                    }
                }
            }

            ui->hardwareInfoWidget->setLayout(formLayout);
        } else {
            QLabel *newLabel = new QLabel("Não foi possível recuperar a informação da coleta.");
            QLayout *layout = new QVBoxLayout();
            layout->addWidget(newLabel);
            ui->hardwareInfoWidget->setLayout(layout);
        }
    } else if(item->text() == "Teclado") {
        if(hardware.contains("Win32_Keyboard") && hardware["Win32_Keyboard"].isObject()) {
            QJsonObject keyboard = hardware["Win32_Keyboard"].toObject();
            QFormLayout *formLayout = new QFormLayout;

            QJsonObject::const_iterator it;
            for(it = keyboard.begin(); it != keyboard.end(); it++) {
                if( it.value().isString() ) {
                    QLineEdit *lineEdit = new QLineEdit;

                    lineEdit->setText(it.value().toString());
                    lineEdit->setReadOnly(true);
                    lineEdit->setAlignment(Qt::AlignRight);

                    if(it.key() == "Caption")
                        formLayout->addRow("Subtítulo",lineEdit);
                    else if(it.key() == "Description")
                        formLayout->addRow("Descrição",lineEdit);
                    else if(it.key() == "DeviceID")
                        formLayout->addRow("ID do dispositivo",lineEdit);
                    else if(it.key() == "Layout")
                        formLayout->addRow("Layout",lineEdit);
                    else if(it.key() == "Name")
                        formLayout->addRow("Nome",lineEdit);
                }
            }

            ui->hardwareInfoWidget->setLayout(formLayout);
        } else {
            QLabel *newLabel = new QLabel("Não foi possível recuperar a informação da coleta.");
            QLayout *layout = new QVBoxLayout();
            layout->addWidget(newLabel);
            ui->hardwareInfoWidget->setLayout(layout);
        }
    } else if(item->text() == "Discos lógicos") {
        if(hardware.contains("Win32_LogicalDisk") && hardware["Win32_LogicalDisk"].isArray()) {
            QJsonArray logicalDisks = hardware["Win32_LogicalDisk"].toArray();

            QVBoxLayout *layout = new QVBoxLayout(this);
            QTabWidget *tabWidget = new QTabWidget(this);

            if( !logicalDisks.isEmpty() ) {
                for(int i = 0; i < logicalDisks.size(); i++) {
                    if(logicalDisks.at(i).isObject() && !logicalDisks.at(i).toObject().isEmpty()) {
                        QJsonObject jsonObj = logicalDisks.at(i).toObject();

                        LogicalDiskTab *newTab = new LogicalDiskTab(jsonObj,tabWidget);

                        tabWidget->addTab(newTab,"Dispositivo" + QString::number(i+1));
                        tabWidget->setCurrentIndex(0);
                    }
                }
            }

            layout->addWidget(tabWidget);
            ui->hardwareInfoWidget->setLayout(layout);
        } else {
            QLabel *newLabel = new QLabel("Não foi possível recuperar a informação da coleta.");
            QLayout *layout = new QVBoxLayout();
            layout->addWidget(newLabel);
            ui->hardwareInfoWidget->setLayout(layout);
        }
    } else if(item->text() == "Mouse") {
        if(hardware.contains("Win32_PointingDevice") && hardware["Win32_PointingDevice"].isArray()) {
            QJsonArray pointingDevices = hardware["Win32_PointingDevice"].toArray();

            QVBoxLayout *layout = new QVBoxLayout(this);
            QTabWidget *tabWidget = new QTabWidget(this);

            if( !pointingDevices.isEmpty() ) {
                for(int i = 0; i < pointingDevices.size(); i++) {
                    if(pointingDevices.at(i).isObject() && !pointingDevices.at(i).toObject().isEmpty()) {
                        QJsonObject jsonObj = pointingDevices.at(i).toObject();

                        PointDeviceTab *newTab = new PointDeviceTab(jsonObj,tabWidget);

                        tabWidget->addTab(newTab,"Dispositivo" + QString::number(i+1));
                        tabWidget->setCurrentIndex(0);
                    }
                }
            }

            layout->addWidget(tabWidget);
            ui->hardwareInfoWidget->setLayout(layout);
        } else {
            QLabel *newLabel = new QLabel("Não foi possível recuperar a informação da coleta.");
            QLayout *layout = new QVBoxLayout();
            layout->addWidget(newLabel);
            ui->hardwareInfoWidget->setLayout(layout);
        }
    } else if(item->text() == "Impressora") {
        if(hardware.contains("Win32_Printer") && hardware["Win32_Printer"].isArray()) {
            QJsonArray printers = hardware["Win32_Printer"].toArray();

            QVBoxLayout *layout = new QVBoxLayout(this);
            QTabWidget *tabWidget = new QTabWidget(this);

            if( !printers.isEmpty() ) {
                for(int i = 0; i < printers.size(); i++) {
                    if(printers.at(i).isObject() && !printers.at(i).toObject().isEmpty()) {
                        QJsonObject jsonObj = printers.at(i).toObject();

                        PrinterTab *newTab = new PrinterTab(jsonObj,tabWidget);

                        tabWidget->addTab(newTab,"Dispositivo" + QString::number(i+1));
                        tabWidget->setCurrentIndex(0);
                    }
                }
            }

            layout->addWidget(tabWidget);
            ui->hardwareInfoWidget->setLayout(layout);
        } else {
            QLabel *newLabel = new QLabel("Não foi possível recuperar a informação da coleta.");
            QLayout *layout = new QVBoxLayout();
            layout->addWidget(newLabel);
            ui->hardwareInfoWidget->setLayout(layout);
        }
    } else if(item->text() == "Processador") {
        if(hardware.contains("Win32_Processor") && hardware["Win32_Processor"].isObject()) {
            QJsonObject processor = hardware["Win32_Processor"].toObject();
            QFormLayout *formLayout = new QFormLayout;

            QJsonObject::const_iterator it;
            for(it = processor.begin(); it != processor.end(); it++) {
                if( it.value().isString() ) {
                    QLineEdit *lineEdit = new QLineEdit;

                    lineEdit->setText(it.value().toString());
                    lineEdit->setReadOnly(true);
                    lineEdit->setAlignment(Qt::AlignRight);

                    if(it.key() == "AddressWidth")
                        formLayout->addRow("Largura de Endereço",lineEdit);
                    else if(it.key() == "Architecture")
                        formLayout->addRow("Arquitetura",lineEdit);
                    else if(it.key() == "Caption")
                        formLayout->addRow("Título",lineEdit);
                    else if(it.key() == "CpuStatus")
                        formLayout->addRow("Status do CPU",lineEdit);
                    else if(it.key() == "CurrentClockSpeed")
                        formLayout->addRow("Velocidade de Clock atual",lineEdit);
                    else if(it.key() == "DataWidth")
                        formLayout->addRow("Largura de Dados",lineEdit);
                    else if(it.key() == "DeviceID")
                        formLayout->addRow("ID do dispositivo",lineEdit);
                    else if(it.key() == "Family")
                        formLayout->addRow("Família",lineEdit);
                    else if(it.key() == "Level")
                        formLayout->addRow("Level",lineEdit);
                    else if(it.key() == "Manufacturer")
                        formLayout->addRow("Fabricante",lineEdit);
                    else if(it.key() == "MaxClockSpeed")
                        formLayout->addRow("Velocidade de Clock máxima",lineEdit);
                    else if(it.key() == "Name")
                        formLayout->addRow("Nome",lineEdit);
                    else if(it.key() == "NumberOfCores")
                        formLayout->addRow("Número de núcleos",lineEdit);
                    else if(it.key() == "NumberOfCores")
                        formLayout->addRow("Número de processadores lógicos",lineEdit);
                }
            }

            ui->hardwareInfoWidget->setLayout(formLayout);
        } else {
            QLabel *newLabel = new QLabel("Não foi possível recuperar a informação da coleta.");
            QLayout *layout = new QVBoxLayout();
            layout->addWidget(newLabel);
            ui->hardwareInfoWidget->setLayout(layout);
        }
    } else if(item->text() == "Controladora SCSI") {
        if(hardware.contains("Win32_SCSIController") && hardware["Win32_SCSIController"].isObject()) {
            QJsonObject controladora = hardware["Win32_SCSIController"].toObject();
            QFormLayout *formLayout = new QFormLayout;

            QJsonObject::const_iterator it;
            for(it = controladora.begin(); it != controladora.end(); it++) {
                if( it.value().isString() ) {
                    QLineEdit *lineEdit = new QLineEdit;

                    lineEdit->setText(it.value().toString());
                    lineEdit->setReadOnly(true);
                    lineEdit->setAlignment(Qt::AlignRight);

                    if(it.key() == "Caption")
                        formLayout->addRow("Subtítulo",lineEdit);
                    else if(it.key() == "Description")
                        formLayout->addRow("Descrição",lineEdit);
                    else if(it.key() == "DeviceID")
                        formLayout->addRow("ID do dispositivo",lineEdit);
                    else if(it.key() == "Manufacturer")
                        formLayout->addRow("Fabricante",lineEdit);
                    else if(it.key() == "Name")
                        formLayout->addRow("Nome",lineEdit);
                }
            }

            ui->hardwareInfoWidget->setLayout(formLayout);
        } else {
            QLabel *newLabel = new QLabel("Não foi possível recuperar a informação da coleta.");
            QLayout *layout = new QVBoxLayout();
            layout->addWidget(newLabel);
            ui->hardwareInfoWidget->setLayout(layout);
        }
    } else if(item->text() == "Gabinete do sistema") {
        if(hardware.contains("Win32_SystemEnclosure") && hardware["Win32_SystemEnclosure"].isObject()) {
            QJsonObject systemEnclosure = hardware["Win32_SystemEnclosure"].toObject();
            QFormLayout *formLayout = new QFormLayout;

            QJsonObject::const_iterator it;
            for(it = systemEnclosure.begin(); it != systemEnclosure.end(); it++) {
                if( it.value().isString() ) {
                    QLineEdit *lineEdit = new QLineEdit;

                    lineEdit->setText(it.value().toString());
                    lineEdit->setReadOnly(true);
                    lineEdit->setAlignment(Qt::AlignRight);

                    if(it.key() == "Manufacturer")
                        formLayout->addRow("Fabricante",lineEdit);
                    else if(it.key() == "Tag")
                        formLayout->addRow("Tag",lineEdit);

                } else if (it.value().isArray()) {
                    QJsonArray jsonArray = it.value().toArray();

                    QString aux;
                    for(int i = 0; i < jsonArray.size(); i++) {
                        if(jsonArray.at(i).isString()) {
                            aux.append(jsonArray.at(i).toString());
                            if(i < jsonArray.size() - 1)
                                aux.append(", ");
                        }
                    }

                    QLineEdit *lineEdit = new QLineEdit;

                    lineEdit->setText(aux);
                    lineEdit->setReadOnly(true);
                    lineEdit->setAlignment(Qt::AlignRight);

                    if(it.key() == "ChassisTypes")
                        formLayout->addRow("Tipos de Chassis",lineEdit);
                }
            }

            ui->hardwareInfoWidget->setLayout(formLayout);
        } else {
            QLabel *newLabel = new QLabel("Não foi possível recuperar a informação da coleta.");
            QLayout *layout = new QVBoxLayout();
            layout->addWidget(newLabel);
            ui->hardwareInfoWidget->setLayout(layout);
        }
    } else if(item->text() == "Controladora USB") {
        if(hardware.contains("Win32_USBController") && hardware["Win32_USBController"].isObject()) {
            QJsonObject usbController = hardware["Win32_USBController"].toObject();
            QFormLayout *formLayout = new QFormLayout;

            QJsonObject::const_iterator it;
            for(it = usbController.begin(); it != usbController.end(); it++) {
                if( it.value().isString() ) {
                    QLineEdit *lineEdit = new QLineEdit;

                    lineEdit->setText(it.value().toString());
                    lineEdit->setReadOnly(true);
                    lineEdit->setAlignment(Qt::AlignRight);

                    if(it.key() == "Caption")
                        formLayout->addRow("Subtítulo",lineEdit);
                    else if(it.key() == "Description")
                        formLayout->addRow("Descrição",lineEdit);
                    else if(it.key() == "DeviceID")
                        formLayout->addRow("ID do dispositivo",lineEdit);
                    else if(it.key() == "Manufacturer")
                        formLayout->addRow("Fabricante",lineEdit);
                    else if(it.key() == "Name")
                        formLayout->addRow("Nome",lineEdit);
                }
            }

            ui->hardwareInfoWidget->setLayout(formLayout);
        } else {
            QLabel *newLabel = new QLabel("Não foi possível recuperar a informação da coleta.");
            QLayout *layout = new QVBoxLayout();
            layout->addWidget(newLabel);
            ui->hardwareInfoWidget->setLayout(layout);
        }
    } else if(item->text() == "Controladora de vídeo") {
        if(hardware.contains("Win32_VideoController") && hardware["Win32_VideoController"].isObject()) {
            QJsonObject videoController = hardware["Win32_VideoController"].toObject();
            QFormLayout *formLayout = new QFormLayout;

            QJsonObject::const_iterator it;
            for(it = videoController.begin(); it != videoController.end(); it++) {
                if( it.value().isString() ) {
                    QLineEdit *lineEdit = new QLineEdit;

                    lineEdit->setText(it.value().toString());
                    lineEdit->setReadOnly(true);
                    lineEdit->setAlignment(Qt::AlignRight);

                    if(it.key() == "Caption")
                        formLayout->addRow("Subtítulo",lineEdit);
                    else if(it.key() == "Description")
                        formLayout->addRow("Descrição",lineEdit);
                    else if(it.key() == "DeviceID")
                        formLayout->addRow("ID do dispositivo",lineEdit);
                    else if(it.key() == "Name")
                        formLayout->addRow("Nome",lineEdit);
                }
            }

            ui->hardwareInfoWidget->setLayout(formLayout);
        } else {
            QLabel *newLabel = new QLabel("Não foi possível recuperar a informação da coleta.");
            QLayout *layout = new QVBoxLayout();
            layout->addWidget(newLabel);
            ui->hardwareInfoWidget->setLayout(layout);
        }
    }
}

void CacicWidget::on_finalizar()
{
    exit(0);
}

void CacicWidget::on_infosClicked()
{
    if(!windowOpen) {
        windowOpen = true;
        logcacic->escrever(LogCacic::InfoLevel,"SetupWidget here");

        QJsonObject coleta = CCacic::getJsonFromFile(cacicMainFolder + "/coleta.json");

        if(!coleta.isEmpty()) {
            setupTabGeral(coleta);
            setupTabHardware(coleta);
            setupTabSoftware(coleta);
        }

        this->show();
    }
}

void CacicWidget::setupTabGeral(const QJsonObject &coleta)
{
    if( coleta.contains("computador") && coleta["computador"].isObject() ) {
        QJsonObject computador = coleta["computador"].toObject();

        if( computador.contains("nmComputador") )
            ui->lineNomeComputador->setText(computador["nmComputador"].toString());
        if( computador.contains("usuario") )
            ui->lineUsuario->setText(computador["usuario"].toString());
        if( computador.contains("versaoAgente") )
            ui->lineVersaoAgente->setText(computador["versaoAgente"].toString());
        if( computador.contains("versaoGercols") )
            ui->lineVersaoGercols->setText(computador["versaoGercols"].toString());

        if( computador.contains("operatingSystem") && computador["operatingSystem"].isObject() ) {
            QJsonObject osJson = computador["operatingSystem"].toObject();

            if( osJson.contains("idOs") )
                ui->lineIdOs->setText(QString::number(osJson["idOs"].toInt()));
            if( osJson.contains("nomeOs") )
                ui->lineNomeOs->setText(osJson["nomeOs"].toString());
            if( osJson.contains("tipo") )
                ui->lineTipoOs->setText(osJson["tipo"].toString());
            if( osJson.contains("upTime") )
                ui->lineUptime->setText(QString::number(osJson["upTime"].toInt()));
        }

        if( computador.contains("networkDevices") && computador["networkDevices"].isArray() ) {
            while(ui->tabNetDevs->currentIndex() != -1) {
                QWidget *currentTab = ui->tabNetDevs->currentWidget();
                ui->tabNetDevs->removeTab(ui->tabNetDevs->currentIndex());
                delete currentTab;
            }

            if( !computador["networkDevices"].toArray().isEmpty() ) {
                QJsonArray networkDevices = computador["networkDevices"].toArray();

                for(int i = 0; i < networkDevices.size(); i++) {
                    if(networkDevices.at(i).isObject() && !networkDevices.at(i).toObject().isEmpty()) {
                        QJsonObject netDev = networkDevices.at(i).toObject();

                        NetDevTab *newNetTab = new NetDevTab(netDev,ui->tabNetDevs);

                        ui->tabNetDevs->addTab(newNetTab,"Dispositivo" + QString::number(i+1));
                        ui->tabNetDevs->setCurrentIndex(0);
                    }
                }
            }
        }
    }
}

void CacicWidget::setupTabHardware(const QJsonObject &coleta)
{
    ui->hardwarePropertiesList->sortItems();
    if( coleta.contains("hardware") && coleta["hardware"].isObject() ) {
        QJsonObject hardware = coleta["hardware"].toObject();

        connect(ui->hardwarePropertiesList,&QListWidget::itemPressed,this,&CacicWidget::hardwareItemPressed,Qt::UniqueConnection);

    } else {
        QLabel *newLabel = new QLabel("Não foi possível recuperar as informações de Hardware.");
        QLayout *layout = new QVBoxLayout();
        layout->addWidget(newLabel);

        ui->tabHardware->setLayout(layout);
    }

}

void CacicWidget::setupTabSoftware(const QJsonObject &coleta)
{
    if( coleta.contains("software") && coleta["software"].isObject() ) {
        QJsonObject software = coleta["software"].toObject();

        QJsonObject::const_iterator it;
        for(it = software.begin(); it != software.end(); it++) {
            if( it.value().isObject() && !it.value().toObject().isEmpty() ) {
                ui->softwareList->addItem(it.key());
            }
        }

        connect(ui->softwareList,&QListWidget::itemPressed,this,&CacicWidget::softwareItemPressed,Qt::UniqueConnection);

    } else {
        QLabel *newLabel = new QLabel("Não foi possível recuperar as informações de Hardware.");
        QLayout *layout = new QVBoxLayout();
        layout->addWidget(newLabel);

        ui->tabHardware->setLayout(layout);
    }
}

void CacicWidget::softwareItemPressed(QListWidgetItem *item)
{
    QJsonObject software = CCacic::getJsonFromFile(cacicMainFolder + "/coleta.json")["software"].toObject();
    qDeleteAll(ui->softwareInfoWidget->children());

    if(software.contains(item->text()) && software[item->text()].isObject()) {
        QJsonObject softObj = software[item->text()].toObject();
        QFormLayout *formLayout = new QFormLayout;

        QJsonObject::const_iterator it;
        for(it = softObj.begin(); it != softObj.end(); it++) {
            if( it.value().isString() ) {
                QLineEdit *lineValue = new QLineEdit;
                lineValue->setText(it.value().toString());
                lineValue->setReadOnly(true);
                lineValue->setAlignment(Qt::AlignRight);

                if( it.key() == "description")
                    formLayout->addRow("Descrição",lineValue);
                else if( it.key() == "installDate")
                    formLayout->addRow("Data de instalação",lineValue);
                else if( it.key() == "installLocation")
                    formLayout->addRow("Local de instalação",lineValue);
                else if( it.key() == "name")
                    formLayout->addRow("Nome",lineValue);
                else if( it.key() == "publisher")
                    formLayout->addRow("Empresa",lineValue);
                else if( it.key() == "quietUninstallString")
                    formLayout->addRow("String de desinstalação silenciosa",lineValue);
                else if( it.key() == "uninstallString")
                    formLayout->addRow("tring de desinstalação",lineValue);
                else if( it.key() == "url")
                    formLayout->addRow("URL",lineValue);
                else if( it.key() == "version")
                    formLayout->addRow("Versão",lineValue);
            }
        }

        ui->softwareInfoWidget->setLayout(formLayout);
    } else {
        QLabel *newLabel = new QLabel("Não foi possível recuperar a informação do software.");
        QLayout *layout = new QVBoxLayout();
        layout->addWidget(newLabel);
        ui->softwareInfoWidget->setLayout(layout);
    }
}
