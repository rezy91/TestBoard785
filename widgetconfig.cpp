#include "widgetconfig.h"

widgetConfig::widgetConfig(QWidget *parent) : QWidget(parent)
{
    MainLayout->addWidget(createConfigAmpGroup());
    MainLayout->addWidget(createConfigGenGroup());


    for(int iDevice = 0; iDevice < E_GEN_ADC_NMB; iDevice++)
    {
        connect(buttSendGenMulAdcx[iDevice],&QPushButton::clicked,[=](bool clicked){

            Q_UNUSED(clicked);

            int dw_nmbChannels = 0;
            int dw_PidMsg = 0xFF;

            if(iDevice == E_GEN_ADC_1)
            {
                dw_nmbChannels = E_NMB_GEN_ADC1;
                dw_PidMsg = PID_SET_ADC1_COEFFICIENTS_MULTIPLE;
            }
            else if(iDevice == E_GEN_ADC_2)
            {
                dw_nmbChannels = E_NMB_GEN_ADC2;
                dw_PidMsg = PID_SET_ADC2_COEFFICIENTS_MULTIPLE;
            }
            else if(iDevice == E_GEN_ADC_3)
            {
                dw_nmbChannels = E_NMB_GEN_ADC3;
                dw_PidMsg = PID_SET_ADC3_COEFFICIENTS_MULTIPLE;
            }
            else if(iDevice == E_GEN_ADC_OTH)
            {
                dw_nmbChannels = E_NMB_GEN_OTHERS;
                dw_PidMsg = PID_SET_ADC_OTHERS_COEFFS_MULTIPLE;
            }

            QString AdcData;
            QString strCmd = QString("%1").arg(QString::number(dw_PidMsg, 16));

            for(int iChannel = 0; iChannel < dw_nmbChannels; iChannel++)
            {
                float flNmb = lineInputGenMulAdcx[iDevice][iChannel]->text().toFloat();
                QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
                QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
                strCmd += QString(arrFloatInArraysHexa);
                AdcData.append(QString("%1").arg(QString::number(flNmb)) + " ");
            }

            emit SaveAdcData(QString(c_nameMul), QString(c_nameGen), iDevice + 1, AdcData);
            emit SendV200specific(strCmd);
        });

        connect(buttSendGenAddAdcx[iDevice],&QPushButton::clicked,[=](bool clicked){

            Q_UNUSED(clicked);

            int dw_nmbChannels = 0;
            int dw_PidMsg = 0xFF;

            if(iDevice == E_GEN_ADC_1)
            {
                dw_nmbChannels = E_NMB_GEN_ADC1;
                dw_PidMsg = PID_SET_ADC1_COEFFICIENTS_ADDITIVE;
            }
            else if(iDevice == E_GEN_ADC_2)
            {
                dw_nmbChannels = E_NMB_GEN_ADC2;
                dw_PidMsg = PID_SET_ADC2_COEFFICIENTS_ADDITIVE;
            }
            else if(iDevice == E_GEN_ADC_3)
            {
                dw_nmbChannels = E_NMB_GEN_ADC3;
                dw_PidMsg = PID_SET_ADC3_COEFFICIENTS_ADDITIVE;
            }
            else if(iDevice == E_GEN_ADC_OTH)
            {
                dw_nmbChannels = E_NMB_GEN_OTHERS;
                dw_PidMsg = PID_SET_ADC_OTHERS_COEFFS_ADDITIVE;
            }

            QString AdcData;
            QString strCmd = QString("%1").arg(QString::number(dw_PidMsg, 16));

            for(int iChannel = 0; iChannel < dw_nmbChannels; iChannel++)
            {
                float flNmb = lineInputGenAddAdcx[iDevice][iChannel]->text().toFloat();
                QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
                QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
                strCmd += QString(arrFloatInArraysHexa);
                AdcData.append(QString("%1").arg(QString::number(flNmb)) + " ");
            }

            emit SaveAdcData(QString(c_nameAdd), QString(c_nameGen), iDevice + 1, AdcData);
            emit SendV200specific(strCmd);
        });
    }

    for(int iDevice = 0; iDevice < E_AMP_ADC_NMB; iDevice++)
    {
        connect(buttSendAmpMulAdcx[iDevice],&QPushButton::clicked,[=](bool clicked){

            Q_UNUSED(clicked);

            int dw_nmbChannels = 0;
            int dw_PidMsg = 0xFF;

            if(iDevice == E_AMP_ADC_1)
            {
                dw_nmbChannels = E_NMB_AMP_ADC1;
                dw_PidMsg = PID_SET_AMP_ADC1_COEFF_MULTIPLE;
            }
            else if(iDevice == E_AMP_ADC_3)
            {
                dw_nmbChannels = E_NMB_AMP_ADC3;
                dw_PidMsg = PID_SET_AMP_ADC3_COEFF_MULTIPLE;
            }

            QString AdcData;
            QString strCmd = QString("%1").arg(QString::number(dw_PidMsg, 16));

            for(int iChannel = 0; iChannel < dw_nmbChannels; iChannel++)
            {
                float flNmb = lineInputAmpMulAdcx[iDevice][iChannel]->text().toFloat();
                QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
                QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
                strCmd += QString(arrFloatInArraysHexa);
                AdcData.append(QString("%1").arg(QString::number(flNmb)) + " ");
            }

            emit SaveAdcData(QString(c_nameMul), QString(c_nameAmp), iDevice + 1, AdcData);
            emit SendV200specific(strCmd);
        });

        connect(buttSendAmpAddAdcx[iDevice],&QPushButton::clicked,[=](bool clicked){

            Q_UNUSED(clicked);

            int dw_nmbChannels = 0;
            int dw_PidMsg = 0xFF;

            if(iDevice == E_AMP_ADC_1)
            {
                dw_nmbChannels = E_NMB_AMP_ADC1;
                dw_PidMsg = PID_SET_AMP_ADC1_COEFF_ADDITIVE;
            }
            else if(iDevice == E_AMP_ADC_3)
            {
                dw_nmbChannels = E_NMB_AMP_ADC3;
                dw_PidMsg = PID_SET_AMP_ADC3_COEFF_ADDITIVE;
            }

            QString AdcData;
            QString strCmd = QString("%1").arg(QString::number(dw_PidMsg, 16));

            for(int iChannel = 0; iChannel < dw_nmbChannels; iChannel++)
            {
                float flNmb = lineInputAmpAddAdcx[iDevice][iChannel]->text().toFloat();
                QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
                QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
                strCmd += QString(arrFloatInArraysHexa);
                AdcData.append(QString("%1").arg(QString::number(flNmb)) + " ");
            }

            emit SaveAdcData(QString(c_nameAdd), QString(c_nameAmp), iDevice + 1, AdcData);
            emit SendV200specific(strCmd);
        });
    }

    connect(buttSendGenRegulator,&QPushButton::clicked,[=](bool clicked){

        Q_UNUSED(clicked);

        QString SaveData;
        QString strCmd = QString("%1").arg(QString::number(PID_SET_REGULATOR, 16));

        for(int iChannel = 0; iChannel < E_NMB_GEN_REGULATOR - 1; iChannel++)
        {
            float flNmb = lineInputGenRegulator[iChannel]->text().toFloat();
            QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
            QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
            strCmd += QString(arrFloatInArraysHexa);
            SaveData.append(QString("%1").arg(QString::number(flNmb)) + " ");
        }

        strCmd += QString::number(lineInputGenRegulator[E_NMB_GEN_REGULATOR - 1]->text().toInt(), 16).rightJustified(2 * 2, '0');
        SaveData.append(QString("%1").arg(QString::number(lineInputGenRegulator[E_NMB_GEN_REGULATOR - 1]->text().toInt())) + " ");

        emit SaveRegulator(SaveData);
        emit SendV200specific(strCmd);
    });

    connect(buttSendGenTestTherapy,&QPushButton::clicked,[=](bool clicked){

        Q_UNUSED(clicked);

        QString SaveData;
        QString strCmd = QString("%1").arg(QString::number(PID_SET_THERAPY_TEST, 16));

        for(int iChannel = 0; iChannel < E_NMB_GEN_TESTTHERAPY; iChannel++)
        {
            strCmd += QString::number(lineInputGenTestTherapy[iChannel]->text().toInt(), 16).rightJustified(1 * 2, '0');
            SaveData.append(QString("%1").arg(QString::number(lineInputGenTestTherapy[iChannel]->text().toInt())) + " ");
        }

        emit SaveTestTherapy(SaveData);
        emit SendV200specific(strCmd);
    });

    connect(buttSendGenPwmCqm,&QPushButton::clicked,[=](bool clicked){

        Q_UNUSED(clicked);

        QString SaveData;
        QString strCmd = QString("%1").arg(QString::number(PID_SET_PWM_CQMS_FREQ, 16));

        for(int iChannel = 0; iChannel < E_NMB_GEN_PWM_CQM; iChannel++)
        {
            strCmd += QString::number(lineInputGenPwmCqm[iChannel]->text().toInt(), 16).rightJustified(3 * 2, '0');
            SaveData.append(QString("%1").arg(QString::number(lineInputGenPwmCqm[iChannel]->text().toInt())) + " ");
        }

        emit SaveTestCqmFreq(SaveData);
        emit SendV200specific(strCmd);
    });
}

