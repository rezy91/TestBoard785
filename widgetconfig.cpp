#include "widgetconfig.h"
#include "common.h"

widgetConfig::widgetConfig(QWidget *parent) : QWidget(parent)
{
    /*
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::green);
    setAutoFillBackground(true);
    setPalette(pal);*/

    MainLayout->addWidget(createConfigurationGroup());
    MainLayout->addWidget(createSettingsGroup());


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

            QString strCmd = QString("%1").arg(QString::number(dw_PidMsg, 16));

            for(int iChannel = 0; iChannel < dw_nmbChannels; iChannel++)
            {
                float flNmb = lineInputGenMulAdcx[iDevice][iChannel]->text().toFloat();
                QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
                QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
                strCmd += QString(arrFloatInArraysHexa);
            }

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

            QString strCmd = QString("%1").arg(QString::number(dw_PidMsg, 16));

            for(int iChannel = 0; iChannel < dw_nmbChannels; iChannel++)
            {
                float flNmb = lineInputGenAddAdcx[iDevice][iChannel]->text().toFloat();
                QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
                QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
                strCmd += QString(arrFloatInArraysHexa);
            }

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

            QString strCmd = QString("%1").arg(QString::number(dw_PidMsg, 16));

            for(int iChannel = 0; iChannel < dw_nmbChannels; iChannel++)
            {
                float flNmb = lineInputAmpMulAdcx[iDevice][iChannel]->text().toFloat();
                QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
                QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
                strCmd += QString(arrFloatInArraysHexa);
            }

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

            QString strCmd = QString("%1").arg(QString::number(dw_PidMsg, 16));

            for(int iChannel = 0; iChannel < dw_nmbChannels; iChannel++)
            {
                float flNmb = lineInputAmpAddAdcx[iDevice][iChannel]->text().toFloat();
                QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
                QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
                strCmd += QString(arrFloatInArraysHexa);
            }

            emit SendV200specific(strCmd);
        });
    }

    connect(buttSendGenRegulator,&QPushButton::clicked,[=](bool clicked){

        Q_UNUSED(clicked);

        QString strCmd = QString("%1").arg(QString::number(PID_SET_REGULATOR, 16));

        for(int iChannel = 0; iChannel < E_NMB_GEN_REGULATOR - 1; iChannel++)
        {
            float flNmb = lineInputGenRegulator[iChannel]->text().toFloat();
            QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
            QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
            strCmd += QString(arrFloatInArraysHexa);
        }

        strCmd += QString::number(lineInputGenRegulator[E_NMB_GEN_REGULATOR - 1]->text().toInt(), 16).rightJustified(2 * 2, '0');

        emit SendV200specific(strCmd);
    });

    connect(buttSendGenTestTherapy,&QPushButton::clicked,[=](bool clicked){

        Q_UNUSED(clicked);

        QString strCmd = QString("%1").arg(QString::number(PID_SET_THERAPY_TEST, 16));

        for(int iChannel = 0; iChannel < E_NMB_GEN_TESTTHERAPY; iChannel++)
        {
            strCmd += QString::number(lineInputGenTestTherapy[iChannel]->text().toInt(), 16).rightJustified(1 * 2, '0');
        }

        emit SendV200specific(strCmd);
    });

    connect(buttSendAmpFreq,&QPushButton::clicked,[=](bool clicked){

        Q_UNUSED(clicked);

        QString strCmd = QString("%1").arg(QString::number(PID_SEND_AMP_SET_FREQUENCY, 16));
        strCmd += QString::number(lineInputAmpFreq->text().toInt(), 16).rightJustified(3 * 2, '0');

        emit SendV200specific(strCmd);
    });

    connect(buttSendAmpPwm,&QPushButton::clicked,[=](bool clicked){

        Q_UNUSED(clicked);

        QString strCmd = QString("%1").arg(QString::number(PID_SEND_AMP_SET_PWM, 16));
        uint16_t w_ModifiedParameter = uint16_t((std::numeric_limits<uint16_t>::max() * lineInputAmpPwm->text().toDouble()) / 100);
        strCmd += QString::number(w_ModifiedParameter, 16).rightJustified(2 * 2, '0');

        emit SendV200specific(strCmd);
    });

    connect(buttSendGenPwmCqm,&QPushButton::clicked,[=](bool clicked){

        Q_UNUSED(clicked);

        QString strCmd = QString("%1").arg(QString::number(PID_SET_PWM_CQMS_FREQ, 16));

        for(int iChannel = 0; iChannel < E_NMB_GEN_PWM_CQM; iChannel++)
        {
            strCmd += QString::number(lineInputGenPwmCqm[iChannel]->text().toInt(), 16).rightJustified(3 * 2, '0');
        }

        emit SendV200specific(strCmd);
    });

    connect(buttSendGenPwmCool,&QPushButton::clicked,[=](bool clicked){

        Q_UNUSED(clicked);

        QString strCmd = QString("%1").arg(QString::number(PID_SET_PWM_COOLS_DUTY, 16));

        for(int iChannel = 0; iChannel < E_NMB_GEN_PWM_COOL; iChannel++)
        {
            strCmd += QString::number(lineInputGenPwmCool[iChannel]->text().toInt(), 16).rightJustified(1 * 2, '0');
        }

        emit SendV200specific(strCmd);
    });

    connect(buttSendGenDac,&QPushButton::clicked,[=](bool clicked){

        Q_UNUSED(clicked);

        QString strCmd = QString("%1").arg(QString::number(PID_SET_DAC, 16));

        for(int iChannel = 0; iChannel < E_NMB_GEN_DAC; iChannel++)
        {
            strCmd += QString::number(lineInputGenDac[iChannel]->text().toInt(), 16).rightJustified(2 * 2, '0');
        }

        emit SendV200specific(strCmd);
    });

    connect(buttSendGenPwrReset,&QPushButton::clicked,[=](bool clicked){

        Q_UNUSED(clicked);
        AssemblyAplxAndPwrResetPacket(true);
    });

    for(int iDevice = 0; iDevice < E_NMB_AMP_OUTPUTS; iDevice++)
    {
        connect(checkAmpOutput[iDevice], &QCheckBox::stateChanged, [=](int checked){

            Q_UNUSED(checked);
            QString strCmd = QString("%1").arg(QString::number(PID_SET_AMP_OUTPUTS, 16));

            for(int jDevice = 0; jDevice < E_NMB_AMP_OUTPUTS; jDevice++)
            {
                strCmd += (checkAmpOutput[jDevice]->checkState() == Qt::Checked ? "01" : "00");
            }

            emit SendV200specific(strCmd);
        });
    }

    for(int iDevice = 0; iDevice < E_NMB_GEN_OUTPUTS; iDevice++)
    {
        connect(checkGenOutputs[iDevice], &QCheckBox::stateChanged, [=](int checked){

            Q_UNUSED(checked);
            QString strCmd = QString("%1").arg(QString::number(PID_SET_OUTPUTS, 16));

            for(int jDevice = 0; jDevice < E_NMB_GEN_OUTPUTS; jDevice++)
            {
                strCmd += (checkGenOutputs[jDevice]->checkState() == Qt::Checked ? "01" : "00");
            }

            emit SendV200specific(strCmd);
        });
    }

    for(int iDevice = 0; iDevice < E_NMB_GEN_APLx; iDevice++)
    {
        connect(checkGenAplX[iDevice], &QCheckBox::stateChanged, [=](int checked){

            Q_UNUSED(checked);
            AssemblyAplxAndPwrResetPacket(false);
        });
    }


    connect(checkGenRawData[0], &QCheckBox::stateChanged, [=](int checked){

        Q_UNUSED(checked);
        QString strCmd = QString("%1").arg(QString::number(PID_SEND_ADCS_RAW_DATA, 16));

        if(checkGenRawData[0]->checkState() == Qt::Checked)
        {
            strCmd += "010000";
            checkGenRawData[1]->setEnabled(false);
            checkGenRawData[2]->setEnabled(false);
        }
        else
        {
            strCmd += "000000";
            checkGenRawData[1]->setEnabled(true);
            checkGenRawData[2]->setEnabled(true);
        }

        emit SendV200specific(strCmd);
    });

    connect(checkGenRawData[1], &QCheckBox::stateChanged, [=](int checked){

        Q_UNUSED(checked);
        QString strCmd = QString("%1").arg(QString::number(PID_SEND_ADCS_RAW_DATA, 16));

        if(checkGenRawData[1]->checkState() == Qt::Checked)
        {
            strCmd += "000100";
            checkGenRawData[0]->setEnabled(false);
            checkGenRawData[2]->setEnabled(false);
        }
        else
        {
            strCmd += "000000";
            checkGenRawData[0]->setEnabled(true);
            checkGenRawData[2]->setEnabled(true);
        }

        emit SendV200specific(strCmd);
    });

    connect(checkGenRawData[2], &QCheckBox::stateChanged, [=](int checked){

        Q_UNUSED(checked);
        QString strCmd = QString("%1").arg(QString::number(PID_SEND_ADCS_RAW_DATA, 16));

        if(checkGenRawData[2]->checkState() == Qt::Checked)
        {
            strCmd += "000001";
            checkGenRawData[1]->setEnabled(false);
            checkGenRawData[0]->setEnabled(false);
        }
        else
        {
            strCmd += "000000";
            checkGenRawData[1]->setEnabled(true);
            checkGenRawData[0]->setEnabled(true);
        }

        emit SendV200specific(strCmd);
    });

    connect(checkGenTestTherapy, &QPushButton::clicked, [=](){

        QString strCmd = QString("%1").arg(QString::number(PID_START_STOP_THERAPY, 16));
        strCmd += (checkGenTestTherapy->checkState() == Qt::Checked ? "0001" : "0000");

        emit SendV200specific(strCmd);
    });
}

