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
                    strCmd += QString::number(ACTION_READ, 16).rightJustified(2, '0');
                    strCmd += QString::number(iLoop, 16).rightJustified(2, '0');
                    emit SendV200specific(strCmd, true);
                }
            }
            else if(iLoop == E_BUTTON_LOAD_FILE)
            {
                loadFileGenerRf();
            }
            else if(iLoop == E_BUTTON_SAVE_MCU)
            {
                saveMcuGenerRf();
            }
            else if(iLoop == E_BUTTON_SAVE_FILE)
            {
                saveFileGenerRf();
            }

        });

        connect(buttConfigAmplf[iLoop], &QPushButton::clicked, [=](bool clicked){

            Q_UNUSED(clicked);

            if(iLoop == E_BUTTON_LOAD_MCU)
            {
                for(int iLoop = 0; iLoop < E_CFG_TYPE_AMPLF_COUNT; iLoop++)
                {
                    QString strCmd;

                    strCmd = QString("%1").arg(QString::number(PID_CONFIGURATION_DEVICE, 16));
                    strCmd += QString::number(ACC_AMPLIFIER, 16).rightJustified(2, '0');
                    strCmd += QString::number(ACTION_READ, 16).rightJustified(2, '0');
                    strCmd += QString::number(iLoop, 16).rightJustified(2, '0');
                    strCmd += QString::number(4 * c_dwVolumeOfPartAmplifier[iLoop], 16).rightJustified(2, '0');
                    emit SendV200specific(strCmd, true);
                }
            }
            else if(iLoop == E_BUTTON_LOAD_FILE)
            {
                loadFileAmplifier();
            }
            else if(iLoop == E_BUTTON_SAVE_MCU)
            {
                saveMcuAmplifier();
            }
            else if(iLoop == E_BUTTON_SAVE_FILE)
            {
                saveFileAmplifier();
            }

        });
    }


    for(int iLoop = 0; iLoop < E_CFG_TYPE_RF_COUNT; iLoop++)
    {
        connect(buttSendPartGenerRf[iLoop],&QPushButton::clicked,[=](bool clicked){

            Q_UNUSED(clicked);

            QString strCmd = QString("%1").arg(QString::number(PID_CONFIGURATION_DEVICE, 16));
            strCmd += QString::number(255, 16).rightJustified(2, '0');
            strCmd += QString::number(ACTION_WRITE, 16).rightJustified(2, '0');
            strCmd += QString::number(iLoop, 16).rightJustified(2, '0');

            ReadLineEditAndAddToMsg(strCmd, CONFIG_TYPES_RF(iLoop));

            emit SendV200specific(strCmd, false);
        });
    }

    for(int iLoop = 0; iLoop < E_CFG_TYPE_AMPLF_COUNT; iLoop++)
    {
        connect(buttSendPartAmplifier[iLoop],&QPushButton::clicked,[=](bool clicked){

            Q_UNUSED(clicked);

            ReadAndAddToMsgAmplifier(iLoop);
        });
    }
}

