#include "novocampo.h"

NovoCampo::NovoCampo()
{
}

QString NovoCampo::getTitle() const
{
    return title;
}

void NovoCampo::setTitle(const QString &value)
{
    title = value;
}

QString NovoCampo::getLabel() const
{
    return label;
}

void NovoCampo::setLabel(const QString &value)
{
    label = value;
}

QString NovoCampo::getDescription() const
{
    return description;
}

void NovoCampo::setDescription(const QString &value)
{
    description = value;
}

QWidget *NovoCampo::getLabelWidget() const
{
    return labelWidget;
}

void NovoCampo::setLabelWidget(QWidget *value)
{
    labelWidget = value;
}

QWidget *NovoCampo::getLineWidget() const
{
    return lineWidget;
}

void NovoCampo::setLineWidget(QWidget *value)
{
    lineWidget = value;
}

