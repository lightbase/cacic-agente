#ifndef INSTALLCACICGUI_H
#define INSTALLCACICGUI_H

#include <QMainWindow>
#include <QFile>
#include "identificadores.h"

namespace Ui {
class InstallCacicGui;
}

class InstallCacicGui : public QMainWindow
{
    Q_OBJECT

public:
    explicit InstallCacicGui(QWidget *parent = 0);
    ~InstallCacicGui();

private slots:
    void on_pushButton_clicked();

private:
    Ui::InstallCacicGui *ui;
    bool verificarDadosInseridos();
    void instanciarObjetos();
    bool iniciarInstalacao(QStringList parametros);
};

#endif // INSTALLCACICGUI_H
