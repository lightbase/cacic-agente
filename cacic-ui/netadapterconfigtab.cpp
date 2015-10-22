#include "netadapterconfigtab.h"
#include "ui_netadapterconfigtab.h"

NetAdapterConfigTab::NetAdapterConfigTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetAdapterConfigTab)
{
    ui->setupUi(this);
}

NetAdapterConfigTab::NetAdapterConfigTab(QJsonObject netObj, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetAdapterConfigTab)
{
    ui->setupUi(this);

    if(!netObj.isEmpty()) {
        if( netObj.contains("ipv4") )
            ui->lineIpv4->setText(netObj["ipv4"].toString());
        if( netObj.contains("ipv6") )
            ui->lineIpv6->setText(netObj["ipv6"].toString());
        if( netObj.contains("mac") )
            ui->lineMac->setText(netObj["mac"].toString());
        if( netObj.contains("netmask_ipv4") )
            ui->lineMaskIpv4->setText(netObj["netmask_ipv4"].toString());
        if( netObj.contains("netmask_ipv6") )
            ui->lineMaskIpv6->setText(netObj["netmask_ipv6"].toString());
        if( netObj.contains("nome") )
            ui->lineNome->setText(netObj["nome"].toString());
    }
}

NetAdapterConfigTab::~NetAdapterConfigTab()
{
    delete ui;
}
