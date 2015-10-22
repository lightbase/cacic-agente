#include "netdevtab.h"
#include "ui_netdevtab.h"

NetDevTab::NetDevTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetDevTab)
{
    ui->setupUi(this);

}

NetDevTab::NetDevTab(QJsonObject netDev, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetDevTab)
{
    ui->setupUi(this);

    if(!netDev.isEmpty()) {
        if( netDev.contains("ipv4") )
            ui->lineIpv4->setText(netDev["ipv4"].toString());
        if( netDev.contains("ipv6") )
            ui->lineIpv6->setText(netDev["ipv6"].toString());
        if( netDev.contains("mac") )
            ui->lineMac->setText(netDev["mac"].toString());
        if( netDev.contains("netmask_ipv4") )
            ui->lineMaskIpv4->setText(netDev["netmask_ipv4"].toString());
        if( netDev.contains("netmask_ipv6") )
            ui->lineMaskIpv6->setText(netDev["netmask_ipv6"].toString());
        if( netDev.contains("nome") )
            ui->lineNomeNet->setText(netDev["nome"].toString());
    }
}

NetDevTab::~NetDevTab()
{
    delete ui;
}