void widgetConfig::ReadConfigGener(QByteArray data)
{
    uint8_t byDevice = uint8_t(data.at(1));
    uint8_t byDirection = uint8_t(data.at(2));
    uint8_t byType = uint8_t(data.at(3));

    if(byDevice == 255)
    {
        if(byDirection == ACTION_READ)
        {
            if(byType == E_CFG_TYPE_RF_ADC1_MUL)
            {
                for(int iLoop = 0; iLoop < c_dwVolumeOfPartGenerRf[E_CFG_TYPE_RF_ADC1_MUL]; iLoop++)
                {
                    float fValue =  DecodeUint32ToFloatClassic(data.mid(iLoop * 4 + 1 + 4));
                    lineInputPartGenerRf[E_CFG_TYPE_RF_ADC1_MUL][iLoop]->setText(QString::number(fValue));
                }
            }
            else if(byType == E_CFG_TYPE_RF_ADC1_ADD)
            {
                for(int iLoop = 0; iLoop < c_dwVolumeOfPartGenerRf[E_CFG_TYPE_RF_ADC1_ADD]; iLoop++)
                {
                    float fValue =  DecodeUint32ToFloatClassic(data.mid(iLoop * 4 + 1 + 4));
                    lineInputPartGenerRf[E_CFG_TYPE_RF_ADC1_ADD][iLoop]->setText(QString::number(fValue));
                }
            }
            else if(byType == E_CFG_TYPE_RF_ADC2_MUL)
            {
                for(int iLoop = 0; iLoop < c_dwVolumeOfPartGenerRf[E_CFG_TYPE_RF_ADC2_MUL]; iLoop++)
                {
                    float fValue =  DecodeUint32ToFloatClassic(data.mid(iLoop * 4 + 1 + 4));
                    lineInputPartGenerRf[E_CFG_TYPE_RF_ADC2_MUL][iLoop]->setText(QString::number(fValue));
                }
            }
            else if(byType == E_CFG_TYPE_RF_ADC2_ADD)
            {
                for(int iLoop = 0; iLoop < c_dwVolumeOfPartGenerRf[E_CFG_TYPE_RF_ADC2_ADD]; iLoop++)
                {
                    float fValue =  DecodeUint32ToFloatClassic(data.mid(iLoop * 4 + 1 + 4));
                    lineInputPartGenerRf[E_CFG_TYPE_RF_ADC2_ADD][iLoop]->setText(QString::number(fValue));
                }
            }
            else if(byType == E_CFG_TYPE_RF_ADC3_MUL)
            {
                for(int iLoop = 0; iLoop < c_dwVolumeOfPartGenerRf[E_CFG_TYPE_RF_ADC3_MUL]; iLoop++)
                {
                    float fValue =  DecodeUint32ToFloatClassic(data.mid(iLoop * 4 + 1 + 4));
                    lineInputPartGenerRf[E_CFG_TYPE_RF_ADC3_MUL][iLoop]->setText(QString::number(fValue));
                }
            }
            else if(byType == E_CFG_TYPE_RF_ADC3_ADD)
            {
                for(int iLoop = 0; iLoop < c_dwVolumeOfPartGenerRf[E_CFG_TYPE_RF_ADC3_ADD]; iLoop++)
                {
                    float fValue =  DecodeUint32ToFloatClassic(data.mid(iLoop * 4 + 1 + 4));
                    lineInputPartGenerRf[E_CFG_TYPE_RF_ADC3_ADD][iLoop]->setText(QString::number(fValue));
                }
            }
            else if(byType == E_CFG_TYPE_RF_OTHERS)
            {
                for(int iLoop = 0; iLoop < c_dwVolumeOfPartGenerRf[E_CFG_TYPE_RF_OTHERS]; iLoop++)
                {
                    float fValue =  DecodeUint32ToFloatClassic(data.mid(iLoop * 4 + 1 + 4));
                    lineInputPartGenerRf[E_CFG_TYPE_RF_OTHERS][iLoop]->setText(QString::number(fValue));
                }

                emit SendReferenceImpedance(lineInputPartGenerRf[E_CFG_TYPE_RF_OTHERS][6]->text().toFloat(), lineInputPartGenerRf[E_CFG_TYPE_RF_OTHERS][7]->text().toFloat(), lineInputPartGenerRf[E_CFG_TYPE_RF_OTHERS][8]->text().toFloat(), lineInputPartGenerRf[E_CFG_TYPE_RF_OTHERS][3]->text().toFloat());
            }
            else if(byType == E_CFG_TYPE_RF_REG_PWR)
            {
                for(int iLoop = 0; iLoop < c_dwVolumeOfPartGenerRf[E_CFG_TYPE_RF_REG_PWR]; iLoop++)
                {
                    if(iLoop < 3)
                    {
                        float fValue =  DecodeUint32ToFloatClassic(data.mid(iLoop * 4 + 1 + 4));
                        lineInputPartGenerRf[E_CFG_TYPE_RF_REG_PWR][iLoop]->setText(QString::number(fValue));
                    }
                    else
                    {
                       uint32_t dwDecodedArray = DecodeBytesToUint32Classic(data.mid(iLoop * 4 + 1 + 4));
                       lineInputPartGenerRf[E_CFG_TYPE_RF_REG_PWR][iLoop]->setText(QString::number(dwDecodedArray));
                    }
                }
            }
            else if(byType == E_CFG_TYPE_RF_REG_COOL)
            {
                for(int iLoop = 0; iLoop < c_dwVolumeOfPartGenerRf[E_CFG_TYPE_RF_REG_COOL]; iLoop++)
                {
                    if(iLoop < 3)
                    {
                        float fValue =  DecodeUint32ToFloatClassic(data.mid(iLoop * 4 + 1 + 4));
                        lineInputPartGenerRf[E_CFG_TYPE_RF_REG_COOL][iLoop]->setText(QString::number(fValue));
                    }
                    else
                    {
                       uint32_t dwDecodedArray = DecodeBytesToUint32Classic(data.mid(iLoop * 4 + 1 + 4));
                       lineInputPartGenerRf[E_CFG_TYPE_RF_REG_COOL][iLoop]->setText(QString::number(dwDecodedArray));
                    }
                }
            }
            else if(byType == E_CFG_TYPE_RF_TEST_THERAPY)
            {
                for(int iLoop = 0; iLoop < c_dwVolumeOfPartGenerRf[E_CFG_TYPE_RF_TEST_THERAPY]; iLoop++)
                {
                    uint32_t dwDecodedArray = DecodeBytesToUint32Classic(data.mid(iLoop * 4 + 1 + 4));
                    lineInputPartGenerRf[E_CFG_TYPE_RF_TEST_THERAPY][iLoop]->setText(QString::number(dwDecodedArray));
                }

            }
            else if(byType == E_CFG_TYPE_RF_CQM_PWM)
            {
                for(int iLoop = 0; iLoop < c_dwVolumeOfPartGenerRf[E_CFG_TYPE_RF_CQM_PWM]; iLoop++)
                {
                    uint32_t dwDecodedArray = DecodeBytesToUint32Classic(data.mid(iLoop * 4 + 1 + 4));
                    lineInputPartGenerRf[E_CFG_TYPE_RF_CQM_PWM][iLoop]->setText(QString::number(dwDecodedArray));
                }
            }
        }
    }
    else if(byDevice == ACC_AMPLIFIER)
    {
        if(byDirection == ACTION_WRITE)
        {

        }
        else if(byDirection == ACTION_READ)
        {
            for(int iLoop = 0; iLoop < c_dwVolumeOfPartAmplifier[byType]; iLoop++)
            {
                float fValue =  DecodeUint32ToFloatAmplifier(data.mid(iLoop * 4 + 1 + 4));
                lineInputPartAmplifier[DeconvertIndexAmplifier(byType)][iLoop]->setText(QString::number(fValue));
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
    int dw_GridOffsetColumn = 0;

    for(int iLoop = 0; iLoop < E_NMB_BUTTONS; iLoop++)
    {
        buttConfigAmplf[iLoop] = new QPushButton(QString("%1").arg(c_nameButtonConfigAmplf[iLoop]));
        hBox->addWidget(buttConfigAmplf[iLoop]);
    }

    for(int iLoop = 0; iLoop < E_CFG_TYPE_AMPLF_COUNT; iLoop++)
    {
        childLayout->addWidget(createNewLabel(c_nameVolumeOfPartAmplifier[iLoop]), iLoop * dw_GridOffsetColumn + 1, 0, Qt::AlignCenter);

        for(int jLoop = 0; jLoop < c_dwVolumeOfPartAmplifier[iLoop]; jLoop++)
        {
            childLayout->addWidget(createNewLabel(c_qstrNameAmplf[iLoop].at(jLoop)), iLoop * dw_GridOffsetColumn, jLoop + 1, Qt::AlignCenter);
            lineInputPartAmplifier[iLoop][jLoop] = new QLineEdit();
            lineInputPartAmplifier[iLoop][jLoop]->setMaximumWidth(dw_LengthLineEdit);
            childLayout->addWidget(lineInputPartAmplifier[iLoop][jLoop], iLoop * dw_GridOffsetColumn + 1, jLoop + 1, Qt::AlignCenter);
        }

        buttSendPartAmplifier[iLoop] = new QPushButton("Send");
        childLayout->addWidget(buttSendPartAmplifier[iLoop], iLoop * dw_GridOffsetColumn + 1, c_dwVolumeOfPartAmplifier[iLoop] + 1, Qt::AlignCenter);
        dw_GridOffsetColumn += 2;
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
        childLayout->addWidget(createNewLabel(c_nameVolumeOfPartGenerRf[iLoop]), iLoop * dw_GridOffsetColumn + 1, 0, Qt::AlignCenter);

        for(int jLoop = 0; jLoop < c_dwVolumeOfPartGenerRf[iLoop]; jLoop++)
        {
            childLayout->addWidget(createNewLabel(c_qstrNameGenerRf[iLoop].at(jLoop)), iLoop * dw_GridOffsetColumn, jLoop + 1, Qt::AlignCenter);
            lineInputPartGenerRf[iLoop][jLoop] = new QLineEdit();
            lineInputPartGenerRf[iLoop][jLoop]->setMaximumWidth(dw_LengthLineEdit);
            childLayout->addWidget(lineInputPartGenerRf[iLoop][jLoop], iLoop * dw_GridOffsetColumn + 1, jLoop + 1, Qt::AlignCenter);
        }

        buttSendPartGenerRf[iLoop] = new QPushButton("Send");
        childLayout->addWidget(buttSendPartGenerRf[iLoop], iLoop * dw_GridOffsetColumn + 1, c_dwVolumeOfPartGenerRf[iLoop] + 1, Qt::AlignCenter);
        dw_GridOffsetColumn += 2;
    }

    vBox->addLayout(hBox);
    vBox->addLayout(childLayout);
    groupBox->setLayout(vBox);

    return groupBox;
}

void widgetConfig::loadFileGenerRf()
{
    QString logPath = QFileDialog::getOpenFileName(this, "Open config file", QString("%1/").arg(QCoreApplication::applicationDirPath()), c_nameTypeFileGenerRf);
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

            byVolumeParameters = c_dwVolumeOfPartGenerRf[byIndexRow];
            pQline = lineInputPartGenerRf[byIndexRow];

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

void widgetConfig::saveFileGenerRf()
{
    QString logPath = QFileDialog::getSaveFileName(this, "Save config file", QString("%1/").arg(QCoreApplication::applicationDirPath()), c_nameTypeFileGenerRf);
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

            for(int jLoop = 0; jLoop < c_dwVolumeOfPartGenerRf[iLoop]; jLoop++)
            {
                float flNmb = lineInputPartGenerRf[iLoop][jLoop]->text().toFloat();
                SaveDataInput.append(QString("%1").arg(QString::number(flNmb)) + " ");
            }

            fileStream << SaveDataInput << endl;
        }
    }

    fileSaveConfig.close();
}

void widgetConfig::saveMcuGenerRf()
{
    QString strCmd = QString("%1").arg(QString::number(PID_CONFIGURATION_DEVICE, 16));
    strCmd += QString::number(255, 16).rightJustified(2, '0');
    strCmd += QString::number(ACTION_WRITE, 16).rightJustified(2, '0');
    strCmd += QString::number(E_CFG_TYPE_RF_ALL, 16).rightJustified(2, '0');

    for(int iLoop = 0; iLoop < E_CFG_TYPE_RF_COUNT; iLoop++)
    {
        ReadLineEditAndAddToMsg(strCmd, CONFIG_TYPES_RF(iLoop));
    }

    emit SendV200specific(strCmd, false);
}

void widgetConfig::loadFileAmplifier()
{
    QString logPath = QFileDialog::getOpenFileName(this, "Open config file", QString("%1/").arg(QCoreApplication::applicationDirPath()), c_nameTypeFileAmplifier);
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

            byVolumeParameters = c_dwVolumeOfPartAmplifier[byIndexRow];
            pQline = lineInputPartAmplifier[byIndexRow];

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

void widgetConfig::saveFileAmplifier()
{
    QString logPath = QFileDialog::getSaveFileName(this, "Save config file", QString("%1/").arg(QCoreApplication::applicationDirPath()), c_nameTypeFileAmplifier);
    QFile fileSaveConfig(logPath);

    if(!fileSaveConfig.open(QFile::ReadWrite))
    {
        qDebug() << "error: cannot open file";
    }
    else
    {
        QTextStream fileStream(&fileSaveConfig);

        for(int iLoop = 0; iLoop < E_CFG_TYPE_AMPLF_COUNT; iLoop++)
        {
            QString SaveDataInput;

            for(int jLoop = 0; jLoop < c_dwVolumeOfPartAmplifier[iLoop]; jLoop++)
            {
                float flNmb = lineInputPartAmplifier[iLoop][jLoop]->text().toFloat();
                SaveDataInput.append(QString("%1").arg(QString::number(flNmb)) + " ");
            }

            fileStream << SaveDataInput << endl;
        }
    }

    fileSaveConfig.close();
}

void widgetConfig::saveMcuAmplifier()
{
    for(int iLoop = 0; iLoop < E_CFG_TYPE_AMPLF_COUNT; iLoop++)
    {
        ReadAndAddToMsgAmplifier(iLoop);
    }
}

float widgetConfig::DecodeUint32ToFloatClassic(QByteArray qByArry)
{
    uint32_t dwValue = DecodeBytesToUint32Classic(qByArry);
    float fValue = *(float*)&dwValue;

    return fValue;
}

uint32_t widgetConfig::DecodeBytesToUint32Classic(QByteArray qByArry)
{
    uint32_t dwValue = uint32_t(qByArry.at(0) << 24) & 0xFF000000;
    dwValue |= uint32_t(qByArry.at(1) << 16) & 0x00FF0000;
    dwValue |= uint32_t(qByArry.at(2) << 8) & 0x0000FF00;
    dwValue |= uint32_t(qByArry.at(3)) & 0x000000FF;

    return dwValue;
}

float widgetConfig::DecodeUint32ToFloatAmplifier(QByteArray qByArry)
{
    uint32_t dwValue = DecodeBytesToUint32Amplifier(qByArry);
    float fValue = *(float*)&dwValue;

    return fValue;
}

uint32_t widgetConfig::DecodeBytesToUint32Amplifier(QByteArray qByArry)
{
    uint32_t dwValue = uint32_t(qByArry.at(0)) & 0x000000FF;
    dwValue |= uint32_t(qByArry.at(1) << 8) & 0x0000FF00;
    dwValue |= uint32_t(qByArry.at(2) << 16) & 0x00FF0000;
    dwValue |= uint32_t(qByArry.at(3) << 24) & 0xFF000000;

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
    for(int iChannel = 0; iChannel < c_dwVolumeOfPartGenerRf[E_CFG_TYPE_RF_ADC1_MUL]; iChannel++)
    {
        float flNmb = lineInputPartGenerRf[E_CFG_TYPE_RF_ADC1_MUL][iChannel]->text().toFloat();
        QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
        QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
        strPacketContent += QString(arrFloatInArraysHexa);
    }
}

void widgetConfig::ReadAndAddToMsgAdc1Add(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < c_dwVolumeOfPartGenerRf[E_CFG_TYPE_RF_ADC1_ADD]; iChannel++)
    {
        float flNmb = lineInputPartGenerRf[E_CFG_TYPE_RF_ADC1_ADD][iChannel]->text().toFloat();
        QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
        QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
        strPacketContent += QString(arrFloatInArraysHexa);
    }
}

void widgetConfig::ReadAndAddToMsgAdc2Mul(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < c_dwVolumeOfPartGenerRf[E_CFG_TYPE_RF_ADC2_MUL]; iChannel++)
    {
        float flNmb = lineInputPartGenerRf[E_CFG_TYPE_RF_ADC2_MUL][iChannel]->text().toFloat();
        QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
        QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
        strPacketContent += QString(arrFloatInArraysHexa);
    }
}

void widgetConfig::ReadAndAddToMsgAdc2Add(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < c_dwVolumeOfPartGenerRf[E_CFG_TYPE_RF_ADC2_ADD]; iChannel++)
    {
        float flNmb = lineInputPartGenerRf[E_CFG_TYPE_RF_ADC2_ADD][iChannel]->text().toFloat();
        QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
        QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
        strPacketContent += QString(arrFloatInArraysHexa);
    }
}

