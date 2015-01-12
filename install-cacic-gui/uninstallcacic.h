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

    InstallCacicGui *getObjInstallCacic() const;
    void setObjInstallCacic(InstallCacicGui *value);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

signals:
    void finished();

private:
    Ui::UninstallCacic *ui;
    InstallCacicGui *objInstallCacic;
};

#endif // UNINSTALLCACIC_H
