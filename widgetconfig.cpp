#include "widgetconfig.h"

#include <QFileDialog>
#include <QTextStream>
#include <QCoreApplication>

widgetConfig::widgetConfig(QWidget *parent) : QWidget(parent)
{
    MainLayout->addWidget(createConfigAmpGroup());
    MainLayout->addWidget(createConfigGenGroup());


    for(int iLoop = 0; iLoop < E_NMB_BUTTONS; iLoop++)
    {
        connect(buttConfigGener[iLoop], &QPushButton::clicked, [=](bool clicked){

            Q_UNUSED(clicked);

            if(iLoop == E_BUTTON_LOAD_MCU)
            {
                QString strCmd;

                strCmd = QString("%1").arg(QString::number(PID_CONFIGURATION_DEVICE, 16));
                strCmd += QString::number(255, 16).rightJustified(2, '0');
                strCmd += QString::number(CONFIG_READ, 16).rightJustified(2, '0');
                strCmd += QString::number(E_CFG_TYPE_RF_ADC1_MUL, 16).rightJustified(2, '0');
                emit SendV200specific(strCmd, true);

                strCmd = QString("%1").arg(QString::number(PID_CONFIGURATION_DEVICE, 16));
                strCmd += QString::number(255, 16).rightJustified(2, '0');
                strCmd += QString::number(CONFIG_READ, 16).rightJustified(2, '0');
                strCmd += QString::number(E_CFG_TYPE_RF_ADC1_ADD, 16).rightJustified(2, '0');
                emit SendV200specific(strCmd, true);

                strCmd = QString("%1").arg(QString::number(PID_CONFIGURATION_DEVICE, 16));
                strCmd += QString::number(255, 16).rightJustified(2, '0');
                strCmd += QString::number(CONFIG_READ, 16).rightJustified(2, '0');
                strCmd += QString::number(E_CFG_TYPE_RF_ADC2_MUL, 16).rightJustified(2, '0');
                emit SendV200specific(strCmd, true);

                strCmd = QString("%1").arg(QString::number(PID_CONFIGURATION_DEVICE, 16));
                strCmd += QString::number(255, 16).rightJustified(2, '0');
                strCmd += QString::number(CONFIG_READ, 16).rightJustified(2, '0');
                strCmd += QString::number(E_CFG_TYPE_RF_ADC2_ADD, 16).rightJustified(2, '0');
                emit SendV200specific(strCmd, true);

                strCmd = QString("%1").arg(QString::number(PID_CONFIGURATION_DEVICE, 16));
                strCmd += QString::number(255, 16).rightJustified(2, '0');
                strCmd += QString::number(CONFIG_READ, 16).rightJustified(2, '0');
                strCmd += QString::number(E_CFG_TYPE_RF_ADC3_MUL, 16).rightJustified(2, '0');
                emit SendV200specific(strCmd, true);

                strCmd = QString("%1").arg(QString::number(PID_CONFIGURATION_DEVICE, 16));
                strCmd += QString::number(255, 16).rightJustified(2, '0');
                strCmd += QString::number(CONFIG_READ, 16).rightJustified(2, '0');
                strCmd += QString::number(E_CFG_TYPE_RF_ADC3_ADD, 16).rightJustified(2, '0');
                emit SendV200specific(strCmd, true);

                strCmd = QString("%1").arg(QString::number(PID_CONFIGURATION_DEVICE, 16));
                strCmd += QString::number(255, 16).rightJustified(2, '0');
                strCmd += QString::number(CONFIG_READ, 16).rightJustified(2, '0');
                strCmd += QString::number(E_CFG_TYPE_RF_OTHERS, 16).rightJustified(2, '0');
                emit SendV200specific(strCmd, true);

                strCmd = QString("%1").arg(QString::number(PID_CONFIGURATION_DEVICE, 16));
                strCmd += QString::number(255, 16).rightJustified(2, '0');
                strCmd += QString::number(CONFIG_READ, 16).rightJustified(2, '0');
                strCmd += QString::number(E_CFG_TYPE_RF_REG_PWR, 16).rightJustified(2, '0');
                emit SendV200specific(strCmd, true);

                strCmd = QString("%1").arg(QString::number(PID_CONFIGURATION_DEVICE, 16));
                strCmd += QString::number(255, 16).rightJustified(2, '0');
                strCmd += QString::number(CONFIG_READ, 16).rightJustified(2, '0');
                strCmd += QString::number(E_CFG_TYPE_RF_REG_COOL, 16).rightJustified(2, '0');
                emit SendV200specific(strCmd, true);

                strCmd = QString("%1").arg(QString::number(PID_CONFIGURATION_DEVICE, 16));
                strCmd += QString::number(255, 16).rightJustified(2, '0');
                strCmd += QString::number(CONFIG_READ, 16).rightJustified(2, '0');
                strCmd += QString::number(E_CFG_TYPE_RF_TEST_THERAPY, 16).rightJustified(2, '0');
                emit SendV200specific(strCmd, true);

                strCmd = QString("%1").arg(QString::number(PID_CONFIGURATION_DEVICE, 16));
                strCmd += QString::number(255, 16).rightJustified(2, '0');
                strCmd += QString::number(CONFIG_READ, 16).rightJustified(2, '0');
                strCmd += QString::number(E_CFG_TYPE_RF_CQM_PWM, 16).rightJustified(2, '0');
                emit SendV200specific(strCmd, true);

            }
            else if(iLoop == E_BUTTON_LOAD_FILE)
            {
                loadFile();
            }
            else if(iLoop == E_BUTTON_SAVE_MCU)
            {
                saveMcuRf();
            }
            else if(iLoop == E_BUTTON_SAVE_FILE)
            {
                saveFile();
            }

        });
    }

    for(int iDevice = 0; iDevice < E_GEN_ADC_NMB; iDevice++)
    {
        connect(buttSendGenMulAdcx[iDevice],&QPushButton::clicked,[=](bool clicked){

            Q_UNUSED(clicked);

            QString strCmd = QString("%1").arg(QString::number(PID_CONFIGURATION_DEVICE, 16));
            strCmd += QString::number(255, 16).rightJustified(2, '0');
            strCmd += QString::number(CONFIG_WRITE, 16).rightJustified(2, '0');

            if(iDevice == E_GEN_ADC_1)
            {
                strCmd += QString::number(E_CFG_TYPE_RF_ADC1_MUL, 16).rightJustified(2, '0');
                ReadLineEditAndAddToMsg(strCmd, E_CFG_TYPE_RF_ADC1_MUL);
            }
            else if(iDevice == E_GEN_ADC_2)
            {
                strCmd += QString::number(E_CFG_TYPE_RF_ADC2_MUL, 16).rightJustified(2, '0');
                ReadLineEditAndAddToMsg(strCmd, E_CFG_TYPE_RF_ADC2_MUL);
            }
            else if(iDevice == E_GEN_ADC_3)
            {
                strCmd += QString::number(E_CFG_TYPE_RF_ADC3_MUL, 16).rightJustified(2, '0');
                ReadLineEditAndAddToMsg(strCmd, E_CFG_TYPE_RF_ADC3_MUL);
            }

            emit SendV200specific(strCmd, false);
        });

        connect(buttSendGenAddAdcx[iDevice],&QPushButton::clicked,[=](bool clicked){

            Q_UNUSED(clicked);

            QString strCmd = QString("%1").arg(QString::number(PID_CONFIGURATION_DEVICE, 16));
            strCmd += QString::number(255, 16).rightJustified(2, '0');
            strCmd += QString::number(CONFIG_WRITE, 16).rightJustified(2, '0');

            if(iDevice == E_GEN_ADC_1)
            {
                strCmd += QString::number(E_CFG_TYPE_RF_ADC1_ADD, 16).rightJustified(2, '0');
                ReadLineEditAndAddToMsg(strCmd, E_CFG_TYPE_RF_ADC1_ADD);
            }
            else if(iDevice == E_GEN_ADC_2)
            {
                strCmd += QString::number(E_CFG_TYPE_RF_ADC2_ADD, 16).rightJustified(2, '0');
                ReadLineEditAndAddToMsg(strCmd, E_CFG_TYPE_RF_ADC2_ADD);
            }
            else if(iDevice == E_GEN_ADC_3)
            {
                strCmd += QString::number(E_CFG_TYPE_RF_ADC3_ADD, 16).rightJustified(2, '0');
                ReadLineEditAndAddToMsg(strCmd, E_CFG_TYPE_RF_ADC3_ADD);
            }

            emit SendV200specific(strCmd, false);
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
            emit SendV200specific(strCmd, false);
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
            emit SendV200specific(strCmd, false);
        });
    }

    connect(buttSendGenOther,&QPushButton::clicked,[=](bool clicked){

        Q_UNUSED(clicked);

        QString strCmd = QString("%1").arg(QString::number(PID_CONFIGURATION_DEVICE, 16));
        strCmd += QString::number(255, 16).rightJustified(2, '0');
        strCmd += QString::number(CONFIG_WRITE, 16).rightJustified(2, '0');
        strCmd += QString::number(E_CFG_TYPE_RF_OTHERS, 16).rightJustified(2, '0');

        ReadLineEditAndAddToMsg(strCmd, E_CFG_TYPE_RF_OTHERS);

        emit SendV200specific(strCmd, false);
        emit SendReferenceImpedance(lineInputGenOthers[6]->text().toFloat(), lineInputGenOthers[7]->text().toFloat(), lineInputGenOthers[8]->text().toFloat(), lineInputGenOthers[3]->text().toFloat());
    });

    connect(buttSendGenRegulatorPower,&QPushButton::clicked,[=](bool clicked){

        Q_UNUSED(clicked);

        QString strCmd = QString("%1").arg(QString::number(PID_CONFIGURATION_DEVICE, 16));
        strCmd += QString::number(255, 16).rightJustified(2, '0');
        strCmd += QString::number(CONFIG_WRITE, 16).rightJustified(2, '0');
        strCmd += QString::number(E_CFG_TYPE_RF_REG_PWR, 16).rightJustified(2, '0');

        ReadLineEditAndAddToMsg(strCmd, E_CFG_TYPE_RF_REG_PWR);

        emit SendV200specific(strCmd, false);
    });

    connect(buttSendGenRegulatorCooling,&QPushButton::clicked,[=](bool clicked){

        Q_UNUSED(clicked);

        QString strCmd = QString("%1").arg(QString::number(PID_CONFIGURATION_DEVICE, 16));
        strCmd += QString::number(255, 16).rightJustified(2, '0');
        strCmd += QString::number(CONFIG_WRITE, 16).rightJustified(2, '0');
        strCmd += QString::number(E_CFG_TYPE_RF_REG_COOL, 16).rightJustified(2, '0');

        ReadLineEditAndAddToMsg(strCmd, E_CFG_TYPE_RF_REG_COOL);

        emit SendV200specific(strCmd, false);
    });

    connect(buttSendGenTestTherapy,&QPushButton::clicked,[=](bool clicked){

        Q_UNUSED(clicked);

        QString strCmd = QString("%1").arg(QString::number(PID_CONFIGURATION_DEVICE, 16));
        strCmd += QString::number(255, 16).rightJustified(2, '0');
        strCmd += QString::number(CONFIG_WRITE, 16).rightJustified(2, '0');
        strCmd += QString::number(E_CFG_TYPE_RF_TEST_THERAPY, 16).rightJustified(2, '0');

        ReadLineEditAndAddToMsg(strCmd, E_CFG_TYPE_RF_TEST_THERAPY);

        emit SendV200specific(strCmd, false);
    });

    connect(buttSendGenPwmCqm,&QPushButton::clicked,[=](bool clicked){

        Q_UNUSED(clicked);

        QString strCmd = QString("%1").arg(QString::number(PID_CONFIGURATION_DEVICE, 16));
        strCmd += QString::number(255, 16).rightJustified(2, '0');
        strCmd += QString::number(CONFIG_WRITE, 16).rightJustified(2, '0');
        strCmd += QString::number(E_CFG_TYPE_RF_CQM_PWM, 16).rightJustified(2, '0');

        ReadLineEditAndAddToMsg(strCmd, E_CFG_TYPE_RF_CQM_PWM);

        emit SendV200specific(strCmd, false);
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
        for(int iChannel = 0; iChannel < dw_nmbChannels; iChannel++)
        {
            if(device == c_nameGen)
            {
                if(type == c_nameAdd)
                {
                    lineInputGenAddAdcx[index][iChannel]->setText(c_defaultValueGenAdcAdd[index][iChannel]);
                }
                else if(type == c_nameMul)
                {
                    lineInputGenMulAdcx[index][iChannel]->setText(c_defaultValueGenAdcMul[index][iChannel]);
                }
            }
            else if(device == c_nameAmp)
            {
                if(type == c_nameAdd)
                {
                    lineInputAmpAddAdcx[index][iChannel]->setText(c_defaultValueAmpAdcAdd[index][iChannel]);
                }
                else if(type == c_nameMul)
                {
                    lineInputAmpMulAdcx[index][iChannel]->setText(c_defaultValueAmpAdcMul[index][iChannel]);
                }
            }
        }
    }
}

void widgetConfig::ReadConfigGenerNew(QByteArray data)
{
    uint8_t byDevice = uint8_t(data.at(1));
    uint8_t byDirection = uint8_t(data.at(2));
    uint8_t byType = uint8_t(data.at(3));

    if(byDevice == 255)
    {
        if(byDirection == CONFIG_READ)
        {
            if(byType == E_CFG_TYPE_RF_ADC1_MUL)
            {
                for(int iLoop = 0; iLoop < E_NMB_GEN_ADC1; iLoop++)
                {
                    float fValue =  DecodeUint32ToFloat(data.mid(iLoop * 4 + 1 + 4));
                    lineInputGenMulAdcx[E_GEN_ADC_1][iLoop]->setText(QString::number(fValue));
                }
            }
            else if(byType == E_CFG_TYPE_RF_ADC1_ADD)
            {
                for(int iLoop = 0; iLoop < E_NMB_GEN_ADC1; iLoop++)
                {
                    float fValue =  DecodeUint32ToFloat(data.mid(iLoop * 4 + 1 + 4));
                    lineInputGenAddAdcx[E_GEN_ADC_1][iLoop]->setText(QString::number(fValue));
                }
            }
            else if(byType == E_CFG_TYPE_RF_ADC2_MUL)
            {
                for(int iLoop = 0; iLoop < E_NMB_GEN_ADC2; iLoop++)
                {
                    float fValue =  DecodeUint32ToFloat(data.mid(iLoop * 4 + 1 + 4));
                    lineInputGenMulAdcx[E_GEN_ADC_2][iLoop]->setText(QString::number(fValue));
                }
            }
            else if(byType == E_CFG_TYPE_RF_ADC2_ADD)
            {
                for(int iLoop = 0; iLoop < E_NMB_GEN_ADC2; iLoop++)
                {
                    float fValue =  DecodeUint32ToFloat(data.mid(iLoop * 4 + 1 + 4));
                    lineInputGenAddAdcx[E_GEN_ADC_2][iLoop]->setText(QString::number(fValue));
                }
            }
            else if(byType == E_CFG_TYPE_RF_ADC3_MUL)
            {
                for(int iLoop = 0; iLoop < E_NMB_GEN_ADC3; iLoop++)
                {
                    float fValue =  DecodeUint32ToFloat(data.mid(iLoop * 4 + 1 + 4));
                    lineInputGenMulAdcx[E_GEN_ADC_3][iLoop]->setText(QString::number(fValue));
                }
            }
            else if(byType == E_CFG_TYPE_RF_ADC3_ADD)
            {
                for(int iLoop = 0; iLoop < E_NMB_GEN_ADC3; iLoop++)
                {
                    float fValue =  DecodeUint32ToFloat(data.mid(iLoop * 4 + 1 + 4));
                    lineInputGenAddAdcx[E_GEN_ADC_3][iLoop]->setText(QString::number(fValue));
                }
            }
            else if(byType == E_CFG_TYPE_RF_OTHERS)
            {
                for(int iLoop = 0; iLoop < E_NMB_GEN_OTHERS; iLoop++)
                {
                    float fValue =  DecodeUint32ToFloat(data.mid(iLoop * 4 + 1 + 4));
                    lineInputGenOthers[iLoop]->setText(QString::number(fValue));
                }
            }
            else if(byType == E_CFG_TYPE_RF_REG_PWR)
            {
                for(int iLoop = 0; iLoop < E_NMB_GEN_REGULATOR_POWER; iLoop++)
                {
                    if(iLoop < 3)
                    {
                        float fValue =  DecodeUint32ToFloat(data.mid(iLoop * 4 + 1 + 4));
                        lineInputGenRegulatorPower[iLoop]->setText(QString::number(fValue));
                    }
                    else
                    {
                       uint32_t dwDecodedArray = DecodeBytesToUint32(data.mid(iLoop * 4 + 1 + 4));
                       lineInputGenRegulatorPower[iLoop]->setText(QString::number(dwDecodedArray));
                    }
                }
            }
            else if(byType == E_CFG_TYPE_RF_REG_COOL)
            {
                for(int iLoop = 0; iLoop < E_NMB_GEN_REGULATOR_COOLING; iLoop++)
                {
                    if(iLoop < 3)
                    {
                        float fValue =  DecodeUint32ToFloat(data.mid(iLoop * 4 + 1 + 4));
                        lineInputGenRegulatorCooling[iLoop]->setText(QString::number(fValue));
                    }
                    else
                    {
                       uint32_t dwDecodedArray = DecodeBytesToUint32(data.mid(iLoop * 4 + 1 + 4));
                       lineInputGenRegulatorCooling[iLoop]->setText(QString::number(dwDecodedArray));
                    }
                }
            }
            else if(byType == E_CFG_TYPE_RF_TEST_THERAPY)
            {
                for(int iLoop = 0; iLoop < E_NMB_GEN_TESTTHERAPY; iLoop++)
                {
                    uint32_t dwDecodedArray = DecodeBytesToUint32(data.mid(iLoop * 4 + 1 + 4));
                    lineInputGenTestTherapy[iLoop]->setText(QString::number(dwDecodedArray));
                }

            }
            else if(byType == E_CFG_TYPE_RF_CQM_PWM)
            {
                for(int iLoop = 0; iLoop < E_NMB_GEN_PWM_CQM; iLoop++)
                {
                    uint32_t dwDecodedArray = DecodeBytesToUint32(data.mid(iLoop * 4 + 1 + 4));
                    lineInputGenPwmCqm[iLoop]->setText(QString::number(dwDecodedArray));
                }
            }
        }
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
    QVBoxLayout* vBox = new QVBoxLayout();
    QHBoxLayout* hBox = new QHBoxLayout();
    QGroupBox *groupBox = new QGroupBox(tr("Amplifier"));
    QGridLayout* childLayout = new QGridLayout();
    int dw_LengthLineEdit = 50;

    for(int iLoop = 0; iLoop < E_NMB_BUTTONS; iLoop++)
    {
        buttConfigAmplf[iLoop] = new QPushButton(QString("%1").arg(c_nameButtonConfigAmplf[iLoop]));
        hBox->addWidget(buttConfigAmplf[iLoop]);
    }

    for(int iDevice = 0; iDevice < E_AMP_ADC_NMB; iDevice++)
    {
        if(iDevice == E_AMP_ADC_3)
        {
            childLayout->addWidget(createNewLabel("ADC_3 multiple"), 3 * iDevice + 1, 0, Qt::AlignCenter);
            childLayout->addWidget(createNewLabel("ADC_3 additive"), 3 * iDevice + 2, 0, Qt::AlignCenter);
        }
        else
        {
            childLayout->addWidget(createNewLabel("ADC_1 multiple"), 3 * iDevice + 1, 0, Qt::AlignCenter);
            childLayout->addWidget(createNewLabel("ADC_1 additive"), 3 * iDevice + 2, 0, Qt::AlignCenter);
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

    vBox->addLayout(hBox);
    vBox->addLayout(childLayout);
    groupBox->setLayout(vBox);

    return groupBox;
}

QGroupBox *widgetConfig::createConfigGenGroup()
{
    QVBoxLayout* vBox = new QVBoxLayout();
    QHBoxLayout* hBox = new QHBoxLayout();
    QGroupBox *groupBox = new QGroupBox(tr("Generator"));
    QGridLayout* childLayout = new QGridLayout();
    int dw_GridOffsetColumn;
    int dw_LengthLineEdit = 50;

    for(int iLoop = 0; iLoop < E_NMB_BUTTONS; iLoop++)
    {
        buttConfigGener[iLoop] = new QPushButton(QString("%1").arg(c_nameButtonConfigGener[iLoop]));
        hBox->addWidget(buttConfigGener[iLoop]);
    }

    for(int iDevice = 0; iDevice < E_GEN_ADC_NMB; iDevice++)
    {
        childLayout->addWidget(createNewLabel(QString("ADC_%1 multiple").arg(QString::number(iDevice + 1))), 3 * iDevice + 1, 0, Qt::AlignCenter);
        childLayout->addWidget(createNewLabel(QString("ADC_%1 additive").arg(QString::number(iDevice + 1))), 3 * iDevice + 2, 0, Qt::AlignCenter);

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
        else
        {
            qDebug() << "error: unsupported ADC device";
            return groupBox;
        }

        for(int iLoop = 0; iLoop < nmbChannels; iLoop++)
        {
            childLayout->addWidget(createNewLabel(QString("%1").arg(allAdxSignalsGener[NMB_ITEMS_TIMERS_GENER - 1 - 2 * iDevice].at(iLoop + 1))), 3 * iDevice, 1 + iLoop, Qt::AlignCenter);

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




    childLayout->addWidget(createNewLabel("Others"), dw_GridOffsetColumn + 1, 0, Qt::AlignCenter);

    for(int iLoop = 0; iLoop < E_NMB_GEN_OTHERS; iLoop++)
    {
        childLayout->addWidget(createNewLabel(c_nameOthers[iLoop]), dw_GridOffsetColumn, 1 + iLoop, Qt::AlignCenter);
        lineInputGenOthers[iLoop] = new QLineEdit();
        lineInputGenOthers[iLoop]->setMaximumWidth(dw_LengthLineEdit);
        childLayout->addWidget(lineInputGenOthers[iLoop], dw_GridOffsetColumn + 1, 1 + iLoop, Qt::AlignCenter);
    }

    buttSendGenOther = new QPushButton("Send");
    childLayout->addWidget(buttSendGenOther, dw_GridOffsetColumn + 1, E_NMB_GEN_OTHERS + 1, Qt::AlignCenter);
    dw_GridOffsetColumn += 2;



    childLayout->addWidget(createNewLabel("Regulator power"), dw_GridOffsetColumn + 1, 0, Qt::AlignCenter);

    for(int iLoop = 0; iLoop < E_NMB_GEN_REGULATOR_POWER; iLoop++)
    {
        childLayout->addWidget(createNewLabel(c_nameGenRegulatorPower[iLoop]), dw_GridOffsetColumn, 1 + iLoop, Qt::AlignCenter);
        lineInputGenRegulatorPower[iLoop] = new QLineEdit();
        lineInputGenRegulatorPower[iLoop]->setMaximumWidth(dw_LengthLineEdit);
        childLayout->addWidget(lineInputGenRegulatorPower[iLoop], dw_GridOffsetColumn + 1, 1 + iLoop, Qt::AlignCenter);
    }

    buttSendGenRegulatorPower = new QPushButton("Send");
    childLayout->addWidget(buttSendGenRegulatorPower, dw_GridOffsetColumn + 1, E_NMB_GEN_REGULATOR_POWER + 1, Qt::AlignCenter);
    dw_GridOffsetColumn += 2;



    childLayout->addWidget(createNewLabel("Regulator cooling"), dw_GridOffsetColumn + 1, 0, Qt::AlignCenter);

    for(int iLoop = 0; iLoop < E_NMB_GEN_REGULATOR_COOLING; iLoop++)
    {
        childLayout->addWidget(createNewLabel(c_nameGenRegulatorCooling[iLoop]), dw_GridOffsetColumn, 1 + iLoop, Qt::AlignCenter);
        lineInputGenRegulatorCooling[iLoop] = new QLineEdit();
        lineInputGenRegulatorCooling[iLoop]->setMaximumWidth(dw_LengthLineEdit);
        childLayout->addWidget(lineInputGenRegulatorCooling[iLoop], dw_GridOffsetColumn + 1, 1 + iLoop, Qt::AlignCenter);
    }

    buttSendGenRegulatorCooling = new QPushButton("Send");
    childLayout->addWidget(buttSendGenRegulatorCooling, dw_GridOffsetColumn + 1, E_NMB_GEN_REGULATOR_COOLING + 1, Qt::AlignCenter);
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


    vBox->addLayout(hBox);
    vBox->addLayout(childLayout);
    groupBox->setLayout(vBox);


    return groupBox;
}

void widgetConfig::loadFile()
{
    QString logPath = QFileDialog::getOpenFileName(this, "Open config file", QString("%1/").arg(QCoreApplication::applicationDirPath()), c_nameTypeFile);
    QFile m_logFile(logPath);

    if(!m_logFile.open(QFile::ReadOnly))
    {
        qDebug() << "error: cannot open file";
    }
    else
    {
        QTextStream fileStream(&m_logFile);

        uint8_t byIndexRow = 0;
        clearLineEdits();

        while (!fileStream.atEnd())
        {
            uint8_t byVolumeParameters;
            QLineEdit **pQline;

            if(byIndexRow == 0)
            {
                byVolumeParameters = E_NMB_GEN_ADC1;
                pQline = lineInputGenMulAdcx[E_GEN_ADC_1];
            }
            else if(byIndexRow == 1)
            {
                byVolumeParameters = E_NMB_GEN_ADC1;
                pQline = lineInputGenAddAdcx[E_GEN_ADC_1];
            }
            else if(byIndexRow == 2)
            {
                byVolumeParameters = E_NMB_GEN_ADC2;
                pQline = lineInputGenMulAdcx[E_GEN_ADC_2];
            }
            else if(byIndexRow == 3)
            {
                byVolumeParameters = E_NMB_GEN_ADC2;
                pQline = lineInputGenAddAdcx[E_GEN_ADC_2];
            }
            else if(byIndexRow == 4)
            {
                byVolumeParameters = E_NMB_GEN_ADC3;
                pQline = lineInputGenMulAdcx[E_GEN_ADC_3];
            }
            else if(byIndexRow == 5)
            {
                byVolumeParameters = E_NMB_GEN_ADC3;
                pQline = lineInputGenAddAdcx[E_GEN_ADC_3];
            }
            else if(byIndexRow == 6)
            {
                byVolumeParameters = E_NMB_GEN_OTHERS;
                pQline = lineInputGenOthers;
            }
            else if(byIndexRow == 7)
            {
                byVolumeParameters = E_NMB_GEN_REGULATOR_POWER;
                pQline = lineInputGenRegulatorPower;
            }
            else if(byIndexRow == 8)
            {
                byVolumeParameters = E_NMB_GEN_REGULATOR_COOLING;
                pQline = lineInputGenRegulatorCooling;
            }
            else if(byIndexRow == 9)
            {
                byVolumeParameters = E_NMB_GEN_TESTTHERAPY;
                pQline = lineInputGenTestTherapy;
            }
            else if(byIndexRow == 10)
            {
                byVolumeParameters = E_NMB_GEN_PWM_CQM;
                pQline = lineInputGenPwmCqm;
            }


            QString newLinereaded = fileStream.readLine();
            QStringList arrListSaved = newLinereaded.split(QRegExp("\\s+"));

            if((arrListSaved.count() - 1) == byVolumeParameters)
            {
                for(int iLoop = 0; iLoop < byVolumeParameters; iLoop++)
                {
                   pQline[iLoop]->setText(arrListSaved.at(iLoop));
                }
            }

            byIndexRow++;
        }
    }

    m_logFile.close();
}

void widgetConfig::saveFile()
{
    QString logPath = QFileDialog::getSaveFileName(this, "Save config file", QString("%1/").arg(QCoreApplication::applicationDirPath()), c_nameTypeFile);
    QFile fileSaveConfig(logPath);

    if(!fileSaveConfig.open(QFile::ReadWrite))
    {
        qDebug() << "error: cannot open file";
    }
    else
    {
        QTextStream fileStream(&fileSaveConfig);

        QString SaveDataMul[E_GEN_ADC_NMB], saveDataAdd[E_GEN_ADC_NMB];
        QString SaveDataOther, SaveDataRegPower, SaveDataRegCooling, SaveDataTestThr, SaveDataPwmCqm;

        for(int iDevice = 0; iDevice < E_GEN_ADC_NMB; iDevice++)
        {
            int dw_nmbChannels;

            if(iDevice == E_GEN_ADC_1)
            {
                dw_nmbChannels = E_NMB_GEN_ADC1;
            }
            else if(iDevice == E_GEN_ADC_2)
            {
                dw_nmbChannels = E_NMB_GEN_ADC2;
            }
            else if(iDevice == E_GEN_ADC_3)
            {
                dw_nmbChannels = E_NMB_GEN_ADC3;
            }

            for(int iChannel = 0; iChannel < dw_nmbChannels; iChannel++)
            {
                float flNmb;

                flNmb = lineInputGenMulAdcx[iDevice][iChannel]->text().toFloat();
                SaveDataMul[iDevice].append(QString("%1").arg(QString::number(flNmb)) + " ");
                flNmb = lineInputGenAddAdcx[iDevice][iChannel]->text().toFloat();
                saveDataAdd[iDevice].append(QString("%1").arg(QString::number(flNmb)) + " ");
            }

            fileStream << SaveDataMul[iDevice] << endl;
            fileStream << saveDataAdd[iDevice] << endl;
        }

        for(int jLoop = 0; jLoop < E_NMB_GEN_OTHERS; jLoop++)
        {
            float flNmb = lineInputGenOthers[jLoop]->text().toFloat();
            SaveDataOther.append(QString("%1").arg(QString::number(flNmb)) + " ");
        }

        for(int jLoop = 0; jLoop < E_NMB_GEN_REGULATOR_POWER; jLoop++)
        {
            float flNmb = lineInputGenRegulatorPower[jLoop]->text().toFloat();
            SaveDataRegPower.append(QString("%1").arg(QString::number(flNmb)) + " ");
        }

        for(int jLoop = 0; jLoop < E_NMB_GEN_REGULATOR_COOLING; jLoop++)
        {
            float flNmb = lineInputGenRegulatorCooling[jLoop]->text().toFloat();
            SaveDataRegCooling.append(QString("%1").arg(QString::number(flNmb)) + " ");
        }

        for(int jLoop = 0; jLoop < E_NMB_GEN_TESTTHERAPY; jLoop++)
        {
            float flNmb = lineInputGenTestTherapy[jLoop]->text().toFloat();
            SaveDataTestThr.append(QString("%1").arg(QString::number(flNmb)) + " ");
        }

        for(int jLoop = 0; jLoop < E_NMB_GEN_PWM_CQM; jLoop++)
        {
            float flNmb = lineInputGenPwmCqm[jLoop]->text().toFloat();
            SaveDataPwmCqm.append(QString("%1").arg(QString::number(flNmb)) + " ");
        }

        fileStream << SaveDataOther << endl;
        fileStream << SaveDataRegPower << endl;
        fileStream << SaveDataRegCooling << endl;
        fileStream << SaveDataTestThr << endl;
        fileStream << SaveDataPwmCqm << endl;
    }

    fileSaveConfig.close();
}

void widgetConfig::saveMcuRf()
{
    QString strCmd = QString("%1").arg(QString::number(PID_CONFIGURATION_DEVICE, 16));
    strCmd += QString::number(255, 16).rightJustified(2, '0');
    strCmd += QString::number(CONFIG_WRITE, 16).rightJustified(2, '0');
    strCmd += QString::number(E_CFG_TYPE_RF_ALL, 16).rightJustified(2, '0');

    ReadLineEditAndAddToMsg(strCmd, E_CFG_TYPE_RF_ADC1_MUL);
    ReadLineEditAndAddToMsg(strCmd, E_CFG_TYPE_RF_ADC1_ADD);
    ReadLineEditAndAddToMsg(strCmd, E_CFG_TYPE_RF_ADC2_MUL);
    ReadLineEditAndAddToMsg(strCmd, E_CFG_TYPE_RF_ADC2_ADD);
    ReadLineEditAndAddToMsg(strCmd, E_CFG_TYPE_RF_ADC3_MUL);
    ReadLineEditAndAddToMsg(strCmd, E_CFG_TYPE_RF_ADC3_ADD);
    ReadLineEditAndAddToMsg(strCmd, E_CFG_TYPE_RF_OTHERS);
    ReadLineEditAndAddToMsg(strCmd, E_CFG_TYPE_RF_REG_PWR);
    ReadLineEditAndAddToMsg(strCmd, E_CFG_TYPE_RF_REG_COOL);
    ReadLineEditAndAddToMsg(strCmd, E_CFG_TYPE_RF_TEST_THERAPY);
    ReadLineEditAndAddToMsg(strCmd, E_CFG_TYPE_RF_CQM_PWM);

    emit SendV200specific(strCmd, false);
}

float widgetConfig::DecodeUint32ToFloat(QByteArray qByArry)
{
    uint32_t dwValue = DecodeBytesToUint32(qByArry);
    float fValue = *(float*)&dwValue;

    return fValue;
}

uint32_t widgetConfig::DecodeBytesToUint32(QByteArray qByArry)
{
    uint32_t dwValue = uint32_t(qByArry.at(0) << 24) & 0xFF000000;
    dwValue |= uint32_t(qByArry.at(1) << 16) & 0x00FF0000;
    dwValue |= uint32_t(qByArry.at(2) << 8) & 0x0000FF00;
    dwValue |= uint32_t(qByArry.at(3)) & 0x000000FF;

    return dwValue;
}

void widgetConfig::ReadLineEditAndAddToMsg(QString &strPacketContent, widgetConfig::CONFIG_TYPES_RF eType)
{
    switch(eType)
    {
    case E_CFG_TYPE_RF_ADC1_MUL:
        ReadAndAddToMsgAdc1Mul(strPacketContent);
        break;
    case E_CFG_TYPE_RF_ADC1_ADD:
        ReadAndAddToMsgAdc1Add(strPacketContent);
        break;
    case E_CFG_TYPE_RF_ADC2_MUL:
        ReadAndAddToMsgAdc2Mul(strPacketContent);
        break;
    case E_CFG_TYPE_RF_ADC2_ADD:
        ReadAndAddToMsgAdc2Add(strPacketContent);
        break;
    case E_CFG_TYPE_RF_ADC3_MUL:
        ReadAndAddToMsgAdc3Mul(strPacketContent);
        break;
    case E_CFG_TYPE_RF_ADC3_ADD:
        ReadAndAddToMsgAdc3Add(strPacketContent);
        break;
    case E_CFG_TYPE_RF_OTHERS:
        ReadAndAddToMsgOthrers(strPacketContent);
        break;
    case E_CFG_TYPE_RF_REG_PWR:
        ReadAndAddToMsgRegPwr(strPacketContent);
        break;
    case E_CFG_TYPE_RF_REG_COOL:
        ReadAndAddToMsgRegCool(strPacketContent);
        break;
    case E_CFG_TYPE_RF_TEST_THERAPY:
        ReadAndAddToMsgTestTher(strPacketContent);
        break;
    case E_CFG_TYPE_RF_CQM_PWM:
        ReadAndAddToMsgCqmPwm(strPacketContent);
        break;
    default:
        break;
    }
}

void widgetConfig::ReadAndAddToMsgAdc1Mul(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < E_NMB_GEN_ADC1; iChannel++)
    {
        float flNmb = lineInputGenMulAdcx[E_GEN_ADC_1][iChannel]->text().toFloat();
        QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
        QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
        strPacketContent += QString(arrFloatInArraysHexa);
    }
}

void widgetConfig::ReadAndAddToMsgAdc1Add(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < E_NMB_GEN_ADC1; iChannel++)
    {
        float flNmb = lineInputGenAddAdcx[E_GEN_ADC_1][iChannel]->text().toFloat();
        QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
        QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
        strPacketContent += QString(arrFloatInArraysHexa);
    }
}

void widgetConfig::ReadAndAddToMsgAdc2Mul(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < E_NMB_GEN_ADC2; iChannel++)
    {
        float flNmb = lineInputGenMulAdcx[E_GEN_ADC_2][iChannel]->text().toFloat();
        QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
        QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
        strPacketContent += QString(arrFloatInArraysHexa);
    }
}

void widgetConfig::ReadAndAddToMsgAdc2Add(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < E_NMB_GEN_ADC2; iChannel++)
    {
        float flNmb = lineInputGenAddAdcx[E_GEN_ADC_2][iChannel]->text().toFloat();
        QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
        QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
        strPacketContent += QString(arrFloatInArraysHexa);
    }
}

void widgetConfig::ReadAndAddToMsgAdc3Mul(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < E_NMB_GEN_ADC3; iChannel++)
    {
        float flNmb = lineInputGenMulAdcx[E_GEN_ADC_3][iChannel]->text().toFloat();
        QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
        QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
        strPacketContent += QString(arrFloatInArraysHexa);
    }
}

void widgetConfig::ReadAndAddToMsgAdc3Add(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < E_NMB_GEN_ADC3; iChannel++)
    {
        float flNmb = lineInputGenAddAdcx[E_GEN_ADC_3][iChannel]->text().toFloat();
        QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
        QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
        strPacketContent += QString(arrFloatInArraysHexa);
    }
}