QLabel *widgetConfig::createNewLabel(const QString &text)
{
    QLabel* newLabel = new QLabel(text);
    newLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    //newLabel->setMinimumHeight(10);
    //newLabel->setStyleSheet("border: 1px solid red");
    return newLabel;
}

QGroupBox *widgetConfig::createConfigurationGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Configuration"));
    QVBoxLayout* layout = new QVBoxLayout();

    layout->addWidget(createConfigAmpGroup());
    layout->addWidget(createConfigGenGroup());

    groupBox->setLayout(layout);

    return groupBox;
}

QGroupBox *widgetConfig::createSettingsGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Settings"));
    QVBoxLayout* layout = new QVBoxLayout();

    layout->addWidget(createSettingsAmpGroup());
    layout->addWidget(createSettingsGenGroup());

    groupBox->setLayout(layout);

    return groupBox;
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
    int dw_HeightWidget = 20;

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
            lineInputGenMulAdcx[iDevice][iLoop]->setMinimumHeight(dw_HeightWidget);
            childLayout->addWidget(lineInputGenMulAdcx[iDevice][iLoop], 3 * iDevice + 1, 1 + iLoop, Qt::AlignCenter);

            lineInputGenAddAdcx[iDevice][iLoop] = new QLineEdit();
            lineInputGenAddAdcx[iDevice][iLoop]->setMaximumWidth(dw_LengthLineEdit);
            lineInputGenAddAdcx[iDevice][iLoop]->setMinimumHeight(dw_HeightWidget);
            childLayout->addWidget(lineInputGenAddAdcx[iDevice][iLoop], 3 * iDevice + 2, 1 + iLoop, Qt::AlignCenter);
        }

        buttSendGenMulAdcx[iDevice] = new QPushButton("Send");
        buttSendGenMulAdcx[iDevice]->setMinimumHeight(dw_HeightWidget);
        childLayout->addWidget(buttSendGenMulAdcx[iDevice], 3 * iDevice + 1, nmbChannels + 1, Qt::AlignCenter);

        buttSendGenAddAdcx[iDevice] = new QPushButton("Send");
        buttSendGenAddAdcx[iDevice]->setMinimumHeight(dw_HeightWidget);
        childLayout->addWidget(buttSendGenAddAdcx[iDevice], 3 * iDevice + 2, nmbChannels + 1, Qt::AlignCenter);
    }

    dw_GridOffsetColumn = E_GEN_ADC_NMB * 3;

    childLayout->addWidget(createNewLabel("Regulator"), dw_GridOffsetColumn + 1, 0, Qt::AlignCenter);

    for(int iLoop = 0; iLoop < E_NMB_GEN_REGULATOR; iLoop++)
    {
        childLayout->addWidget(createNewLabel(c_nameGenRegulator[iLoop]), dw_GridOffsetColumn, 1 + iLoop, Qt::AlignCenter);
        lineInputGenRegulator[iLoop] = new QLineEdit();
        lineInputGenRegulator[iLoop]->setMinimumHeight(dw_HeightWidget);
        lineInputGenRegulator[iLoop]->setMaximumWidth(dw_LengthLineEdit);
        childLayout->addWidget(lineInputGenRegulator[iLoop], dw_GridOffsetColumn + 1, 1 + iLoop, Qt::AlignCenter);
    }

    buttSendGenRegulator = new QPushButton("Send");
    buttSendGenRegulator->setMinimumHeight(dw_HeightWidget);
    childLayout->addWidget(buttSendGenRegulator, dw_GridOffsetColumn + 1, E_NMB_GEN_REGULATOR + 1, Qt::AlignCenter);
    dw_GridOffsetColumn += 2;


    childLayout->addWidget(createNewLabel("Test Therapy"), dw_GridOffsetColumn + 1, 0, Qt::AlignCenter);

    for(int iLoop = 0; iLoop < E_NMB_GEN_TESTTHERAPY; iLoop++)
    {
        childLayout->addWidget(createNewLabel(c_nameGenTestTherapy[iLoop]), dw_GridOffsetColumn, 1 + iLoop, Qt::AlignCenter);
        lineInputGenTestTherapy[iLoop] = new QLineEdit();
        lineInputGenTestTherapy[iLoop]->setMinimumHeight(dw_HeightWidget);
        lineInputGenTestTherapy[iLoop]->setMaximumWidth(dw_LengthLineEdit);
        childLayout->addWidget(lineInputGenTestTherapy[iLoop], dw_GridOffsetColumn + 1, 1 + iLoop, Qt::AlignCenter);
    }

    buttSendGenTestTherapy = new QPushButton("Send");
    buttSendGenTestTherapy->setMinimumHeight(dw_HeightWidget);
    childLayout->addWidget(buttSendGenTestTherapy, dw_GridOffsetColumn + 1, E_NMB_GEN_TESTTHERAPY + 1, Qt::AlignCenter);
    dw_GridOffsetColumn += 2;





    childLayout->addWidget(createNewLabel("Pwm CQM frequency"), dw_GridOffsetColumn + 1, 0, Qt::AlignCenter);

    for(int iLoop = 0; iLoop < E_NMB_GEN_PWM_CQM; iLoop++)
    {
        childLayout->addWidget(createNewLabel(c_nameGenCQM[iLoop]), dw_GridOffsetColumn, 1 + iLoop, Qt::AlignCenter);
        lineInputGenPwmCqm[iLoop] = new QLineEdit();
        lineInputGenPwmCqm[iLoop]->setMinimumHeight(dw_HeightWidget);
        lineInputGenPwmCqm[iLoop]->setMaximumWidth(dw_LengthLineEdit);
        childLayout->addWidget(lineInputGenPwmCqm[iLoop], dw_GridOffsetColumn + 1, 1 + iLoop, Qt::AlignCenter);
    }

    buttSendGenPwmCqm = new QPushButton("Send");
    buttSendGenPwmCqm->setMinimumHeight(dw_HeightWidget);
    childLayout->addWidget(buttSendGenPwmCqm, dw_GridOffsetColumn + 1, E_NMB_GEN_PWM_CQM + 1, Qt::AlignCenter);
    dw_GridOffsetColumn += 2;


    groupBox->setLayout(childLayout);

    return groupBox;
}

