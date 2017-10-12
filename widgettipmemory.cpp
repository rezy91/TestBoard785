#include "widgettipmemory.h"
#include <QDebug>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QCoreApplication>

widgetTipMemory::widgetTipMemory(QWidget *parent) : QWidget(parent)
{
    dwCurrentChannel = dwChannelDefault;

    listOfChannels->addItem("Select channel");
    listOfChannels->setMaximumWidth(100);

    for(int iLoop = 0; iLoop < nmbChannelsAppls; iLoop++)
    {
        listOfChannels->addItem(QString("channel_%1").arg(QString::number(iLoop)));
        qobject_cast<QStandardItemModel*>(listOfChannels->model())->item(iLoop + 1)->setEnabled(false);
    }


    c_AddressParameters[0] = 0;

    for(int iLoop = 0; iLoop < E_NMB_PARAMETERS_IN_MEMORY; iLoop++)
    {
        if(iLoop > 0)
        {
            c_AddressParameters[iLoop] = c_AddressParameters[iLoop - 1] + c_VolumeParameters[iLoop - 1];
        }

        gridParams->addWidget(createNewLabel(QString("%1 (%2) [%3]").arg(c_nameParametersInMemory[iLoop]).arg(c_VolumeParameters[iLoop]).arg(c_nameUnitInMemory[iLoop])), iLoop, 0, Qt::AlignRight);
        lineInputParameter[iLoop] = new QLineEdit();
        gridParams->addWidget(lineInputParameter[iLoop], iLoop, 1, Qt::AlignCenter);
        buttSend[iLoop] = new QPushButton("Send");
        gridParams->addWidget(buttSend[iLoop], iLoop, 2, Qt::AlignLeft);
    }

    hBox->addWidget(listOfChannels);

    for(int iLoop = 0; iLoop < E_NMB_BUTTONS; iLoop++)
    {
        buttConfig[iLoop] = new QPushButton(QString("%1").arg(c_nameButtonConfig[iLoop]));
        hBox->addWidget(buttConfig[iLoop]);
    }

    vBox->addLayout(hBox);
    vBox->addLayout(gridParams);


    connect(listOfChannels,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[=](int nValue){

        dwCurrentChannel = nValue - 1;

        if(dwCurrentChannel == dwChannelDefault)
        {
            clearLineInput();
        }
    });

    for(int iLoop = 0; iLoop < E_NMB_PARAMETERS_IN_MEMORY; iLoop++)
    {
        connect(buttSend[iLoop],&QPushButton::clicked,[=](bool clicked){

            Q_UNUSED(clicked);

            QString strInput = lineInputParameter[iLoop]->text();

            QString strCmd = QString("%1").arg(QString::number(PID_WRITE_TO_MEMORY, 16));
            strCmd += QString::number(dwCurrentChannel).rightJustified(1 * 2, '0');
            strCmd += QString::number(c_AddressParameters[iLoop], 16).rightJustified(2 * 2, '0');
            strCmd += QString::number(c_VolumeParameters[iLoop], 16).rightJustified(2 * 2, '0');

            if(CollectStringParameter(iLoop, strInput, strCmd))
            {
                qDebug() << strCmd;
                emit SendV200specific(strCmd);
            }

        });
    }

    for(int iLoop = 0; iLoop < E_NMB_BUTTONS; iLoop++)
    {
        connect(buttConfig[iLoop], &QPushButton::clicked, [=](bool clicked){

            Q_UNUSED(clicked);

            if(iLoop == E_BUTTON_LOAD_MCU)
            {
                if(dwCurrentChannel >= 0)
                {
                    decodeTipMemory(uint8_t(dwCurrentChannel));
                }
                else
                {
                    QString strMsg = QString("Select channel");
                    QMessageBox::information(this, "Invalid channel", strMsg);
                }
            }
            else if(iLoop == E_BUTTON_LOAD_FILE)
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
                    QStringList stringsSplittedTotal;

                    while (!fileStream.atEnd())
                    {
                        QString newLinereaded = fileStream.readLine();
                        stringsSplittedTotal.append(newLinereaded);
                    }

                    m_logFile.close();

                    if((stringsSplittedTotal.count()) <= E_NMB_PARAMETERS_IN_MEMORY)
                    {
                        for(int jLoop = 0; jLoop < E_NMB_PARAMETERS_IN_MEMORY; jLoop++)
                        {
                            lineInputParameter[jLoop]->setText(stringsSplittedTotal.at(jLoop));
                        }
                    }
                    else
                    {
                        for(int jLoop = 0; jLoop < E_NMB_PARAMETERS_IN_MEMORY; jLoop++)
                        {
                            lineInputParameter[jLoop]->setText(QString("x"));
                        }
                    }

                }
            }
            else if(iLoop == E_BUTTON_SAVE_MCU)
            {
                if(dwCurrentChannel >= 0)
                {
                    QString strCmd = QString("%1").arg(QString::number(PID_WRITE_TO_MEMORY, 16));
                    strCmd += QString::number(dwCurrentChannel).rightJustified(1 * 2, '0');
                    strCmd += QString::number(0, 16).rightJustified(2 * 2, '0');
                    strCmd += QString::number(MAX_TIP_MEMORY_STRUCT_SIZE, 16).rightJustified(2 * 2, '0');

                    for(int jLoop = 0; jLoop < E_NMB_PARAMETERS_IN_MEMORY; jLoop++)
                    {
                        QString strInput = lineInputParameter[jLoop]->text();

                        if(!CollectStringParameter(jLoop, strInput, strCmd))
                        {
                            return;
                        }
                    }

                    qDebug() << strCmd;
                    emit SendV200specific(strCmd);
                }
                else
                {
                    QString strMsg = QString("Select channel");
                    QMessageBox::information(this, "Invalid channel", strMsg);
                }
            }
            else if(iLoop == E_BUTTON_SAVE_FILE)
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

                    for(int jLoop = 0; jLoop < E_NMB_PARAMETERS_IN_MEMORY; jLoop++)
                    {
                        fileStream << lineInputParameter[jLoop]->text() << endl;
                    }

                    fileSaveConfig.close();
                }
            }

        });
    }
}