void widgetConfig::ReadAdcData(QString type, QString device, int index, QString data)
{
    int dw_nmbChannels = 0;

    if(device == c_nameGen)
    {
        if(index == E_GEN_ADC_1)
        {
            dw_nmbChannels = E_NMB_GEN_ADC1;
        }
        else if(index == E_GEN_ADC_2)
        {
            dw_nmbChannels = E_NMB_GEN_ADC2;
        }
        else if(index == E_GEN_ADC_3)
        {
            dw_nmbChannels = E_NMB_GEN_ADC3;
        }
        else if(index == E_GEN_ADC_OTH)
        {
            dw_nmbChannels = E_NMB_GEN_OTHERS;
        }
    }
    else if(device == c_nameAmp)
    {
        if(index == E_AMP_ADC_1)
        {
            dw_nmbChannels = E_NMB_AMP_ADC1;
        }
        else if(index == E_AMP_ADC_3)
        {
            dw_nmbChannels = E_NMB_AMP_ADC3;
        }
    }

    QStringList arrListSaved;
    arrListSaved = data.split(QRegExp("\\s+"));

    if((arrListSaved.count() - 1) == dw_nmbChannels)
    {
        for(int iChannel = 0; iChannel < dw_nmbChannels; iChannel++)
        {
            if(device == c_nameGen)
            {
                if(type == c_nameAdd)
                {
                    lineInputGenAddAdcx[index][iChannel]->setText(arrListSaved.at(iChannel));
                }
                else if(type == c_nameMul)
                {
                    lineInputGenMulAdcx[index][iChannel]->setText(arrListSaved.at(iChannel));
                }
            }
            else if(device == c_nameAmp)
            {
                if(type == c_nameAdd)
                {
                    lineInputAmpAddAdcx[index][iChannel]->setText(arrListSaved.at(iChannel));
                }
                else if(type == c_nameMul)
                {
                    lineInputAmpMulAdcx[index][iChannel]->setText(arrListSaved.at(iChannel));
                }
            }

        }
    }
    else
    {

    }

}

