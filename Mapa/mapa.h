#ifndef MAPA_H
#define MAPA_H

#include <QMainWindow>
#include <QtCore>
#include <QMessageBox>
#include "cacic_comm.h"
#include "ccacic.h"
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
    bool checarPreenchimento() const;

    LogCacic *logcacic;
    QString mainFolder;
    Ui::Mapa *ui;
};

#endif // MAPA_H
