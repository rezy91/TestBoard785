#include "widgetreading.h"

#include <QDesktopServices>
#include <QLabel>

widgetReading::widgetReading(QWidget *parent) : QWidget(parent)
{
    for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_APLS_AND_USN; iLoop++)
    {
        chBoxAplUsn[iLoop] = new QCheckBox(allNamesAplUsn[iLoop], this);
        chBoxAplUsn[iLoop]->setEnabled(false);
        aplUsnTimes[iLoop] = new QSpinBox(this);
        qGridLyout->addWidget(aplUsnTimes[iLoop], iLoop, 0, Qt::AlignRight);
        qGridLyout->addWidget(chBoxAplUsn[iLoop], iLoop, 1, Qt::AlignLeft);

        aplUsnTimes[iLoop]->setMaximum(1000);
        aplUsnTimes[iLoop]->setSingleStep(10);
    }

    for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_AMPLF; iLoop++)
    {
        chBoxAmp[iLoop] = new QCheckBox(allNamesAmp[iLoop], this);
        chBoxAmp[iLoop]->setEnabled(false);
        amplfTimes[iLoop] = new QSpinBox(this);
        qGridLyout->addWidget(amplfTimes[iLoop], iLoop, 2, Qt::AlignRight);
        qGridLyout->addWidget(chBoxAmp[iLoop], iLoop, 3, Qt::AlignLeft);

        amplfTimes[iLoop]->setMaximum(1000);
        amplfTimes[iLoop]->setSingleStep(10);
    }

    for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_GENER; iLoop++)
    {
        chBoxGen[iLoop] = new QCheckBox(allNamesGen[iLoop], this);
        chBoxGen[iLoop]->setEnabled(false);
        generTimes[iLoop] = new QSpinBox(this);
        qGridLyout->addWidget(generTimes[iLoop], iLoop, 4, Qt::AlignRight);
        qGridLyout->addWidget(chBoxGen[iLoop], iLoop, 5, Qt::AlignLeft);

        generTimes[iLoop]->setMaximum(1000);
        generTimes[iLoop]->setSingleStep(10);
    }

    for(int iLoop = 0; iLoop < E_NMB_BIT_FLAGS_STATUS / 2; iLoop++)
    {
        chBoxBitStatus[iLoop] = new QCheckBox(allNamesBitStatus[iLoop], this);
        qGridLyout->addWidget(chBoxBitStatus[iLoop], iLoop, 6, Qt::AlignLeft);
    }

    for(int iLoop = 0; iLoop < E_NMB_BIT_FLAGS_STATUS / 2; iLoop++)
    {
        chBoxBitStatus[E_NMB_BIT_FLAGS_STATUS / 2 + iLoop] = new QCheckBox(allNamesBitStatus[E_NMB_BIT_FLAGS_STATUS / 2 + iLoop], this);
        qGridLyout->addWidget(chBoxBitStatus[E_NMB_BIT_FLAGS_STATUS / 2 + iLoop], iLoop, 7, Qt::AlignLeft);
    }

    for(int iLoop = 0; iLoop < E_NMB_ITEMS_STATUS; iLoop++)
    {
        lineInputItemsStatus[iLoop] = new QLabel(this);
        qGridLyout->addWidget(new QLabel(allNamesItemsStatus[iLoop]), iLoop, 8, Qt::AlignRight);
        qGridLyout->addWidget(lineInputItemsStatus[iLoop], iLoop, 9, Qt::AlignLeft);
    }


    for(int iLoop = 0; iLoop < E_NMB_SLAVE_DEVICES; iLoop++)
    {
        labelFirmwareVersion[iLoop] = new QLabel(this);
        qGridLyout->addWidget(new QLabel(allNamesFirmwareVersion[iLoop]), E_NMB_ITEMS_STATUS + 2 + iLoop, 8, Qt::AlignRight);
        qGridLyout->addWidget(labelFirmwareVersion[iLoop], E_NMB_ITEMS_STATUS + 2 + iLoop, 9, Qt::AlignLeft);

        ReceiveFirmwareVersion(iLoop, 0);
    }


    vBox->addLayout(qGridLyout);
    vBox->addWidget(textBrowser);
    vBox->addWidget(buttClear);

    connect(buttClear,&QPushButton::clicked,[=](bool clicked){

        Q_UNUSED(clicked);
        textBrowser->clear();
    });

    connect(textBrowser,&QTextBrowser::anchorClicked,[=](const QUrl &arg1){

        QDesktopServices::openUrl(arg1);
    });

    for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_AMPLF; iLoop++)
    {
        connect(chBoxAmp[iLoop],&QCheckBox::clicked,[=](bool clicked){

            Q_UNUSED(clicked);
            //qDebug() << iLoop << "amp checkbox:" << chBoxAmp[iLoop]->checkState();


            QString SaveData;

            for(int jLoop = 0; jLoop < NMB_ITEMS_TIMERS_AMPLF; jLoop++)
            {
                SaveData.append(QString("%1").arg(chBoxAmp[jLoop]->checkState() ==  Qt::Checked ? "1" : "0") + " ");
            }

            emit SaveReadMsgsAmplf(SaveData);
            emit SendV200Requirement(chBoxAmp[iLoop]->checkState(), 0, iLoop);
        });

        connect(amplfTimes[iLoop],static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int value){

            emit SendNewTime(value, 0, iLoop);
        });
    }

    for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_GENER; iLoop++)
    {
        connect(chBoxGen[iLoop],&QCheckBox::clicked,[=](bool clicked){

            Q_UNUSED(clicked);
            //qDebug() << iLoop << "gen checkbox:" << chBoxGen[iLoop]->checkState();

            QString SaveData;

            for(int jLoop = 0; jLoop < NMB_ITEMS_TIMERS_GENER; jLoop++)
            {
                SaveData.append(QString("%1").arg(chBoxGen[jLoop]->checkState() ==  Qt::Checked ? "1" : "0") + " ");
            }

            emit SaveReadMsgsGener(SaveData);
            emit SendV200Requirement(chBoxGen[iLoop]->checkState(), 1, iLoop);
        });

        connect(generTimes[iLoop],static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int value){

            emit SendNewTime(value, 1, iLoop);
        });
    }

    for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_APLS_AND_USN; iLoop++)
    {
        connect(chBoxAplUsn[iLoop],&QCheckBox::clicked,[=](bool clicked){

            Q_UNUSED(clicked);
            //qDebug() << iLoop << "aplUsn checkbox:" << chBoxAplUsn[iLoop]->checkState();

            QString SaveData;

            for(int jLoop = 0; jLoop < NMB_ITEMS_TIMERS_APLS_AND_USN; jLoop++)
            {
                SaveData.append(QString("%1").arg(chBoxAplUsn[jLoop]->checkState() ==  Qt::Checked ? "1" : "0") + " ");
            }

            emit SaveReadMsgsAplUsn(SaveData);
            emit SendV200Requirement(chBoxAplUsn[iLoop]->checkState(), 2, iLoop);
        });

        connect(aplUsnTimes[iLoop],static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int value){

            emit SendNewTime(value, 2, iLoop);
        });
    }

}

