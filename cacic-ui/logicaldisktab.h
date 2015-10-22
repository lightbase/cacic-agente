#ifndef LOGICALDISKTAB_H
#define LOGICALDISKTAB_H

#include <QJsonObject>
#include <QWidget>
#include <cmath>

namespace Ui {
class LogicalDiskTab;
}

class LogicalDiskTab : public QWidget
{
    Q_OBJECT

public:
    explicit LogicalDiskTab(QWidget *parent = 0);
    LogicalDiskTab(QJsonObject jsonObj, QWidget *parent = 0);
    ~LogicalDiskTab();

private:
    Ui::LogicalDiskTab *ui;
};

#endif // LOGICALDISKTAB_H