void widgetConfig::ReadAndAddToMsgOthrers(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < E_NMB_GEN_OTHERS; iChannel++)
    {
        float flNmb = lineInputGenOthers[iChannel]->text().toFloat();
        QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
        QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
        strPacketContent += QString(arrFloatInArraysHexa);
    }
}

void widgetConfig::ReadAndAddToMsgRegCool(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < E_NMB_GEN_REGULATOR_COOLING - 4; iChannel++)
    {
        float flNmb = lineInputGenRegulatorCooling[iChannel]->text().toFloat();
        QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
        QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
        strPacketContent += QString(arrFloatInArraysHexa);
    }

    strPacketContent += QString::number(lineInputGenRegulatorCooling[E_NMB_GEN_REGULATOR_COOLING - 4]->text().toInt(), 16).rightJustified(4 * 2, '0');
    strPacketContent += QString::number(lineInputGenRegulatorCooling[E_NMB_GEN_REGULATOR_COOLING - 3]->text().toInt(), 16).rightJustified(4 * 2, '0');
    strPacketContent += QString::number(lineInputGenRegulatorCooling[E_NMB_GEN_REGULATOR_COOLING - 2]->text().toInt(), 16).rightJustified(4 * 2, '0');
    strPacketContent += QString::number(lineInputGenRegulatorCooling[E_NMB_GEN_REGULATOR_COOLING - 1]->text().toInt(), 16).rightJustified(4 * 2, '0');
}

