#ifndef WIDGETADMIN_H
#define WIDGETADMIN_H

#include <QWidget>
#include <QObject>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QGridLayout>
#include "common.h"

class widgetAdmin : public QWidget
{
    Q_OBJECT
public:
    explicit widgetAdmin(QWidget *parent = 0);

private:
    const QString c_nameItems[E_NMB_ITEMS_ADMIN] = {"max_P [W]", "cooling_max [°C]", "RESERVE"};
    const QString c_defaultValueItems[E_NMB_ITEMS_ADMIN] = {"100", "37", "0"};


    QLabel *createNewLabel(const QString &text);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QGridLayout* childLayout = new QGridLayout();

    QLineEdit *lineInputItems[E_NMB_ITEMS_ADMIN];

signals:
    void SaveAdmin(int iLoop, QString text);
    void SetLimitSlider(int index, QString data);

public slots:
    void ReadAdmin(int index, QString data);
};

#endif // WIDGETADMIN_H
