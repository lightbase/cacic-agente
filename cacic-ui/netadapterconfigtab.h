#ifndef NETADAPTERCONFIGTAB_H
#define NETADAPTERCONFIGTAB_H

#include <QJsonObject>
#include <QWidget>

namespace Ui {
class NetAdapterConfigTab;
}

class NetAdapterConfigTab : public QWidget
{
    Q_OBJECT

public:
    explicit NetAdapterConfigTab(QWidget *parent = 0);
    NetAdapterConfigTab(QJsonObject netObj, QWidget *parent = 0);
    ~NetAdapterConfigTab();

private:
    Ui::NetAdapterConfigTab *ui;
};

#endif // NETADAPTERCONFIGTAB_H