QLabel *widgetTipMemory::createNewLabel(const QString &text)
{
    QLabel* newLabel = new QLabel(text);
    newLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
    return newLabel;
}

void widgetTipMemory::decodeTipMemory(uint8_t byChannel)
{
    uint8_t bySumIndex = 0;

    for(uint8_t iLoop = 0; iLoop < E_NMB_PARAMETERS_IN_MEMORY; iLoop++)
    {
        uint32_t gBytes = 0;

        for(uint8_t jLoop = c_VolumeParameters[iLoop]; jLoop > 0; jLoop--)
        {
            if(iLoop != E_EXPIRATION && iLoop != E_SERIAL && iLoop != E_DATE_MANUFACT && iLoop != E_DATE_ACTIVE)
            {
                gBytes |= uint32_t(arr_byTipContent[byChannel][bySumIndex] << (8 * (jLoop - 1)));
            }

            bySumIndex++;
        }

        if(iLoop != E_EXPIRATION && iLoop != E_SERIAL && iLoop != E_DATE_MANUFACT && iLoop != E_DATE_ACTIVE)
        {
            lineInputParameter[iLoop]->setText(QString::number(gBytes));
        }
    }


    uint8_t byNmbExpiration = 0;

    for(uint8_t iLoop = 0; iLoop < c_VolumeParameters[E_EXPIRATION]; iLoop++)
    {
        for(uint8_t jLoop = 0; jLoop < 8; jLoop++)
        {
            if(arr_byTipContent[byChannel][c_AddressParameters[E_EXPIRATION] + iLoop] & (1 << jLoop))
            {
                byNmbExpiration++;
            }
        }
    }

    lineInputParameter[E_EXPIRATION]->setText(QString::number(byNmbExpiration));


    uint8_t arr_bySerNumber[c_VolumeParameters[E_SERIAL] + 1];

    for(uint8_t iLoop = 0; iLoop < c_VolumeParameters[E_SERIAL]; iLoop++)
    {
        arr_bySerNumber[iLoop] = arr_byTipContent[byChannel][c_AddressParameters[E_SERIAL] + iLoop];
    }

    arr_bySerNumber[c_VolumeParameters[E_SERIAL]] = '\0';
    QString strSer = QString::fromUtf8((char*)(arr_bySerNumber));
    lineInputParameter[E_SERIAL]->setText(strSer);


    uint8_t arr_byDateManuf[c_VolumeParameters[E_DATE_MANUFACT] + 1];

    for(uint8_t iLoop = 0; iLoop < c_VolumeParameters[E_DATE_MANUFACT]; iLoop++)
    {
        arr_byDateManuf[iLoop] = arr_byTipContent[byChannel][c_AddressParameters[E_DATE_MANUFACT] + iLoop];
    }

    arr_byDateManuf[c_VolumeParameters[E_DATE_MANUFACT]] = '\0';
    QString strDateManuf = QString::fromUtf8((char*)(arr_byDateManuf));
    lineInputParameter[E_DATE_MANUFACT]->setText(strDateManuf);


    uint8_t arr_byDateActive[c_VolumeParameters[E_DATE_ACTIVE] + 1];

    for(uint8_t iLoop = 0; iLoop < c_VolumeParameters[E_DATE_ACTIVE]; iLoop++)
    {
        arr_byDateActive[iLoop] = arr_byTipContent[byChannel][c_AddressParameters[E_DATE_ACTIVE] + iLoop];
    }

    arr_byDateActive[c_VolumeParameters[E_DATE_ACTIVE]] = '\0';
    QString strDateActive = QString::fromUtf8((char*)(arr_byDateActive));
    lineInputParameter[E_DATE_ACTIVE]->setText(strDateActive);


    uint32_t sValue;

    sValue = uint16_t(arr_byTipContent[byChannel][c_AddressParameters[E_USN_FREQ_CRYSTAL]] << 8) + uint16_t(arr_byTipContent[byChannel][c_AddressParameters[E_USN_FREQ_CRYSTAL] + 1]);
    lineInputParameter[E_USN_FREQ_CRYSTAL]->setText(QString::number(sValue * 100));
    sValue = uint16_t(arr_byTipContent[byChannel][c_AddressParameters[E_USN_VOLTAGEV100]] << 8) + uint16_t(arr_byTipContent[byChannel][c_AddressParameters[E_USN_VOLTAGEV100] + 1]);
    lineInputParameter[E_USN_VOLTAGEV100]->setText(QString::number(sValue * 10));
    sValue = uint16_t(arr_byTipContent[byChannel][c_AddressParameters[E_USN_VALUE_AIR]] << 8) + uint16_t(arr_byTipContent[byChannel][c_AddressParameters[E_USN_VALUE_AIR] + 1]);
    lineInputParameter[E_USN_VALUE_AIR]->setText(QString::number(sValue * 10));
    sValue = uint16_t(arr_byTipContent[byChannel][c_AddressParameters[E_USN_VALUE_WATER]] << 8) + uint16_t(arr_byTipContent[byChannel][c_AddressParameters[E_USN_VALUE_WATER] + 1]);
    lineInputParameter[E_USN_VALUE_WATER]->setText(QString::number(sValue * 10));

    uint8_t byControlType = arr_byTipContent[byChannel][c_AddressParameters[E_USN_CONTROL_TYPE]];

    if(byControlType != 2)
    {
        sValue = uint16_t(arr_byTipContent[byChannel][c_AddressParameters[E_USN_CONTROL_VARIABLE]] << 8) + uint16_t(arr_byTipContent[byChannel][c_AddressParameters[E_USN_CONTROL_VARIABLE] + 1]);
        lineInputParameter[E_USN_CONTROL_VARIABLE]->setText(QString::number(sValue * 10));
    }

}

