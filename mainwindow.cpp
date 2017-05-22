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


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    static int mMainTimer = 0;

    installEventFilter(this);

    ui->setupUi(this);
    ui->statusBar->showMessage("Start app");

    qDebug() << "Start of application.";


    SetAvaiblePorts();

    ui->comboBox_1->addItem(QString("Generator (ID = %1d)").arg(constGenerID));
    ui->comboBox_1->addItem(QString("Amplifier (ID = %1d)").arg(constAmpID));

    timeCurrent.start();

    for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_GENER; iLoop++)
    {
        eRequestsGenerAdcx[iLoop].timer.CurrentTime_ms = 0;
        eRequestsGenerAdcx[iLoop].timer.bEnable = false;
        eRequestsGenerAdcx[iLoop].timer.RequirementTime_ms = 50;
        eRequestsGenerAdcx[iLoop].isInProgress = false;
    }

    for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_AMPLF; iLoop++)
    {
        eRequestsAmplifAdcx[iLoop].timer.CurrentTime_ms = 0;
        eRequestsAmplifAdcx[iLoop].timer.bEnable = false;
        eRequestsAmplifAdcx[iLoop].timer.RequirementTime_ms = 50;
        eRequestsAmplifAdcx[iLoop].isInProgress = false;;
    }


    eRequestGenerInput.timer.CurrentTime_ms = 0;
    eRequestGenerInput.timer.bEnable = false;
    eRequestGenerInput.timer.RequirementTime_ms = 50;
    eRequestGenerInput.isInProgress = false;

    eRequestAmplfInput.timer.CurrentTime_ms = 0;
    eRequestAmplfInput.timer.bEnable = false;
    eRequestAmplfInput.timer.RequirementTime_ms = 50;
    eRequestAmplfInput.isInProgress = false;

    //ui->horizontalLayout_3->addWidget(p_WidgetConfig);
    ui->verticalLayout_2->addWidget(p_WidgetReading);
    ui->verticalLayout_3->addWidget(p_WidgetGraph);
    ui->verticalLayout_3->addWidget(p_WidgetSmith);


    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::changed_table);

    connect(&TmrMstr,&QTimer::timeout,[this](){

        HasTimerRequestsExpired(GENERATOR_SOURCE);
        HasTimerRequestsExpired(AMPLIFIER_SOURCE);

        HasTimerInputExpired(GENERATOR_SOURCE);
        HasTimerInputExpired(AMPLIFIER_SOURCE);

        if(++mMainTimer >= 500)//only for some tests
        {
            mMainTimer = 0;

            QString p_sGetStatus = "0A";
            m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(p_sGetStatus.toStdString().c_str()), true);
        }
    });

    connect(ui->comboBox_1,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[=](int nValue){
        selectedDeviceSetAccordingSaved(nValue);

        //-m_pSettingStrorage->StoreSelectedDevice(ui->comboBox_SelectDevice->currentIndex());
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

    refreshPlot();
}

void MainWindow::on_connectButton_clicked()
{
    ui->checkBox->setEnabled(false);
    ui->comboBox->setEnabled(false);
    ui->comboBox_1->setEnabled(false);

    m_CommProt.data()->SetTargetMedium(ui->comboBox->currentText());

    if(m_bSaveData)
    {
        m_oFile.setFileName(QString("SavedData_%1.log").arg(QDateTime::currentDateTime().toTime_t()));
        if(!m_oFile.open(QFile::WriteOnly))
        {
            qDebug() << "error: cannot open file";
            return;
        }
    }
}

void MainWindow::on_disconnectButton_clicked()
{
    ui->checkBox->setEnabled(true);
    ui->comboBox->setEnabled(true);
    ui->comboBox_1->setEnabled(true);

    m_CommProt.data()->SetTargetMedium("");

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

    eRequestGenerInput.timer.bEnable = false;
    eRequestAmplfInput.timer.bEnable = false;

    ui->checkBox_2->setChecked(false);
    ui->checkBox_3->setChecked(false);
    ui->checkBox_4->setChecked(false);
    ui->checkBox_5->setChecked(false);
    ui->checkBox_6->setChecked(false);
    ui->checkBox_7->setChecked(false);
    ui->checkBox_8->setChecked(false);
    ui->checkBox_9->setChecked(false);
    ui->checkBox_10->setChecked(false);
    ui->checkBox_11->setChecked(false);
    ui->checkBox_12->setChecked(false);
    ui->checkBox_13->setChecked(false);

    if(m_oFile.isOpen())
    {
        m_oFile.close();
        QFileInfo oFileInfo(m_oFile);
        AppendText(timeCurrent, QString("Data saved to <a href=\"%1\">%1</a>, file size is %2 kB").arg(oFileInfo.absoluteFilePath()).arg(static_cast<double>(oFileInfo.size()) / 1024, 0, 'f', 2));

        qDebug() << "Data saved";

    }
}

void MainWindow::on_clearButton_clicked()
{
    ui->textBrowser->clear();
}

void MainWindow::on_checkBox_clicked()
{
    m_bSaveData = ui->checkBox->isChecked();
}

void MainWindow::on_checkBox_2_clicked()
{
    universalRequestMessageProtocol(ui->checkBox_2->checkState(), PID_TIMERS_ADCX_GENER + 0);
}

void MainWindow::on_checkBox_3_clicked()
{
    universalRequestMessageProtocol(ui->checkBox_3->checkState(), PID_TIMERS_ADCX_GENER + 1);
}

void MainWindow::on_checkBox_4_clicked()
{
    universalRequestMessageProtocol(ui->checkBox_4->checkState(), PID_TIMERS_ADCX_GENER + 2);
}

void MainWindow::on_checkBox_5_clicked()
{
    universalRequestMessageProtocol(ui->checkBox_5->checkState(), PID_TIMERS_ADCX_GENER + 3);
}

void MainWindow::on_checkBox_6_clicked()
{
    universalRequestMessageProtocol(ui->checkBox_6->checkState(), PID_TIMERS_ADCX_GENER + 4);
}

void MainWindow::on_checkBox_7_clicked()
{
    universalRequestMessageProtocol(ui->checkBox_7->checkState(), PID_TIMERS_ADCX_GENER + 5);
}

void MainWindow::on_checkBox_8_clicked()
{
    universalRequestMessageProtocol(ui->checkBox_8->checkState(), PID_TIMERS_ADCX_GENER + 6);
}

void MainWindow::universalRequestMessageProtocol(Qt::CheckState eState, int wIndex)
{
    QString strCmd = QString("%1").arg(wIndex);
    strCmd += QString("0%1").arg(eState == Qt::Unchecked ? "0" : "1");

    qDebug() << strCmd;

    if(wIndex == PID_TIMER_INPUT_GENER)
    {
        SetTimerinput(eState == Qt::Unchecked ? false : true, strCmd, GENERATOR_SOURCE);
    }
    else if(wIndex == PID_TIMER_INPUT_AMPLF)
    {
        SetTimerinput(eState == Qt::Unchecked ? false : true, strCmd, AMPLIFIER_SOURCE);
    }
    else
    {
        SetTimerRequests(wIndex, eState == Qt::Unchecked ? false : true, strCmd, GENERATOR_SOURCE);
        SetTimerRequests(wIndex, eState == Qt::Unchecked ? false : true, strCmd, AMPLIFIER_SOURCE);
    }
}

void MainWindow::on_checkBox_9_clicked()
{
    universalRequestMessageProtocol(ui->checkBox_9->checkState(), PID_TIMERS_ADCX_AMPLF + 0);
}

void MainWindow::on_checkBox_10_clicked()
{
    universalRequestMessageProtocol(ui->checkBox_10->checkState(), PID_TIMERS_ADCX_AMPLF + 1);
}

void MainWindow::on_checkBox_11_clicked()
{
    universalRequestMessageProtocol(ui->checkBox_11->checkState(), PID_TIMERS_ADCX_AMPLF + 2);
}

void MainWindow::on_checkBox_12_clicked()
{
    universalRequestMessageProtocol(ui->checkBox_12->checkState(), PID_TIMERS_ADCX_AMPLF + 3);
}

void MainWindow::on_checkBox_13_clicked()
{
    universalRequestMessageProtocol(ui->checkBox_13->checkState(), PID_TIMERS_ADCX_AMPLF + 4);
}

void MainWindow::newDataV200(QByteArray aData)
{
    //actualize time
    timeCurrent.restart();

    ui->statusBar->showMessage("Data received: " + QString(aData.toHex()));



    if(aData.at(0) == 'g')//Gener´s data
    {
        if(aData.at(1) == '3' && aData.at(2) == 'c')//ADC3 adjusted data
        {
            //recognizeIfDisplayNewDataAllSignals(timeShot, &myStringOnlyNumbers, 0);
            if(eRequestsGenerAdcx[0].isInProgress == true)
            {
                eRequestsGenerAdcx[0].isInProgress = false;
            }
        }
        else if(aData.at(1) == '3' && aData.at(2) == 's')//ADC3 average data
        {
            //recognizeIfDisplayNewDataAllSignals(timeShot, &myStringOnlyNumbers, 1);
            if(eRequestsGenerAdcx[1].isInProgress == true)
            {
                eRequestsGenerAdcx[1].isInProgress = false;
            }
        }
        else if(aData.at(1) == '2' && aData.at(2) == 'c')//ADC2 adjusted data
        {
            //recognizeIfDisplayNewDataAllSignals(timeShot, &myStringOnlyNumbers, 2);
            if(eRequestsGenerAdcx[2].isInProgress == true)
            {
                eRequestsGenerAdcx[2].isInProgress = false;
            }
        }
        else if(aData.at(1) == '2' && aData.at(2) == 's')//ADC2 average data
        {
            //recognizeIfDisplayNewDataAllSignals(timeShot, &myStringOnlyNumbers, 3);
            if(eRequestsGenerAdcx[3].isInProgress == true)
            {
                eRequestsGenerAdcx[3].isInProgress = false;
            }
        }
        else if(aData.at(1) == '1' && aData.at(2) == 'c')//ADC1 adjusted data
        {
            //recognizeIfDisplayNewDataAllSignals(timeShot, &myStringOnlyNumbers, 4);
            if(eRequestsGenerAdcx[4].isInProgress == true)
            {
                eRequestsGenerAdcx[4].isInProgress = false;
            }
        }
        else if(aData.at(1) == '1' && aData.at(2) == 's')//ADC1 average data
        {
            //recognizeIfDisplayNewDataAllSignals(timeShot, &myStringOnlyNumbers, 5);
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
    }
    else if(aData.at(0) == 'a')//Amp´s data
    {
        if(aData.at(1) == 't')//timers adjusted data
        {
            //recognizeIfDisplayNewDataAllSignals(timeShot, &myStringOnlyNumbers, NMB_ITEMS_TIMERS_GENER);
            if(eRequestsAmplifAdcx[0].isInProgress == true)
            {
                eRequestsAmplifAdcx[0].isInProgress = false;
            }
        }
        else if(aData.at(1) == '3' && aData.at(2) == 's')//ADC3 average data
        {
            //recognizeIfDisplayNewDataAllSignals(timeShot, &myStringOnlyNumbers, NMB_ITEMS_TIMERS_GENER + 1);
            if(eRequestsAmplifAdcx[1].isInProgress == true)
            {
                eRequestsAmplifAdcx[1].isInProgress = false;
            }
        }
        else if(aData.at(1) == 'x')//reserve data
        {
            //recognizeIfDisplayNewDataAllSignals(timeShot, &myStringOnlyNumbers, NMB_ITEMS_TIMERS_GENER + 2);
            if(eRequestsAmplifAdcx[2].isInProgress == true)
            {
                eRequestsAmplifAdcx[2].isInProgress = false;
            }
        }
        else if(aData.at(1) == '1' && aData.at(2) == 's')//ADC1 average data
        {
            //recognizeIfDisplayNewDataAllSignals(timeShot, &myStringOnlyNumbers, NMB_ITEMS_TIMERS_GENER + 3);
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
    }


    if(aData.at(0) != 0x1A)
    {
        AppendText(timeCurrent, QString(aData));

        if(m_bSaveData)
        {
            m_oFile.write(myTimeStamp(timeCurrent).toUtf8() + "\t" + QString(aData).simplified().toUtf8() + "\r\n");
            m_oFile.flush();
        }
    }


}

void MainWindow::refreshPlot()
{
    int currentTab = ui->tabWidget->currentIndex();
    QSize currentSize;

    if(currentTab == 0)
    {
        currentSize = ui->Configuration->size();

        /*p_WidgetConfig->setFixedSize(currentSize);
        p_WidgetConfig->SetQSize(currentSize);
        p_WidgetConfig->repaint();*/
    }
    else if(currentTab == 1)
    {
        currentSize = ui->Reading->size();

        p_WidgetReading->setFixedSize(currentSize);
        p_WidgetReading->SetQSize(currentSize);
        p_WidgetReading->repaint();
    }
    else if(currentTab == 2)
    {
        currentSize = ui->Graph->size();

        p_WidgetSmith->setFixedSize(QSize(currentSize.width(), currentSize.height() / 2));
        p_WidgetSmith->SetQStart(QSize(0,0));
        p_WidgetSmith->SetQSize(QSize(currentSize.width(), currentSize.height() / 2));
        p_WidgetSmith->repaint();

        p_WidgetGraph->setFixedSize(QSize(currentSize.width(), currentSize.height() / 2));
        p_WidgetGraph->repaint();
    }
}

void MainWindow::AppendText(QTime timestamp, QString strText)
{
    ui->textBrowser->append(myTimeStamp(timestamp) + "\t" + strText);
}

QString MainWindow::myTimeStamp(QTime time)
{
    return QString("%1:%2:%3,%4").arg(time.hour(), 2, 10, QChar('0')).arg(time.minute(), 2, 10, QChar('0')).arg(time.second(), 2, 10, QChar('0')).arg(time.msec(), 3, 10, QChar('0'));
}

void MainWindow::SetAvaiblePorts()
{
    ui->comboBox->clear();

    //QString strLastPortName = m_pSettingStrorage->RestorePortName();

    for(auto comPort : QSerialPortInfo::availablePorts())
    {
        ui->comboBox->addItem(comPort.portName());

        /*if(comPort.portName() == strLastPortName)
        {
            ui->comboBox->setCurrentText(strLastPortName);
        }*/
    }
}

void MainWindow::selectedDeviceSetAccordingSaved(quint32 value)
{
    if(value == GENERATOR_SOURCE)
    {
        m_nDeviceAddress = constGenerID;

        ui->checkBox_3->setEnabled(true);
        ui->checkBox_4->setEnabled(true);
        ui->checkBox_5->setEnabled(true);
        ui->checkBox_7->setEnabled(true);
        ui->checkBox_8->setEnabled(true);
    }
    else if(value == AMPLIFIER_SOURCE)
    {
        m_nDeviceAddress = constAmpID;

        ui->checkBox_3->setEnabled(false);
        ui->checkBox_4->setEnabled(false);
        ui->checkBox_5->setEnabled(false);
        ui->checkBox_7->setEnabled(false);
        ui->checkBox_8->setEnabled(false);
    }
}

void MainWindow::SetTimerRequests(int wIndex, bool bOnOff, QString sCommand, MainWindow::SOURCE_DEVICE eSourceStream)
{
    qint32 dwPidItems = (eSourceStream == GENERATOR_SOURCE) ? PID_TIMERS_ADCX_GENER : PID_TIMERS_ADCX_AMPLF;
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

void MainWindow::HasTimerRequestsExpired(MainWindow::SOURCE_DEVICE eSourceStream)
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