void widgetConfig::ReadAndAddToMsgAdc3Mul(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < c_dwVolumeOfPartGenerRf[E_CFG_TYPE_RF_ADC3_MUL]; iChannel++)
    {
        float flNmb = lineInputPartGenerRf[E_CFG_TYPE_RF_ADC3_MUL][iChannel]->text().toFloat();
        QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
        QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
        strPacketContent += QString(arrFloatInArraysHexa);
    }
}

void widgetConfig::ReadAndAddToMsgAdc3Add(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < c_dwVolumeOfPartGenerRf[E_CFG_TYPE_RF_ADC3_ADD]; iChannel++)
    {
        float flNmb = lineInputPartGenerRf[E_CFG_TYPE_RF_ADC3_ADD][iChannel]->text().toFloat();
        QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
        QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
        strPacketContent += QString(arrFloatInArraysHexa);
    }
}

void widgetConfig::ReadAndAddToMsgOthrers(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < c_dwVolumeOfPartGenerRf[E_CFG_TYPE_RF_OTHERS]; iChannel++)
    {
        float flNmb = lineInputPartGenerRf[E_CFG_TYPE_RF_OTHERS][iChannel]->text().toFloat();
        QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
        QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
        strPacketContent += QString(arrFloatInArraysHexa);
    }

    emit SendReferenceImpedance(lineInputPartGenerRf[E_CFG_TYPE_RF_OTHERS][6]->text().toFloat(), lineInputPartGenerRf[E_CFG_TYPE_RF_OTHERS][7]->text().toFloat(), lineInputPartGenerRf[E_CFG_TYPE_RF_OTHERS][8]->text().toFloat(), lineInputPartGenerRf[E_CFG_TYPE_RF_OTHERS][3]->text().toFloat());
}

