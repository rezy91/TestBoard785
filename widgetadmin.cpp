#include "widgetadmin.h"
#include <QDebug>

widgetAdmin::widgetAdmin(QWidget *parent) : QWidget(parent)
{
    for(int iLoop = 0; iLoop < E_NMB_ITEMS_ADMIN; iLoop++)
    {
        childLayout->addWidget(createNewLabel(c_nameItems[iLoop]), iLoop, 0, Qt::AlignRight);
        lineInputItems[iLoop] = new QLineEdit();
        childLayout->addWidget(lineInputItems[iLoop], iLoop, 1, Qt::AlignLeft);
    }

    mainLayout->addLayout(childLayout);



    for(int iLoop = 0; iLoop < E_NMB_ITEMS_ADMIN; iLoop++)
    {
        connect(lineInputItems[iLoop],&QLineEdit::textChanged,[=](const QString &text){

            emit SaveAdmin(iLoop, text);
            emit SetLimitSlider(iLoop, text);
        });
    }

}

QLabel *widgetAdmin::createNewLabel(const QString &text)
{
    QLabel* newLabel = new QLabel(text);
    newLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    return newLabel;
}

void widgetAdmin::ReadAdmin(int index, QString data)
{
    lineInputItems[index]->setText(data.isEmpty() ? c_defaultValueItems[index] : data);

    emit SetLimitSlider(index, lineInputItems[index]->text());
}
