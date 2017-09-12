#include "widgettherapy.h"

#include <QStandardItemModel>

widgetTherapy::widgetTherapy(QWidget *parent) : QWidget(parent)
{
    dwCurrentChannel = 0;

    for(int iLoop = 0; iLoop < E_PARAMS_NMB; iLoop++)
    {
        therapyParams[iLoop].name = new QLabel(this);
        therapyParams[iLoop].value = new QLabel(this);
        therapyParams[iLoop].slider = new QSlider(this);

        therapyParams[iLoop].slider->setMaximum(maxSlider[iLoop]);
        therapyParams[iLoop].slider->setMinimum(minSlider[iLoop]);
        therapyParams[iLoop].slider->setSingleStep(stepSlider[iLoop]);
        therapyParams[iLoop].slider->setEnabled(false);

        therapyParams[iLoop].name->setText(namesParams[iLoop]);
        therapyParams[iLoop].value->setText(QString("%1 %2").arg(therapyParams[iLoop].slider->value()).arg(unitParams[iLoop]));
    }

    listOfChannels->addItem("Select channel");

    for(int iLoop = 0; iLoop < nmbChannelsAppls; iLoop++)
    {
        listOfChannels->addItem(QString("channel_%1").arg(QString::number(iLoop)));
        qobject_cast<QStandardItemModel*>(listOfChannels->model())->item(iLoop + 1)->setEnabled(false);
    }

    stopButton->setEnabled(false);
    userButton->setEnabled(false);

    startButton->setText("START");
    stopButton->setText("STOP");
    userButton->setText("USER");

    startButton->setStyleSheet("background-color:green;");
    stopButton->setStyleSheet("background-color:red;");

    for(int iLoop = 0; iLoop < E_PARAMS_NMB; iLoop++)
    {
        connect(therapyParams[iLoop].slider, &QSlider::valueChanged, [=](int value){
            //qDebug() << "slider:" << iLoop << "has value:" << value;
            therapyParams[iLoop].value->setText(QString("%1 %2").arg(iLoop == E_ULTRASOUND ? float(value) / 10 : value).arg(unitParams[iLoop]));

            QString msgTherapy = QString("%1").arg(QString::number(PID_SET_THERAPY_SETTINGS, 16));
            msgTherapy += QString::number(QString("%1").arg(therapyParams[E_POWER].slider->value()).toInt(), 16).rightJustified(4, '0');
            msgTherapy += QString::number(QString("%1").arg(therapyParams[E_DUTYCYCLE].slider->value()).toInt(), 16).rightJustified(2, '0');
            msgTherapy += QString::number(QString("%1").arg(0).toInt(), 16).rightJustified(2, '0');
            msgTherapy += QString::number(QString("%1").arg(therapyParams[E_COOLING].slider->value()).toInt(), 16).rightJustified(2, '0');
            msgTherapy += QString::number(QString("%1").arg(therapyParams[E_ULTRASOUND].slider->value()).toInt(), 16).rightJustified(2, '0');

            emit SendV200specific(msgTherapy);
        });

    }

    connect(listOfChannels,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[=](int nValue){

        dwCurrentChannel = nValue;

        if(nValue != 0)
        {
            //qDebug() << "selected channel:" << nValue - 1;

            EnableSliders(E_STATE_OFF);

            QString msgChannel = QString("%1").arg(QString::number(PID_SET_CHOOSE_CURRENT_CHANNEL, 16));
            msgChannel += QString::number(QString("%1").arg(nValue - 1).toInt(), 16).rightJustified(2, '0');

            emit SendV200specific(msgChannel);
        }
        else
        {
            EnableSliders(E_STATE_OFF);
        }

    });

    connect(startButton, &QPushButton::clicked, [=](){

        QString msgRun = QString("%1").arg(QString::number(PID_SET_STATE_OF_THERAPY, 16));
        msgRun += QString("01");

        emit SendV200specific(msgRun);
    });

    connect(stopButton, &QPushButton::clicked, [=](){

        QString msgRun = QString("%1").arg(QString::number(PID_SET_STATE_OF_THERAPY, 16));
        msgRun += QString("00");
        userButton->setChecked(false);

        emit SendV200specific(msgRun);
    });

    connect(userButton, &QRadioButton::clicked, [=](){

        QString msgRun = QString("%1").arg(QString::number(PID_SET_STATE_OF_THERAPY, 16));

        if(userButton->isChecked())
        {
            msgRun += QString("02");
        }
        else
        {
            msgRun += QString("01");
        }

        emit SendV200specific(msgRun);
    });
}

