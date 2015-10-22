#ifndef POINTDEVICETAB_H
#define POINTDEVICETAB_H

#include <QJsonObject>
#include <QWidget>

namespace Ui {
class PointDeviceTab;
}

class PointDeviceTab : public QWidget
{
    Q_OBJECT

public:
    explicit PointDeviceTab(QWidget *parent = 0);
    PointDeviceTab(QJsonObject jsonObj, QWidget *parent = 0);
    ~PointDeviceTab();

private:
    Ui::PointDeviceTab *ui;
};

#endif // POINTDEVICETAB_H
