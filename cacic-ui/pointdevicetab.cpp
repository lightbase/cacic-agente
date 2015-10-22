#include "pointdevicetab.h"
#include "ui_pointdevicetab.h"

PointDeviceTab::PointDeviceTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PointDeviceTab)
{
    ui->setupUi(this);
}

PointDeviceTab::PointDeviceTab(QJsonObject jsonObj, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PointDeviceTab)
{
    ui->setupUi(this);

    if(!jsonObj.isEmpty()) {
        if( jsonObj.contains("Caption") )
            ui->lineCaption->setText(jsonObj["Caption"].toString());
        if( jsonObj.contains("Description") )
            ui->lineDescription->setText(jsonObj["Description"].toString());
        if( jsonObj.contains("DeviceID") )
            ui->lineDeviceID->setText(jsonObj["DeviceID"].toString());
        if( jsonObj.contains("DeviceInterface") )
            ui->lineDeviceInterface->setText(jsonObj["DeviceInterface"].toString());
        if( jsonObj.contains("Manufacturer") )
            ui->lineManufacturer->setText(jsonObj["Manufacturer"].toString());
        if( jsonObj.contains("PointingType") )
            ui->linePointingType->setText(jsonObj["PointingType"].toString());
    }
}

PointDeviceTab::~PointDeviceTab()
{
    delete ui;
}