void widgetConfig::ReadAndAddToMsgRegPwr(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < E_NMB_GEN_REGULATOR_POWER - 1; iChannel++)
    {
        float flNmb = lineInputGenRegulatorPower[iChannel]->text().toFloat();
        QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
        QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
        strPacketContent += QString(arrFloatInArraysHexa);
    }

    strPacketContent += QString::number(lineInputGenRegulatorPower[E_NMB_GEN_REGULATOR_POWER - 1]->text().toInt(), 16).rightJustified(4 * 2, '0');
}

void widgetConfig::ReadAndAddToMsgTestTher(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < E_NMB_GEN_TESTTHERAPY; iChannel++)
    {
        strPacketContent += QString::number(lineInputGenTestTherapy[iChannel]->text().toInt(), 16).rightJustified(4 * 2, '0');
    }
}

void widgetConfig::ReadAndAddToMsgCqmPwm(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < E_NMB_GEN_PWM_CQM; iChannel++)
    {
        strPacketContent += QString::number(lineInputGenPwmCqm[iChannel]->text().toInt(), 16).rightJustified(4 * 2, '0');
    }
}

void widgetConfig::clearLineEdits()
{
    int byVolumeParameters;


    for(int iDevice = 0; iDevice < E_GEN_ADC_NMB; iDevice++)
    {
        if(iDevice == E_GEN_ADC_1)
        {
            byVolumeParameters = E_NMB_GEN_ADC1;
        }
        else if(iDevice == E_GEN_ADC_2)
        {
            byVolumeParameters = E_NMB_GEN_ADC2;
        }
        else if(iDevice == E_GEN_ADC_3)
        {
            byVolumeParameters = E_NMB_GEN_ADC3;
        }

        for(int iChannel = 0; iChannel < byVolumeParameters; iChannel++)
        {
            lineInputGenMulAdcx[iDevice][iChannel]->clear();
            lineInputGenAddAdcx[iDevice][iChannel]->clear();
        }
    }

    for(int jLoop = 0; jLoop < E_NMB_GEN_OTHERS; jLoop++)
    {
        lineInputGenOthers[jLoop]->clear();
    }

    for(int jLoop = 0; jLoop < E_NMB_GEN_REGULATOR_POWER; jLoop++)
    {
        lineInputGenRegulatorPower[jLoop]->clear();
    }

    for(int jLoop = 0; jLoop < E_NMB_GEN_REGULATOR_COOLING; jLoop++)
    {
        lineInputGenRegulatorCooling[jLoop]->clear();
    }

    for(int jLoop = 0; jLoop < E_NMB_GEN_TESTTHERAPY; jLoop++)
    {
        lineInputGenTestTherapy[jLoop]->clear();
    }

    for(int jLoop = 0; jLoop < E_NMB_GEN_PWM_CQM; jLoop++)
    {
        lineInputGenPwmCqm[jLoop]->clear();
    }
}

void widgetConfig::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);

    QPainter painter(this);

}
