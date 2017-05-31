#include "widgettherapy.h"
#include "common.h"

#include <QStandardItemModel>

widgetTherapy::widgetTherapy(QWidget *parent) : QWidget(parent)
{
    for(int iLoop = 0; iLoop < E_PARAMS_NMB; iLoop++)
    {
        therapyParams[iLoop].name = new QLabel(this);
        therapyParams[iLoop].value = new QLabel(this);
        therapyParams[iLoop].slider = new QSlider(this);

        therapyParams[iLoop].slider->setMaximum(maxSlider[iLoop]);
        therapyParams[iLoop].slider->setMinimum(minSlider[iLoop]);
        therapyParams[iLoop].slider->setSingleStep(stepSlider[iLoop]);
        therapyParams[iLoop].name->setText(namesParams[iLoop]);
        therapyParams[iLoop].value->setText(QString("%1 %2").arg(therapyParams[iLoop].slider->value()).arg(unitParams[iLoop]));
    }


    //temporary, until this function be available in generator
    therapyParams[E_DUTYCYCLE].slider->setEnabled(false);
    therapyParams[E_FREQUENCY].slider->setEnabled(false);

    listOfChannels->addItem("Select channel");

    for(int iLoop = 0; iLoop < nmbChannelsAppls; iLoop++)
    {
        listOfChannels->addItem(QString("channel_%1").arg(QString::number(iLoop)));
        qobject_cast<QStandardItemModel*>(listOfChannels->model())->item(iLoop + 1)->setEnabled(false);
    }

    startButton->setText("START");
    stopButton->setText("STOP");

    startButton->setStyleSheet("background-color:green;");
    stopButton->setStyleSheet("background-color:red;");

    for(int iLoop = 0; iLoop < E_PARAMS_NMB; iLoop++)
    {
        connect(therapyParams[iLoop].slider, &QSlider::valueChanged, [=](int value){
            //qDebug() << "slider:" << iLoop << "has value:" << value;
            therapyParams[iLoop].value->setText(QString("%1 %2").arg(value).arg(unitParams[iLoop]));
        });

    }

    connect(listOfChannels,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[=](int nValue){

        if(nValue != 0)
        {
            qDebug() << "selected channel:" << nValue - 1;

            QString msgChannel = QString("%1").arg(QString::number(PID_SET_CHOOSE_CURRENT_CHANNEL, 16));
            msgChannel += QString::number(QString("%1").arg(nValue - 1).toInt(), 16).rightJustified(2, '0');

            emit SendV200specific(msgChannel);
        }

    });

    connect(startButton, &QPushButton::clicked, [=](){

        startButton->setEnabled(false);
        stopButton->setEnabled(true);

        for(int iLoop = 0; iLoop < E_POWER + 1; iLoop++)
        {
           therapyParams[iLoop].slider->setEnabled(false);
        }

        QString msgTherapy = QString("%1").arg(QString::number(PID_SET_THERAPY_SETTINGS, 16));
        msgTherapy += QString::number(QString("%1").arg(therapyParams[E_POWER].slider->value()).toInt(), 16).rightJustified(4, '0');
        msgTherapy += QString::number(QString("%1").arg(therapyParams[E_DUTYCYCLE].slider->value()).toInt(), 16).rightJustified(2, '0');
        msgTherapy += QString::number(QString("%1").arg(therapyParams[E_FREQUENCY].slider->value()).toInt(), 16).rightJustified(2, '0');

        emit SendV200specific(msgTherapy);

        QString msgRun = QString("%1").arg(QString::number(PID_START_STOP_THERAPY, 16));
        msgRun += QString("0100");

        emit SendV200specific(msgRun);
    });

    connect(stopButton, &QPushButton::clicked, [=](){

        startButton->setEnabled(true);
        stopButton->setEnabled(false);

        for(int iLoop = 0; iLoop < E_POWER + 1; iLoop++)
        {
           therapyParams[iLoop].slider->setEnabled(true);
        }

        QString msgRun = QString("%1").arg(QString::number(PID_START_STOP_THERAPY, 16));
        msgRun += QString("0000");

        emit SendV200specific(msgRun);
    });
}

void widgetTherapy::ReceivePartStatusReg(unsigned char value)
{
    for(int iLoop = 0; iLoop < nmbChannelsAppls; iLoop++)
    {
        if(value & (0x02 << iLoop))
        {
            //qDebug() << "channel" << iLoop << "connected";
            qobject_cast<QStandardItemModel*>(listOfChannels->model())->item(iLoop + 1)->setEnabled(true);
        }
        else
        {
            //qDebug() << "channel" << iLoop << "disconnected";
            qobject_cast<QStandardItemModel*>(listOfChannels->model())->item(iLoop + 1)->setEnabled(false);
        }
    }
}

void widgetTherapy::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);


    for(int iLoop = 0; iLoop < E_PARAMS_NMB; iLoop++)
    {
       therapyParams[iLoop].value->setGeometry(width() / 10 * iLoop, 0, width() / 15, 30);
       therapyParams[iLoop].slider->setGeometry(width() / 10 * iLoop, 30, width() / 15, height() - 60);
       therapyParams[iLoop].name->setGeometry(width() / 10 * iLoop, height() - 35, width() / 15, 30);
    }
    listOfChannels->setGeometry((width() / 3) , height() / 2, width() / 8, 25);
    startButton->setGeometry((width() / 4) * 2, height() / 4, width() / 4, height() / 2);
    stopButton->setGeometry((width() / 4) * 3, height() / 4, width() / 4, height() / 2);
}