Qt::CheckState widgetReading::GetCheckStateAmplf(int nIndex)
{
    return chBoxAmp[nIndex]->checkState();
}

Qt::CheckState widgetReading::GetCheckStateGener(int nIndex)
{
    return chBoxGen[nIndex]->checkState();
}

Qt::CheckState widgetReading::GetCheckStateAplUsn(int nIndex)
{
    return chBoxAplUsn[nIndex]->checkState();
}

bool widgetReading::GetEnabledAmplf(int nIndex)
{
    return chBoxAmp[nIndex]->isEnabled();
}

bool widgetReading::GetEnabledGener(int nIndex)
{
    return chBoxGen[nIndex]->isEnabled();
}

bool widgetReading::GetEnabledAplUsn(int nIndex)
{
    return chBoxAplUsn[nIndex]->isEnabled();
}

void widgetReading::disableAll()
{
    for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_APLS_AND_USN; iLoop++)
    {
        chBoxAplUsn[iLoop]->setEnabled(false);
    }

    for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_AMPLF; iLoop++)
    {
        chBoxAmp[iLoop]->setEnabled(false);
    }

    for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_GENER; iLoop++)
    {
        chBoxGen[iLoop]->setEnabled(false);
    }

    for(int iLoop = 0; iLoop < E_NMB_SLAVE_DEVICES; iLoop++)
    {
        ReceiveFirmwareVersion(iLoop, 0);
    }
}

