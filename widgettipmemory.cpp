#include "widgettipmemory.h"
#include <QDebug>
#include <QStandardItemModel>
#include <QMessageBox>

widgetTipMemory::widgetTipMemory(QWidget *parent) : QWidget(parent)
{
    dwCurrentChannel = 0;

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

        gridParams->addWidget(createNewLabel(QString("%1 [%2]").arg(c_nameParametersInMemory[iLoop]).arg(c_VolumeParameters[iLoop])), iLoop, 0, Qt::AlignRight);
        lineInputParameter[iLoop] = new QLineEdit();
        gridParams->addWidget(lineInputParameter[iLoop], iLoop, 1, Qt::AlignCenter);
        buttSend[iLoop] = new QPushButton("Send");
        gridParams->addWidget(buttSend[iLoop], iLoop, 2, Qt::AlignLeft);
    }

    vBox->addWidget(listOfChannels, Qt::AlignCenter);
    vBox->addLayout(gridParams);

    connect(listOfChannels,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[=](int nValue){

        dwCurrentChannel = nValue - 1;

        if(dwCurrentChannel >= 0)
        {
            decodeTipMemory(uint8_t(dwCurrentChannel));
        }
        else
        {
            clearAll();
        }
    });

    for(int iLoop = 0; iLoop < E_NMB_PARAMETERS_IN_MEMORY; iLoop++)
    {
        connect(buttSend[iLoop],&QPushButton::clicked,[=](bool clicked){

            Q_UNUSED(clicked);

            QString strInput = lineInputParameter[iLoop]->text();

            if(iLoop == E_SERIAL)
            {
                if(IsPrintable(strInput))
                {
                    if(strInput.length() > c_VolumeParameters[iLoop])
                    {
                        QString strMsg = QString("'%1' (%2) is longer than maximum length (%3)").arg(strInput).arg(strInput.length()).arg(c_VolumeParameters[iLoop]);
                        QMessageBox::information(this, "Invalid input", strMsg);
                    }
                    else
                    {
                        QString strCmd = QString("%1").arg(QString::number(PID_WRITE_TO_MEMORY, 16));
                        strCmd += QString::number(dwCurrentChannel).rightJustified(1 * 2, '0');
                        strCmd += QString::number(c_AddressParameters[iLoop], 16).rightJustified(2 * 2, '0');
                        strCmd += QString::number(c_VolumeParameters[iLoop], 16).rightJustified(2 * 2, '0');

                        for(int jLoop = 0; jLoop < strInput.length(); jLoop++)
                        {
                            strCmd += QString::number(strInput.at(jLoop).unicode(), 16);
                        }

                        strCmd += QString::number(0, 16).rightJustified(2, '0');//null character

                        qDebug() << strCmd;
                        emit SendV200specific(strCmd);
                    }
                }
                else
                {
                    QString strMsg = QString("'%1' has character which is not letter or number").arg(strInput);
                    QMessageBox::information(this, "Invalid input", strMsg);
                }
            }
            else if(iLoop == E_DATE_MANUFACT || iLoop == E_DATE_ACTIVE)
            {
                if(IsStringNumber(strInput))
                {
                    if(strInput.length() > c_VolumeParameters[iLoop])
                    {
                        QString strMsg = QString("'%1' (%2) is longer than maximum length (%3)").arg(strInput).arg(strInput.length()).arg(c_VolumeParameters[iLoop]);
                        QMessageBox::information(this, "Invalid input", strMsg);
                    }
                    else
                    {
                        QString strCmd = QString("%1").arg(QString::number(PID_WRITE_TO_MEMORY, 16));
                        strCmd += QString::number(dwCurrentChannel).rightJustified(1 * 2, '0');
                        strCmd += QString::number(c_AddressParameters[iLoop], 16).rightJustified(2 * 2, '0');
                        strCmd += QString::number(c_VolumeParameters[iLoop], 16).rightJustified(2 * 2, '0');

                        for(int jLoop = 0; jLoop < strInput.length(); jLoop++)
                        {
                            strCmd += QString::number(strInput.at(jLoop).unicode(), 16);
                        }

                        qDebug() << strCmd;
                        emit SendV200specific(strCmd);
                    }
                }
                else
                {
                    QString strMsg = QString("'%1' is not positive number").arg(strInput);
                    QMessageBox::information(this, "Invalid input", strMsg);
                }
            }
            else
            {
                if(IsStringNumber(strInput))
                {
                    if(c_VolumeParameters[iLoop] == 1 && strInput.toUInt() > UINT8_MAX)
                    {
                        QString strMsg = QString("'%1' is bigger than maximum (%2)").arg(strInput).arg(UINT8_MAX);
                        QMessageBox::information(this, "Invalid input", strMsg);
                    }
                    else if(c_VolumeParameters[iLoop] == 2 && strInput.toUInt() > UINT16_MAX)
                    {
                        QString strMsg = QString("'%1' is bigger than maximum (%2)").arg(strInput).arg(UINT16_MAX);
                        QMessageBox::information(this, "Invalid input", strMsg);
                    }
                    else if(iLoop == E_EXPIRATION && strInput.toUInt() > uint8_t(c_VolumeParameters[E_EXPIRATION] * 8))
                    {
                        QString strMsg = QString("'%1' is bigger than maximum (%2)").arg(strInput).arg(c_VolumeParameters[E_EXPIRATION] * 8);
                        QMessageBox::information(this, "Invalid input", strMsg);
                    }
                    else
                    {
                        QString strCmd = QString("%1").arg(QString::number(PID_WRITE_TO_MEMORY, 16));
                        strCmd += QString::number(dwCurrentChannel).rightJustified(1 * 2, '0');
                        strCmd += QString::number(c_AddressParameters[iLoop], 16).rightJustified(2 * 2, '0');
                        strCmd += QString::number(c_VolumeParameters[iLoop], 16).rightJustified(2 * 2, '0');

                        if(iLoop == E_EXPIRATION)
                        {
                            ulong uBits = 0;

                            for(uint jLoop = 0; jLoop < strInput.toUInt(); jLoop++)
                            {
                                uBits |= (1 << jLoop);
                            }

                            strCmd += QString::number(uBits, 16).rightJustified(2 * c_VolumeParameters[iLoop], '0');
                        }
                        else
                        {
                            strCmd += QString::number(strInput.toUInt(), 16).rightJustified(2 * c_VolumeParameters[iLoop], '0');
                        }

                        qDebug() << strCmd;
                        emit SendV200specific(strCmd);
                    }
                }
                else
                {
                    QString strMsg = QString("'%1' is not positive number").arg(strInput);
                    QMessageBox::information(this, "Invalid input", strMsg);
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
        if (!strInput[i].isPrint())
        {
            return false;
        }
    }

    return true;
}

void widgetTipMemory::clearAll()
{
    for(int iLoop = 0; iLoop < E_NMB_PARAMETERS_IN_MEMORY; iLoop++)
    {
        lineInputParameter[iLoop]->clear();
    }

    dwCurrentChannel = 0;

    listOfChannels->setCurrentIndex(0);
    qobject_cast<QStandardItemModel*>(listOfChannels->model())->item(1)->setEnabled(false);
    qobject_cast<QStandardItemModel*>(listOfChannels->model())->item(2)->setEnabled(false);
    qobject_cast<QStandardItemModel*>(listOfChannels->model())->item(3)->setEnabled(false);
    qobject_cast<QStandardItemModel*>(listOfChannels->model())->item(4)->setEnabled(false);
}

void widgetTipMemory::ReceiveStatusReg(STATUS_REGISTER eStatusReg)
{
    qobject_cast<QStandardItemModel*>(listOfChannels->model())->item(1)->setEnabled(eStatusReg.m_Reg.m_Bit.StateAcc0 == 1 ? true : false);
    qobject_cast<QStandardItemModel*>(listOfChannels->model())->item(2)->setEnabled(eStatusReg.m_Reg.m_Bit.StateAcc1 == 1 ? true : false);
    qobject_cast<QStandardItemModel*>(listOfChannels->model())->item(3)->setEnabled(eStatusReg.m_Reg.m_Bit.StateAcc2 == 1 ? true : false);
    qobject_cast<QStandardItemModel*>(listOfChannels->model())->item(4)->setEnabled(eStatusReg.m_Reg.m_Bit.StateAcc3 == 1 ? true : false);

    if(dwCurrentChannel == 0 && eStatusReg.m_Reg.m_Bit.ChangeAcc0 && eStatusReg.m_Reg.m_Bit.StateAcc0 == 0)
    {
        clearAll();
    }
    else if(dwCurrentChannel == 1 && eStatusReg.m_Reg.m_Bit.ChangeAcc1 && eStatusReg.m_Reg.m_Bit.StateAcc1 == 0)
    {
        clearAll();
    }
    else if(dwCurrentChannel == 2 && eStatusReg.m_Reg.m_Bit.ChangeAcc2 && eStatusReg.m_Reg.m_Bit.StateAcc2 == 0)
    {
        clearAll();
    }
    else if(dwCurrentChannel == 3 && eStatusReg.m_Reg.m_Bit.ChangeAcc3 && eStatusReg.m_Reg.m_Bit.StateAcc3 == 0)
    {
        clearAll();
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
        uint32_t dwVersion = uint32_t(arr_byTipContent[dwChannel][c_AddressParameters[E_VERSION]] << 8) + uint32_t(arr_byTipContent[dwChannel][c_AddressParameters[E_VERSION] + 1]);

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
