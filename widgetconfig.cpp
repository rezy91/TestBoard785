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
                for(int iLoop = 0; iLoop < E_CFG_TYPE_RF_COUNT; iLoop++)
                {
                    QString strCmd;

                    strCmd = QString("%1").arg(QString::number(PID_CONFIGURATION_DEVICE, 16));
                    strCmd += QString::number(255, 16).rightJustified(2, '0');
                    strCmd += QString::number(CONFIG_READ, 16).rightJustified(2, '0');
                    strCmd += QString::number(iLoop, 16).rightJustified(2, '0');
                    emit SendV200specific(strCmd, true);
                }
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


    for(int iLoop = 0; iLoop < E_CFG_TYPE_RF_COUNT; iLoop++)
    {
        connect(buttSendPart[iLoop],&QPushButton::clicked,[=](bool clicked){

            Q_UNUSED(clicked);

            QString strCmd = QString("%1").arg(QString::number(PID_CONFIGURATION_DEVICE, 16));
            strCmd += QString::number(255, 16).rightJustified(2, '0');
            strCmd += QString::number(CONFIG_WRITE, 16).rightJustified(2, '0');
            strCmd += QString::number(iLoop, 16).rightJustified(2, '0');

            ReadLineEditAndAddToMsg(strCmd, CONFIG_TYPES_RF(iLoop));

            qDebug() << strCmd;

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
}

void widgetConfig::ReadAdcData(QString type, QString device, int index, QString data)
{
    int dw_nmbChannels = 0;

    if(device == c_nameAmp)
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
            if(device == c_nameAmp)
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
            if(device == c_nameAmp)
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

void widgetConfig::ReadConfigGener(QByteArray data)
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
                for(int iLoop = 0; iLoop < c_dwVolumeOfPart[E_CFG_TYPE_RF_ADC1_MUL]; iLoop++)
                {
                    float fValue =  DecodeUint32ToFloat(data.mid(iLoop * 4 + 1 + 4));
                    lineInputPart[E_CFG_TYPE_RF_ADC1_MUL][iLoop]->setText(QString::number(fValue));
                }
            }
            else if(byType == E_CFG_TYPE_RF_ADC1_ADD)
            {
                for(int iLoop = 0; iLoop < c_dwVolumeOfPart[E_CFG_TYPE_RF_ADC1_ADD]; iLoop++)
                {
                    float fValue =  DecodeUint32ToFloat(data.mid(iLoop * 4 + 1 + 4));
                    lineInputPart[E_CFG_TYPE_RF_ADC1_ADD][iLoop]->setText(QString::number(fValue));
                }
            }
            else if(byType == E_CFG_TYPE_RF_ADC2_MUL)
            {
                for(int iLoop = 0; iLoop < c_dwVolumeOfPart[E_CFG_TYPE_RF_ADC2_MUL]; iLoop++)
                {
                    float fValue =  DecodeUint32ToFloat(data.mid(iLoop * 4 + 1 + 4));
                    lineInputPart[E_CFG_TYPE_RF_ADC2_MUL][iLoop]->setText(QString::number(fValue));
                }
            }
            else if(byType == E_CFG_TYPE_RF_ADC2_ADD)
            {
                for(int iLoop = 0; iLoop < c_dwVolumeOfPart[E_CFG_TYPE_RF_ADC2_ADD]; iLoop++)
                {
                    float fValue =  DecodeUint32ToFloat(data.mid(iLoop * 4 + 1 + 4));
                    lineInputPart[E_CFG_TYPE_RF_ADC2_ADD][iLoop]->setText(QString::number(fValue));
                }
            }
            else if(byType == E_CFG_TYPE_RF_ADC3_MUL)
            {
                for(int iLoop = 0; iLoop < c_dwVolumeOfPart[E_CFG_TYPE_RF_ADC3_MUL]; iLoop++)
                {
                    float fValue =  DecodeUint32ToFloat(data.mid(iLoop * 4 + 1 + 4));
                    lineInputPart[E_CFG_TYPE_RF_ADC3_MUL][iLoop]->setText(QString::number(fValue));
                }
            }
            else if(byType == E_CFG_TYPE_RF_ADC3_ADD)
            {
                for(int iLoop = 0; iLoop < c_dwVolumeOfPart[E_CFG_TYPE_RF_ADC3_ADD]; iLoop++)
                {
                    float fValue =  DecodeUint32ToFloat(data.mid(iLoop * 4 + 1 + 4));
                    lineInputPart[E_CFG_TYPE_RF_ADC3_ADD][iLoop]->setText(QString::number(fValue));
                }
            }
            else if(byType == E_CFG_TYPE_RF_OTHERS)
            {
                for(int iLoop = 0; iLoop < c_dwVolumeOfPart[E_CFG_TYPE_RF_OTHERS]; iLoop++)
                {
                    float fValue =  DecodeUint32ToFloat(data.mid(iLoop * 4 + 1 + 4));
                    lineInputPart[E_CFG_TYPE_RF_OTHERS][iLoop]->setText(QString::number(fValue));
                }

                emit SendReferenceImpedance(lineInputPart[E_CFG_TYPE_RF_OTHERS][6]->text().toFloat(), lineInputPart[E_CFG_TYPE_RF_OTHERS][7]->text().toFloat(), lineInputPart[E_CFG_TYPE_RF_OTHERS][8]->text().toFloat(), lineInputPart[E_CFG_TYPE_RF_OTHERS][3]->text().toFloat());
            }
            else if(byType == E_CFG_TYPE_RF_REG_PWR)
            {
                for(int iLoop = 0; iLoop < c_dwVolumeOfPart[E_CFG_TYPE_RF_REG_PWR]; iLoop++)
                {
                    if(iLoop < 3)
                    {
                        float fValue =  DecodeUint32ToFloat(data.mid(iLoop * 4 + 1 + 4));
                        lineInputPart[E_CFG_TYPE_RF_REG_PWR][iLoop]->setText(QString::number(fValue));
                    }
                    else
                    {
                       uint32_t dwDecodedArray = DecodeBytesToUint32(data.mid(iLoop * 4 + 1 + 4));
                       lineInputPart[E_CFG_TYPE_RF_REG_PWR][iLoop]->setText(QString::number(dwDecodedArray));
                    }
                }
            }
            else if(byType == E_CFG_TYPE_RF_REG_COOL)
            {
                for(int iLoop = 0; iLoop < c_dwVolumeOfPart[E_CFG_TYPE_RF_REG_COOL]; iLoop++)
                {
                    if(iLoop < 3)
                    {
                        float fValue =  DecodeUint32ToFloat(data.mid(iLoop * 4 + 1 + 4));
                        lineInputPart[E_CFG_TYPE_RF_REG_COOL][iLoop]->setText(QString::number(fValue));
                    }
                    else
                    {
                       uint32_t dwDecodedArray = DecodeBytesToUint32(data.mid(iLoop * 4 + 1 + 4));
                       lineInputPart[E_CFG_TYPE_RF_REG_COOL][iLoop]->setText(QString::number(dwDecodedArray));
                    }
                }
            }
            else if(byType == E_CFG_TYPE_RF_TEST_THERAPY)
            {
                for(int iLoop = 0; iLoop < c_dwVolumeOfPart[E_CFG_TYPE_RF_TEST_THERAPY]; iLoop++)
                {
                    uint32_t dwDecodedArray = DecodeBytesToUint32(data.mid(iLoop * 4 + 1 + 4));
                    lineInputPart[E_CFG_TYPE_RF_TEST_THERAPY][iLoop]->setText(QString::number(dwDecodedArray));
                }

            }
            else if(byType == E_CFG_TYPE_RF_CQM_PWM)
            {
                for(int iLoop = 0; iLoop < c_dwVolumeOfPart[E_CFG_TYPE_RF_CQM_PWM]; iLoop++)
                {
                    uint32_t dwDecodedArray = DecodeBytesToUint32(data.mid(iLoop * 4 + 1 + 4));
                    lineInputPart[E_CFG_TYPE_RF_CQM_PWM][iLoop]->setText(QString::number(dwDecodedArray));
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
    int dw_LengthLineEdit = 50;
    int dw_GridOffsetColumn = 0;

    for(int iLoop = 0; iLoop < E_NMB_BUTTONS; iLoop++)
    {
        buttConfigGener[iLoop] = new QPushButton(QString("%1").arg(c_nameButtonConfigGener[iLoop]));
        hBox->addWidget(buttConfigGener[iLoop]);
    }

    for(int iLoop = 0; iLoop < E_CFG_TYPE_RF_COUNT; iLoop++)
    {
        childLayout->addWidget(createNewLabel(c_nameVolumeOfPart[iLoop]), iLoop * dw_GridOffsetColumn + 1, 0, Qt::AlignCenter);

        for(int jLoop = 0; jLoop < c_dwVolumeOfPart[iLoop]; jLoop++)
        {
            childLayout->addWidget(createNewLabel(c_qstrName[iLoop].at(jLoop)), iLoop * dw_GridOffsetColumn, jLoop + 1, Qt::AlignCenter);
            lineInputPart[iLoop][jLoop] = new QLineEdit();
            lineInputPart[iLoop][jLoop]->setMaximumWidth(dw_LengthLineEdit);
            childLayout->addWidget(lineInputPart[iLoop][jLoop], iLoop * dw_GridOffsetColumn + 1, jLoop + 1, Qt::AlignCenter);
        }

        buttSendPart[iLoop] = new QPushButton("Send");
        childLayout->addWidget(buttSendPart[iLoop], iLoop * dw_GridOffsetColumn + 1, c_dwVolumeOfPart[iLoop] + 1, Qt::AlignCenter);
        dw_GridOffsetColumn += 2;
    }

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

            byVolumeParameters = c_dwVolumeOfPart[byIndexRow];
            pQline = lineInputPart[byIndexRow];

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

        for(int iLoop = 0; iLoop < E_CFG_TYPE_RF_COUNT; iLoop++)
        {
            QString SaveDataInput;

            for(int jLoop = 0; jLoop < c_dwVolumeOfPart[iLoop]; jLoop++)
            {
                float flNmb = lineInputPart[iLoop][jLoop]->text().toFloat();
                SaveDataInput.append(QString("%1").arg(QString::number(flNmb)) + " ");
            }

            fileStream << SaveDataInput << endl;
        }
    }

    fileSaveConfig.close();
}

void widgetConfig::saveMcuRf()
{
    QString strCmd = QString("%1").arg(QString::number(PID_CONFIGURATION_DEVICE, 16));
    strCmd += QString::number(255, 16).rightJustified(2, '0');
    strCmd += QString::number(CONFIG_WRITE, 16).rightJustified(2, '0');
    strCmd += QString::number(E_CFG_TYPE_RF_ALL, 16).rightJustified(2, '0');

    for(int iLoop = 0; iLoop < E_CFG_TYPE_RF_COUNT; iLoop++)
    {
        ReadLineEditAndAddToMsg(strCmd, CONFIG_TYPES_RF(iLoop));
    }

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
    for(int iChannel = 0; iChannel < c_dwVolumeOfPart[E_CFG_TYPE_RF_ADC1_MUL]; iChannel++)
    {
        float flNmb = lineInputPart[E_CFG_TYPE_RF_ADC1_MUL][iChannel]->text().toFloat();
        QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
        QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
        strPacketContent += QString(arrFloatInArraysHexa);
    }
}

void widgetConfig::ReadAndAddToMsgAdc1Add(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < c_dwVolumeOfPart[E_CFG_TYPE_RF_ADC1_ADD]; iChannel++)
    {
        float flNmb = lineInputPart[E_CFG_TYPE_RF_ADC1_ADD][iChannel]->text().toFloat();
        QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
        QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
        strPacketContent += QString(arrFloatInArraysHexa);
    }
}

void widgetConfig::ReadAndAddToMsgAdc2Mul(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < c_dwVolumeOfPart[E_CFG_TYPE_RF_ADC2_MUL]; iChannel++)
    {
        float flNmb = lineInputPart[E_CFG_TYPE_RF_ADC2_MUL][iChannel]->text().toFloat();
        QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
        QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
        strPacketContent += QString(arrFloatInArraysHexa);
    }
}