void widgetReading::showTextLog(QString showText)
{
    textBrowser->append(showText);
}

void widgetReading::ReadTimeRequests(int device, int index, int value)
{
    if(device == 0)
    {
        amplfTimes[index]->setValue(value);
    }
    else if(device == 1)
    {
        generTimes[index]->setValue(value);
    }
    else if(device == 2)
    {
        aplUsnTimes[index]->setValue(value);
    }
}

void widgetReading::ReceiveStatusReg(STATUS_REGISTER eStatusReg)
{
    chBoxBitStatus[0]->setChecked(eStatusReg.m_Reg.m_Bit.ChangeAcc0 == 1 ? true : false);
    chBoxBitStatus[1]->setChecked(eStatusReg.m_Reg.m_Bit.ChangeAcc1 == 1 ? true : false);
    chBoxBitStatus[2]->setChecked(eStatusReg.m_Reg.m_Bit.ChangeAcc2 == 1 ? true : false);
    chBoxBitStatus[3]->setChecked(eStatusReg.m_Reg.m_Bit.ChangeAcc3 == 1 ? true : false);
    chBoxBitStatus[4]->setChecked(eStatusReg.m_Reg.m_Bit.Empty0 == 1 ? true : false);
    chBoxBitStatus[5]->setChecked(eStatusReg.m_Reg.m_Bit.SelfTestDone == 1 ? true : false);
    chBoxBitStatus[6]->setChecked(eStatusReg.m_Reg.m_Bit.Restart == 1 ? true : false);
    chBoxBitStatus[7]->setChecked(eStatusReg.m_Reg.m_Bit.Error == 1 ? true : false);
    chBoxBitStatus[8]->setChecked(eStatusReg.m_Reg.m_Bit.ButtonPressed == 1 ? true : false);
    chBoxBitStatus[9]->setChecked(eStatusReg.m_Reg.m_Bit.KeyChanged == 1 ? true : false);
    chBoxBitStatus[10]->setChecked(eStatusReg.m_Reg.m_Bit.TherapyRunning == 1 ? true : false);
    chBoxBitStatus[11]->setChecked(eStatusReg.m_Reg.m_Bit.ParChangedByMas == 1 ? true : false);
    chBoxBitStatus[12]->setChecked(eStatusReg.m_Reg.m_Bit.ParChangedByApp == 1 ? true : false);
    chBoxBitStatus[13]->setChecked(eStatusReg.m_Reg.m_Bit.SyncWaiting == 1 ? true : false);
    chBoxBitStatus[14]->setChecked(eStatusReg.m_Reg.m_Bit.EmergencyPressed == 1 ? true : false);
    chBoxBitStatus[15]->setChecked(eStatusReg.m_Reg.m_Bit.LogRequest == 1 ? true : false);

    chBoxBitStatus[16]->setChecked(eStatusReg.m_Reg.m_Bit.ChangeSmartDevice0 == 1 ? true : false);
    chBoxBitStatus[17]->setChecked(eStatusReg.m_Reg.m_Bit.StateAcc0 == 1 ? true : false);
    chBoxBitStatus[18]->setChecked(eStatusReg.m_Reg.m_Bit.StateAcc1 == 1 ? true : false);
    chBoxBitStatus[19]->setChecked(eStatusReg.m_Reg.m_Bit.StateAcc2 == 1 ? true : false);
    chBoxBitStatus[20]->setChecked(eStatusReg.m_Reg.m_Bit.StateAcc3 == 1 ? true : false);
    chBoxBitStatus[21]->setChecked(eStatusReg.m_Reg.m_Bit.StateSmartDevice0 == 1 ? true : false);
    chBoxBitStatus[22]->setChecked(eStatusReg.m_Reg.m_Bit.ContactPatient == 1 ? true : false);
    chBoxBitStatus[23]->setChecked(eStatusReg.m_Reg.m_Bit.ContactNeutral == 1 ? true : false);


    if(eStatusReg.m_Reg.m_Bit.ChoosedChannel == 3)
    {
        chBoxBitStatus[24]->setChecked(true);
        chBoxBitStatus[25]->setChecked(true);
    }
    else if(eStatusReg.m_Reg.m_Bit.ChoosedChannel == 2)
    {
        chBoxBitStatus[24]->setChecked(false);
        chBoxBitStatus[25]->setChecked(true);
    }
    else if(eStatusReg.m_Reg.m_Bit.ChoosedChannel == 1)
    {
        chBoxBitStatus[24]->setChecked(true);
        chBoxBitStatus[25]->setChecked(false);
    }
    else if(eStatusReg.m_Reg.m_Bit.ChoosedChannel == 0)
    {
        chBoxBitStatus[24]->setChecked(false);
        chBoxBitStatus[25]->setChecked(false);
    }

    chBoxBitStatus[26]->setChecked(eStatusReg.m_Reg.m_Bit.ChsdChannelCorrect == 1 ? true : false);
    chBoxBitStatus[27]->setChecked(eStatusReg.m_Reg.m_Bit.SlavesSynchOnDone == 1 ? true : false);
    chBoxBitStatus[28]->setChecked(eStatusReg.m_Reg.m_Bit.ChangeSmartDevice1 == 1 ? true : false);
    chBoxBitStatus[29]->setChecked(eStatusReg.m_Reg.m_Bit.StateSmartDevice1 == 1 ? true : false);

    if(eStatusReg.m_Reg.m_Bit.StateTherapy == 3)
    {
        chBoxBitStatus[30]->setChecked(true);
        chBoxBitStatus[31]->setChecked(true);
    }
    else if(eStatusReg.m_Reg.m_Bit.StateTherapy == 2)
    {
        chBoxBitStatus[30]->setChecked(false);
        chBoxBitStatus[31]->setChecked(true);
    }
    else if(eStatusReg.m_Reg.m_Bit.StateTherapy == 1)
    {
        chBoxBitStatus[30]->setChecked(true);
        chBoxBitStatus[31]->setChecked(false);
    }
    else if(eStatusReg.m_Reg.m_Bit.StateTherapy == 0)
    {
        chBoxBitStatus[30]->setChecked(false);
        chBoxBitStatus[31]->setChecked(false);
    }



    lineInputItemsStatus[0]->setText(QString("%1").arg(QString::number(float(eStatusReg.m_bySetIntensityUsn) / 10)));
    lineInputItemsStatus[1]->setText(QString("%1").arg(QString::number(eStatusReg.m_wMeasuredPower)));
    lineInputItemsStatus[2]->setText(QString("%1").arg(QString::number(eStatusReg.m_wSetPower)));
    double dbTempPatient = (float)eStatusReg.m_wMeasuredTemperaturePatient / 100.0f;
    lineInputItemsStatus[3]->setText(QString("%1").arg(QString::number(dbTempPatient)));
    lineInputItemsStatus[4]->setText(QString("%1").arg(QString::number(eStatusReg.m_bySetTemperaturePatient)));
}

