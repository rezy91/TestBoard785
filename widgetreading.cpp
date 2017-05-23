#include "widgetreading.h"

#include <QDesktopServices>

widgetReading::widgetReading(QWidget *parent) : QWidget(parent)
{

    for(int iLoop = 0; iLoop < NMB_TIMERS; iLoop++)
    {
        generTimes[iLoop] = new QDoubleSpinBox(this);
        hBoxForSpinGen->addWidget(generTimes[iLoop]);
    }

    for(int iLoop = 0; iLoop < NMB_TIMERS; iLoop++)
    {
        amplfTimes[iLoop] = new QDoubleSpinBox(this);
        hBoxForSpinAmp->addWidget(amplfTimes[iLoop]);
    }


    for(int iLoop = 0; iLoop < NMB_GEN_MSGS; iLoop++)
    {
        chBoxGen[iLoop] = new QCheckBox(allNamesGen[iLoop], this);

        if(iLoop >= NMB_AMP_MSGS)
        {
            qFrmLyout->addRow(NULL, chBoxGen[iLoop]);
        }
        else
        {
            chBoxAmp[iLoop] = new QCheckBox(allNamesAmp[iLoop], this);
            qFrmLyout->addRow(chBoxAmp[iLoop], chBoxGen[iLoop]);
        }
    }

    chBoxGen[0]->setEnabled(false);
    chBoxGen[4]->setEnabled(false);

    chBoxAmp[2]->setEnabled(false);


    vBox->addLayout(qFrmLyout);
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
    }

    for(int iLoop = 0; iLoop < NMB_GEN_MSGS; iLoop++)
    {
        connect(chBoxGen[iLoop],&QCheckBox::clicked,[=](bool clicked){

            Q_UNUSED(clicked);
            //qDebug() << iLoop << "gen checkbox:" << chBoxGen[iLoop]->checkState();
            emit SendV200Requirement(chBoxGen[iLoop]->checkState(), 1, iLoop);
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