void widgetConfig::ReadAndAddToMsgAdc2Add(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < c_dwVolumeOfPart[E_CFG_TYPE_RF_ADC2_ADD]; iChannel++)
    {
        float flNmb = lineInputPart[E_CFG_TYPE_RF_ADC2_ADD][iChannel]->text().toFloat();
        QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
        QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
        strPacketContent += QString(arrFloatInArraysHexa);
    }
}

void widgetConfig::ReadAndAddToMsgAdc3Mul(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < c_dwVolumeOfPart[E_CFG_TYPE_RF_ADC3_MUL]; iChannel++)
    {
        float flNmb = lineInputPart[E_CFG_TYPE_RF_ADC3_MUL][iChannel]->text().toFloat();
        QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
        QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
        strPacketContent += QString(arrFloatInArraysHexa);
    }
}

void widgetConfig::ReadAndAddToMsgAdc3Add(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < c_dwVolumeOfPart[E_CFG_TYPE_RF_ADC3_ADD]; iChannel++)
    {
        float flNmb = lineInputPart[E_CFG_TYPE_RF_ADC3_ADD][iChannel]->text().toFloat();
        QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
        QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
        strPacketContent += QString(arrFloatInArraysHexa);
    }
}

void widgetConfig::ReadAndAddToMsgOthrers(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < c_dwVolumeOfPart[E_CFG_TYPE_RF_OTHERS]; iChannel++)
    {
        float flNmb = lineInputPart[E_CFG_TYPE_RF_OTHERS][iChannel]->text().toFloat();
        QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
        QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
        strPacketContent += QString(arrFloatInArraysHexa);
    }

    emit SendReferenceImpedance(lineInputPart[E_CFG_TYPE_RF_OTHERS][6]->text().toFloat(), lineInputPart[E_CFG_TYPE_RF_OTHERS][7]->text().toFloat(), lineInputPart[E_CFG_TYPE_RF_OTHERS][8]->text().toFloat(), lineInputPart[E_CFG_TYPE_RF_OTHERS][3]->text().toFloat());
}