void widgetReading::ReceiveFirmwareVersion(int nIndex, uint nValue)
{
    qDebug() << nIndex;

    if(nValue)
    {
        labelFirmwareVersion[nIndex]->setText(QString("%1").arg(QString::number(nValue)));

        if(nIndex == 0)
        {
            chBoxAplUsn[E_APL_LARGE]->setEnabled(true);
            SendV200Requirement(chBoxAplUsn[E_APL_LARGE]->checkState(), 2, E_APL_LARGE);
        }
        else if(nIndex == 1)
        {
            chBoxAplUsn[E_APL_SMALL_1]->setEnabled(true);
            SendV200Requirement(chBoxAplUsn[E_APL_SMALL_1]->checkState(), 2, E_APL_SMALL_1);
        }
        else if(nIndex == 2)
        {
            chBoxAplUsn[E_APL_SMALL_2]->setEnabled(true);
            SendV200Requirement(chBoxAplUsn[E_APL_SMALL_2]->checkState(), 2, E_APL_SMALL_2);
        }
        else if(nIndex == 3)
        {
            chBoxAplUsn[E_APL_SMALL_3]->setEnabled(true);
            SendV200Requirement(chBoxAplUsn[E_APL_SMALL_3]->checkState(), 2, E_APL_SMALL_3);
        }
        else if(nIndex == 4)
        {
            for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_AMPLF; iLoop++)
            {
                if(iLoop != 2)
                {
                    chBoxAmp[iLoop]->setEnabled(true);
                    SendV200Requirement(chBoxAmp[iLoop]->checkState(), 0, iLoop);
                }
            }
        }
        else if(nIndex == 5)
        {
            for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_GENER; iLoop++)
            {
                if(iLoop != 4)
                {
                    chBoxGen[iLoop]->setEnabled(true);
                    SendV200Requirement(chBoxGen[iLoop]->checkState(), 1, iLoop);
                }
            }
        }
        else if(nIndex == 6)
        {
            chBoxAplUsn[E_USN]->setEnabled(true);
            SendV200Requirement(chBoxAplUsn[E_USN]->checkState(), 2, E_USN);
        }
    }
    else
    {
        labelFirmwareVersion[nIndex]->setText(QString("unknown"));

        if(nIndex == 0)
        {
            chBoxAplUsn[E_APL_LARGE]->setEnabled(false);
        }
        else if(nIndex == 1)
        {
            chBoxAplUsn[E_APL_SMALL_1]->setEnabled(false);
        }
        else if(nIndex == 2)
        {
            chBoxAplUsn[E_APL_SMALL_2]->setEnabled(false);
        }
        else if(nIndex == 3)
        {
            chBoxAplUsn[E_APL_SMALL_3]->setEnabled(false);
        }
        else if(nIndex == 4)
        {
            for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_AMPLF; iLoop++)
            {
                if(iLoop != 2)
                {
                    chBoxAmp[iLoop]->setEnabled(false);
                }
            }
        }
        else if(nIndex == 5)
        {
            for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_GENER; iLoop++)
            {
                if(iLoop != 4)
                {
                    chBoxGen[iLoop]->setEnabled(false);
                }
            }
        }
        else if(nIndex == 6)
        {
            chBoxAplUsn[E_USN]->setEnabled(false);
        }
    }


}

