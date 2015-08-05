#ifndef NOVOCAMPO_H
#define NOVOCAMPO_H

#include <QString>
#include <QWidget>

class NovoCampo
{
public:
    NovoCampo();

    QString getTitle() const;
    void setTitle(const QString &value);
    QString getLabel() const;
    void setLabel(const QString &value);
    QString getDescription() const;
    void setDescription(const QString &value);
    QWidget *getLabelWidget() const;
    void setLabelWidget(QWidget *value);
    QWidget *getLineWidget() const;
    void setLineWidget(QWidget *value);

protected:

private:
    QString title;
    QString label;
    QString description;
    QWidget *labelWidget;
    QWidget *lineWidget;
};

#endif // NOVOCAMPO_H