void widgetConfig::ReadAndAddToMsgRegCool(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < c_dwVolumeOfPart[E_CFG_TYPE_RF_REG_COOL] - 4; iChannel++)
    {
        float flNmb = lineInputPart[E_CFG_TYPE_RF_REG_COOL][iChannel]->text().toFloat();
        QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
        QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
        strPacketContent += QString(arrFloatInArraysHexa);
    }

    strPacketContent += QString::number(lineInputPart[E_CFG_TYPE_RF_REG_COOL][c_dwVolumeOfPart[E_CFG_TYPE_RF_REG_COOL] - 4]->text().toInt(), 16).rightJustified(4 * 2, '0');
    strPacketContent += QString::number(lineInputPart[E_CFG_TYPE_RF_REG_COOL][c_dwVolumeOfPart[E_CFG_TYPE_RF_REG_COOL] - 3]->text().toInt(), 16).rightJustified(4 * 2, '0');
    strPacketContent += QString::number(lineInputPart[E_CFG_TYPE_RF_REG_COOL][c_dwVolumeOfPart[E_CFG_TYPE_RF_REG_COOL] - 2]->text().toInt(), 16).rightJustified(4 * 2, '0');
    strPacketContent += QString::number(lineInputPart[E_CFG_TYPE_RF_REG_COOL][c_dwVolumeOfPart[E_CFG_TYPE_RF_REG_COOL] - 1]->text().toInt(), 16).rightJustified(4 * 2, '0');
}

