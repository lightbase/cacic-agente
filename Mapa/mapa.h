#ifndef MAPA_H
#define MAPA_H

#include <QMainWindow>
#include <QtCore>
#include <QMessageBox>
#include "cacic_comm.h"
#include "ccacic.h"
#include "cacic_computer.h"
#include "logcacic.h"
#include "identificadores.h"
#include "ui_mapa_default.h"

namespace Ui {
class Mapa;
}

class Mapa : public QMainWindow
{
    Q_OBJECT

public:
    explicit Mapa(QWidget *parent = 0);
    ~Mapa();

private slots:
    void on_okButton_clicked();

private:
    bool checarCampos();
    bool checarPreenchimento() const;
    bool enviarInfo(const QJsonObject &jsonMapa);

    CACIC_Computer computer;
    LogCacic *logcacic;
    QString mainFolder;
    Ui::Mapa *ui;
};

#endif // MAPA_H
