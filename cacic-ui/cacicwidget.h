#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include "cacicsystray.h"
#include "../src/ccacic.h"
#include "../src/logcacic.h"
#include "uiclient.h"

namespace Ui {
class Widget;
}

class CacicWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CacicWidget(QWidget *parent = 0);
    ~CacicWidget();

private slots:
    void on_infosClicked();

private:
    LogCacic *logcacic;
    Ui::Widget *ui;
    QString cacicMainFolder;
    CacicSysTray *cacicSysTray;
    UiClient *cliente;
};

#endif // WIDGET_H
