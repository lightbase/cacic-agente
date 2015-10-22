#ifndef PRINTERTAB_H
#define PRINTERTAB_H

#include <QJsonObject>
#include <QWidget>

namespace Ui {
class PrinterTab;
}

class PrinterTab : public QWidget
{
    Q_OBJECT

public:
    explicit PrinterTab(QWidget *parent = 0);
    PrinterTab(QJsonObject jsonObj, QWidget *parent = 0);
    ~PrinterTab();

private:
    Ui::PrinterTab *ui;
};

#endif // PRINTERTAB_H
