#include "cddrivetab.h"
#include "ui_cddrivetab.h"

CdDriveTab::CdDriveTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CdDriveTab)
{
    ui->setupUi(this);
}

CdDriveTab::CdDriveTab(QJsonObject jsonObj, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CdDriveTab)
{
    ui->setupUi(this);

    if(!jsonObj.isEmpty()) {
        if( jsonObj.contains("Caption") )
            ui->lineCaption->setText(jsonObj["Caption"].toString());
        if( jsonObj.contains("Description") )
            ui->lineDescription->setText(jsonObj["Description"].toString());
        if( jsonObj.contains("DeviceID") )
            ui->lineDeviceID->setText(jsonObj["DeviceID"].toString());
        if( jsonObj.contains("Manufacturer") )
            ui->lineManufacturer->setText(jsonObj["Manufacturer"].toString());
        if( jsonObj.contains("MediaType") )
            ui->lineMediaType->setText(jsonObj["MediaType"].toString());
        if( jsonObj.contains("Name") )
            ui->lineName->setText(jsonObj["Name"].toString());
        if( jsonObj.contains("Size") ) {
            double sizeBytes = jsonObj["Size"].toString().toDouble();

            double sizeGb = (sizeBytes/pow(10.0,9.0));
            double sizeMb = (sizeBytes/pow(10.0,6.0));
            double sizeKb = (sizeBytes/pow(10.0,3.0));

            if ( floor(sizeGb) != 0 )
                ui->lineSize->setText(QString::number(sizeGb) + " GB");
            else if ( floor(sizeMb) != 0 )
                ui->lineSize->setText(QString::number(sizeMb) + " MB");
            else if ( floor(sizeKb) != 0 )
                ui->lineSize->setText(QString::number(sizeKb) + " KB");
            else
                ui->lineSize->setText(jsonObj["Size"].toString() + " B");
        }
    }
}

CdDriveTab::~CdDriveTab()
{
    delete ui;
}
