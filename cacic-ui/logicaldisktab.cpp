#include "logicaldisktab.h"
#include "ui_logicaldisktab.h"

LogicalDiskTab::LogicalDiskTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogicalDiskTab)
{
    ui->setupUi(this);
}

LogicalDiskTab::LogicalDiskTab(QJsonObject jsonObj, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogicalDiskTab)
{
    ui->setupUi(this);

    if(!jsonObj.isEmpty()) {
        if( jsonObj.contains("Caption") )
            ui->lineCaption->setText(jsonObj["Caption"].toString());
        if( jsonObj.contains("DeviceID") )
            ui->lineDeviceID->setText(jsonObj["DeviceID"].toString());
        if( jsonObj.contains("DriveType") )
            ui->lineDriveType->setText(jsonObj["DriveType"].toString());
        if( jsonObj.contains("FileSystem") )
            ui->lineFileSystem->setText(jsonObj["FileSystem"].toString());
        if( jsonObj.contains("MediaType") )
            ui->lineMediaType->setText(jsonObj["MediaType"].toString());
        if( jsonObj.contains("VolumeName") )
            ui->lineVolumeName  ->setText(jsonObj["VolumeName"].toString());
        if( jsonObj.contains("FreeSpace") ) {
            double sizeBytes = jsonObj["FreeSpace"].toString().toDouble();

            double sizeGb = (sizeBytes/pow(10.0,9.0));
            double sizeMb = (sizeBytes/pow(10.0,6.0));
            double sizeKb = (sizeBytes/pow(10.0,3.0));

            if ( floor(sizeGb) != 0 )
                ui->lineFreeSpace->setText(QString::number(sizeGb) + " GB");
            else if ( floor(sizeMb) != 0 )
                ui->lineFreeSpace->setText(QString::number(sizeMb) + " MB");
            else if ( floor(sizeKb) != 0 )
                ui->lineFreeSpace->setText(QString::number(sizeKb) + " KB");
            else
                ui->lineFreeSpace->setText(jsonObj["Size"].toString() + " B");
        }
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

LogicalDiskTab::~LogicalDiskTab()
{
    delete ui;
}
