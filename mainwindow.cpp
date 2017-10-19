/***************************************************************************
**
** Copyright (C) 2017 Medical Technologies CZ a.s.. All rights reserved.
** Contact: http://www.medictech.com
**
** Without prior written permission from Medical Technologies CZ a.s.,
** the file must not be modified, distributed and/or otherwise used.
**
****************************************************************************/

/*!
  \file   mainwindow.cpp
  \author Radek Rezac
  \date   22. 5. 2017
  \brief  GUI testing
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QTextStream>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    static int mMainTimer = 0;

    ui->setupUi(this);

    installEventFilter(this);

    ui->statusBar->showMessage(QString("Start app %1").arg(APP_VERSION));
    qDebug() << "Start of application.";
    qDebug() << "screen width:" << QApplication::desktop()->screenGeometry().width() << ", screen height:" << QApplication::desktop()->screenGeometry().height();


    connect(this, &MainWindow::SendNewImpedanceData, p_WidgetSmith, &widgetSmith::ReceivedNewData);
    connect(this, &MainWindow::SendUpdateGraph, p_WidgetGraph, &widgetGraph::refreshGraph);
    connect(this, &MainWindow::SendTextIntoLog, p_WidgetReading, &widgetReading::showTextLog);

    connect(p_WidgetReading, &widgetReading::SendV200Requirement, this, &MainWindow::onNewMsgReqReceived);
    connect(p_WidgetReading, &widgetReading::SendNewTime, this, &MainWindow::onNewTimeRequest);
    connect(p_WidgetTherapy, &widgetTherapy::SendV200specific, this, &MainWindow::specificMessageProtocol);
    connect(p_WidgetConfig, &widgetConfig::SendV200specific, this, &MainWindow::specificMessageProtocol);
    connect(p_widgetSettings, &widgetSettings::SendV200specific, this, &MainWindow::specificMessageProtocol);
    connect(p_WidgetTipMemory, &widgetTipMemory::SendV200specific, this, &MainWindow::specificMessageProtocol);

    connect(p_WidgetSmith, &widgetSmith::SaveData, appSettings, &settings::StoreSmithPoints);
    connect(this, &MainWindow::SendSmithPoints, p_WidgetSmith, &widgetSmith::ReadData);

    connect(this, &MainWindow::SendTimeRequests, p_WidgetReading, &widgetReading::ReadTimeRequests);

    connect(p_WidgetGraph, &widgetGraph::SaveAxisRangeHigh, appSettings, &settings::StoreHighValueSignal);
    connect(p_WidgetGraph, &widgetGraph::SaveAxisRangeLow, appSettings, &settings::StoreLowValueSignal);
    connect(this, &MainWindow::SendAxisHigh, p_WidgetGraph, &widgetGraph::readAxisHigh);
    connect(this, &MainWindow::SendAxisLow, p_WidgetGraph, &widgetGraph::readAxisLow);

    connect(p_WidgetConfig, &widgetConfig::SaveAdcData, appSettings, &settings::StoreAdcData);
    connect(this, &MainWindow::SendAdcData, p_WidgetConfig, &widgetConfig::ReadAdcData);

    connect(this, &MainWindow::SendConfigGener, p_WidgetConfig, &widgetConfig::ReadConfigGener);
    connect(this, &MainWindow::SendSettingsGener, p_widgetSettings, &widgetSettings::ReadSettingsGener);

    connect(p_widgetSettings, &widgetSettings::SaveAmpFreq, appSettings, &settings::StoreAmpFreq);
    connect(this, &MainWindow::SendAmpFreq, p_widgetSettings, &widgetSettings::ReadAmpFreq);
    connect(p_widgetSettings, &widgetSettings::SaveAmpPwm, appSettings, &settings::StoreAmpPwm);
    connect(this, &MainWindow::SendAmpPwm, p_widgetSettings, &widgetSettings::ReadAmpPwm);

    connect(this, &MainWindow::SendStatusReg, p_WidgetTherapy, &widgetTherapy::ReceiveStatusReg);
    connect(this, &MainWindow::SendStatusReg, p_WidgetReading, &widgetReading::ReceiveStatusReg);
    connect(this, &MainWindow::SendStatusReg, p_WidgetTipMemory, &widgetTipMemory::ReceiveStatusReg);

    connect(p_WidgetConfig, &widgetConfig::SendReferenceImpedance, p_WidgetSmith, &widgetSmith::ReadDefaultReferenceImpedance);

    connect(this, &MainWindow::SendFirmwareVersion, p_WidgetReading, &widgetReading::ReceiveFirmwareVersion);

    connect(p_WidgetAdmin, &widgetAdmin::SaveAdmin, appSettings, &settings::StoreAdmin);
    connect(this, &MainWindow::SendAdmin, p_WidgetAdmin, &widgetAdmin::ReadAdmin);

    connect(p_WidgetAdmin, &widgetAdmin::SetLimitSlider, p_WidgetTherapy, &widgetTherapy::ReceiveLimitSlider);

    connect(p_WidgetReading, &widgetReading::SaveReadMsgsAmplf, appSettings, &settings::StoreRcvMsgAmp);
    connect(p_WidgetConfig, &widgetConfig::SendReferenceImpedance, p_WidgetSmith, &widgetSmith::ReadDefaultReferenceImpedance);
    connect(p_WidgetReading, &widgetReading::SaveReadMsgsGener, appSettings, &settings::StoreRcvMsgGen);
    connect(this, &MainWindow::SendRcvMsgGen, p_WidgetReading, &widgetReading::ReceiveRcvMsgGen);
    connect(p_WidgetReading, &widgetReading::SaveReadMsgsAplUsn, appSettings, &settings::StoreRcvMsgAplUsn);
    connect(this, &MainWindow::SendRcvMsgAplUsn, p_WidgetReading, &widgetReading::ReceiveRcvMsgAplUsn);

    connect(this, &MainWindow::SendTipMemory, p_WidgetTipMemory, &widgetTipMemory::ReceiveTipMemory);
    connect(p_WidgetTherapy, &widgetTherapy::ChoosedChannel, p_WidgetTipMemory, &widgetTipMemory::DecodeChoosedChannel);
    connect(p_WidgetTipMemory, &widgetTipMemory::SendReferenceImpedance, p_WidgetSmith, &widgetSmith::ReadReferenceImpedance);
    connect(p_WidgetTipMemory, &widgetTipMemory::SendDefaultReferenceImpedance, p_WidgetSmith, &widgetSmith::SetDefaultReferenceImpedance);
    connect(p_WidgetTipMemory, &widgetTipMemory::SendMaximalPower, p_WidgetTherapy, &widgetTherapy::ReceiveMaxPower);
    connect(p_WidgetTipMemory, &widgetTipMemory::SendDefaultMaximalPower, p_WidgetTherapy, &widgetTherapy::ReceiveDefaultMaxPower);

    ui->comboBox_1->addItem(QString("Generator (ID = %1d)").arg(constGenerID));
    ui->comboBox_1->addItem(QString("Amplifier (ID = %1d)").arg(constAmpID));


    restoreGeometry(appSettings->RestoreGeometryMain());
    SetAvaiblePorts();
    m_bSaveData = appSettings->RestoreSaveDataBox();
    ui->checkBox->setChecked(m_bSaveData);
    ui->comboBox_1->setCurrentIndex(appSettings->RestoreSelectedDevice());
    emit SendSmithPoints(appSettings->RestoreSmithPoints());
    emit SendAmpFreq(appSettings->RestoreAmpFreq());
    emit SendAmpPwm(appSettings->RestoreAmpPwm());
    emit SendRcvMsgAmp(appSettings->RestoreRcvMsgAmp());
    emit SendRcvMsgGen(appSettings->RestoreRcvMsgGen());
    emit SendRcvMsgAplUsn(appSettings->RestoreRcvMsgAplUsn());

    for(int iLoop = 0; iLoop < E_AMP_ADC_NMB; iLoop++)
    {
        emit SendAdcData(QString(c_nameAdd), QString(c_nameAmp), iLoop, appSettings->RestoreAdcData(QString(c_nameAdd), QString(c_nameAmp), iLoop + 1));
        emit SendAdcData(QString(c_nameMul), QString(c_nameAmp), iLoop, appSettings->RestoreAdcData(QString(c_nameMul), QString(c_nameAmp), iLoop + 1));
    }

    for(int iLoop = 0; iLoop < E_NMB_ITEMS_ADMIN; iLoop++)
    {
        emit SendAdmin(iLoop, appSettings->RestoreAdmin(iLoop));
    }

    for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
    {
        recvItems[iLoop] = 0.0;
        recStat[iLoop] = 0;
        sourceSignal[iLoop] = 0;
        sourceAd[iLoop] = 0;
        sourceSignText[iLoop] = "\0";

        emit SendAxisHigh(iLoop, appSettings->RestoreHighValueSignal(iLoop));
        emit SendAxisLow(iLoop, appSettings->RestoreLowValueSignal(iLoop));
    }

    for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_GENER; iLoop++)
    {
        eRequestsGenerAdcx[iLoop].timer.CurrentTime_ms = 0;
        eRequestsGenerAdcx[iLoop].timer.bEnable = false;
        eRequestsGenerAdcx[iLoop].timer.RequirementTime_ms = 50;
        eRequestsGenerAdcx[iLoop].isInProgress = false;

        emit SendTimeRequests(1, iLoop, appSettings->RestoreRefreshGener(iLoop));
    }

    for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_AMPLF; iLoop++)
    {
        eRequestsAmplifAdcx[iLoop].timer.CurrentTime_ms = 0;
        eRequestsAmplifAdcx[iLoop].timer.bEnable = false;
        eRequestsAmplifAdcx[iLoop].timer.RequirementTime_ms = 50;
        eRequestsAmplifAdcx[iLoop].isInProgress = false;

        emit SendTimeRequests(0, iLoop, appSettings->RestoreRefreshAmplif(iLoop));
    }

    for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_APLS_AND_USN; iLoop++)
    {
        eRequestsAplUsn[iLoop].timer.CurrentTime_ms = 0;
        eRequestsAplUsn[iLoop].timer.bEnable = false;
        eRequestsAplUsn[iLoop].timer.RequirementTime_ms = 50;
        eRequestsAplUsn[iLoop].isInProgress = false;

        emit SendTimeRequests(2, iLoop, appSettings->RestoreRefreshAplUsn(iLoop));
    }


    eRequestGenerInput.timer.CurrentTime_ms = 0;
    eRequestGenerInput.timer.bEnable = false;
    eRequestGenerInput.timer.RequirementTime_ms = 50;
    eRequestGenerInput.isInProgress = false;

    eRequestAmplfInput.timer.CurrentTime_ms = 0;
    eRequestAmplfInput.timer.bEnable = false;
    eRequestAmplfInput.timer.RequirementTime_ms = 50;
    eRequestAmplfInput.isInProgress = false;

    ui->verticalLayout->addWidget(p_WidgetReading);
    ui->verticalLayout_2->addWidget(p_WidgetConfig);
    ui->verticalLayout_4->addWidget(p_widgetSettings);
    ui->verticalLayout_3->addWidget(p_WidgetGraph);
    ui->verticalLayout_3->addWidget(p_WidgetSmith);
    ui->verticalLayout_3->addWidget(p_WidgetTherapy);
    ui->verticalLayout_5->addWidget(p_WidgetAdmin);
    ui->verticalLayout_6->addWidget(p_WidgetTipMemory);

    timeCurrent.start();



    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::changed_table);

    connect(&TmrMstr,&QTimer::timeout,[this](){

        HasTimerRequestsExpiredGenAmpl(GENERATOR_SOURCE);
        HasTimerRequestsExpiredGenAmpl(AMPLIFIER_SOURCE);
        HasTimerRequestsExpiredAplUsn();

        HasTimerInputExpired(GENERATOR_SOURCE);
        HasTimerInputExpired(AMPLIFIER_SOURCE);

        if(++mMainTimer >= 100)//Read Status register each xxx ms
        {
            mMainTimer = 0;

            QString strCmd = QString("%1").arg(QString::number(PID_SEND_STATUS_REGISTER, 16));
            m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(strCmd.toStdString().c_str()), true);
        }
    });

    connect(p_WidgetGraph, static_cast<void (widgetGraph::*)(int, int)>(&widgetGraph::FlagSignalChoosed),[=](int signalIndex, int value){

        if(sourceDataStream == LOG_STREAM && value >= 0)
        {
            CheckedIfIndexInQlist(signalIndex, 0);//send fake in order to clear history signal
        }
        CheckedIfIndexInQlist(signalIndex, value);
    });

    connect(ui->comboBox_1,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[=](int nValue){
        selectedDeviceSetAccordingSaved(nValue);

        appSettings->StoreSelectedDevice(ui->comboBox_1->currentIndex());
        //-FillTableContent();

    });


    m_CommProt.reset(CommProtV200_Create(this));
    Q_ASSERT(!m_CommProt.isNull());

    connect(m_CommProt.data(), &CommProtV200::DataReceived, [this](qint32 nTransactionID, qint32 nDeviceID, QByteArray arrData) {
        // parse received data here
        Q_UNUSED(nTransactionID);
        Q_UNUSED(nDeviceID);

        newDataV200(arrData);
    });

    connect(m_CommProt.data(), &CommProtV200::Log, [this](QString strClassName, QTime oTime, QString strMessage) {
        Q_UNUSED(strClassName);
        Q_UNUSED(oTime);
        Q_UNUSED(strMessage);
        //qDebug() << "log:" << strMessage;
    });

    connect(m_CommProt.data(), &CommProtV200::Error, [this](qint32 nError, QString strError, QVariant vErrorData, QTime oErrorTime) {
        Q_UNUSED(nError);
        Q_UNUSED(oErrorTime);

        qDebug() << "Communication error:" << strError << "with internal ID:" << vErrorData.toInt();
        ui->statusBar->showMessage("Communication error: " + strError);

        for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_GENER; iLoop++)
        {
            if(eRequestsGenerAdcx[iLoop].internalIdClass == vErrorData.toInt())
            {
               eRequestsGenerAdcx[iLoop].isInProgress = false;
               return;
            }
        }

        for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_AMPLF; iLoop++)
        {
            if(eRequestsAmplifAdcx[iLoop].internalIdClass == vErrorData.toInt())
            {
               eRequestsAmplifAdcx[iLoop].isInProgress = false;
               return;
            }
        }

        for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_APLS_AND_USN; iLoop++)
        {
            if(eRequestsAplUsn[iLoop].internalIdClass == vErrorData.toInt())
            {
               eRequestsAplUsn[iLoop].isInProgress = false;
               return;
            }
        }

        if(eRequestGenerInput.internalIdClass == vErrorData.toInt())
        {
           eRequestGenerInput.isInProgress = false;
           return;
        }

        if(eRequestAmplfInput.internalIdClass == vErrorData.toInt())
        {
           eRequestAmplfInput.isInProgress = false;
           return;
        }
    });

    connect(m_CommProt.data(), &CommProtV200::StateChanged, [this](CommProtInterface::CommProtState eState) {
        qDebug() << "state:";
        switch (eState) {
        case CommProtInterface::Disabled:
            TmrMstr.stop();
            qDebug() << "Disabled";
            break;
        case CommProtInterface::Connected:
            qDebug() << "Connected";
            m_CommProt.data()->SetEnabled(true);
            break;
        case CommProtInterface::Enabled:
            TmrMstr.start(1);
            qDebug() << "Enabled";

            //initial delay for properly run out of device
            QThread::msleep(800);

            for(qint32 iLoop = 0; iLoop < NMB_ITEMS_TIMERS_AMPLF; iLoop++)
            {
                if(p_WidgetReading->GetEnabledAmplf(iLoop))
                {
                    onNewMsgReqReceived(p_WidgetReading->GetCheckStateAmplf(iLoop), 0, iLoop);
                }
            }

            for(qint32 iLoop = 0; iLoop < NMB_ITEMS_TIMERS_GENER; iLoop++)
            {
                if(p_WidgetReading->GetEnabledGener(iLoop))
                {
                    onNewMsgReqReceived(p_WidgetReading->GetCheckStateGener(iLoop), 1, iLoop);
                }
            }

            for(qint32 iLoop = 0; iLoop < NMB_ITEMS_TIMERS_APLS_AND_USN; iLoop++)
            {
                if(p_WidgetReading->GetEnabledAplUsn(iLoop))
                {
                    onNewMsgReqReceived(p_WidgetReading->GetCheckStateAplUsn(iLoop), 2, iLoop);
                }
            }
            break;
        case CommProtInterface::Detecting:
            qDebug() << "Detecting";
            break;
        default:
            break;
        }
    });


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changed_table(int index)
{
    Q_UNUSED(index);

    if(index == 2)
    {
        p_widgetSettings->RefreshPage();
    }

    refreshPlot();
}

void MainWindow::onNewMsgReqReceived(Qt::CheckState m_newState, int m_device, int m_indexMsg)
{
    if(m_device == 0)
    {
        universalRequestMessageProtocol(m_newState, PID_SEND_AMP_TIMERS_RESULTS + m_indexMsg);
    }
    else if(m_device == 1)
    {
        universalRequestMessageProtocol(m_newState, PID_SEND_ADC3_ADJUSTED_DATA_GENER + m_indexMsg);
    }
    else if(m_device == 2)
    {
        uint32_t dwPidMsg;

        if(m_indexMsg == E_USN)
        {
            dwPidMsg = PID_READ_MEASURE_DATA_USN;
        }
        else if(m_indexMsg == E_APL_LARGE)
        {
            dwPidMsg = PID_READ_MEASURE_DATA_APL_L;
        }
        else if(m_indexMsg == E_APL_SMALL_1)
        {
            dwPidMsg = PID_READ_MEAS_APL_S_1_DEVEL;
        }
        else if(m_indexMsg == E_APL_SMALL_2)
        {
            dwPidMsg = PID_READ_MEAS_APL_S_2_DEVEL;
        }
        else if(m_indexMsg == E_APL_SMALL_3)
        {
            dwPidMsg = PID_READ_MEAS_APL_S_3_DEVEL;
        }

        universalRequestMessageProtocol(m_newState, dwPidMsg);
    }
    //qDebug() << "slot occours" << m_indexMsg;
}

void MainWindow::onNewTimeRequest(int valueTime, int m_device, int m_indexMsg)
{
    if(m_device == 0)
    {
        appSettings->StoreRefreshAmplif(m_indexMsg, valueTime);
        eRequestsAmplifAdcx[m_indexMsg].timer.RequirementTime_ms = valueTime;
    }
    else if(m_device == 1)
    {
        appSettings->StoreRefreshGener(m_indexMsg, valueTime);
        eRequestsGenerAdcx[m_indexMsg].timer.RequirementTime_ms = valueTime;
    }
    else if(m_device == 2)
    {
        appSettings->StoreRefreshAplUsn(m_indexMsg, valueTime);
        eRequestsAplUsn[m_indexMsg].timer.RequirementTime_ms = valueTime;
    }
}

void MainWindow::selectedDeviceSetAccordingSaved(quint32 value)
{
    if(value == GENERATOR_SOURCE)
    {
        m_nDeviceAddress = constGenerID;
    }
    else if(value == AMPLIFIER_SOURCE)
    {
        m_nDeviceAddress = constAmpID;
    }
}

void MainWindow::AppendText(QTime timestamp, QString strText, TEXT_BROWSERS eIndex)
{
    emit SendTextIntoLog(myTimeStamp(timestamp) + "\t" + strText, eIndex);
}

void MainWindow::on_connectButton_clicked()
{
    ui->checkBox->setEnabled(false);
    ui->comboBox->setEnabled(false);
    ui->comboBox_1->setEnabled(false);

    sourceDataStream = RECEIVE_STREAM;

    m_CommProt.data()->SetTargetMedium(ui->comboBox->currentText());
    appSettings->StorePortName(ui->comboBox->currentText());

    if(m_bSaveData)
    {
        for(int iLoop = 0; iLoop < E_BROWSER_NMB; iLoop++)
        {
            m_oFile[iLoop].setFileName(QString("%1_%2.log").arg(allNamedFiles[iLoop]).arg(QDateTime::currentDateTime().toTime_t()));
            if(!m_oFile[iLoop].open(QFile::WriteOnly))
            {
                qDebug() << "error: cannot open file" << iLoop;
                return;
            }
        }
    }

    m_bGeneratorConnected = false;
}

void MainWindow::SetAvaiblePorts()
{
    ui->comboBox->clear();

    QString strLastPortName = appSettings->RestorePortName();

    for(auto comPort : QSerialPortInfo::availablePorts())
    {
        ui->comboBox->addItem(comPort.portName());

        if(comPort.portName() == strLastPortName)
        {
            ui->comboBox->setCurrentText(strLastPortName);
        }
    }
}

void MainWindow::newDataV200(QByteArray aData)
{
    //actualize time
    timeCurrent.restart();

    //qDebug() << aData;

    ui->statusBar->showMessage("Data received: " + QString(aData.toHex()));
    QStringList myStringOnlyNumbers = adjustRowDataIntoOnlyNumber(aData);

    switch(uint8_t(aData.at(0)))
    {
    case 'g'://Gener´s data
        AppendText(timeCurrent, QString(aData), E_BROWSER_FOR_OSCILLOSCOPE);

        if(m_bSaveData)
        {
            m_oFile[E_BROWSER_FOR_OSCILLOSCOPE].write(myTimeStamp(timeCurrent).toUtf8() + "\t" + QString(aData).simplified().toUtf8() + "\r\n");
            m_oFile[E_BROWSER_FOR_OSCILLOSCOPE].flush();
        }

        if(aData.at(1) == '3' && aData.at(2) == 'c')//ADC3 adjusted data
        {
            recognizeIfDisplayNewDataAllSignals(timeCurrent, &myStringOnlyNumbers, 0);
            if(eRequestsGenerAdcx[0].isInProgress == true)
            {
                eRequestsGenerAdcx[0].isInProgress = false;
            }
        }
        else if(aData.at(1) == '3' && aData.at(2) == 's')//ADC3 average data
        {
            recognizeIfDisplayNewDataAllSignals(timeCurrent, &myStringOnlyNumbers, 1);
            if(eRequestsGenerAdcx[1].isInProgress == true)
            {
                eRequestsGenerAdcx[1].isInProgress = false;
            }
        }
        else if(aData.at(1) == '2' && aData.at(2) == 'c')//ADC2 adjusted data
        {
            recognizeIfDisplayNewDataAllSignals(timeCurrent, &myStringOnlyNumbers, 2);
            if(eRequestsGenerAdcx[2].isInProgress == true)
            {
                eRequestsGenerAdcx[2].isInProgress = false;
            }

            recognizeIfDisplayNewDataAllSignals(timeCurrent, &myStringOnlyNumbers, 2);

            emit SendNewImpedanceData(qreal(myStringOnlyNumbers.at(1).toFloat()), qreal(myStringOnlyNumbers.at(2).toFloat()),qreal(myStringOnlyNumbers.at(3).toFloat()), qreal(myStringOnlyNumbers.at(4).toFloat()));
        }
        else if(aData.at(1) == '2' && aData.at(2) == 's')//ADC2 average data
        {
            recognizeIfDisplayNewDataAllSignals(timeCurrent, &myStringOnlyNumbers, 3);
            if(eRequestsGenerAdcx[3].isInProgress == true)
            {
                eRequestsGenerAdcx[3].isInProgress = false;
            }
        }
        else if(aData.at(1) == '1' && aData.at(2) == 'c')//ADC1 adjusted data
        {
            recognizeIfDisplayNewDataAllSignals(timeCurrent, &myStringOnlyNumbers, 4);
            if(eRequestsGenerAdcx[4].isInProgress == true)
            {
                eRequestsGenerAdcx[4].isInProgress = false;
            }
        }
        else if(aData.at(1) == '1' && aData.at(2) == 's')//ADC1 average data
        {
            recognizeIfDisplayNewDataAllSignals(timeCurrent, &myStringOnlyNumbers, 5);
            if(eRequestsGenerAdcx[5].isInProgress == true)
            {
                eRequestsGenerAdcx[5].isInProgress = false;
            }
        }
        else if(aData.at(1) == 'i')//Digital input readed
        {
            if(eRequestGenerInput.isInProgress == true)
            {
                eRequestGenerInput.isInProgress = false;
            }
        }
        break;
    case 'u'://usn´s data
        AppendText(timeCurrent, QString(aData), E_BROWSER_FOR_OSCILLOSCOPE);

        if(m_bSaveData)
        {
            m_oFile[E_BROWSER_FOR_OSCILLOSCOPE].write(myTimeStamp(timeCurrent).toUtf8() + "\t" + QString(aData).simplified().toUtf8() + "\r\n");
            m_oFile[E_BROWSER_FOR_OSCILLOSCOPE].flush();
        }

        recognizeIfDisplayNewDataAllSignals(timeCurrent, &myStringOnlyNumbers, NMB_ITEMS_TIMERS_GENER + NMB_ITEMS_TIMERS_AMPLF + E_USN);

        if(eRequestsAplUsn[E_USN].isInProgress == true)
        {
            eRequestsAplUsn[E_USN].isInProgress = false;
        }

        break;
    case 'l'://app_l´s data
        AppendText(timeCurrent, QString(aData), E_BROWSER_FOR_OSCILLOSCOPE);

        if(m_bSaveData)
        {
            m_oFile[E_BROWSER_FOR_OSCILLOSCOPE].write(myTimeStamp(timeCurrent).toUtf8() + "\t" + QString(aData).simplified().toUtf8() + "\r\n");
            m_oFile[E_BROWSER_FOR_OSCILLOSCOPE].flush();
        }

        recognizeIfDisplayNewDataAllSignals(timeCurrent, &myStringOnlyNumbers, NMB_ITEMS_TIMERS_GENER + NMB_ITEMS_TIMERS_AMPLF + E_APL_LARGE);

        if(eRequestsAplUsn[E_APL_LARGE].isInProgress == true)
        {
            eRequestsAplUsn[E_APL_LARGE].isInProgress = false;
        }

        break;
    case 's'://app_s´s data
        AppendText(timeCurrent, QString(aData), E_BROWSER_FOR_OSCILLOSCOPE);

        if(m_bSaveData)
        {
            m_oFile[E_BROWSER_FOR_OSCILLOSCOPE].write(myTimeStamp(timeCurrent).toUtf8() + "\t" + QString(aData).simplified().toUtf8() + "\r\n");
            m_oFile[E_BROWSER_FOR_OSCILLOSCOPE].flush();
        }

        if(aData.at(1) == '1')//small_1
        {
            recognizeIfDisplayNewDataAllSignals(timeCurrent, &myStringOnlyNumbers, NMB_ITEMS_TIMERS_GENER + NMB_ITEMS_TIMERS_AMPLF + E_APL_SMALL_1);

            if(eRequestsAplUsn[E_APL_SMALL_1].isInProgress == true)
            {
                eRequestsAplUsn[E_APL_SMALL_1].isInProgress = false;
            }
        }
        else if(aData.at(1) == '2')//small_2
        {
            recognizeIfDisplayNewDataAllSignals(timeCurrent, &myStringOnlyNumbers, NMB_ITEMS_TIMERS_GENER + NMB_ITEMS_TIMERS_AMPLF + E_APL_SMALL_2);

            if(eRequestsAplUsn[E_APL_SMALL_2].isInProgress == true)
            {
                eRequestsAplUsn[E_APL_SMALL_2].isInProgress = false;
            }
        }
        else if(aData.at(1) == '3')//small_3
        {
            recognizeIfDisplayNewDataAllSignals(timeCurrent, &myStringOnlyNumbers, NMB_ITEMS_TIMERS_GENER + NMB_ITEMS_TIMERS_AMPLF + E_APL_SMALL_3);

            if(eRequestsAplUsn[E_APL_SMALL_3].isInProgress == true)
            {
                eRequestsAplUsn[E_APL_SMALL_3].isInProgress = false;
            }
        }

        break;
    case 'a'://Amp´s data
        AppendText(timeCurrent, QString(aData), E_BROWSER_FOR_OSCILLOSCOPE);

        if(m_bSaveData)
        {
            m_oFile[E_BROWSER_FOR_OSCILLOSCOPE].write(myTimeStamp(timeCurrent).toUtf8() + "\t" + QString(aData).simplified().toUtf8() + "\r\n");
            m_oFile[E_BROWSER_FOR_OSCILLOSCOPE].flush();
        }

        if(aData.at(1) == 't')//timers adjusted data
        {
            recognizeIfDisplayNewDataAllSignals(timeCurrent, &myStringOnlyNumbers, NMB_ITEMS_TIMERS_GENER);
            if(eRequestsAmplifAdcx[0].isInProgress == true)
            {
                eRequestsAmplifAdcx[0].isInProgress = false;
            }
        }
        else if(aData.at(1) == '3' && aData.at(2) == 's')//ADC3 average data
        {
            recognizeIfDisplayNewDataAllSignals(timeCurrent, &myStringOnlyNumbers, NMB_ITEMS_TIMERS_GENER + 1);
            if(eRequestsAmplifAdcx[1].isInProgress == true)
            {
                eRequestsAmplifAdcx[1].isInProgress = false;
            }
        }
        else if(aData.at(1) == 'x')//reserve data
        {
            recognizeIfDisplayNewDataAllSignals(timeCurrent, &myStringOnlyNumbers, NMB_ITEMS_TIMERS_GENER + 2);
            if(eRequestsAmplifAdcx[2].isInProgress == true)
            {
                eRequestsAmplifAdcx[2].isInProgress = false;
            }
        }
        else if(aData.at(1) == '1' && aData.at(2) == 's')//ADC1 average data
        {
            recognizeIfDisplayNewDataAllSignals(timeCurrent, &myStringOnlyNumbers, NMB_ITEMS_TIMERS_GENER + 3);
            if(eRequestsAmplifAdcx[3].isInProgress == true)
            {
                eRequestsAmplifAdcx[3].isInProgress = false;
            }
        }
        else if(aData.at(1) == 'i')//Digital input readed
        {
            if(eRequestAmplfInput.isInProgress == true)
            {
                eRequestAmplfInput.isInProgress = false;
            }
        }
        break;

    //generator config data
    case PID_REPLY_CONFIGURATION_DEVICE:
        SendConfigGener(aData);
        break;
    case PID_REPLY_SETTINGS_DEVICE:
        SendSettingsGener(aData);
        break;
    case PID_REPLY_TOUCH_MEMORY_OK:
        emit SendTipMemory(uint8_t(aData.at(1)), uint8_t(aData.at(4)), aData.mid(5));

        break;
    case PID_REPLY_SEND_STATUS_REGISTER:
        STATUS_REGISTER eStatusReg;

        eStatusReg.m_Reg.m_dwLong = uint32_t(aData.at(1) << 24) & 0xFF000000;
        eStatusReg.m_Reg.m_dwLong |= uint32_t(aData.at(2) << 16) & 0x00FF0000;
        eStatusReg.m_Reg.m_dwLong |= uint32_t(aData.at(3) << 8) & 0x0000FF00;
        eStatusReg.m_Reg.m_dwLong |= uint32_t(aData.at(4)) & 0x000000FF;
        eStatusReg.m_bySetTemperaturePatient = aData.at(5);
        eStatusReg.m_bySetIntensityUsn = aData.at(6);
        eStatusReg.m_wMeasuredPower = uint16_t(uint16_t(aData.at(7) << 8) & 0xFF00) | (uint16_t(aData.at(8)) & 0x00FF);
        eStatusReg.m_wSetPower = uint16_t(uint16_t(aData.at(9) << 8) & 0xFF00) | (uint16_t(aData.at(10)) & 0x00FF);
        eStatusReg.m_wMeasuredTemperaturePatient = int16_t(uint16_t(aData.at(11) << 8) & 0xFF00) | (uint16_t(aData.at(12)) & 0x00FF);
        eStatusReg.m_wSetDutyFactor = aData.at(13);
        eStatusReg.m_ByteExtended.mByExtended = aData.at(14);

        if(m_bGeneratorConnected == false)
        {
            m_bGeneratorConnected = true;

            QString strCmd = QString("%1").arg(QString::number(PID_SEND_HW_CONFIG, 16));
            strCmd += QString::number(2, 16).rightJustified(1 * 2, '0');
            m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(strCmd.toStdString().c_str()), true);
        }

        if(eStatusReg.m_Reg.m_Bit.ChangeSmartDevice1)
        {
            if(eStatusReg.m_Reg.m_Bit.StateSmartDevice1)
            {
                QString strCmd = QString("%1").arg(QString::number(PID_SEND_HW_CONFIG, 16));
                strCmd += QString::number(14, 16).rightJustified(1 * 2, '0');
                strCmd += QString::number(5, 16).rightJustified(1 * 2, '0');
                m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(strCmd.toStdString().c_str()), true);
            }
            else
            {
                emit SendFirmwareVersion(6, 0);
            }
        }

        if(eStatusReg.m_Reg.m_Bit.ChangeSmartDevice0)
        {
            if(eStatusReg.m_Reg.m_Bit.StateSmartDevice0)
            {
                QString strCmd = QString("%1").arg(QString::number(PID_SEND_HW_CONFIG, 16));
                strCmd += QString::number(14, 16).rightJustified(1 * 2, '0');
                strCmd += QString::number(4, 16).rightJustified(1 * 2, '0');
                m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(strCmd.toStdString().c_str()), true);
            }
            else
            {
                emit SendFirmwareVersion(4, 0);
            }
        }

        if(eStatusReg.m_Reg.m_Bit.ChangeAcc0)
        {
            if(eStatusReg.m_Reg.m_Bit.StateAcc0)
            {
                QString strCmd = QString("%1").arg(QString::number(PID_SEND_HW_CONFIG, 16));
                strCmd += QString::number(9, 16).rightJustified(1 * 2, '0');
                m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(strCmd.toStdString().c_str()), true);

                QString strCmdTip = QString("%1").arg(QString::number(PID_TOUCH_MEMORY, 16));
                strCmdTip += QString::number(128 + 0, 16).rightJustified(1 * 2, '0');
                m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(strCmdTip.toStdString().c_str()), true);
            }
            else
            {
                emit SendFirmwareVersion(0, 0);
            }
        }

        if(eStatusReg.m_Reg.m_Bit.ChangeAcc1)
        {
            if(eStatusReg.m_Reg.m_Bit.StateAcc1)
            {
                QString strCmd = QString("%1").arg(QString::number(PID_SEND_HW_CONFIG, 16));
                strCmd += QString::number(10, 16).rightJustified(1 * 2, '0');
                m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(strCmd.toStdString().c_str()), true);

                QString strCmdTip = QString("%1").arg(QString::number(PID_TOUCH_MEMORY, 16));
                strCmdTip += QString::number(128 + 1, 16).rightJustified(1 * 2, '0');
                m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(strCmdTip.toStdString().c_str()), true);
            }

            else
            {
                emit SendFirmwareVersion(1, 0);
            }
        }

        if(eStatusReg.m_Reg.m_Bit.ChangeAcc2)
        {
            if(eStatusReg.m_Reg.m_Bit.StateAcc2)
            {
                QString strCmd = QString("%1").arg(QString::number(PID_SEND_HW_CONFIG, 16));
                strCmd += QString::number(11, 16).rightJustified(1 * 2, '0');
                m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(strCmd.toStdString().c_str()), true);

                QString strCmdTip = QString("%1").arg(QString::number(PID_TOUCH_MEMORY, 16));
                strCmdTip += QString::number(128 + 2, 16).rightJustified(1 * 2, '0');
                m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(strCmdTip.toStdString().c_str()), true);
            }
            else
            {
                emit SendFirmwareVersion(2, 0);
            }
        }

        if(eStatusReg.m_Reg.m_Bit.ChangeAcc3)
        {
            if(eStatusReg.m_Reg.m_Bit.StateAcc3)
            {
                QString strCmd = QString("%1").arg(QString::number(PID_SEND_HW_CONFIG, 16));
                strCmd += QString::number(12, 16).rightJustified(1 * 2, '0');
                m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(strCmd.toStdString().c_str()), true);

                QString strCmdTip = QString("%1").arg(QString::number(PID_TOUCH_MEMORY, 16));
                strCmdTip += QString::number(128 + 3, 16).rightJustified(1 * 2, '0');
                m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(strCmdTip.toStdString().c_str()), true);
            }
            else
            {
                emit SendFirmwareVersion(3, 0);
            }
        }

        if(eStatusReg.m_Reg.m_Bit.LogRequest)
        {
            QString strCmd = QString("%1").arg(QString::number(PID_SET_LOGGING, 16));
            strCmd += QString::number(2, 16).rightJustified(1 * 2, '0');
            strCmd += QString::number(0, 16).rightJustified(1 * 2, '0');
            strCmd += QString::number(0, 16).rightJustified(1 * 2, '0');
            strCmd += QString::number(0, 16).rightJustified(1 * 2, '0');
            strCmd += QString::number(0, 16).rightJustified(1 * 2, '0');
            m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(strCmd.toStdString().c_str()), true);
        }

        emit SendStatusReg(eStatusReg);

        break;

    case PID_REPLY_SEND_LOGGING_ASCII:
        AppendText(timeCurrent, QString(aData.mid(3)), E_BROWSER_LOG_ASCII);

        if(m_bSaveData)
        {
            m_oFile[E_BROWSER_LOG_ASCII].write(myTimeStamp(timeCurrent).toUtf8() + "\t" + QString(aData.mid(3)).simplified().toUtf8() + "\r\n");
            m_oFile[E_BROWSER_LOG_ASCII].flush();
        }
        break;
    case PID_REPLY_SEND_HW_CONFIG:
        if((aData.at(1) >= 9 && aData.at(1) <= 12) || aData.at(1) == 14 || aData.at(1) == 2)
        {
            int nReceivedBytes = (uchar(aData.at(2)) << 8) + uchar(aData.at(3));

            if(nReceivedBytes >= 192)
            {
                uint nFirmwareVersion = (uchar(aData.at(4 + 88 + 3)) << 24) + (uchar(aData.at(4 + 88 + 2)) << 16) + (uchar(aData.at(4 + 88 + 1)) << 8) + uchar(aData.at(4 + 88));

                int nIndex;

                switch (aData.at(1))
                {
                case 2:
                    nIndex = 5;
                    break;
                case 14:
                    if(aData.at(4 + nReceivedBytes) == 4)
                    {
                        nIndex = 4;
                    }
                    else if(aData.at(4 + nReceivedBytes) == 5)
                    {
                        nIndex = 6;
                    }
                    break;
                default:
                    nIndex = int(aData.at(1) - 9);
                    break;
                }

                emit SendFirmwareVersion(nIndex, nFirmwareVersion);
            }
        }
        break;
    default:
        break;
    }
}

void MainWindow::CheckedIfIndexInQlist(int NumberComboBox, int indexInComboBox)
{
    //qDebug() << "NumberComboBox_i" << NumberComboBox << "CheckedIfIndexInQlist_i: " << indexInComboBox;

    if(indexInComboBox >= 1)
    {
        recStat[NumberComboBox] = 1;
        int absoluteIndex = indexInComboBox - 1;

        if(GetIndexFromQlistGenAmp(GENERATOR_SOURCE, absoluteIndex, NumberComboBox, indexInComboBox))
        {
            return;
        }
        else if(GetIndexFromQlistGenAmp(AMPLIFIER_SOURCE, absoluteIndex, NumberComboBox, indexInComboBox))
        {
            return;
        }
        else
        {
            GetIndexFromQlistAplUsn(absoluteIndex, NumberComboBox, indexInComboBox);
        }
    }
    else
    {
        sourceSignText[NumberComboBox] = "\0";
        sourceSignal[NumberComboBox] = -1;
        recStat[NumberComboBox] = 0;
        emit SendUpdateGraph(timeCurrent, recvItems[NumberComboBox], recStat[NumberComboBox], sourceSignText[NumberComboBox], NumberComboBox, sourceDataStream, 0);
    }
}

bool MainWindow::GetIndexFromQlistGenAmp(MainWindow::SOURCE_DEVICE eSourceStream, int &dwAbsIndex, int dwNumberCmbBx, int dwIndexCmbBx)
{
    qint32 dwVolumeItems = (eSourceStream == GENERATOR_SOURCE) ? NMB_ITEMS_TIMERS_GENER : NMB_ITEMS_TIMERS_AMPLF;
    PERIODIC_REQUEST* p_sRequests = (eSourceStream == GENERATOR_SOURCE) ? eRequestsGenerAdcx : eRequestsAmplifAdcx;
    bool* p_bLogged = (eSourceStream == GENERATOR_SOURCE) ? flagIfSourceIsLoggedGener : flagIfSourceIsLoggedAmplf;
    const QStringList* p_sStringList = (eSourceStream == GENERATOR_SOURCE) ? allAdxSignalsGener : allAdxSignalsAmplf;
    const QString* p_sBaseString = (eSourceStream == GENERATOR_SOURCE) ? allSignalsBaseOnlyGener : allSignalsBaseOnlyAmplf;


    for(qint32 iLoop = 0; iLoop < dwVolumeItems; iLoop++)
    {
        if((sourceDataStream == RECEIVE_STREAM && p_sRequests[iLoop].timer.bEnable) || (sourceDataStream == LOG_STREAM && p_bLogged[iLoop]))
        {
            if((dwAbsIndex - p_sStringList[iLoop].count()) < 0)
            {
                sourceAd[dwNumberCmbBx] = (eSourceStream == GENERATOR_SOURCE) ? iLoop : NMB_ITEMS_TIMERS_GENER + iLoop;
                sourceSignal[dwNumberCmbBx] = dwAbsIndex;
                sourceSignText[dwNumberCmbBx] = p_sBaseString[iLoop];

                //qDebug() << "has been found signal:" << allAdxSignalsGener[iLoop].at(dwAbsIndex) << endl;
                //qDebug() << sourceSignText[dwNumberCmbBx];

                if(sourceDataStream == LOG_STREAM)
                {
                    QFile m_logFile(logPath);

                    if(!m_logFile.open(QFile::ReadOnly))
                    {
                        qDebug() << "error: cannot open file";
                    }
                    else
                    {
                        QTextStream fileStream(&m_logFile);

                        while (!fileStream.atEnd())
                        {
                            QString newLinereaded = fileStream.readLine();
                            QStringList stringsSplitted = newLinereaded.split(QRegExp("\\s+"));

                            //qDebug() << newLinereaded;


                            if(stringsSplitted[1] == sourceSignText[dwNumberCmbBx])//founded row appeared
                            {
                                QTime timeLog = QTime::fromString(stringsSplitted[0], "hh:mm:ss,zzz");
                                QStringList myStringOnlyNumbers = adjustRowDataIntoOnlyNumber(newLinereaded);

                                DisplayNewDataFromSignal(timeLog, &myStringOnlyNumbers, dwNumberCmbBx);
                            }

                        }

                        //send flag, that has been sending last one
                        emit SendUpdateGraph(QTime(0,0,0), 0, 0, " ", dwIndexCmbBx, sourceDataStream, 1);
                    }

                    m_logFile.close();
                }
                return true;
            }
            dwAbsIndex -= p_sStringList[iLoop].count();
        }
    }
    return false;
}

bool MainWindow::GetIndexFromQlistAplUsn(int &dwAbsIndex, int dwNumberCmbBx, int dwIndexCmbBx)
{
    for(qint32 iLoop = 0; iLoop < NMB_ITEMS_TIMERS_APLS_AND_USN; iLoop++)
    {
        if((sourceDataStream == RECEIVE_STREAM && eRequestsAplUsn[iLoop].timer.bEnable) || (sourceDataStream == LOG_STREAM && flagIfSourceIsLoggedAplUsn[iLoop]))
        {
            if((dwAbsIndex - allAdxSignalsAplUsn[iLoop].count()) < 0)
            {
                sourceAd[dwNumberCmbBx] = NMB_ITEMS_TIMERS_GENER + NMB_ITEMS_TIMERS_AMPLF + iLoop;
                sourceSignal[dwNumberCmbBx] = dwAbsIndex;
                sourceSignText[dwNumberCmbBx] = allSignalsBaseOnlyAplUsn[iLoop];

                //qDebug() << "has been found signal:" << allAdxSignalsGener[iLoop].at(dwAbsIndex) << endl;
                //qDebug() << sourceSignText[dwNumberCmbBx];

                if(sourceDataStream == LOG_STREAM)
                {
                    QFile m_logFile(logPath);

                    if(!m_logFile.open(QFile::ReadOnly))
                    {
                        qDebug() << "error: cannot open file";
                    }
                    else
                    {
                        QTextStream fileStream(&m_logFile);

                        while (!fileStream.atEnd())
                        {
                            QString newLinereaded = fileStream.readLine();
                            QStringList stringsSplitted = newLinereaded.split(QRegExp("\\s+"));

                            //qDebug() << newLinereaded;


                            if(stringsSplitted[1] == sourceSignText[dwNumberCmbBx])//founded row appeared
                            {
                                QTime timeLog = QTime::fromString(stringsSplitted[0], "hh:mm:ss,zzz");
                                QStringList myStringOnlyNumbers = adjustRowDataIntoOnlyNumber(newLinereaded);

                                DisplayNewDataFromSignal(timeLog, &myStringOnlyNumbers, dwNumberCmbBx);
                            }

                        }

                        //send flag, that has been sending last one
                        emit SendUpdateGraph(QTime(0,0,0), 0, 0, " ", dwIndexCmbBx, sourceDataStream, 1);
                    }

                    m_logFile.close();
                }
                return true;
            }
            dwAbsIndex -= allAdxSignalsAplUsn[iLoop].count();
        }
    }
    return false;
}

void MainWindow::SetTimerRequestsGenAmp(int wIndex, bool bOnOff, QString sCommand, MainWindow::SOURCE_DEVICE eSourceStream)
{
    qint32 dwPidItems = (eSourceStream == GENERATOR_SOURCE) ? QString::number(PID_SEND_ADC3_ADJUSTED_DATA_GENER, 16).toInt() : QString::number(PID_SEND_AMP_TIMERS_RESULTS, 16).toInt();
    qint32 dwVolumeItems = (eSourceStream == GENERATOR_SOURCE) ? NMB_ITEMS_TIMERS_GENER : NMB_ITEMS_TIMERS_AMPLF;
    PERIODIC_REQUEST* p_sRequests = (eSourceStream == GENERATOR_SOURCE) ? eRequestsGenerAdcx : eRequestsAmplifAdcx;


    for(qint32 row = 0; row < dwVolumeItems; row++)
    {
        if(dwPidItems + row == wIndex)
        {
            if(bOnOff == true)
            {
                p_sRequests[row].timer.CurrentTime_ms = p_sRequests[row].timer.RequirementTime_ms;
                p_sRequests[row].timer.bEnable = true;

                p_sRequests[row].assemblyMsq = QByteArray::fromHex(sCommand.toStdString().c_str());
                p_sRequests[row].respExp = true;
            }
            else
            {
                p_sRequests[row].timer.bEnable = false;
                p_sRequests[row].respExp = false;
            }

            break;
        }
    }
}

void MainWindow::SetTimerRequestsAplUsn(bool bOnOff, QString sCommand, MainWindow::SOURCE_DEVICE eSourceStream)
{
    uint32_t dwIndex;


    if(eSourceStream == APL_LARGE_SOURCE)
    {
        dwIndex = E_APL_LARGE;
    }
    else if(eSourceStream == APL_SMALL_1_SOURCE)
    {
        dwIndex = E_APL_SMALL_1;
    }
    else if(eSourceStream == APL_SMALL_2_SOURCE)
    {
        dwIndex = E_APL_SMALL_2;
    }
    else if(eSourceStream == APL_SMALL_3_SOURCE)
    {
        dwIndex = E_APL_SMALL_3;
    }
    else if(eSourceStream == USN_GENER_SOURCE)
    {
        dwIndex = E_USN;
    }

    if(bOnOff == true)
    {
        eRequestsAplUsn[dwIndex].timer.CurrentTime_ms = eRequestsAplUsn[dwIndex].timer.RequirementTime_ms;
        eRequestsAplUsn[dwIndex].timer.bEnable = true;

        eRequestsAplUsn[dwIndex].assemblyMsq = QByteArray::fromHex(sCommand.toStdString().c_str());
        eRequestsAplUsn[dwIndex].respExp = true;
    }
    else
    {
        eRequestsAplUsn[dwIndex].timer.bEnable = false;
        eRequestsAplUsn[dwIndex].respExp = false;
    }

}

void MainWindow::ShowSignalsIntoComboBox(SOURCE_STREAM eSourceStream)
{
    p_WidgetGraph->clearAll();
    p_WidgetGraph->addDashAll();

    qint32 dwIndexStart = 1;

    ShowSignalsGenAmplIfShould(eSourceStream, GENERATOR_SOURCE, dwIndexStart, COLOR_BLUE_DARK);
    ShowSignalsGenAmplIfShould(eSourceStream, AMPLIFIER_SOURCE, dwIndexStart, COLOR_BROWN_DARK);
    ShowSignalsAplUsnIfShould(eSourceStream, dwIndexStart, COLOR_GREEN_DARK);
}

void MainWindow::ShowSignalsGenAmplIfShould(SOURCE_STREAM eSourceStream, MainWindow::SOURCE_DEVICE eSourceDevice, qint32 &dwStartIndex, QColor eBackgrColor)
{
    qint32 dwVolumeItems = (eSourceDevice == GENERATOR_SOURCE) ? NMB_ITEMS_TIMERS_GENER : NMB_ITEMS_TIMERS_AMPLF;
    const QStringList* p_sStringList = (eSourceDevice == GENERATOR_SOURCE) ? allAdxSignalsGener : allAdxSignalsAmplf;
    bool* p_bLogged = (eSourceDevice == GENERATOR_SOURCE) ? flagIfSourceIsLoggedGener : flagIfSourceIsLoggedAmplf;
    PERIODIC_REQUEST* p_sRequests = (eSourceDevice == GENERATOR_SOURCE) ? eRequestsGenerAdcx : eRequestsAmplifAdcx;


    for(qint32 row = 0; row < dwVolumeItems; row++)
    {
        if(((eSourceStream == LOG_STREAM) && p_bLogged[row]) || ((eSourceStream == RECEIVE_STREAM) && p_sRequests[row].timer.bEnable))
        {
            p_WidgetGraph->addItems(p_sStringList[row]);

            for(qint32 iLoop = 0; iLoop <= p_sStringList[row].count(); iLoop++)
            {
                p_WidgetGraph->setItemData(dwStartIndex + iLoop, QBrush(eBackgrColor), Qt::BackgroundRole);
                p_WidgetGraph->setItemData(dwStartIndex + iLoop, QBrush(Qt::white), Qt::TextColorRole);
            }

            dwStartIndex += p_sStringList[row].count();
        }
    }
}

void MainWindow::ShowSignalsAplUsnIfShould(SOURCE_STREAM eSourceStream, qint32 &dwStartIndex, QColor eBackgrColor)
{
    for(qint32 row = 0; row < NMB_ITEMS_TIMERS_APLS_AND_USN; row++)
    {
        if(((eSourceStream == LOG_STREAM) && flagIfSourceIsLoggedAplUsn[row]) || ((eSourceStream == RECEIVE_STREAM) && eRequestsAplUsn[row].timer.bEnable))
        {
            p_WidgetGraph->addItems(allAdxSignalsAplUsn[row]);

            for(qint32 iLoop = 0; iLoop <= allAdxSignalsAplUsn[row].count(); iLoop++)
            {
                p_WidgetGraph->setItemData(dwStartIndex + iLoop, QBrush(eBackgrColor), Qt::BackgroundRole);
                p_WidgetGraph->setItemData(dwStartIndex + iLoop, QBrush(Qt::white), Qt::TextColorRole);
            }

            dwStartIndex += allAdxSignalsAplUsn[row].count();
        }
    }
}

void MainWindow::SetTimerinput(bool bOnOff, QString sCommand, MainWindow::SOURCE_DEVICE eSourceStream)
{
    PERIODIC_REQUEST* p_sRequest = (eSourceStream == GENERATOR_SOURCE) ? &eRequestGenerInput : &eRequestAmplfInput;


    if(bOnOff == true)
    {
        p_sRequest->timer.CurrentTime_ms = p_sRequest->timer.RequirementTime_ms;
        p_sRequest->timer.bEnable = true;

        p_sRequest->assemblyMsq = QByteArray::fromHex(sCommand.toStdString().c_str());
        p_sRequest->respExp = true;
    }
    else
    {
        p_sRequest->timer.bEnable = false;
        p_sRequest->respExp = false;
    }
}

void MainWindow::HasTimerRequestsExpiredGenAmpl(MainWindow::SOURCE_DEVICE eSourceStream)
{
    qint32 dwVolumeItems = (eSourceStream == GENERATOR_SOURCE) ? NMB_ITEMS_TIMERS_GENER : NMB_ITEMS_TIMERS_AMPLF;
    PERIODIC_REQUEST* p_sRequests = (eSourceStream == GENERATOR_SOURCE) ? eRequestsGenerAdcx : eRequestsAmplifAdcx;


    for(qint32 loop = 0; loop < dwVolumeItems; loop++)
    {
        if(p_sRequests[loop].timer.bEnable == true)
        {
            p_sRequests[loop].timer.CurrentTime_ms++;
            if(p_sRequests[loop].timer.CurrentTime_ms >= p_sRequests[loop].timer.RequirementTime_ms)
            {
                //qDebug() << "Timer requests" << loop << "tick";
                if(p_sRequests[loop].isInProgress == true)
                {
                    //qDebug() << "Timer requests" << loop << "busy";
                }
                else
                {
                    //qDebug() << "Timer requests" << loop << "again";
                    p_sRequests[loop].isInProgress = true;
                    p_sRequests[loop].timer.CurrentTime_ms = 0;
                    p_sRequests[loop].internalIdClass = m_CommProt.data()->SendData(m_nDeviceAddress, p_sRequests[loop].assemblyMsq, p_sRequests[loop].respExp);
                }
            }
        }
    }
}

void MainWindow::HasTimerRequestsExpiredAplUsn()
{
    for(qint32 loop = 0; loop < NMB_ITEMS_TIMERS_APLS_AND_USN; loop++)
    {
        if(eRequestsAplUsn[loop].timer.bEnable == true)
        {
            eRequestsAplUsn[loop].timer.CurrentTime_ms++;
            if(eRequestsAplUsn[loop].timer.CurrentTime_ms >= eRequestsAplUsn[loop].timer.RequirementTime_ms)
            {
                //qDebug() << "Timer requests" << loop << "tick";
                if(eRequestsAplUsn[loop].isInProgress == true)
                {
                    //qDebug() << "Timer requests" << loop << "busy";
                }
                else
                {
                    //qDebug() << "Timer requests" << loop << "again";
                    eRequestsAplUsn[loop].isInProgress = true;
                    eRequestsAplUsn[loop].timer.CurrentTime_ms = 0;
                    eRequestsAplUsn[loop].internalIdClass = m_CommProt.data()->SendData(m_nDeviceAddress, eRequestsAplUsn[loop].assemblyMsq, eRequestsAplUsn[loop].respExp);
                }
            }
        }
    }
}

void MainWindow::HasTimerInputExpired(MainWindow::SOURCE_DEVICE eSourceStream)
{
    PERIODIC_REQUEST* p_sRequest = (eSourceStream == GENERATOR_SOURCE) ? &eRequestGenerInput : &eRequestAmplfInput;


    if(p_sRequest->timer.bEnable == true)
    {
        p_sRequest->timer.CurrentTime_ms++;
        if(p_sRequest->timer.CurrentTime_ms >= p_sRequest->timer.RequirementTime_ms)
        {
            //qDebug() << "Timer input" << eSourceStream << "tick";
            if(p_sRequest->isInProgress == true)
            {
                //qDebug() << "Timer input" << eSourceStream << "busy";
            }
            else
            {
                //qDebug() << "Timer input" << eSourceStream << "again";
                p_sRequest->isInProgress = true;
                p_sRequest->timer.CurrentTime_ms = 0;
                p_sRequest->internalIdClass = m_CommProt.data()->SendData(m_nDeviceAddress, p_sRequest->assemblyMsq, p_sRequest->respExp);
            }
        }
    }
}

void MainWindow::recognizeIfDisplayNewDataAllSignals(QTime timestamp, QStringList* listOfNumbers, int adx)
{
    for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
    {
        if((sourceAd[iLoop] == adx) && (sourceSignal[iLoop] >= 0))
        {
            //qDebug() << "recognizing adx:" << adx << "at signal:" << iLoop;
            DisplayNewDataFromSignal(timestamp, listOfNumbers, iLoop);
        }
    }
}

void MainWindow::DisplayNewDataFromSignal(QTime timestamp, QStringList *listOfNumbers, int indexInSignal)
{
    QString textToShow;


    if(sourceSignal[indexInSignal] >= listOfNumbers->count())
    {
        qDebug() << "!!!!!!! received messegage isn´t according template";
        return;
    }

    recvItems[indexInSignal] = listOfNumbers->at(sourceSignal[indexInSignal]).toDouble();

    if(sourceAd[indexInSignal] < NMB_ITEMS_TIMERS_GENER)
    {
        textToShow.append(allAdxSignalsGener[sourceAd[indexInSignal]].at(sourceSignal[indexInSignal]));
    }
    else if(sourceAd[indexInSignal] < (NMB_ITEMS_TIMERS_GENER + NMB_ITEMS_TIMERS_AMPLF))
    {
        textToShow.append(allAdxSignalsAmplf[sourceAd[indexInSignal] - NMB_ITEMS_TIMERS_GENER].at(sourceSignal[indexInSignal]));
    }
    else
    {
        textToShow.append(allAdxSignalsAplUsn[sourceAd[indexInSignal] - NMB_ITEMS_TIMERS_GENER - NMB_ITEMS_TIMERS_AMPLF].at(sourceSignal[indexInSignal]));
    }

    emit SendUpdateGraph(timestamp, recvItems[indexInSignal], recStat[indexInSignal], textToShow, indexInSignal, sourceDataStream, 0);
}

QString MainWindow::myTimeStamp(QTime time)
{
    return QString("%1:%2:%3,%4").arg(time.hour(), 2, 10, QChar('0')).arg(time.minute(), 2, 10, QChar('0')).arg(time.second(), 2, 10, QChar('0')).arg(time.msec(), 3, 10, QChar('0'));
}

QStringList MainWindow::adjustRowDataIntoOnlyNumber(QString rowData)
{
    QStringList stringsSplitted = rowData.split(QRegExp("(\\s+| |=)"));
    QStringList stringsNumbers;


    for(int iLoop = 0; iLoop < stringsSplitted.count(); iLoop++)
    {
        //qDebug() << "all: " + stringsSplitted.at(iLoop) << endl;

        bool isNumber;
        stringsSplitted.at(iLoop).toFloat(&isNumber);

        if(isNumber)
        {
            stringsNumbers.append(stringsSplitted.at(iLoop));
            //qDebug() << "number: " + stringsSplitted.at(iLoop) << endl;
        }
    }

    return stringsNumbers;
}

void MainWindow::on_disconnectButton_clicked()
{
    ui->checkBox->setEnabled(true);
    ui->comboBox->setEnabled(true);
    ui->comboBox_1->setEnabled(true);

    m_CommProt.data()->SetTargetMedium("");

    SetAvaiblePorts();

    for(qint32 loop = 0; loop < NMB_ITEMS_TIMERS_GENER; loop++)
    {
        eRequestsGenerAdcx[loop].timer.bEnable = false;
        eRequestsGenerAdcx[loop].isInProgress = false;
    }

    for(qint32 loop = 0; loop < NMB_ITEMS_TIMERS_AMPLF; loop++)
    {
        eRequestsAmplifAdcx[loop].timer.bEnable = false;
        eRequestsAmplifAdcx[loop].isInProgress = false;
    }

    for(qint32 loop = 0; loop < NMB_ITEMS_TIMERS_APLS_AND_USN; loop++)
    {
        eRequestsAplUsn[loop].timer.bEnable = false;
        eRequestsAplUsn[loop].isInProgress = false;
    }

    eRequestGenerInput.timer.bEnable = false;
    eRequestAmplfInput.timer.bEnable = false;

    for(int iLoop = 0; iLoop < E_BROWSER_NMB; iLoop++)
    {

        if(m_oFile[iLoop].isOpen())
        {
            m_oFile[iLoop].close();
            QFileInfo oFileInfo(m_oFile[iLoop]);
            AppendText(timeCurrent, QString("Data saved to <a href=\"%1\">%1</a>, file size is %2 kB").arg(oFileInfo.absoluteFilePath()).arg(static_cast<double>(oFileInfo.size()) / 1024, 0, 'f', 2), TEXT_BROWSERS(iLoop));

            //qDebug() << "Data saved";
        }
    }

    p_WidgetGraph->clearAll();
    p_WidgetTherapy->resetValues();
    p_WidgetReading->disableAll();
    p_WidgetTipMemory->clearAll();
    p_WidgetConfig->clearLineEdits();

    sourceDataStream = NO_STREAM;
}


void MainWindow::on_checkBox_clicked()
{
    m_bSaveData = ui->checkBox->isChecked();
    appSettings->StoreSaveDataBox(m_bSaveData);
}

void MainWindow::openlogButtonPressed()
{
    sourceDataStream = LOG_STREAM;

    for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_GENER; iLoop++)
    {
        flagIfSourceIsLoggedGener[iLoop] = false;
    }

    for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_AMPLF; iLoop++)
    {
        flagIfSourceIsLoggedAmplf[iLoop] = false;
    }

    for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_APLS_AND_USN; iLoop++)
    {
        flagIfSourceIsLoggedAplUsn[iLoop] = false;
    }

    //QString savedPath = m_pSettingStrorage->RestorePathLog();
    logPath = QFileDialog::getOpenFileName(this, "Open log file", QString("%1/").arg(QCoreApplication::applicationDirPath()), "Log File (*.log)");
    //m_pSettingStrorage->StorePathLog(logPath);

    QFile m_logFile(logPath);

    if(!m_logFile.open(QFile::ReadOnly))
    {
        qDebug() << "error: cannot open file";
    }
    else
    {
        QTextStream fileStream(&m_logFile);

        while (!fileStream.atEnd())
        {
            QString newLinereaded = fileStream.readLine();
            QStringList stringsSplitted = newLinereaded.split(QRegExp("\\s+"));

            for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_GENER; iLoop++)
            {
                if(stringsSplitted[1] == allSignalsBaseOnlyGener[iLoop])
                {
                    flagIfSourceIsLoggedGener[iLoop] = true;
                }
            }

            for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_AMPLF; iLoop++)
            {
                if(stringsSplitted[1] == allSignalsBaseOnlyAmplf[iLoop])
                {
                    flagIfSourceIsLoggedAmplf[iLoop] = true;
                }
            }

            for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_APLS_AND_USN; iLoop++)
            {
                if(stringsSplitted[1] == allSignalsBaseOnlyAplUsn[iLoop])
                {
                    flagIfSourceIsLoggedAplUsn[iLoop] = true;
                }
            }
        }

        //qDebug("sources gener: %d, %d, %d, %d, %d, %d", flagIfSourceIsLoggedGener[0], flagIfSourceIsLoggedGener[1], flagIfSourceIsLoggedGener[2], flagIfSourceIsLoggedGener[3], flagIfSourceIsLoggedGener[4], flagIfSourceIsLoggedGener[5]);
        //qDebug("sources amplf: %d, %d, %d, %d", flagIfSourceIsLoggedAmplf[0], flagIfSourceIsLoggedAmplf[1], flagIfSourceIsLoggedAmplf[2], flagIfSourceIsLoggedAmplf[3]);

        ShowSignalsIntoComboBox(LOG_STREAM);

        m_logFile.close();
    }
}

void MainWindow::refreshPlot()
{
    int currentTab = ui->tabWidget->currentIndex();
    QSize currentSize;

    if(currentTab == 0)
    {
        currentSize = ui->Reading->size();

        p_WidgetReading->setFixedSize(currentSize);
    }
    else if(currentTab == 1)
    {
        currentSize = ui->Configuration->size();

        p_WidgetConfig->setFixedSize(currentSize);
    }
    else if(currentTab == 2)
    {
        currentSize = ui->Settings->size();

        p_widgetSettings->setFixedSize(currentSize);
    }
    else if(currentTab == 3)
    {
        currentSize = ui->Graph->size();

        p_WidgetGraph->setFixedSize(QSize(currentSize.width(), (currentSize.height() / 10) * 6));
        p_WidgetSmith->setFixedSize(QSize(currentSize.width(), (currentSize.height() / 10) * 2));
        p_WidgetTherapy->setFixedSize(QSize(currentSize.width(), (currentSize.height() / 10) * 2));
    }
    else if(currentTab == 4)
    {
        currentSize = ui->Admin->size();

        p_WidgetAdmin->setFixedSize(currentSize);
    }
    else if(currentTab == 5)
    {
        currentSize = ui->TipMemory->size();

        p_WidgetTipMemory->setFixedSize(currentSize);
    }
}

void MainWindow::universalRequestMessageProtocol(Qt::CheckState eState, int wIndex)
{
    int wIndexHex = QString::number(wIndex, 16).toInt();
    QString strCmd = QString("%1").arg(QString::number(wIndex, 16));


    strCmd += QString("0%1").arg(eState == Qt::Unchecked ? "0" : "1");

    //qDebug() << "+++++++" << strCmd << wIndex << wIndexHex;

    if(wIndex == QString::number(PID_READ_INPUT).toInt())
    {
        SetTimerinput(eState == Qt::Unchecked ? false : true, strCmd, GENERATOR_SOURCE);
    }
    else if(wIndex == QString::number(PID_READ_AMP_INPUT).toInt())
    {
        SetTimerinput(eState == Qt::Unchecked ? false : true, strCmd, AMPLIFIER_SOURCE);
    }
    else if(wIndex == QString::number(PID_READ_MEASURE_DATA_USN).toInt())
    {
        SetTimerRequestsAplUsn(eState == Qt::Unchecked ? false : true, strCmd, USN_GENER_SOURCE);
    }
    else if(wIndex == QString::number(PID_READ_MEASURE_DATA_APL_L).toInt())
    {
        SetTimerRequestsAplUsn(eState == Qt::Unchecked ? false : true, strCmd, APL_LARGE_SOURCE);
    }
    else if(wIndex == QString::number(PID_READ_MEAS_APL_S_1_DEVEL).toInt())
    {
        SetTimerRequestsAplUsn(eState == Qt::Unchecked ? false : true, strCmd, APL_SMALL_1_SOURCE);
    }
    else if(wIndex == QString::number(PID_READ_MEAS_APL_S_2_DEVEL).toInt())
    {
        SetTimerRequestsAplUsn(eState == Qt::Unchecked ? false : true, strCmd, APL_SMALL_2_SOURCE);
    }
    else if(wIndex == QString::number(PID_READ_MEAS_APL_S_3_DEVEL).toInt())
    {
        SetTimerRequestsAplUsn(eState == Qt::Unchecked ? false : true, strCmd, APL_SMALL_3_SOURCE);
    }
    else
    {
        SetTimerRequestsGenAmp(wIndexHex, eState == Qt::Unchecked ? false : true, strCmd, GENERATOR_SOURCE);
        SetTimerRequestsGenAmp(wIndexHex, eState == Qt::Unchecked ? false : true, strCmd, AMPLIFIER_SOURCE);
    }

    ShowSignalsIntoComboBox(RECEIVE_STREAM);
}

void MainWindow::specificMessageProtocol(QString message, bool bExpectedResponse)
{
    m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(message.toStdString().c_str()), bExpectedResponse);
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    Q_UNUSED(object);

    bool retVal;

    if(event->type() == QEvent::Show || event->type() == QEvent::Resize)
    {
        refreshPlot();

        retVal = true;
    }
    else
    {
        retVal = false;
    }

    return retVal;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);

    appSettings->StoreGeometryMain(saveGeometry());
}