bool widgetTipMemory::IsStringNumber(QString strInput)
{
    for (int i = 0; i < strInput.length(); i++)
    {
        if (!strInput[i].isDigit())
        {
            return false;
        }
    }

    return true;
}

bool widgetTipMemory::IsPrintable(QString strInput)
{
    for (int i = 0; i < strInput.length(); i++)
    {
        if (!strInput[i].isLetterOrNumber())
        {
            return false;
        }
    }

    return true;
}

bool widgetTipMemory::CollectStringAscii(int iIndex, QString strInputString, QString &text)
{
    if(strInputString.length() > c_VolumeParameters[iIndex])
    {
        QString strMsg = QString("'%1' (%2) is longer than maximum length (%3)").arg(strInputString).arg(strInputString.length()).arg(c_VolumeParameters[iIndex]);
        QMessageBox::information(this, "Invalid input", strMsg);
        return false;
    }
    else
    {
        for(int jLoop = 0; jLoop < c_VolumeParameters[iIndex]; jLoop++)
        {
            if(jLoop < strInputString.length())
            {
                text += QString::number(strInputString.at(jLoop).unicode(), 16);
            }
            else
            {
                text += QString::number(0, 16).rightJustified(2, '0');
            }
        }

        return true;
    }

    return false;
}

bool widgetTipMemory::CollectStringParameter(int iIndex, QString strInputString, QString &text)
{
    uint32_t dwInput = strInputString.toUInt();


    if(iIndex == E_SERIAL)
    {
        if(IsPrintable(strInputString))
        {
            if(!CollectStringAscii(iIndex, strInputString, text))
            {
                return false;
            }
        }
        else
        {
            QString strMsg = QString("'%1' has character which is not letter or number").arg(strInputString);
            QMessageBox::information(this, "Invalid input", strMsg);
            return false;
        }
    }
    else
    {
        if(IsStringNumber(strInputString))
        {
            if(iIndex == E_EXPIRATION)
            {
                if(dwInput > uint32_t(c_VolumeParameters[iIndex] * 8))
                {
                    QString strMsg = QString("'%1' is too big").arg(strInputString);
                    QMessageBox::information(this, "Invalid input", strMsg);
                    return false;
                }
                else
                {
                    ulong uBits = 0;

                    for(uint jLoop = 0; jLoop < dwInput; jLoop++)
                    {
                        uBits |= (1 << jLoop);
                    }

                    text += QString::number(uBits, 16).rightJustified(2 * c_VolumeParameters[iIndex], '0');
                }
            }
            if(iIndex == E_DATE_MANUFACT || iIndex == E_DATE_ACTIVE)
            {
                if(!CollectStringAscii(iIndex, strInputString, text))
                {
                    return false;
                }
            }
            else if(iIndex == E_USN_FREQ_CRYSTAL)
            {
                if(dwInput > uint32_t(UINT16_MAX * 100))
                {
                    QString strMsg = QString("'%1' is too big").arg(strInputString);
                    QMessageBox::information(this, "Invalid input", strMsg);
                    return false;
                }
                else
                {
                    uint16_t sDividedValue = dwInput / 100;
                    text += QString::number(sDividedValue, 16).rightJustified(2 * c_VolumeParameters[iIndex], '0');
                }
            }
            else if(iIndex == E_USN_VOLTAGEV100 || iIndex == E_USN_VALUE_AIR || iIndex == E_USN_VALUE_WATER)
            {
                if(dwInput > uint32_t(UINT16_MAX * 10))
                {
                    QString strMsg = QString("'%1' is too big").arg(strInputString);
                    QMessageBox::information(this, "Invalid input", strMsg);
                    return false;
                }
                else
                {
                    uint16_t sDividedValue = dwInput / 10;
                    text += QString::number(sDividedValue, 16).rightJustified(2 * c_VolumeParameters[iIndex], '0');
                }
            }
            else if(iIndex == E_USN_CONTROL_TYPE)
            {
                if(dwInput > 3)
                {
                    QString strMsg = QString("'%1' is too big").arg(strInputString);
                    QMessageBox::information(this, "Invalid input", strMsg);
                    return false;
                }
                else
                {
                    uint16_t sDividedValue = dwInput;
                    text += QString::number(sDividedValue, 16).rightJustified(2 * c_VolumeParameters[iIndex], '0');
                }
            }
            else if(iIndex == E_USN_CONTROL_VARIABLE)
            {
                int dwControlType = lineInputParameter[E_USN_CONTROL_TYPE]->text().toUInt();

                if(dwControlType == 0 || dwControlType == 1 || dwControlType == 3)//voltage
                {
                    if(dwInput > uint32_t(UINT16_MAX * 10))
                    {
                        QString strMsg = QString("'%1' is too big").arg(strInputString);
                        QMessageBox::information(this, "Invalid input", strMsg);
                        return false;
                    }
                    else
                    {
                        uint16_t sDividedValue = dwInput / 10;
                        text += QString::number(sDividedValue, 16).rightJustified(2 * c_VolumeParameters[iIndex], '0');
                    }
                }
                else if(dwControlType == 2)//current
                {
                    if(dwInput > uint32_t(UINT16_MAX))
                    {
                        QString strMsg = QString("'%1' is too big").arg(strInputString);
                        QMessageBox::information(this, "Invalid input", strMsg);
                        return false;
                    }
                    else
                    {
                        uint16_t sDividedValue = dwInput;
                        text += QString::number(sDividedValue, 16).rightJustified(2 * c_VolumeParameters[iIndex], '0');
                    }
                }
                else
                {
                    QString strMsg = QString("CONTROL TYPE out of range");
                    QMessageBox::information(this, "Invalid input", strMsg);
                    return false;
                }
            }
            else if(iIndex == E_RF_POWER_MAX)
            {
                if(dwInput > uint32_t(UINT16_MAX))
                {
                    QString strMsg = QString("'%1' is too big").arg(strInputString);
                    QMessageBox::information(this, "Invalid input", strMsg);
                    return false;
                }
                else
                {
                    uint16_t sDividedValue = dwInput;
                    text += QString::number(sDividedValue, 16).rightJustified(2 * c_VolumeParameters[iIndex], '0');
                }
            }
            else if(iIndex == E_REFER_IMPEDANCE_MODUL || iIndex == E_REFER_IMPEDANCE_PHASE || iIndex == E_REFER_IMPEDANCE_REFL || iIndex == E_CURRN_IMPEDANCE_REFL || iIndex == E_TIP_TYPE || iIndex == E_VERSION)
            {
                if(dwInput > uint32_t(UINT8_MAX))
                {
                    QString strMsg = QString("'%1' is too big").arg(strInputString);
                    QMessageBox::information(this, "Invalid input", strMsg);
                    return false;
                }
                else
                {
                    uint16_t sDividedValue = dwInput;
                    text += QString::number(sDividedValue, 16).rightJustified(2 * c_VolumeParameters[iIndex], '0');
                }
            }
        }
        else
        {
            QString strMsg = QString("'%1' is not positive integer").arg(strInputString);
            QMessageBox::information(this, "Invalid input", strMsg);
            return false;
        }
    }

    return true;
}