QGroupBox *widgetConfig::createSettingsAmpGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Amplifier"));
    QGridLayout* childLayout = new QGridLayout();
    int dw_GridOffsetColumn = 0;
    int dw_LengthLineEdit = 50;


    //Frequency
    childLayout->addWidget(createNewLabel("Frequency (resolution is 100 Hz)"), 0, dw_GridOffsetColumn, Qt::AlignCenter);

    lineInputAmpFreq = new QLineEdit();
    lineInputAmpFreq->setMaximumWidth(dw_LengthLineEdit);
    buttSendAmpFreq = new QPushButton("Send");

    childLayout->addWidget(lineInputAmpFreq, 1, dw_GridOffsetColumn, Qt::AlignCenter);
    childLayout->addWidget(buttSendAmpFreq, 2, dw_GridOffsetColumn, Qt::AlignCenter);
    dw_GridOffsetColumn++;

    //Pwm
    childLayout->addWidget(createNewLabel("Pwm (from 0.0 to 100.0 %)"), 0, dw_GridOffsetColumn, Qt::AlignCenter);

    lineInputAmpPwm = new QLineEdit();
    lineInputAmpPwm->setMaximumWidth(dw_LengthLineEdit);
    buttSendAmpPwm = new QPushButton("Send");

    childLayout->addWidget(lineInputAmpPwm, 1, dw_GridOffsetColumn, Qt::AlignCenter);
    childLayout->addWidget(buttSendAmpPwm, 2, dw_GridOffsetColumn, Qt::AlignCenter);
    dw_GridOffsetColumn++;

    //Outputs
    for(int iLoop = 0; iLoop < E_NMB_AMP_OUTPUTS; iLoop++)
    {
        childLayout->addWidget(createNewLabel(c_nameAmpOutputs[iLoop]), 0, dw_GridOffsetColumn + iLoop, Qt::AlignCenter);
        checkAmpOutput[iLoop] = new QCheckBox();
        checkAmpOutput[iLoop]->setStyleSheet(QString("QCheckBox::indicator { width: %1px; height:%1px;}").arg(c_dw_SizeCheckBox));
        childLayout->addWidget(checkAmpOutput[iLoop], 1, dw_GridOffsetColumn + iLoop, Qt::AlignCenter);
    }

    groupBox->setLayout(childLayout);

    return groupBox;
}

