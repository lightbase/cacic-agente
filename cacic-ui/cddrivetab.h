#ifndef CDDRIVETAB_H
#define CDDRIVETAB_H

#include <QJsonObject>
#include <QWidget>
#include <cmath>

namespace Ui {
class CdDriveTab;
}

class CdDriveTab : public QWidget
{
    Q_OBJECT

public:
    explicit CdDriveTab(QWidget *parent = 0);
    CdDriveTab(QJsonObject jsonObj, QWidget *parent = 0);
    ~CdDriveTab();

private:
    Ui::CdDriveTab *ui;
};

#endif // CDDRIVETAB_H