void widgetConfig::ReadRegulator(QString data)
{
    QStringList arrListSaved;
    arrListSaved = data.split(QRegExp("\\s+"));

    if((arrListSaved.count() - 1) == E_NMB_GEN_REGULATOR)
    {
        for(int iLoop = 0; iLoop < E_NMB_GEN_REGULATOR - 1; iLoop++)
        {
            lineInputGenRegulator[iLoop]->setText(arrListSaved.at(iLoop));
        }

        lineInputGenRegulator[E_NMB_GEN_REGULATOR - 1]->setText(arrListSaved.at(E_NMB_GEN_REGULATOR - 1));
    }
    else
    {

    }
}

void widgetConfig::ReadTestTherapy(QString data)
{
    QStringList arrListSaved;
    arrListSaved = data.split(QRegExp("\\s+"));

    if((arrListSaved.count() - 1) == E_NMB_GEN_TESTTHERAPY)
    {
        for(int iLoop = 0; iLoop < E_NMB_GEN_TESTTHERAPY; iLoop++)
        {
            lineInputGenTestTherapy[iLoop]->setText(arrListSaved.at(iLoop));
        }
    }
    else
    {

    }
}

void widgetConfig::ReadTestCqmFreq(QString data)
{
    QStringList arrListSaved;
    arrListSaved = data.split(QRegExp("\\s+"));

    if((arrListSaved.count() - 1) == E_NMB_GEN_PWM_CQM)
    {
        for(int iLoop = 0; iLoop < E_NMB_GEN_PWM_CQM; iLoop++)
        {
            lineInputGenPwmCqm[iLoop]->setText(arrListSaved.at(iLoop));
        }
    }
    else
    {

    }
}