void widgetConfig::ReadAndAddToMsgRegCool(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < c_dwVolumeOfPartGenerRf[E_CFG_TYPE_RF_REG_COOL] - 4; iChannel++)
    {
        float flNmb = lineInputPartGenerRf[E_CFG_TYPE_RF_REG_COOL][iChannel]->text().toFloat();
        QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
        QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
        strPacketContent += QString(arrFloatInArraysHexa);
    }

    strPacketContent += QString::number(lineInputPartGenerRf[E_CFG_TYPE_RF_REG_COOL][c_dwVolumeOfPartGenerRf[E_CFG_TYPE_RF_REG_COOL] - 4]->text().toInt(), 16).rightJustified(4 * 2, '0');
    strPacketContent += QString::number(lineInputPartGenerRf[E_CFG_TYPE_RF_REG_COOL][c_dwVolumeOfPartGenerRf[E_CFG_TYPE_RF_REG_COOL] - 3]->text().toInt(), 16).rightJustified(4 * 2, '0');
    strPacketContent += QString::number(lineInputPartGenerRf[E_CFG_TYPE_RF_REG_COOL][c_dwVolumeOfPartGenerRf[E_CFG_TYPE_RF_REG_COOL] - 2]->text().toInt(), 16).rightJustified(4 * 2, '0');
    strPacketContent += QString::number(lineInputPartGenerRf[E_CFG_TYPE_RF_REG_COOL][c_dwVolumeOfPartGenerRf[E_CFG_TYPE_RF_REG_COOL] - 1]->text().toInt(), 16).rightJustified(4 * 2, '0');
}