void widgetTipMemory::clearComboBox(int iIndex)
{
    dwCurrentChannel = dwChannelDefault;
    listOfChannels->setCurrentIndex(0);

    qobject_cast<QStandardItemModel*>(listOfChannels->model())->item(iIndex)->setEnabled(false);

    clearLineInput();
}

void widgetTipMemory::clearLineInput()
{
    for(int iLoop = 0; iLoop < E_NMB_PARAMETERS_IN_MEMORY; iLoop++)
    {
        lineInputParameter[iLoop]->clear();
    }
}

void widgetTipMemory::clearAll()
{
    clearComboBox(1);
    clearComboBox(2);
    clearComboBox(3);
    clearComboBox(4);
}

void widgetTipMemory::ReceiveStatusReg(STATUS_REGISTER eStatusReg)
{
    qobject_cast<QStandardItemModel*>(listOfChannels->model())->item(1)->setEnabled(eStatusReg.m_Reg.m_Bit.StateAcc0 == 1 ? true : false);
    qobject_cast<QStandardItemModel*>(listOfChannels->model())->item(2)->setEnabled(eStatusReg.m_Reg.m_Bit.StateAcc1 == 1 ? true : false);
    qobject_cast<QStandardItemModel*>(listOfChannels->model())->item(3)->setEnabled(eStatusReg.m_Reg.m_Bit.StateAcc2 == 1 ? true : false);
    qobject_cast<QStandardItemModel*>(listOfChannels->model())->item(4)->setEnabled(eStatusReg.m_Reg.m_Bit.StateAcc3 == 1 ? true : false);

    if(dwCurrentChannel == 0 && eStatusReg.m_Reg.m_Bit.ChangeAcc0 && eStatusReg.m_Reg.m_Bit.StateAcc0 == 0)
    {
        clearComboBox(1);
    }
    else if(dwCurrentChannel == 1 && eStatusReg.m_Reg.m_Bit.ChangeAcc1 && eStatusReg.m_Reg.m_Bit.StateAcc1 == 0)
    {
        clearComboBox(2);
    }
    else if(dwCurrentChannel == 2 && eStatusReg.m_Reg.m_Bit.ChangeAcc2 && eStatusReg.m_Reg.m_Bit.StateAcc2 == 0)
    {
        clearComboBox(3);
    }
    else if(dwCurrentChannel == 3 && eStatusReg.m_Reg.m_Bit.ChangeAcc3 && eStatusReg.m_Reg.m_Bit.StateAcc3 == 0)
    {
        clearComboBox(4);
    }
}

