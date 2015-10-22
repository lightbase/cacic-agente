#include "printertab.h"
#include "ui_printertab.h"

PrinterTab::PrinterTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrinterTab)
{
    ui->setupUi(this);
}

PrinterTab::PrinterTab(QJsonObject jsonObj, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrinterTab)
{
    ui->setupUi(this);

    if(!jsonObj.isEmpty()) {
        if( jsonObj.contains("DeviceID") )
            ui->lineDeviceID->setText(jsonObj["DeviceID"].toString());
        if( jsonObj.contains("DriverName") )
            ui->lineDriverName->setText(jsonObj["DriverName"].toString());
        if( jsonObj.contains("HorizontalResolution") )
            ui->lineHorizontalResolution->setText(jsonObj["HorizontalResolution"].toString());
        if( jsonObj.contains("Name") )
            ui->lineName->setText(jsonObj["Name"].toString());
        if( jsonObj.contains("Network") )
            ui->lineNetwork->setText(jsonObj["Network"].toString());
        if( jsonObj.contains("PortName") )
            ui->linePortName->setText(jsonObj["PortName"].toString());
        if( jsonObj.contains("Shared") )
            ui->lineShared->setText(jsonObj["Shared"].toString());
        if( jsonObj.contains("VerticalResolution") )
            ui->lineVerticalResolution->setText(jsonObj["VerticalResolution"].toString());
    }
}

PrinterTab::~PrinterTab()
{
    delete ui;
}