void widgetConfig::ReadAndAddToMsgRegPwr(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < c_dwVolumeOfPartGenerRf[E_CFG_TYPE_RF_REG_PWR] - 1; iChannel++)
    {
        float flNmb = lineInputPartGenerRf[E_CFG_TYPE_RF_REG_PWR][iChannel]->text().toFloat();
        QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
        QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
        strPacketContent += QString(arrFloatInArraysHexa);
    }

    strPacketContent += QString::number(lineInputPartGenerRf[E_CFG_TYPE_RF_REG_PWR][c_dwVolumeOfPartGenerRf[E_CFG_TYPE_RF_REG_PWR] - 1]->text().toInt(), 16).rightJustified(4 * 2, '0');
}

void widgetConfig::ReadAndAddToMsgTestTher(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < c_dwVolumeOfPartGenerRf[E_CFG_TYPE_RF_TEST_THERAPY]; iChannel++)
    {
        strPacketContent += QString::number(lineInputPartGenerRf[E_CFG_TYPE_RF_TEST_THERAPY][iChannel]->text().toInt(), 16).rightJustified(4 * 2, '0');
    }
}

void widgetConfig::ReadAndAddToMsgCqmPwm(QString &strPacketContent)
{
    for(int iChannel = 0; iChannel < c_dwVolumeOfPartGenerRf[E_CFG_TYPE_RF_CQM_PWM]; iChannel++)
    {
        strPacketContent += QString::number(lineInputPartGenerRf[E_CFG_TYPE_RF_CQM_PWM][iChannel]->text().toInt(), 16).rightJustified(4 * 2, '0');
    }
}

