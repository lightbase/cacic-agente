#ifndef NETDEVTAB_H
#define NETDEVTAB_H

#include <QJsonObject>
#include <QWidget>

namespace Ui {
class NetDevTab;
}

class NetDevTab : public QWidget
{
    Q_OBJECT

public:
    explicit NetDevTab(QWidget *parent = 0);
    NetDevTab(QJsonObject netDev, QWidget *parent = 0);
    ~NetDevTab();

private:
    Ui::NetDevTab *ui;
};

#endif // NETDEVTAB_H
