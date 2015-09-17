#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include "cacicsystray.h"
#include "../src/ccacic.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    QString cacicMainFolder;
    CacicSysTray *cacicSysTray;
};

#endif // WIDGET_H