QLabel *widgetConfig::createNewLabel(const QString &text)
{
    QLabel* newLabel = new QLabel(text);
    newLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    //newLabel->setMinimumHeight(10);
    //newLabel->setStyleSheet("border: 1px solid red");
    return newLabel;
}

QGroupBox *widgetConfig::createConfigAmpGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Amplifier"));
    QGridLayout* childLayout = new QGridLayout();
    int dw_LengthLineEdit = 50;



    for(int iDevice = 0; iDevice < E_AMP_ADC_NMB; iDevice++)
    {
        if(iDevice == E_AMP_ADC_3)
        {
            childLayout->addWidget(createNewLabel("AMP_ADC_3 multiple"), 3 * iDevice + 1, 0, Qt::AlignCenter);
            childLayout->addWidget(createNewLabel("AMP_ADC_3 additive"), 3 * iDevice + 2, 0, Qt::AlignCenter);
        }
        else
        {
            childLayout->addWidget(createNewLabel("AMP_ADC_1 multiple"), 3 * iDevice + 1, 0, Qt::AlignCenter);
            childLayout->addWidget(createNewLabel("AMP_ADC_1 additive"), 3 * iDevice + 2, 0, Qt::AlignCenter);
        }

        int nmbChannels;

        if(iDevice == E_AMP_ADC_1)
        {
            nmbChannels = E_NMB_AMP_ADC1;
        }
        else if(iDevice == E_AMP_ADC_3)
        {
            nmbChannels = E_NMB_AMP_ADC3;
        }
        else
        {
            qDebug() << "error: unsupported ADC device";
            return groupBox;
        }

        for(int iLoop = 0; iLoop < nmbChannels; iLoop++)
        {
            //childLayout->addWidget(createNewLabel(QString("%1").arg(QString::number(iLoop + 1))), 3 * iDevice, 1 + iLoop, Qt::AlignCenter);
            childLayout->addWidget(createNewLabel(QString("%1").arg(allAdxSignalsAmplf[NMB_ITEMS_TIMERS_AMPLF - 1 - 2 * iDevice].at(iLoop + 1))), 3 * iDevice, 1 + iLoop, Qt::AlignCenter);

            lineInputAmpMulAdcx[iDevice][iLoop] = new QLineEdit();
            lineInputAmpMulAdcx[iDevice][iLoop]->setMaximumWidth(dw_LengthLineEdit);
            childLayout->addWidget(lineInputAmpMulAdcx[iDevice][iLoop], 3 * iDevice + 1, 1 + iLoop, Qt::AlignCenter);

            lineInputAmpAddAdcx[iDevice][iLoop] = new QLineEdit();
            lineInputAmpAddAdcx[iDevice][iLoop]->setMaximumWidth(dw_LengthLineEdit);
            childLayout->addWidget(lineInputAmpAddAdcx[iDevice][iLoop], 3 * iDevice + 2, 1 + iLoop, Qt::AlignCenter);
        }

        buttSendAmpMulAdcx[iDevice] = new QPushButton("Send");
        childLayout->addWidget(buttSendAmpMulAdcx[iDevice], 3 * iDevice + 1, nmbChannels + 1, Qt::AlignCenter);

        buttSendAmpAddAdcx[iDevice] = new QPushButton("Send");
        childLayout->addWidget(buttSendAmpAddAdcx[iDevice], 3 * iDevice + 2, nmbChannels + 1, Qt::AlignCenter);
    }

    groupBox->setLayout(childLayout);

    return groupBox;
}

