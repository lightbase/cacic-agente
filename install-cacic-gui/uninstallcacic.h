#ifndef UNINSTALLCACIC_H
#define UNINSTALLCACIC_H

#include <QMainWindow>
#include <QMessageBox>
#include "installcacicgui.h"

namespace Ui {
class UninstallCacic;
}

class UninstallCacic : public QMainWindow
{
    Q_OBJECT

public:
    explicit UninstallCacic(QWidget *parent = 0);
    ~UninstallCacic();
    void setObjInstallCacic(InstallCacicGui *value);
    void setOCCacic(CCacic *value);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

signals:
    void finished();

private:
    Ui::UninstallCacic *ui;
    InstallCacicGui *objInstallCacic;
    CCacic *oCCacic;
    InstallCacicGui *getObjInstallCacic() const;
    CCacic *getOCCacic() const;
};

#endif // UNINSTALLCACIC_H