void widgetConfig::ReadAndAddToMsgAmplifier(int iIndex)
{
    QString strCmd = QString("%1").arg(QString::number(PID_CONFIGURATION_DEVICE, 16));
    strCmd += QString::number(ACC_AMPLIFIER, 16).rightJustified(2, '0');
    strCmd += QString::number(ACTION_WRITE, 16).rightJustified(2, '0');
    strCmd += QString::number(ConvertIndexAmplifier(iIndex), 16).rightJustified(2, '0');
    strCmd += QString::number(4 * c_dwVolumeOfPartAmplifier[iIndex], 16).rightJustified(2, '0');

    for(int jLoop = 0; jLoop < c_dwVolumeOfPartAmplifier[iIndex]; jLoop++)
    {
        float flNmb = lineInputPartAmplifier[iIndex][jLoop]->text().toFloat();
        QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&flNmb), sizeof (flNmb));
        QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
        strCmd += QString(arrFloatInArraysHexa);
    }

    emit SendV200specific(strCmd, false);
}

int widgetConfig::ConvertIndexAmplifier(int iIndex)
{
    //input: c_nameVolumeOfPartAmplifier, output: CONFIG_TYPES_AMPLIFIER
    if(iIndex == 0)
    {
        return 0;
    }
    else if(iIndex == 1)
    {
        return 3;
    }
    else if(iIndex == 2)
    {
        return 1;
    }
    else if(iIndex == 3)
    {
        return 4;
    }
    else if(iIndex == 4)
    {
        return 2;
    }
    else if(iIndex == 5)
    {
        return 5;
    }

    return 0;
}

