#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QString folder = CCacic::getValueFromRegistry("Lightbase", "Cacic", "mainFolder").toString();
    cacicMainFolder = !folder.isEmpty() && !folder.isNull() ? folder : Identificadores::ENDERECO_PATCH_CACIC;

    cacicSysTray = new CacicSysTray(cacicMainFolder,this);
    cacicSysTray->iniciarSysTray();
}

Widget::~Widget()
{
    delete ui;
    delete cacicSysTray;
}