QGroupBox *widgetConfig::createConfigGenGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Generator"));
    QGridLayout* childLayout = new QGridLayout();
    int dw_GridOffsetColumn;
    int dw_LengthLineEdit = 50;

    for(int iDevice = 0; iDevice < E_GEN_ADC_NMB; iDevice++)
    {
        if(iDevice == E_GEN_ADC_OTH)
        {
            childLayout->addWidget(createNewLabel("OTHERS multiple"), 3 * iDevice + 1, 0, Qt::AlignCenter);
            childLayout->addWidget(createNewLabel("OTHERS additive"), 3 * iDevice + 2, 0, Qt::AlignCenter);
        }
        else
        {
            childLayout->addWidget(createNewLabel(QString("GEN_ADC_%1 multiple").arg(QString::number(iDevice + 1))), 3 * iDevice + 1, 0, Qt::AlignCenter);
            childLayout->addWidget(createNewLabel(QString("GEN_ADC_%1 additive").arg(QString::number(iDevice + 1))), 3 * iDevice + 2, 0, Qt::AlignCenter);
        }

        int nmbChannels;

        if(iDevice == E_GEN_ADC_1)
        {
            nmbChannels = E_NMB_GEN_ADC1;
        }
        else if(iDevice == E_GEN_ADC_2)
        {
            nmbChannels = E_NMB_GEN_ADC2;
        }
        else if(iDevice == E_GEN_ADC_3)
        {
            nmbChannels = E_NMB_GEN_ADC3;
        }
        else if(iDevice == E_GEN_ADC_OTH)
        {
            nmbChannels = E_NMB_GEN_OTHERS;
        }
        else
        {
            qDebug() << "error: unsupported ADC device";
            return groupBox;
        }

        for(int iLoop = 0; iLoop < nmbChannels; iLoop++)
        {
            if(iDevice == E_GEN_ADC_OTH)
            {
                childLayout->addWidget(createNewLabel(QString("%1").arg(coeffsOthersNameAdditive[iLoop])), 3 * iDevice, 1 + iLoop, Qt::AlignCenter);
            }
            else
            {
                childLayout->addWidget(createNewLabel(QString("%1").arg(allAdxSignalsGener[NMB_ITEMS_TIMERS_GENER - 1 - 2 * iDevice].at(iLoop + 1))), 3 * iDevice, 1 + iLoop, Qt::AlignCenter);
            }

            lineInputGenMulAdcx[iDevice][iLoop] = new QLineEdit();
            lineInputGenMulAdcx[iDevice][iLoop]->setMaximumWidth(dw_LengthLineEdit);
            childLayout->addWidget(lineInputGenMulAdcx[iDevice][iLoop], 3 * iDevice + 1, 1 + iLoop, Qt::AlignCenter);

            lineInputGenAddAdcx[iDevice][iLoop] = new QLineEdit();
            lineInputGenAddAdcx[iDevice][iLoop]->setMaximumWidth(dw_LengthLineEdit);
            childLayout->addWidget(lineInputGenAddAdcx[iDevice][iLoop], 3 * iDevice + 2, 1 + iLoop, Qt::AlignCenter);
        }

        buttSendGenMulAdcx[iDevice] = new QPushButton("Send");
        childLayout->addWidget(buttSendGenMulAdcx[iDevice], 3 * iDevice + 1, nmbChannels + 1, Qt::AlignCenter);

        buttSendGenAddAdcx[iDevice] = new QPushButton("Send");
        childLayout->addWidget(buttSendGenAddAdcx[iDevice], 3 * iDevice + 2, nmbChannels + 1, Qt::AlignCenter);
    }

    dw_GridOffsetColumn = E_GEN_ADC_NMB * 3;

    childLayout->addWidget(createNewLabel("Regulator"), dw_GridOffsetColumn + 1, 0, Qt::AlignCenter);

    for(int iLoop = 0; iLoop < E_NMB_GEN_REGULATOR; iLoop++)
    {
        childLayout->addWidget(createNewLabel(c_nameGenRegulator[iLoop]), dw_GridOffsetColumn, 1 + iLoop, Qt::AlignCenter);
        lineInputGenRegulator[iLoop] = new QLineEdit();
        lineInputGenRegulator[iLoop]->setMaximumWidth(dw_LengthLineEdit);
        childLayout->addWidget(lineInputGenRegulator[iLoop], dw_GridOffsetColumn + 1, 1 + iLoop, Qt::AlignCenter);
    }

    buttSendGenRegulator = new QPushButton("Send");
    childLayout->addWidget(buttSendGenRegulator, dw_GridOffsetColumn + 1, E_NMB_GEN_REGULATOR + 1, Qt::AlignCenter);
    dw_GridOffsetColumn += 2;


    childLayout->addWidget(createNewLabel("Test Therapy"), dw_GridOffsetColumn + 1, 0, Qt::AlignCenter);

    for(int iLoop = 0; iLoop < E_NMB_GEN_TESTTHERAPY; iLoop++)
    {
        childLayout->addWidget(createNewLabel(c_nameGenTestTherapy[iLoop]), dw_GridOffsetColumn, 1 + iLoop, Qt::AlignCenter);
        lineInputGenTestTherapy[iLoop] = new QLineEdit();
        lineInputGenTestTherapy[iLoop]->setMaximumWidth(dw_LengthLineEdit);
        childLayout->addWidget(lineInputGenTestTherapy[iLoop], dw_GridOffsetColumn + 1, 1 + iLoop, Qt::AlignCenter);
    }

    buttSendGenTestTherapy = new QPushButton("Send");
    childLayout->addWidget(buttSendGenTestTherapy, dw_GridOffsetColumn + 1, E_NMB_GEN_TESTTHERAPY + 1, Qt::AlignCenter);
    dw_GridOffsetColumn += 2;





    childLayout->addWidget(createNewLabel("Pwm CQM frequency"), dw_GridOffsetColumn + 1, 0, Qt::AlignCenter);

    for(int iLoop = 0; iLoop < E_NMB_GEN_PWM_CQM; iLoop++)
    {
        childLayout->addWidget(createNewLabel(c_nameGenCQM[iLoop]), dw_GridOffsetColumn, 1 + iLoop, Qt::AlignCenter);
        lineInputGenPwmCqm[iLoop] = new QLineEdit();
        lineInputGenPwmCqm[iLoop]->setMaximumWidth(dw_LengthLineEdit);
        childLayout->addWidget(lineInputGenPwmCqm[iLoop], dw_GridOffsetColumn + 1, 1 + iLoop, Qt::AlignCenter);
    }

    buttSendGenPwmCqm = new QPushButton("Send");
    childLayout->addWidget(buttSendGenPwmCqm, dw_GridOffsetColumn + 1, E_NMB_GEN_PWM_CQM + 1, Qt::AlignCenter);
    dw_GridOffsetColumn += 2;


    groupBox->setLayout(childLayout);

    return groupBox;
}

void widgetConfig::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);

    QPainter painter(this);

}
