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
            qDebug() << "input:" << lineInputGenMulAdcx[iDevice][0]->text();

            float flNmb = lineInputGenMulAdcx[iDevice][0]->text().toFloat();

            qDebug() << "float:" << flNmb;
        });
    }
}

QLabel *widgetConfig::createNewLabel(const QString &text)
{
    QLabel* newLabel = new QLabel(text);
    //newLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
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
            childLayout->addWidget(createNewLabel("AMP_ADC_1 additive"), 3 * iDevice + 1, 0, Qt::AlignCenter);
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
        childLayout->addWidget(createNewLabel(QString("%1").arg(QString::number(iLoop + 1))), dw_GridOffsetColumn, 1 + iLoop, Qt::AlignCenter);
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
        childLayout->addWidget(createNewLabel(QString("%1").arg(QString::number(iLoop + 1))), dw_GridOffsetColumn, 1 + iLoop, Qt::AlignCenter);
        lineInputGenTestTherapy[iLoop] = new QLineEdit();
        lineInputGenTestTherapy[iLoop]->setMinimumHeight(dw_HeightWidget);
        lineInputGenTestTherapy[iLoop]->setMaximumWidth(dw_LengthLineEdit);
        childLayout->addWidget(lineInputGenTestTherapy[iLoop], dw_GridOffsetColumn + 1, 1 + iLoop, Qt::AlignCenter);
    }

    buttSendGenTestTherapy = new QPushButton("Send");
    buttSendGenTestTherapy->setMinimumHeight(dw_HeightWidget);
    childLayout->addWidget(buttSendGenTestTherapy, dw_GridOffsetColumn + 1, E_NMB_GEN_TESTTHERAPY + 1, Qt::AlignCenter);

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
    childLayout->addWidget(createNewLabel("Frequency"), 0, dw_GridOffsetColumn, Qt::AlignCenter);

    lineInputAmpFreq = new QLineEdit();
    lineInputAmpFreq->setMaximumWidth(dw_LengthLineEdit);
    buttSendAmpFreq = new QPushButton("Send");

    childLayout->addWidget(lineInputAmpFreq, 1, dw_GridOffsetColumn, Qt::AlignCenter);
    childLayout->addWidget(buttSendAmpFreq, 2, dw_GridOffsetColumn, Qt::AlignCenter);
    dw_GridOffsetColumn++;

    //Pwm
    childLayout->addWidget(createNewLabel("Pwm"), 0, dw_GridOffsetColumn, Qt::AlignCenter);

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
    childLayout->addWidget(createNewLabel("Pwm cooling"), 0, dw_GridOffsetColumn, Qt::AlignCenter);

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
    childLayout->addWidget(createNewLabel("Dac cooling"), 0, dw_GridOffsetColumn, Qt::AlignCenter);

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
    childLayout->addWidget(createNewLabel("Pwr reset"), 0, dw_GridOffsetColumn, Qt::AlignCenter);

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

void widgetConfig::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);

    QPainter painter(this);

}
