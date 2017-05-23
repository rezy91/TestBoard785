#include "widgetreading.h"

#include <QDesktopServices>

widgetReading::widgetReading(QWidget *parent) : QWidget(parent)
{
    for(int iLoop = 0; iLoop < NMB_GEN_MSGS; iLoop++)
    {
        chBoxGen[iLoop] = new QCheckBox(allNamesGen[iLoop], this);
        generTimes[iLoop] = new QSpinBox(this);
        qGridLyout->addWidget(chBoxGen[iLoop], iLoop, 2);
        qGridLyout->addWidget(generTimes[iLoop], iLoop, 3);

        generTimes[iLoop]->setMaximum(1000);
        generTimes[iLoop]->setSingleStep(10);
        generTimes[iLoop]->setValue(100);

        if(iLoop < NMB_AMP_MSGS)
        {
            chBoxAmp[iLoop] = new QCheckBox(allNamesAmp[iLoop], this);
            amplfTimes[iLoop] = new QSpinBox(this);
            qGridLyout->addWidget(chBoxAmp[iLoop], iLoop, 0);
            qGridLyout->addWidget(amplfTimes[iLoop], iLoop, 1);

            amplfTimes[iLoop]->setMaximum(1000);
            amplfTimes[iLoop]->setSingleStep(10);
            amplfTimes[iLoop]->setValue(100);
        }
    }

    chBoxGen[0]->setEnabled(false);
    chBoxGen[4]->setEnabled(false);

    chBoxAmp[2]->setEnabled(false);


    vBox->addLayout(qGridLyout);
    vBox->addLayout(hBoxForSpinGen);
    vBox->addLayout(hBoxForSpinAmp);
    vBox->addWidget(textBrowser);
    vBox->addWidget(buttClear);

    connect(buttClear,&QPushButton::clicked,[=](bool clicked){

        Q_UNUSED(clicked);
        textBrowser->clear();
    });

    connect(textBrowser,&QTextBrowser::anchorClicked,[=](const QUrl &arg1){

        QDesktopServices::openUrl(arg1);
    });

    for(int iLoop = 0; iLoop < NMB_AMP_MSGS; iLoop++)
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

    for(int iLoop = 0; iLoop < NMB_GEN_MSGS; iLoop++)
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

void widgetReading::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);

}