void widgetConfig::ReadAndAddToMsgRegPwr(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < c_dwVolumeOfPart[E_CFG_TYPE_RF_REG_PWR] - 1; iChannel++)
    {
        float flNmb = lineInputPart[E_CFG_TYPE_RF_REG_PWR][iChannel]->text().toFloat();
        QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
        QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
        strPacketContent += QString(arrFloatInArraysHexa);
    }

    strPacketContent += QString::number(lineInputPart[E_CFG_TYPE_RF_REG_PWR][c_dwVolumeOfPart[E_CFG_TYPE_RF_REG_PWR] - 1]->text().toInt(), 16).rightJustified(4 * 2, '0');
}

void widgetConfig::ReadAndAddToMsgTestTher(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < c_dwVolumeOfPart[E_CFG_TYPE_RF_TEST_THERAPY]; iChannel++)
    {
        strPacketContent += QString::number(lineInputPart[E_CFG_TYPE_RF_TEST_THERAPY][iChannel]->text().toInt(), 16).rightJustified(4 * 2, '0');
    }
}

void widgetConfig::ReadAndAddToMsgCqmPwm(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < c_dwVolumeOfPart[E_CFG_TYPE_RF_CQM_PWM]; iChannel++)
    {
        strPacketContent += QString::number(lineInputPart[E_CFG_TYPE_RF_CQM_PWM][iChannel]->text().toInt(), 16).rightJustified(4 * 2, '0');
    }
}

void widgetConfig::clearLineEdits()
{
    for(int iLoop = 0; iLoop < E_CFG_TYPE_RF_COUNT; iLoop++)
    {
        for(int jLoop = 0; jLoop < c_dwVolumeOfPart[iLoop]; jLoop++)
        {
            lineInputPart[iLoop][jLoop]->clear();
        }
    }
}

void widgetConfig::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);

    QPainter painter(this);

}
