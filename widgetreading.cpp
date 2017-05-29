#include "widgetreading.h"

#include <QDesktopServices>

widgetReading::widgetReading(QWidget *parent) : QWidget(parent)
{
    for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_GENER; iLoop++)
    {
        chBoxGen[iLoop] = new QCheckBox(allNamesGen[iLoop], this);
        generTimes[iLoop] = new QSpinBox(this);
        qGridLyout->addWidget(chBoxGen[iLoop], iLoop, 2, Qt::AlignRight);
        qGridLyout->addWidget(generTimes[iLoop], iLoop, 3, Qt::AlignLeft);

        generTimes[iLoop]->setMaximum(1000);
        generTimes[iLoop]->setSingleStep(10);

        if(iLoop < NMB_ITEMS_TIMERS_AMPLF)
        {
            chBoxAmp[iLoop] = new QCheckBox(allNamesAmp[iLoop], this);
            amplfTimes[iLoop] = new QSpinBox(this);
            qGridLyout->addWidget(chBoxAmp[iLoop], iLoop, 0, Qt::AlignRight);
            qGridLyout->addWidget(amplfTimes[iLoop], iLoop, 1, Qt::AlignLeft);

            amplfTimes[iLoop]->setMaximum(1000);
            amplfTimes[iLoop]->setSingleStep(10);
        }
    }

    chBoxGen[0]->setEnabled(false);
    chBoxGen[4]->setEnabled(false);

    chBoxAmp[2]->setEnabled(false);


    vBox->addLayout(qGridLyout);
    vBox->addWidget(textBrowser);
    vBox->addWidget(buttClear);

    connect(buttClear,&QPushButton::clicked,[=](bool clicked){

        Q_UNUSED(clicked);
        textBrowser->clear();
    });

    connect(textBrowser,&QTextBrowser::anchorClicked,[=](const QUrl &arg1){

        QDesktopServices::openUrl(arg1);
    });

    for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_AMPLF; iLoop++)
    {
        connect(chBoxAmp[iLoop],&QCheckBox::clicked,[=](bool clicked){

            Q_UNUSED(clicked);
            //qDebug() << iLoop << "amp checkbox:" << chBoxAmp[iLoop]->checkState();
            emit SendV200Requirement(chBoxAmp[iLoop]->checkState(), 0, iLoop);
        });

        connect(amplfTimes[iLoop],static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int value){

            emit SendNewTime(value, 0, iLoop);
        });
    }

    for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_GENER; iLoop++)
    {
        connect(chBoxGen[iLoop],&QCheckBox::clicked,[=](bool clicked){

            Q_UNUSED(clicked);
            //qDebug() << iLoop << "gen checkbox:" << chBoxGen[iLoop]->checkState();
            emit SendV200Requirement(chBoxGen[iLoop]->checkState(), 1, iLoop);
        });

        connect(generTimes[iLoop],static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int value){

            emit SendNewTime(value, 1, iLoop);
        });
    }

}

void widgetReading::showTextLog(QString showText)
{
    textBrowser->append(showText);
}

void widgetReading::ReadTimeRequests(bool device, int index, int value)
{
    if(device == 0)
    {
        amplfTimes[index]->setValue(value);
    }
    else
    {
        generTimes[index]->setValue(value);
    }
}

void widgetReading::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);

}