QGroupBox *widgetConfig::createSettingsGenGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Generator"));
    QGridLayout* childLayout = new QGridLayout();
    int dw_GridOffsetColumn = 0;
    int dw_GridOffsetRow = 0;
    int dw_LengthLineEdit = 50;


    //Pwm
    childLayout->addWidget(createNewLabel("Pwm cooling (0 - 100 %)"), 0, dw_GridOffsetColumn, Qt::AlignCenter);

    for(int iLoop = 0; iLoop < E_NMB_GEN_PWM_COOL; iLoop++)
    {
        lineInputGenPwmCool[iLoop] = new QLineEdit();
        lineInputGenPwmCool[iLoop]->setMaximumWidth(dw_LengthLineEdit);
        childLayout->addWidget(lineInputGenPwmCool[iLoop], 1 + iLoop, dw_GridOffsetColumn, Qt::AlignCenter);
    }

    buttSendGenPwmCool = new QPushButton("Send");
    childLayout->addWidget(buttSendGenPwmCool, E_NMB_GEN_PWM_COOL + 1, dw_GridOffsetColumn, Qt::AlignCenter);
    dw_GridOffsetColumn++;

    //Dac
    childLayout->addWidget(createNewLabel("Dac cooling (0 - 4095)"), 0, dw_GridOffsetColumn, Qt::AlignCenter);

    for(int iLoop = 0; iLoop < E_NMB_GEN_DAC; iLoop++)
    {
        lineInputGenDac[iLoop] = new QLineEdit();
        lineInputGenDac[iLoop]->setMaximumWidth(dw_LengthLineEdit);
        childLayout->addWidget(lineInputGenDac[iLoop], 1 + iLoop, dw_GridOffsetColumn, Qt::AlignCenter);
    }

    buttSendGenDac = new QPushButton("Send");
    childLayout->addWidget(buttSendGenDac, E_NMB_GEN_DAC + 1, dw_GridOffsetColumn, Qt::AlignCenter);
    dw_GridOffsetColumn++;

    //Pwr reset
    childLayout->addWidget(createNewLabel("Pwr reset (0 - 100 us)"), 0, dw_GridOffsetColumn, Qt::AlignCenter);

    lineInputGenPwrReset = new QLineEdit();
    lineInputGenPwrReset->setMaximumWidth(dw_LengthLineEdit);
    buttSendGenPwrReset = new QPushButton("Send");

    childLayout->addWidget(lineInputGenPwrReset, 1, dw_GridOffsetColumn, Qt::AlignCenter);
    childLayout->addWidget(buttSendGenPwrReset, 2, dw_GridOffsetColumn, Qt::AlignCenter);
    dw_GridOffsetColumn++;

    //Raw data
    for(int iLoop = 0; iLoop < E_NMB_GEN_RAW_RECEIVING; iLoop++)
    {
        childLayout->addWidget(createNewLabel(QString("receive raw ADC_%1").arg(QString::number(iLoop + 1))), 0, dw_GridOffsetColumn + iLoop, Qt::AlignCenter);
        checkGenRawData[iLoop] = new QCheckBox();
        checkGenRawData[iLoop]->setStyleSheet(QString("QCheckBox::indicator { width: %1px; height:%1px;}").arg(c_dw_SizeCheckBox));
        childLayout->addWidget(checkGenRawData[iLoop], 1, dw_GridOffsetColumn + iLoop, Qt::AlignCenter);
    }
    dw_GridOffsetRow += 2;

    //Outputs
    for(int iLoop = 0; iLoop < E_NMB_GEN_OUTPUTS; iLoop++)
    {
        childLayout->addWidget(createNewLabel(c_nameGenOutputs[iLoop]), dw_GridOffsetRow, dw_GridOffsetColumn + iLoop, Qt::AlignCenter);
        checkGenOutputs[iLoop] = new QCheckBox();
        checkGenOutputs[iLoop]->setStyleSheet(QString("QCheckBox::indicator { width: %1px; height:%1px;}").arg(c_dw_SizeCheckBox));
        childLayout->addWidget(checkGenOutputs[iLoop], dw_GridOffsetRow + 1, dw_GridOffsetColumn + iLoop, Qt::AlignCenter);
    }
    dw_GridOffsetRow += 2;

    //AplX
    for(int iLoop = 0; iLoop < E_NMB_GEN_APLx; iLoop++)
    {
        childLayout->addWidget(createNewLabel(c_nameGenAplX[iLoop]), dw_GridOffsetRow, dw_GridOffsetColumn + iLoop, Qt::AlignCenter);
        checkGenAplX[iLoop] = new QCheckBox();
        checkGenAplX[iLoop]->setStyleSheet(QString("QCheckBox::indicator { width: %1px; height:%1px;}").arg(c_dw_SizeCheckBox));
        childLayout->addWidget(checkGenAplX[iLoop], dw_GridOffsetRow + 1, dw_GridOffsetColumn + iLoop, Qt::AlignCenter);
    }

    //TestTherapy
    childLayout->addWidget(createNewLabel("Test therapy"), dw_GridOffsetRow, dw_GridOffsetColumn + E_NMB_GEN_OUTPUTS, Qt::AlignCenter);
    checkGenTestTherapy = new QCheckBox();
    checkGenTestTherapy->setStyleSheet(QString("QCheckBox::indicator { width: %1px; height:%1px;}").arg(c_dw_SizeCheckBox));
    childLayout->addWidget(checkGenTestTherapy, dw_GridOffsetRow + 1, dw_GridOffsetColumn + E_NMB_GEN_OUTPUTS, Qt::AlignCenter);

    groupBox->setLayout(childLayout);

    return groupBox;
}

void widgetConfig::AssemblyAplxAndPwrResetPacket(bool source)
{
    QString strCmd = QString("%1").arg(QString::number(PID_SET_APLS, 16));

    for(int jDevice = 0; jDevice < E_NMB_GEN_APLx; jDevice++)
    {
        strCmd += (checkGenAplX[jDevice]->checkState() == Qt::Checked ? "01" : "00");

        if(jDevice == 0)
        {
            QString strHexNumber;

            if(source)
            {
                strHexNumber = QString::number(lineInputGenPwrReset->text().toInt(), 16);
            }
            else
            {
                strHexNumber = QString::number(0);
            }
            strCmd += strHexNumber.rightJustified(1 * 2, '0');

        }
    }

    emit SendV200specific(strCmd);
}

void widgetConfig::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);

    QPainter painter(this);

}