void widgetReading::ReceiveRcvMsgAmp(QString data)
{
    QStringList arrListSaved;
    arrListSaved = data.split(QRegExp("\\s+"));

    if((arrListSaved.count() - 1) == NMB_ITEMS_TIMERS_AMPLF)
    {
        for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_AMPLF; iLoop++)
        {
            chBoxAmp[iLoop]->setCheckState(arrListSaved.at(iLoop) == "1" ? Qt::Checked : Qt::Unchecked);
        }
    }
    else
    {
        for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_AMPLF; iLoop++)
        {
            if(iLoop != 2)
            {
                chBoxAmp[iLoop]->setCheckState(Qt::Checked);
            }
        }
    }
}

void widgetReading::ReceiveRcvMsgGen(QString data)
{
    QStringList arrListSaved;
    arrListSaved = data.split(QRegExp("\\s+"));

    if((arrListSaved.count() - 1) == NMB_ITEMS_TIMERS_GENER)
    {
        for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_GENER; iLoop++)
        {
            chBoxGen[iLoop]->setCheckState(arrListSaved.at(iLoop) == "1" ? Qt::Checked : Qt::Unchecked);
        }
    }
    else
    {
        for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_GENER; iLoop++)
        {
            if(iLoop != 4)
            {
                chBoxGen[iLoop]->setCheckState(Qt::Checked);
            }
        }
    }
}

void widgetReading::ReceiveRcvMsgAplUsn(QString data)
{
    QStringList arrListSaved;
    arrListSaved = data.split(QRegExp("\\s+"));

    if((arrListSaved.count() - 1) == NMB_ITEMS_TIMERS_APLS_AND_USN)
    {
        for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_APLS_AND_USN; iLoop++)
        {
            chBoxAplUsn[iLoop]->setCheckState(arrListSaved.at(iLoop) == "1" ? Qt::Checked : Qt::Unchecked);
        }
    }
    else
    {
        for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_APLS_AND_USN; iLoop++)
        {
            if(iLoop != 4)
            {
                chBoxAplUsn[iLoop]->setCheckState(Qt::Checked);
            }
        }
    }
}

void widgetReading::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);

}