void widgetTherapy::TherapyRuns(unsigned char stateTherapy)
{
    startButton->setEnabled(false);
    stopButton->setEnabled(true);
    userButton->setEnabled(true);
    listOfChannels->setEnabled(false);

    EnableSliders(TherapyState(stateTherapy));

    userButton->setChecked(stateTherapy == E_STATE_ON ? true : false);
}

void widgetTherapy::TherapyDoesnotRun()
{
    startButton->setEnabled(true);
    stopButton->setEnabled(false);
    userButton->setEnabled(false);
    listOfChannels->setEnabled(true);

    EnableSliders(E_STATE_OFF);
}

void widgetTherapy::EnableSliders(widgetTherapy::TherapyState eState)
{
   if(eState == E_STATE_ON || dwCurrentChannel == 0)
   {
       for(int iLoop = 0; iLoop < E_PARAMS_NMB; iLoop++)
       {
           therapyParams[iLoop].slider->setEnabled(false);
       }
   }
   else
   {
       therapyParams[E_POWER].slider->setEnabled(true);
       therapyParams[E_ULTRASOUND].slider->setEnabled(true);

       //large
       if(dwCurrentChannel == 1)
       {
           therapyParams[E_DUTYCYCLE].slider->setEnabled(false);
           therapyParams[E_COOLING].slider->setEnabled(true);
       }
       else
       {
           therapyParams[E_DUTYCYCLE].slider->setEnabled(true);
           therapyParams[E_COOLING].slider->setEnabled(false);
       }
   }
}

void widgetTherapy::resetValues()
{
    listOfChannels->setCurrentIndex(0);

    for(int iLoop = 0; iLoop < E_PARAMS_NMB; iLoop++)
    {
       therapyParams[iLoop].value->setText(QString("%1 %2").arg(minSlider[iLoop]).arg(unitParams[iLoop]));
       therapyParams[iLoop].slider->setValue(minSlider[iLoop]);
    }

    dwCurrentChannel = 0;
}

void widgetTherapy::ReceiveStatusReg(STATUS_REGISTER eStatusReg)
{
    qobject_cast<QStandardItemModel*>(listOfChannels->model())->item(1)->setEnabled(eStatusReg.m_Reg.m_Bit.StateAcc0 == 1 ? true : false);
    qobject_cast<QStandardItemModel*>(listOfChannels->model())->item(2)->setEnabled(eStatusReg.m_Reg.m_Bit.StateAcc1 == 1 ? true : false);
    qobject_cast<QStandardItemModel*>(listOfChannels->model())->item(3)->setEnabled(eStatusReg.m_Reg.m_Bit.StateAcc2 == 1 ? true : false);
    qobject_cast<QStandardItemModel*>(listOfChannels->model())->item(4)->setEnabled(eStatusReg.m_Reg.m_Bit.StateAcc3 == 1 ? true : false);

    (eStatusReg.m_Reg.m_Bit.StateTherapy != E_STATE_OFF) ? TherapyRuns(eStatusReg.m_Reg.m_Bit.StateTherapy) : TherapyDoesnotRun();

    /*therapyParams[E_POWER].slider->setValue(eStatusReg.m_wSetPower);
    therapyParams[E_COOLING].slider->setValue(eStatusReg.m_bySetTemperaturePatient);
    therapyParams[E_ULTRASOUND].slider->setValue(eStatusReg.m_bySetIntensityUsn);
    */
}

void widgetTherapy::ReceiveLimitSlider(int index, QString text)
{
    if(index == 0)
    {
        therapyParams[E_POWER].slider->setMaximum(text.toInt());
    }
    else if(index == 1)
    {
        therapyParams[E_COOLING].slider->setMaximum(text.toInt());
    }
    else if(index == 2)
    {

    }

    //qDebug() << index << text;
}

void widgetTherapy::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);


    for(int iLoop = 0; iLoop < E_PARAMS_NMB; iLoop++)
    {
       therapyParams[iLoop].value->setGeometry(width() / 15 * iLoop, 0, width() / 20, 30);
       therapyParams[iLoop].slider->setGeometry(width() / 15 * iLoop, 30, width() / 20, height() - 60);
       therapyParams[iLoop].name->setGeometry(width() / 15 * iLoop, height() - 35, width() / 20, 30);
    }
    listOfChannels->setGeometry((width() / 3) , height() / 2, width() / 8, 25);
    startButton->setGeometry((width() / 4) * 2, height() / 4, width() / 4, height() / 2);
    stopButton->setGeometry((width() / 4) * 3, height() / 4, width() / 4, height() / 2);
    userButton->setGeometry((width() / 2), 0, 100, 40);
}
