#include "widgettherapy.h"

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

    connect(startButton, &QPushButton::clicked, [=](){

        startButton->setEnabled(false);
        stopButton->setEnabled(true);

        for(int iLoop = 0; iLoop < E_PARAMS_NMB; iLoop++)
        {
           therapyParams[iLoop].slider->setEnabled(false);
        }

        QString msgTherapy = QString("23");
        msgTherapy += QString::number(QString("%1").arg(therapyParams[E_POWER].slider->value()).toInt(), 16).rightJustified(4, '0');
        msgTherapy += QString::number(QString("%1").arg(therapyParams[E_DUTYCYCLE].slider->value()).toInt(), 16).rightJustified(2, '0');
        msgTherapy += QString::number(QString("%1").arg(therapyParams[E_FREQUENCY].slider->value()).toInt(), 16).rightJustified(2, '0');

        qDebug() << msgTherapy;


        emit SendV200specific(msgTherapy);

        QString msgRun = QString("5C0100");
        emit SendV200specific(msgRun);
    });

    connect(stopButton, &QPushButton::clicked, [=](){

        startButton->setEnabled(true);
        stopButton->setEnabled(false);

        for(int iLoop = 0; iLoop < E_PARAMS_NMB; iLoop++)
        {
           therapyParams[iLoop].slider->setEnabled(true);
        }

        QString msgRun = QString("5C0000");
        emit SendV200specific(msgRun);
    });
}

void widgetTherapy::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);


    for(int iLoop = 0; iLoop < E_PARAMS_NMB; iLoop++)
    {
       therapyParams[iLoop].value->setGeometry(width() / 15 * iLoop, 0, width() / 15, 30);
       therapyParams[iLoop].slider->setGeometry(width() / 15 * iLoop, 30, width() / 20, height() - 60);
       therapyParams[iLoop].name->setGeometry(width() / 15 * iLoop, height() - 35, width() / 15, 30);
    }

    startButton->setGeometry(width() / 4, height() / 2, 60, 30);
    stopButton->setGeometry(width() / 3, height() / 2, 60, 30);
}