int widgetConfig::DeconvertIndexAmplifier(int iIndex)
{
    //input: CONFIG_TYPES_AMPLIFIER, output: c_nameVolumeOfPartAmplifier
    if(iIndex == 0)
    {
        return 0;
    }
    else if(iIndex == 1)
    {
        return 2;
    }
    else if(iIndex == 2)
    {
        return 4;
    }
    else if(iIndex == 3)
    {
        return 1;
    }
    else if(iIndex == 4)
    {
        return 3;
    }
    else if(iIndex == 5)
    {
        return 5;
    }

    return 0;
}

void widgetConfig::clearLineEdits()
{
    for(int iLoop = 0; iLoop < E_CFG_TYPE_RF_COUNT; iLoop++)
    {
        for(int jLoop = 0; jLoop < c_dwVolumeOfPartGenerRf[iLoop]; jLoop++)
        {
            lineInputPartGenerRf[iLoop][jLoop]->clear();
        }
    }

    for(int iLoop = 0; iLoop < E_CFG_TYPE_AMPLF_COUNT; iLoop++)
    {
        for(int jLoop = 0; jLoop < c_dwVolumeOfPartAmplifier[iLoop]; jLoop++)
        {
            lineInputPartAmplifier[iLoop][jLoop]->clear();
        }
    }
}

void widgetConfig::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);

    QPainter painter(this);

}