void widgetTipMemory::ReceiveTipMemory(uint8_t byChannel, uint8_t byBytes, QByteArray byBuffer)
{
    for(uint8_t iLoop = 0; iLoop < byBytes; iLoop++)
    {
        arr_byTipContent[byChannel][iLoop] = uint8_t(byBuffer.at(iLoop));
    }
}

void widgetTipMemory::DecodeChoosedChannel(int dwChannel)
{
    if(dwChannel >= 0)
    {
        uint32_t dwVersion = uint32_t(arr_byTipContent[dwChannel][c_AddressParameters[E_VERSION]]);

        if(dwVersion == uint32_t(TIP_MEMORY_VERSION))
        {
            emit SendReferenceImpedance(float(arr_byTipContent[dwChannel][c_AddressParameters[E_REFER_IMPEDANCE_MODUL]]), float((arr_byTipContent[dwChannel][c_AddressParameters[E_REFER_IMPEDANCE_PHASE]]) * 3.14) / 180, float(arr_byTipContent[dwChannel][c_AddressParameters[E_REFER_IMPEDANCE_REFL]]) / 100, float(arr_byTipContent[dwChannel][c_AddressParameters[E_CURRN_IMPEDANCE_REFL]]) / 100);

            uint16_t dwMaxPower = uint32_t(arr_byTipContent[dwChannel][c_AddressParameters[E_RF_POWER_MAX]] << 8) + uint32_t(arr_byTipContent[dwChannel][c_AddressParameters[E_RF_POWER_MAX] + 1]);
            emit SendMaximalPower(dwMaxPower);
        }
        else
        {
            emit SendDefaultReferenceImpedance();
            emit SendDefaultMaximalPower();
        }
    }
}
