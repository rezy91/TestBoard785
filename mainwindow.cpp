/***************************************************************************
**
** Copyright (C) 2016 Medical Technologies CZ a.s.. All rights reserved.
** Contact: http://www.medictech.com
**
** This file is a part of BTL088 project, projects based on BTL088
** or related utilities.
**
** Without prior written permission from Medical Technologies CZ a.s.,
** the file must not be modified, distributed and/or otherwise used.
**
****************************************************************************/

/*!
  \file   mainwindow.cpp
  \author Pavel Hübner
  \date   22. 2. 2016
  \brief  GUI zasílající a přijímající zprávy generu
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QTextStream>

#include <windows.h>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    static int mMainTimer = 0;


    o_smith->setWindowIcon(QIcon(":/iconSmith.png"));
    o_smith->setWindowTitle("Smith diagram");
    o_smith->show();

    connect(this, &MainWindow::SendNewImpedanceData, o_smith, &SmithMain::ReceivedNewData);


    o_graph->setWindowIcon(QIcon(":/iconGraph.jpg"));
    o_graph->setWindowTitle("Graph");
    o_graph->show();




    connect(this, &MainWindow::SendHighLevel, o_graph, &Grapmain::refreshHighLevel);
    connect(this, &MainWindow::SendLowLevel, o_graph, &Grapmain::refreshLowLevel);
    connect(this, &MainWindow::SendUpdateGraph, o_graph, &Grapmain::refreshGraph);


    ui->setupUi(this);


    ui->sendButton->setEnabled(false);
    ui->tableWidget->setColumnWidth(1, 250);

    SetAvaiblePorts();
    SetLastPort();
    TmrMstr.start(1);



    ui->comboBox_SelectDevice->addItem(QString("Generator (ID = %1d").arg(constGenerID));
    ui->comboBox_SelectDevice->addItem(QString("Amplifier (ID = %1d").arg(constAmpID));

    for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_GENER; iLoop++)
    {
        eRequestsGenerAdcx[iLoop].timer.CurrentTime_ms = 0;
        eRequestsGenerAdcx[iLoop].timer.bEnable = false;
    }

    for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_AMPLF; iLoop++)
    {
        eRequestsAmplifAdcx[iLoop].timer.CurrentTime_ms = 0;
        eRequestsAmplifAdcx[iLoop].timer.bEnable = false;
    }


    eRequestGenerInput.timer.CurrentTime_ms = 0;
    eRequestGenerInput.timer.bEnable = false;

    eRequestAmplfInput.timer.CurrentTime_ms = 0;
    eRequestAmplfInput.timer.bEnable = false;

    restoreAllSettings();
    timeCurrent.start();


    connect(&TmrMstr,&QTimer::timeout,[this](){
        for(qint32 loop = 0; loop < NMB_ITEMS_TIMERS_GENER; loop++)
        {
            if(eRequestsGenerAdcx[loop].timer.bEnable == true)
            {
                eRequestsGenerAdcx[loop].timer.CurrentTime_ms++;
                if(eRequestsGenerAdcx[loop].timer.CurrentTime_ms >= eRequestsGenerAdcx[loop].timer.RequirementTime_ms)
                {
                    eRequestsGenerAdcx[loop].timer.CurrentTime_ms = 0;
                    //qDebug() << "Timer gener" << loop << "tick";
                    m_CommProt.data()->SendData(m_nDeviceAddress, eRequestsGenerAdcx[loop].assemblyMsq, eRequestsGenerAdcx[loop].respExp);
                }
            }
        }

        for(qint32 loop = 0; loop < NMB_ITEMS_TIMERS_AMPLF; loop++)
        {
            if(eRequestsAmplifAdcx[loop].timer.bEnable == true)
            {
                eRequestsAmplifAdcx[loop].timer.CurrentTime_ms++;
                if(eRequestsAmplifAdcx[loop].timer.CurrentTime_ms >= eRequestsAmplifAdcx[loop].timer.RequirementTime_ms)
                {
                    eRequestsAmplifAdcx[loop].timer.CurrentTime_ms = 0;
                    //qDebug() << "Timer amplf" << loop << "tick";
                    m_CommProt.data()->SendData(m_nDeviceAddress, eRequestsAmplifAdcx[loop].assemblyMsq, eRequestsAmplifAdcx[loop].respExp);
                }
            }
        }


        if(eRequestGenerInput.timer.bEnable == true)
        {
            eRequestGenerInput.timer.CurrentTime_ms++;
            if(eRequestGenerInput.timer.CurrentTime_ms >= eRequestGenerInput.timer.RequirementTime_ms)
            {
                eRequestGenerInput.timer.CurrentTime_ms = 0;
                //qDebug() << "Timer GEN input tick";
                m_CommProt.data()->SendData(m_nDeviceAddress, eRequestGenerInput.assemblyMsq, eRequestGenerInput.respExp);
            }
        }

        if(eRequestAmplfInput.timer.bEnable == true)
        {
            eRequestAmplfInput.timer.CurrentTime_ms++;
            if(eRequestAmplfInput.timer.CurrentTime_ms >= eRequestAmplfInput.timer.RequirementTime_ms)
            {
                eRequestAmplfInput.timer.CurrentTime_ms = 0;
                //qDebug() << "Timer AMP input tick";
                m_CommProt.data()->SendData(m_nDeviceAddress, eRequestAmplfInput.assemblyMsq, eRequestAmplfInput.respExp);
            }
        }

        if(++mMainTimer >= 100)
        {
            mMainTimer = 0;
        }
    });

    connect(ui->comboBox_2,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[=](int nValue){
        if(sourceDataStream == LOG_STREAM && nValue >= 0)
        {
            CheckedIfIndexInQlist(0, 0);//send fake in order to clear history signal
        }
        CheckedIfIndexInQlist(0, nValue);
        emit SendHighLevel(ui->doubleSpinBox->value(), 0);
        emit SendLowLevel(ui->doubleSpinBox_5->value(), 0);
    });
    connect(ui->comboBox_3,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[=](int nValue){
        if(sourceDataStream == LOG_STREAM && nValue >= 0)
        {
            CheckedIfIndexInQlist(1, 0);//send fake in order to clear history signal
        }
        CheckedIfIndexInQlist(1, nValue);
        emit SendHighLevel(ui->doubleSpinBox_2->value(), 1);
        emit SendLowLevel(ui->doubleSpinBox_6->value(), 1);
    });
    connect(ui->comboBox_4,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[=](int nValue){
        if(sourceDataStream == LOG_STREAM && nValue >= 0)
        {
            CheckedIfIndexInQlist(2, 0);//send fake in order to clear history signal
        }
        CheckedIfIndexInQlist(2, nValue);
        emit SendHighLevel(ui->doubleSpinBox_3->value(), 2);
        emit SendLowLevel(ui->doubleSpinBox_7->value(), 2);
    });
    connect(ui->comboBox_5,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[=](int nValue){
        if(sourceDataStream == LOG_STREAM && nValue >= 0)
        {
            CheckedIfIndexInQlist(3, 0);//send fake in order to clear history signal
        }
        CheckedIfIndexInQlist(3, nValue);
        emit SendHighLevel(ui->doubleSpinBox_4->value(), 3);
        emit SendLowLevel(ui->doubleSpinBox_8->value(), 3);
    });
    connect(ui->comboBox_SelectDevice,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[=](int nValue){
        selectedDeviceSetAccordingSaved(nValue);

        m_pSettingStrorage->StoreSelectedDevice(ui->comboBox_SelectDevice->currentIndex());
        FillTableContent();

    });


    connect(ui->doubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),[=](double nValue){

        adjustCoefficientSingleStep(ui->doubleSpinBox, nValue);
        emit SendHighLevel(nValue, 0);
    });

    connect(ui->doubleSpinBox_2,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),[=](double nValue){

        adjustCoefficientSingleStep(ui->doubleSpinBox_2, nValue);
        emit SendHighLevel(nValue, 1);
    });

    connect(ui->doubleSpinBox_3,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),[=](double nValue){

        adjustCoefficientSingleStep(ui->doubleSpinBox_3, nValue);
        emit SendHighLevel(nValue, 2);
    });

    connect(ui->doubleSpinBox_4,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),[=](double nValue){

        adjustCoefficientSingleStep(ui->doubleSpinBox_4, nValue);
        emit SendHighLevel(nValue, 3);
    });

    connect(ui->doubleSpinBox_5,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),[=](double nValue){

        adjustCoefficientSingleStep(ui->doubleSpinBox_5, nValue);
        emit SendLowLevel(nValue, 0);
    });

    connect(ui->doubleSpinBox_6,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),[=](double nValue){

        adjustCoefficientSingleStep(ui->doubleSpinBox_6, nValue);
        emit SendLowLevel(nValue, 1);
    });

    connect(ui->doubleSpinBox_7,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),[=](double nValue){

        adjustCoefficientSingleStep(ui->doubleSpinBox_7, nValue);
        emit SendLowLevel(nValue, 2);
    });

    connect(ui->doubleSpinBox_8,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),[=](double nValue){

        adjustCoefficientSingleStep(ui->doubleSpinBox_8, nValue);
        emit SendLowLevel(nValue, 3);
    });

    connect(ui->spinBox,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int nValue){
        m_pSettingStrorage->StoreRefreshGener(0, nValue);
        eRequestsGenerAdcx[0].timer.RequirementTime_ms = ui->spinBox->value();
    });

    connect(ui->spinBox_2,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int nValue){
        m_pSettingStrorage->StoreRefreshGener(1, nValue);
        eRequestsGenerAdcx[1].timer.RequirementTime_ms = ui->spinBox_2->value();
    });

    connect(ui->spinBox_3,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int nValue){
        m_pSettingStrorage->StoreRefreshGener(2, nValue);
        eRequestsGenerAdcx[2].timer.RequirementTime_ms = ui->spinBox_3->value();
    });

    connect(ui->spinBox_4,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int nValue){
        m_pSettingStrorage->StoreRefreshGener(3, nValue);
        eRequestsGenerAdcx[3].timer.RequirementTime_ms = ui->spinBox_4->value();
    });

    connect(ui->spinBox_5,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int nValue){
        m_pSettingStrorage->StoreRefreshGener(4, nValue);
        eRequestsGenerAdcx[4].timer.RequirementTime_ms = ui->spinBox_5->value();
    });

    connect(ui->spinBox_6,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int nValue){
        m_pSettingStrorage->StoreRefreshGener(5, nValue);
        eRequestsGenerAdcx[5].timer.RequirementTime_ms = ui->spinBox_6->value();
    });


    connect(ui->spinBox_7,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int nValue){
        m_pSettingStrorage->StoreRefreshAmplif(0, nValue);
        eRequestsAmplifAdcx[0].timer.RequirementTime_ms = ui->spinBox_7->value();
    });

    connect(ui->spinBox_8,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int nValue){
        m_pSettingStrorage->StoreRefreshAmplif(1, nValue);
        eRequestsAmplifAdcx[1].timer.RequirementTime_ms = ui->spinBox_8->value();
    });

    connect(ui->spinBox_9,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int nValue){
        m_pSettingStrorage->StoreRefreshAmplif(2, nValue);
        eRequestsAmplifAdcx[2].timer.RequirementTime_ms = ui->spinBox_9->value();
    });

    connect(ui->spinBox_10,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int nValue){
        m_pSettingStrorage->StoreRefreshAmplif(3, nValue);
        eRequestsAmplifAdcx[3].timer.RequirementTime_ms = ui->spinBox_10->value();
    });


    // vytvoříme instanci CommProt200
    m_CommProt.reset(CommProtV200_Create(this));
    Q_ASSERT(!m_CommProt.isNull());

    // napojení signálů a slotů
    connect(m_CommProt.data(), &CommProtV200::DataReceived, [this](qint32 nTransactionID, qint32 nDeviceID, QByteArray arrData) {
        // parse received data here
        Q_UNUSED(nTransactionID);
        Q_UNUSED(nDeviceID);

        newDataV200(arrData);
    });

    connect(m_CommProt.data(), &CommProtV200::Log, [this](QString strClassName, QTime oTime, QString strMessage) {
        Q_UNUSED(strClassName);
        Q_UNUSED(oTime);
        qDebug() << "log: " << strMessage;
    });

    connect(m_CommProt.data(), &CommProtV200::Error, [this](qint32 nError, QString strError, QVariant vErrorData, QTime oErrorTime) {
        Q_UNUSED(nError);
        Q_UNUSED(vErrorData);
        Q_UNUSED(oErrorTime);
        ui->statusBar->showMessage("Communication error: " + strError);

    });

    connect(m_CommProt.data(), &CommProtV200::StateChanged, [this](CommProtInterface::CommProtState eState) {
        qDebug() << "state:";
        switch (eState) {
        case CommProtInterface::Disabled:
            ui->statusBar->showMessage("Communication status: Disabled");
            ui->sendButton->setEnabled(false);
            qDebug() << "Disabled";
            break;
        case CommProtInterface::Connected:
            qDebug() << "Connected";
            ui->statusBar->showMessage("Communication status: Connected");
            m_CommProt.data()->SetEnabled(true);
            StoreValue("portname", m_CommProt.data()->GetLastSetTargetMedium());
            break;
        case CommProtInterface::Enabled:
            ui->statusBar->showMessage("Communication status: Enabled");
            ui->sendButton->setEnabled(true);
            qDebug() << "Enabled";
            break;
        case CommProtInterface::Detecting:
            ui->statusBar->showMessage("Communication status: Detecting");
            qDebug() << "Detecting";
            break;
        default:
            break;
        }
    });

    ui->statusBar->showMessage("Not connected");
    FillTableContent();

    eRequestGenerInput.timer.RequirementTime_ms = 500;
    eRequestAmplfInput.timer.RequirementTime_ms = 500;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sendButton_clicked()
{
    QModelIndexList oTableSelection = ui->tableWidget->selectionModel()->selectedIndexes();

    if(!oTableSelection.isEmpty())
    {
        ui->statusBar->clearMessage();
        QString strCmd;

        for(qint32 nItemIndex = 0; nItemIndex < oTableSelection.count(); ++nItemIndex)
        {
            if(nItemIndex == 1) continue;

            qint32 nAlignment = oTableSelection.at(nItemIndex).data(TableRoles::ByteCount).toInt() * 2;
            if(oTableSelection.at(nItemIndex).data(TableRoles::NumeralSystem) == TableRoles::Hex)
            {
                strCmd += oTableSelection.at(nItemIndex).data().toString().rightJustified(nAlignment, '0');
            }
            else if(oTableSelection.at(nItemIndex).data(TableRoles::NumeralSystem) == TableRoles::Decimal)
            {
                QString strNumber = oTableSelection.at(nItemIndex).data().toString();
                QString strHexNumber = QString::number(strNumber.toInt(), 16);
                strCmd += strHexNumber.rightJustified(nAlignment, '0');
            }
            else if(oTableSelection.at(nItemIndex).data(TableRoles::NumeralSystem) == TableRoles::DecimalFloat)
            {
                const QVariant& vDisplayText = oTableSelection.at(nItemIndex).data(Qt::DisplayRole);
                qint32 nNumber = vDisplayText.toDouble() * oTableSelection.at(nItemIndex).data(TableRoles::DivisorPosition).toInt();
                QString strHexNumber = QString::number(nNumber, 16);
                strCmd += strHexNumber.rightJustified(nAlignment, '0');

            }
            else if(oTableSelection.at(nItemIndex).data(TableRoles::NumeralSystem) == TableRoles::Float)
            {
                float f_convertedValue = oTableSelection.at(nItemIndex).data().toFloat();

                QByteArray arrFloatInArraysDec(reinterpret_cast<const char *>(&f_convertedValue), sizeof (f_convertedValue));
                QByteArray arrFloatInArraysHexa = arrFloatInArraysDec.toHex();
                strCmd += QString(arrFloatInArraysHexa);

                //qDebug() <<  f_convertedValue << arrFloatInArraysHexa;
            }
        }
        qDebug() << strCmd;

        prepareComboBoxesWithSignals();

        SetTimerRequests(oTableSelection, strCmd, GENERATOR_SOURCE);
        SetTimerRequests(oTableSelection, strCmd, AMPLIFIER_SOURCE);


        for(qint32 row = 0; row < NMB_ITEMS_TIMERS_GENER; row++)
        {
            if(eRequestsGenerAdcx[row].timer.bEnable)
            {
                ui->comboBox_2->addItems(allAdxSignalsGener[row]);
                ui->comboBox_3->addItems(allAdxSignalsGener[row]);
                ui->comboBox_4->addItems(allAdxSignalsGener[row]);
                ui->comboBox_5->addItems(allAdxSignalsGener[row]);
            }
        }

        for(qint32 row = 0; row < NMB_ITEMS_TIMERS_AMPLF; row++)
        {
            if(eRequestsAmplifAdcx[row].timer.bEnable)
            {
                ui->comboBox_2->addItems(allAdxSignalsAmplf[row]);
                ui->comboBox_3->addItems(allAdxSignalsAmplf[row]);
                ui->comboBox_4->addItems(allAdxSignalsAmplf[row]);
                ui->comboBox_5->addItems(allAdxSignalsAmplf[row]);
            }
        }

        if(oTableSelection.at(0).data().toInt() == PID_TIMER_INPUT_GENER)
        {
            if(oTableSelection.at(2).data().toInt() == 1)
            {
                m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(strCmd.toStdString().c_str()), true);

                eRequestGenerInput.timer.CurrentTime_ms = 0;
                eRequestGenerInput.timer.bEnable = true;

                eRequestGenerInput.assemblyMsq = QByteArray::fromHex(strCmd.toStdString().c_str());
                eRequestGenerInput.respExp = true;
            }
            else if(oTableSelection.at(2).data().toInt() == 0)
            {
                m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(strCmd.toStdString().c_str()), false);

                eRequestGenerInput.timer.bEnable = false;
                eRequestGenerInput.respExp = false;

                ui->toolButton->setStyleSheet("font:10px");
                ui->toolButton_2->setStyleSheet("font:10px");
            }
        }

        if(oTableSelection.at(0).data().toInt() == PID_TIMER_INPUT_AMPLF)
        {
            if(oTableSelection.at(2).data().toInt() == 1)
            {
                m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(strCmd.toStdString().c_str()), true);

                eRequestAmplfInput.timer.CurrentTime_ms = 0;
                eRequestAmplfInput.timer.bEnable = true;

                eRequestAmplfInput.assemblyMsq = QByteArray::fromHex(strCmd.toStdString().c_str());
                eRequestAmplfInput.respExp = true;
            }
            else if(oTableSelection.at(2).data().toInt() == 0)
            {
                m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(strCmd.toStdString().c_str()), false);

                eRequestAmplfInput.timer.bEnable = false;
                eRequestAmplfInput.respExp = false;

                ui->toolButton_3->setStyleSheet("font:10px");
                ui->toolButton_4->setStyleSheet("font:10px");
                ui->toolButton_5->setStyleSheet("font:10px");
                ui->toolButton_6->setStyleSheet("font:10px");
                ui->toolButton_7->setStyleSheet("font:10px");
                ui->toolButton_8->setStyleSheet("font:10px");
            }
        }

        m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(strCmd.toStdString().c_str()), false);

        //AppendText(QString("Data sent: %1").arg(strCmd));
    }
    else
    {
        ui->statusBar->showMessage("Empty selection, select row to send.");
    }
}

void MainWindow::prepareComboBoxesWithSignals()
{
    ui->comboBox_2->clear();
    ui->comboBox_3->clear();
    ui->comboBox_4->clear();
    ui->comboBox_5->clear();

    ui->comboBox_2->addItem("-");
    ui->comboBox_3->addItem("-");
    ui->comboBox_4->addItem("-");
    ui->comboBox_5->addItem("-");
}

void MainWindow::adjustCoefficientSingleStep(QDoubleSpinBox *p_oubleSpinBox, double newValue)
{
    if(abs(newValue) >= 10000)
    {
        p_oubleSpinBox->setSingleStep(1000.0);
    }
    else if(abs(newValue) >= 1000)
    {
        p_oubleSpinBox->setSingleStep(100.0);
    }
    else if(abs(newValue) >= 100)
    {
        p_oubleSpinBox->setSingleStep(10.0);
    }
    else if(abs(newValue) >= 10)
    {
        p_oubleSpinBox->setSingleStep(1.0);
    }
    else if(abs(newValue) >= 1)
    {
        p_oubleSpinBox->setSingleStep(0.1);
    }
    else if(abs(newValue) >= 0.1)
    {
        p_oubleSpinBox->setSingleStep(0.01);
    }
}

void MainWindow::selectedDeviceSetAccordingSaved(quint32 value)
{
    if(value == 0)
    {
        m_nDeviceAddress = constGenerID;

        ui->spinBox->setEnabled(true);
        ui->spinBox_2->setEnabled(true);
        ui->spinBox_3->setEnabled(true);
        ui->spinBox_4->setEnabled(true);
        ui->spinBox_5->setEnabled(true);
        ui->spinBox_6->setEnabled(true);
    }
    else if(value == 1)
    {
        m_nDeviceAddress = constAmpID;

        ui->spinBox->setEnabled(false);
        ui->spinBox_2->setEnabled(false);
        ui->spinBox_3->setEnabled(false);
        ui->spinBox_4->setEnabled(false);
        ui->spinBox_5->setEnabled(false);
        ui->spinBox_6->setEnabled(false);
    }
}

void MainWindow::AppendText(QTime timestamp, QString strText)
{
    ui->textBrowser->append(myTimeStamp(timestamp) + "\t" + strText);
}

void MainWindow::FillTableContent()
{
    ui->tableWidget->setRowCount(0);

    if(m_nDeviceAddress == constGenerID)
    {
        FillCommandTableAmplifier();
        FillCommandTableGenerator();
    }
    else if(m_nDeviceAddress == constAmpID)
    {
        FillCommandTableAmplifier();
    }
}

void MainWindow::FillCommandTableGenerator()
{
    bool b_dataSaved = false;

    QString StoredItems = m_pSettingStrorage->RestoreRowItemGener();
    QStringList arrListSaved;
    arrListSaved = StoredItems.split(QRegExp("\\s+"));

    if((arrListSaved.count() - 1) == m_NumberOfFilledTablesGenerator)
    {
        b_dataSaved = true;
    }

    int w_IndexInList = 0;


    for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_GENER; iLoop++)
    {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
        QTableWidgetItem *pvalueXPacketID = new QTableWidgetItem(QString::number(PID_TIMERS_ADCX_GENER + iLoop));                  // paket id
        pvalueXPacketID->setBackground(COLOR_BLUE_DARK);
        pvalueXPacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
        pvalueXPacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalueXPacketID);   // insert item to created row to the first column

        // the second column (it has no impact on data to be sent)
        QTableWidgetItem *pvalueXPacketName = new QTableWidgetItem(allSignalsNameGener[iLoop]);     // readable description
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalueXPacketName); // insert item to created row to the second column

        QTableWidgetItem *pvalueXPacketArg0 = new QTableWidgetItem();
        pvalueXPacketArg0->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("1"));
        pvalueXPacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
        pvalueXPacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
        pvalueXPacketArg0->setData(Qt::ToolTipRole, "[0-1] disable/enable");     // a hint which is displayed when mouse hovers over
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalueXPacketArg0); // insert item to created row to the fourth column
    }



    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue4PacketID = new QTableWidgetItem(QString::number(PID_TIMER_INPUT_GENER));                  // paket id
    pvalue4PacketID->setBackground(COLOR_BLUE_DARK);
    pvalue4PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue4PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue4PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue4PacketName = new QTableWidgetItem("READ INPUT");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue4PacketName); // insert item to created row to the second column

    // the fourth column
    QTableWidgetItem *pvalue4PacketArg0 = new QTableWidgetItem();
    pvalue4PacketArg0->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("1"));
    pvalue4PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue4PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue4PacketArg0->setData(Qt::ToolTipRole, "[0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue4PacketArg0); // insert item to created row to the fourth column




    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue14PacketID = new QTableWidgetItem("47");                  // paket id
    pvalue14PacketID->setBackground(COLOR_BLUE_LIGHT);
    pvalue14PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue14PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue14PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue14PacketName = new QTableWidgetItem("ADC(X)_RECEIVE_RAW_DATA");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue14PacketName); // insert item to created row to the second column

    QTableWidgetItem *pvalue14PacketArg0 = new QTableWidgetItem();
    pvalue14PacketArg0->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("0"));
    pvalue14PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue14PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue14PacketArg0->setData(Qt::ToolTipRole, "(3)[0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue14PacketArg0); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue14PacketArg1 = new QTableWidgetItem();
    pvalue14PacketArg1->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("0"));
    pvalue14PacketArg1->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue14PacketArg1->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue14PacketArg1->setData(Qt::ToolTipRole, "(2)[0-1] disable/enable ");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue14PacketArg1); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue14PacketArg2 = new QTableWidgetItem();
    pvalue14PacketArg2->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("0"));
    pvalue14PacketArg2->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue14PacketArg2->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue14PacketArg2->setData(Qt::ToolTipRole, "(1)[0-1] disable/enable ");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, pvalue14PacketArg2); // insert item to created row to the fourth column



    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue1PacketID = new QTableWidgetItem("48");                  // paket id
    pvalue1PacketID->setBackground(COLOR_BLUE_LIGHT);
    pvalue1PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue1PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue1PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue1PacketName = new QTableWidgetItem("SET_PWM_COOL_(X)_DUTY");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue1PacketName); // insert item to created row to the second column

    // the third column
    QTableWidgetItem *pvalue1PacketArg0 = new QTableWidgetItem();
    pvalue1PacketArg0->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("15"));
    pvalue1PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue1PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue1PacketArg0->setData(Qt::ToolTipRole, "(1) [0-100] in %");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue1PacketArg0); // insert item to created row to the fourth column

    // the fourth column
    QTableWidgetItem *pvalue2PacketArg1 = new QTableWidgetItem();
    pvalue2PacketArg1->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("50"));
    pvalue2PacketArg1->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue2PacketArg1->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue2PacketArg1->setData(Qt::ToolTipRole, "(2) [0-100] in %");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue2PacketArg1); // insert item to created row to the fourth column


    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue3PacketID = new QTableWidgetItem("49");                  // paket id
    pvalue3PacketID->setBackground(COLOR_BLUE_LIGHT);
    pvalue3PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue3PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue3PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue3PacketName = new QTableWidgetItem("SET_DAC_(X)");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue3PacketName); // insert item to created row to the second column

    // the fourth column
    QTableWidgetItem *pvalue3PacketArg0 = new QTableWidgetItem();             // the value it contains
    pvalue3PacketArg0->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("200"));
    pvalue3PacketArg0->setData(TableRoles::ByteCount, 2);                            // the value is 3 bytes
    pvalue3PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue3PacketArg0->setData(Qt::ToolTipRole, "(1) [0-4095]");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue3PacketArg0); // insert item to created row to the fourth column

    // the fourth column
    QTableWidgetItem *pvalue3PacketArg1 = new QTableWidgetItem();
    pvalue3PacketArg1->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("2000"));
    pvalue3PacketArg1->setData(TableRoles::ByteCount, 2);                            // the value is 3 bytes
    pvalue3PacketArg1->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue3PacketArg1->setData(Qt::ToolTipRole, "(2) [0-4095]");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue3PacketArg1); // insert item to created row to the fourth column


    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue6PacketID = new QTableWidgetItem("4A");                  // paket id
    pvalue6PacketID->setBackground(COLOR_BLUE_LIGHT);
    pvalue6PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue6PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue6PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue6PacketName = new QTableWidgetItem("SET_PWM_CQM_(X)_FREQ");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue6PacketName); // insert item to created row to the second column

    // the fourth column
    QTableWidgetItem *pvalue6PacketArg0 = new QTableWidgetItem();
    pvalue6PacketArg0->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("10000"));
    pvalue6PacketArg0->setData(TableRoles::ByteCount, 3);                            // the value is 3 bytes
    pvalue6PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue6PacketArg0->setData(Qt::ToolTipRole, "(1) [1000 - 100000]");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue6PacketArg0); // insert item to created row to the fourth column

    // the fourth column
    QTableWidgetItem *pvalue6PacketArg1 = new QTableWidgetItem();
    pvalue6PacketArg1->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("5000"));
    pvalue6PacketArg1->setData(TableRoles::ByteCount, 3);                            // the value is 3 bytes
    pvalue6PacketArg1->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue6PacketArg1->setData(Qt::ToolTipRole, "(2) [1000 - 100000]");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue6PacketArg1); // insert item to created row to the fourth column



    // the first column
    for(int iLoop = NMB_ITEMS_TIMERS_GENER; iLoop > 0; iLoop--)
    {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
        int numberPID = 50 + NMB_ITEMS_TIMERS_GENER - iLoop;
        QTableWidgetItem *pvalueADCCoeffsPacketID = new QTableWidgetItem(QString::number(numberPID));// paket id
        pvalueADCCoeffsPacketID->setBackground(COLOR_BLUE_LIGHT);
        pvalueADCCoeffsPacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
        pvalueADCCoeffsPacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalueADCCoeffsPacketID);   // insert item to created row to the first column

        // the second column (it has no impact on data to be sent)
        QTableWidgetItem *pvalueADCCoeffsPacketName = new QTableWidgetItem();
        QString textDescriptionPID;
        int iNmbChannels;

        if(!(iLoop % 2))
        {
            textDescriptionPID = QString("ADC%1_SET_COEFFICIENTS_MULTIPLE").arg(iLoop / 2);
            if(iLoop >= 5)
            {
                iNmbChannels = 4;
            }
            else if(iLoop >= 3)
            {
                iNmbChannels = 7;
            }
            else if(iLoop >= 1)
            {
                iNmbChannels = 9;
            }
        }
        else
        {
            textDescriptionPID = QString("ADC%1_SET_COEFFICIENTS_ADDITIVE").arg((iLoop + 1) / 2);
        }

        pvalueADCCoeffsPacketName->setText(textDescriptionPID);
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalueADCCoeffsPacketName); // insert item to created row to the second column



        for(qint32 jLoop = 0; jLoop < iNmbChannels; jLoop++)
        {
            QTableWidgetItem *pvalueADCCoeffsPacketArg = new QTableWidgetItem();


            if(b_dataSaved)
            {
                pvalueADCCoeffsPacketArg->setText(arrListSaved.at(w_IndexInList++));
            }
            else
            {
                if(iLoop % 2)
                {
                    pvalueADCCoeffsPacketArg->setText("0.0");
                }
                else
                {
                    pvalueADCCoeffsPacketArg->setText("1.0");
                }
            }
            pvalueADCCoeffsPacketArg->setData(Qt::ToolTipRole, "Channel(" + QString::number(jLoop) + ")");


            pvalueADCCoeffsPacketArg->setData(TableRoles::ByteCount, 4);
            pvalueADCCoeffsPacketArg->setData(TableRoles::NumeralSystem, TableRoles::Float);
            ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2 + jLoop, pvalueADCCoeffsPacketArg);
        }
    }



    // the first column
    for(int iLoop = 0; iLoop < 2; iLoop++)
    {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
        int numberPID = 56 + iLoop;
        QTableWidgetItem *pvalueOthersCoeffsPacketID = new QTableWidgetItem(QString::number(numberPID));// paket id
        pvalueOthersCoeffsPacketID->setBackground(COLOR_BLUE_LIGHT);
        pvalueOthersCoeffsPacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
        pvalueOthersCoeffsPacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalueOthersCoeffsPacketID);   // insert item to created row to the first column

        // the second column (it has no impact on data to be sent)
        QTableWidgetItem *pvalueOthersCoeffsPacketName = new QTableWidgetItem();
        QString textDescriptionPID;

        if(!(iLoop % 2))
        {
            textDescriptionPID = QString("ADC_OTHERS_SET_COEFFICIENTS_MULTIPLE");
        }
        else
        {
            textDescriptionPID = QString("ADC_OTHERS_SET_COEFFICIENTS_ADDITIVE");
        }

        pvalueOthersCoeffsPacketName->setText(textDescriptionPID);
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalueOthersCoeffsPacketName); // insert item to created row to the second column



        for(qint32 jLoop = 0; jLoop < NMB_COEFFICIENTS_OTHERS; jLoop++)
        {
            QTableWidgetItem *pvalueOthersCoeffsPacketArg = new QTableWidgetItem();

            if(b_dataSaved)
            {
                pvalueOthersCoeffsPacketArg->setText(arrListSaved.at(w_IndexInList++));
            }
            else
            {
                if(iLoop % 2)
                {
                    pvalueOthersCoeffsPacketArg->setText("0.0");
                }
                else
                {
                    pvalueOthersCoeffsPacketArg->setText("1.0");
                }
            }
            pvalueOthersCoeffsPacketArg->setData(Qt::ToolTipRole, QString("%1").arg(coeffsOthersName[jLoop]));



            pvalueOthersCoeffsPacketArg->setData(TableRoles::ByteCount, 4);
            pvalueOthersCoeffsPacketArg->setData(TableRoles::NumeralSystem, TableRoles::Float);
            ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2 + jLoop, pvalueOthersCoeffsPacketArg);
        }
    }

    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue18PacketID = new QTableWidgetItem("58");                  // paket id
    pvalue18PacketID->setBackground(COLOR_BLUE_LIGHT);
    pvalue18PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue18PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue18PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue18PacketName = new QTableWidgetItem("(RE)SET OUTPUT (X)");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue18PacketName); // insert item to created row to the second column

    // the fourth column
    QTableWidgetItem *pvalue18PacketArg0 = new QTableWidgetItem();
    pvalue18PacketArg0->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("0"));
    pvalue18PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg0->setData(Qt::ToolTipRole, "(REL_OUT_1) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue18PacketArg0); // insert item to created row to the fourth column

    // the fourth column
    QTableWidgetItem *pvalue18PacketArg1 = new QTableWidgetItem();
    pvalue18PacketArg1->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("0"));
    pvalue18PacketArg1->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg1->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg1->setData(Qt::ToolTipRole, "(REL_OUT_2) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue18PacketArg1); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg2 = new QTableWidgetItem();
    pvalue18PacketArg2->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("0"));
    pvalue18PacketArg2->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg2->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg2->setData(Qt::ToolTipRole, "(REL_OUT_3) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, pvalue18PacketArg2); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg4 = new QTableWidgetItem();
    pvalue18PacketArg4->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("0"));
    pvalue18PacketArg4->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg4->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg4->setData(Qt::ToolTipRole, "(REL_OUT_4) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 5, pvalue18PacketArg4); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg5 = new QTableWidgetItem();
    pvalue18PacketArg5->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("0"));
    pvalue18PacketArg5->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg5->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg5->setData(Qt::ToolTipRole, "(nAMP_RESET) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 6, pvalue18PacketArg5); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg6 = new QTableWidgetItem();
    pvalue18PacketArg6->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("0"));
    pvalue18PacketArg6->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg6->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg6->setData(Qt::ToolTipRole, "(Amp_PWR_ON) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 7, pvalue18PacketArg6); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg8 = new QTableWidgetItem();
    pvalue18PacketArg8->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("0"));
    pvalue18PacketArg8->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg8->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg8->setData(Qt::ToolTipRole, "(nGenErr_uP) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 8, pvalue18PacketArg8); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg9 = new QTableWidgetItem();
    pvalue18PacketArg9->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("0"));
    pvalue18PacketArg9->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg9->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg9->setData(Qt::ToolTipRole, "(Fraction/Exilis) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 9, pvalue18PacketArg9); // insert item to created row to the fourth column


    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue19PacketID = new QTableWidgetItem("59");                  // paket id
    pvalue19PacketID->setBackground(COLOR_BLUE_LIGHT);
    pvalue19PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue19PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue19PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue19PacketName = new QTableWidgetItem("apl_(X)");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue19PacketName); // insert item to created row to the second column

    QTableWidgetItem *pvalue19PacketArg0 = new QTableWidgetItem();
    pvalue19PacketArg0->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("0"));
    pvalue19PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue19PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue19PacketArg0->setData(Qt::ToolTipRole, "(1_ON) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue19PacketArg0); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue19PacketArg1 = new QTableWidgetItem();
    pvalue19PacketArg1->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("10"));
    pvalue19PacketArg1->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue19PacketArg1->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue19PacketArg1->setData(Qt::ToolTipRole, "(1_PWR_RESET) [0-100] in us");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue19PacketArg1); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue19PacketArg2 = new QTableWidgetItem();
    pvalue19PacketArg2->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("0"));
    pvalue19PacketArg2->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue19PacketArg2->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue19PacketArg2->setData(Qt::ToolTipRole, "(1_PWR_ON) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, pvalue19PacketArg2); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue19PacketArg3 = new QTableWidgetItem();
    pvalue19PacketArg3->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("0"));
    pvalue19PacketArg3->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue19PacketArg3->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue19PacketArg3->setData(Qt::ToolTipRole, "(2_ON) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 5, pvalue19PacketArg3); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue19PacketArg4 = new QTableWidgetItem();
    pvalue19PacketArg4->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("0"));
    pvalue19PacketArg4->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue19PacketArg4->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue19PacketArg4->setData(Qt::ToolTipRole, "(3_ON) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 6, pvalue19PacketArg4); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue19PacketArg5 = new QTableWidgetItem();
    pvalue19PacketArg5->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("0"));
    pvalue19PacketArg5->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue19PacketArg5->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue19PacketArg5->setData(Qt::ToolTipRole, "(4_ON) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 7, pvalue19PacketArg5); // insert item to created row to the fourth column



    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue20PacketID = new QTableWidgetItem("5A");                  // paket id
    pvalue20PacketID->setBackground(COLOR_BLUE_LIGHT);
    pvalue20PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue20PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue20PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue20PacketName = new QTableWidgetItem("REGULATOR_SETTINGS_(X)");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue20PacketName); // insert item to created row to the second column



    QTableWidgetItem *pvalue20PacketArg0 = new QTableWidgetItem();
    pvalue20PacketArg0->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("1.0"));
    pvalue20PacketArg0->setData(TableRoles::ByteCount, 4);                            // the value is 3 bytes
    pvalue20PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Float);      // packet id is displayed as decimal
    pvalue20PacketArg0->setData(Qt::ToolTipRole, "PROPORCIAL");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue20PacketArg0); // insert item to created row to the fourth column


    QTableWidgetItem *pvalue20PacketArg1 = new QTableWidgetItem();
    pvalue20PacketArg1->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("1.0"));
    pvalue20PacketArg1->setData(TableRoles::ByteCount, 4);                            // the value is 3 bytes
    pvalue20PacketArg1->setData(TableRoles::NumeralSystem, TableRoles::Float);      // packet id is displayed as decimal
    pvalue20PacketArg1->setData(Qt::ToolTipRole, "INTEGRAL");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue20PacketArg1); // insert item to created row to the fourth column


    QTableWidgetItem *pvalue20PacketArg2 = new QTableWidgetItem();
    pvalue20PacketArg2->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("1.0"));
    pvalue20PacketArg2->setData(TableRoles::ByteCount, 4);                            // the value is 3 bytes
    pvalue20PacketArg2->setData(TableRoles::NumeralSystem, TableRoles::Float);      // packet id is displayed as decimal
    pvalue20PacketArg2->setData(Qt::ToolTipRole, "DERIVATIVE");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, pvalue20PacketArg2); // insert item to created row to the fourth column


    QTableWidgetItem *pvalue20PacketArg3 = new QTableWidgetItem();
    pvalue20PacketArg3->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("100"));
    pvalue20PacketArg3->setData(TableRoles::ByteCount, 2);                            // the value is 3 bytes
    pvalue20PacketArg3->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue20PacketArg3->setData(Qt::ToolTipRole, "Period [10-1000] in ms");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 5, pvalue20PacketArg3); // insert item to created row to the fourth column



    //! set packet
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue21PacketID = new QTableWidgetItem("5B");                  // paket id
    pvalue21PacketID->setBackground(COLOR_BLUE_LIGHT);
    pvalue21PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue21PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue21PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue21PacketName = new QTableWidgetItem("REGULATION_POWER");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue21PacketName); // insert item to created row to the second column

    // the third column
    QTableWidgetItem *pvalue21PacketArg0 = new QTableWidgetItem();
    pvalue21PacketArg0->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("5"));
    pvalue21PacketArg0->setData(TableRoles::ByteCount, 2);                            // the value is 3 bytes
    pvalue21PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue21PacketArg0->setData(Qt::ToolTipRole, "[0-300] in W");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue21PacketArg0); // insert item to created row to the fourth column
}

void MainWindow::FillCommandTableAmplifier()
{
    bool b_dataSaved = false;

    QString StoredItems = m_pSettingStrorage->RestoreRowItemAmp();
    QStringList arrListSaved;
    arrListSaved = StoredItems.split(QRegExp("\\s+"));

    if((arrListSaved.count() - 1) == m_NumberOfFilledTablesAmplifier)
    {
        b_dataSaved = true;
    }

    int w_IndexInList = 0;


    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pFrequencyPacketID = new QTableWidgetItem("30");                  // paket id
    pFrequencyPacketID->setBackground(COLOR_BROWN_LIGHT);
    pFrequencyPacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pFrequencyPacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pFrequencyPacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pFrequencyPacketName = new QTableWidgetItem("SET_FREQUENCY");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pFrequencyPacketName); // insert item to created row to the second column

    // the fourth column
    QTableWidgetItem *pFrequencyPacketArg0 = new QTableWidgetItem();
    pFrequencyPacketArg0->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("30000"));
    pFrequencyPacketArg0->setData(TableRoles::ByteCount, 3);                            // the value is 3 bytes
    pFrequencyPacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pFrequencyPacketArg0->setData(Qt::ToolTipRole, "minimal resolution is 100 Hz");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pFrequencyPacketArg0); // insert item to created row to the fourth column




    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pPacketID = new QTableWidgetItem("31");                           // paket id
    pPacketID->setBackground(COLOR_BROWN_LIGHT);
    pPacketID->setData(TableRoles::ByteCount, 1);                                       // paket id is 1 byte
    pPacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);                     // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pPacketID);            // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pPacketName = new QTableWidgetItem("SET_VOLTAGE");                    // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pPacketName);          // insert item to created row to the second column


    // the fourth column
    QTableWidgetItem *pPacketArg0 = new QTableWidgetItem();
    pPacketArg0->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("50.0"));
    pPacketArg0->setData(TableRoles::ByteCount, 2);                                     // the value is 3 bytes
    pPacketArg0->setData(TableRoles::NumeralSystem, TableRoles::DecimalFloat);          // packet id is displayed as float in decimal
    pPacketArg0->setData(TableRoles::DivisorPosition, 10);
    pPacketArg0->setData(Qt::ToolTipRole, "from 0.0 to 100.0 %");                     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pPacketArg0);          // insert item to created row to the third column


    for(int iLoop = 0; iLoop < NMB_ITEMS_TIMERS_AMPLF; iLoop++)
    {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
        QTableWidgetItem *pvalueYPacketID = new QTableWidgetItem(QString::number(PID_TIMERS_ADCX_AMPLF + iLoop));                  // paket id
        pvalueYPacketID->setBackground(COLOR_BROWN_DARK);
        pvalueYPacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
        pvalueYPacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalueYPacketID);   // insert item to created row to the first column

        // the second column (it has no impact on data to be sent)
        QTableWidgetItem *pvalueYPacketName = new QTableWidgetItem(allSignalsNameAmplf[iLoop]);     // readable description
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalueYPacketName); // insert item to created row to the second column

        QTableWidgetItem *pvalueYPacketArg0 = new QTableWidgetItem();
        pvalueYPacketArg0->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("1"));
        pvalueYPacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
        pvalueYPacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
        pvalueYPacketArg0->setData(Qt::ToolTipRole, "[0-1] disable/enable");     // a hint which is displayed when mouse hovers over
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalueYPacketArg0); // insert item to created row to the fourth column
    }


    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue4PacketID = new QTableWidgetItem(QString::number(PID_TIMER_INPUT_AMPLF));                  // paket id
    pvalue4PacketID->setBackground(COLOR_BROWN_DARK);
    pvalue4PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue4PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue4PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue4PacketName = new QTableWidgetItem("READ INPUT");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue4PacketName); // insert item to created row to the second column

    // the fourth column
    QTableWidgetItem *pvalue4PacketArg0 = new QTableWidgetItem();
    pvalue4PacketArg0->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("1"));
    pvalue4PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue4PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue4PacketArg0->setData(Qt::ToolTipRole, "[0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue4PacketArg0); // insert item to created row to the fourth column





    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue18PacketID = new QTableWidgetItem("37");                  // paket id
    pvalue18PacketID->setBackground(COLOR_BROWN_LIGHT);
    pvalue18PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue18PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue18PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue18PacketName = new QTableWidgetItem("(RE)SET OUTPUT (X)");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue18PacketName); // insert item to created row to the second column

    // the fourth column
    QTableWidgetItem *pvalue18PacketArg0 = new QTableWidgetItem();
    pvalue18PacketArg0->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("0"));
    pvalue18PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg0->setData(Qt::ToolTipRole, "(REL_OUT_1) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue18PacketArg0); // insert item to created row to the fourth column

    // the fourth column
    QTableWidgetItem *pvalue18PacketArg1 = new QTableWidgetItem();
    pvalue18PacketArg1->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("0"));
    pvalue18PacketArg1->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg1->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg1->setData(Qt::ToolTipRole, "(REL_OUT_2) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue18PacketArg1); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg2 = new QTableWidgetItem();
    pvalue18PacketArg2->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("0"));
    pvalue18PacketArg2->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg2->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg2->setData(Qt::ToolTipRole, "(REL_OUT_3) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, pvalue18PacketArg2); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg4 = new QTableWidgetItem();
    pvalue18PacketArg4->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("0"));
    pvalue18PacketArg4->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg4->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg4->setData(Qt::ToolTipRole, "(REL_OUT_4) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 5, pvalue18PacketArg4); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg5 = new QTableWidgetItem();
    pvalue18PacketArg5->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("0"));
    pvalue18PacketArg5->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg5->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg5->setData(Qt::ToolTipRole, "(REL_OUT_5) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 6, pvalue18PacketArg5); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg6 = new QTableWidgetItem();
    pvalue18PacketArg6->setText(b_dataSaved == true ? arrListSaved.at(w_IndexInList++) : ("0"));
    pvalue18PacketArg6->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg6->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg6->setData(Qt::ToolTipRole, "(REL_OUT_6) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 7, pvalue18PacketArg6); // insert item to created row to the fourth column

}

void MainWindow::StoreValue(const QString &strKey, const QVariant &vValue)
{
    m_pAppSettings->setValue(strKey, vValue);
}

QVariant MainWindow::LoadValue(const QString &strKey)
{
    return m_pAppSettings->value(strKey);
}

void MainWindow::on_clearButton_clicked()
{
    ui->textBrowser->clear();
}

void MainWindow::on_connectButton_clicked()
{
    ui->checkBox->setEnabled(false);
    ui->comboBox_SelectDevice->setEnabled(false);
    ui->comboBox->setEnabled(false);

    sourceDataStream = RECEIVE_STREAM;


    eRequestsGenerAdcx[0].timer.RequirementTime_ms = ui->spinBox->value();
    eRequestsGenerAdcx[1].timer.RequirementTime_ms = ui->spinBox_2->value();
    eRequestsGenerAdcx[2].timer.RequirementTime_ms = ui->spinBox_3->value();
    eRequestsGenerAdcx[3].timer.RequirementTime_ms = ui->spinBox_4->value();
    eRequestsGenerAdcx[4].timer.RequirementTime_ms = ui->spinBox_5->value();
    eRequestsGenerAdcx[5].timer.RequirementTime_ms = ui->spinBox_6->value();

    eRequestsAmplifAdcx[0].timer.RequirementTime_ms = ui->spinBox_7->value();
    eRequestsAmplifAdcx[1].timer.RequirementTime_ms = ui->spinBox_8->value();
    eRequestsAmplifAdcx[2].timer.RequirementTime_ms = ui->spinBox_9->value();
    eRequestsAmplifAdcx[3].timer.RequirementTime_ms = ui->spinBox_10->value();

    m_CommProt.data()->SetTargetMedium(ui->comboBox->currentText());

    m_pSettingStrorage->StorePortName(ui->comboBox->currentText());


    m_nDeviceAddress = (ui->comboBox_SelectDevice->currentIndex() == 0) ? constGenerID : constAmpID;
    /*if(ui->comboBox_SelectDevice->currentIndex() == 0)
    {
        m_nDeviceAddress = constGenerID;
    }
    else
    {
        m_nDeviceAddress = constAmpID;
    }*/


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

void MainWindow::SetAvaiblePorts()
{
    ui->comboBox->clear();

    QString strLastPortName = m_pSettingStrorage->RestorePortName();

    for(auto comPort : QSerialPortInfo::availablePorts())
    {
        ui->comboBox->addItem(comPort.portName());

        if(comPort.portName() == strLastPortName)
        {
            ui->comboBox->setCurrentText(strLastPortName);
        }
    }
}

void MainWindow::SetLastPort()
{
    QString strLastPortName = LoadValue("portname").toString();

    for(auto comPort : QSerialPortInfo::availablePorts())
    {
        if(comPort.portName() == strLastPortName)
        {
            ui->comboBox->setCurrentText(strLastPortName);
        }
    }
}

void MainWindow::restoreAllSettings()
{
    this->restoreGeometry(m_pSettingStrorage->RestoreGeometryMain());
    o_smith->restoreGeometry(m_pSettingStrorage->RestoreGeometrySmith());
    o_graph->restoreGeometry(m_pSettingStrorage->RestoreGeometryGraph());

    ui->checkBox->setChecked(m_pSettingStrorage->RestoreSaveDataBox());
    m_bSaveData = m_pSettingStrorage->RestoreSaveDataBox();

    ui->spinBox->setValue(m_pSettingStrorage->RestoreRefreshGener(0));
    ui->spinBox_2->setValue(m_pSettingStrorage->RestoreRefreshGener(1));
    ui->spinBox_3->setValue(m_pSettingStrorage->RestoreRefreshGener(2));
    ui->spinBox_4->setValue(m_pSettingStrorage->RestoreRefreshGener(3));
    ui->spinBox_5->setValue(m_pSettingStrorage->RestoreRefreshGener(4));
    ui->spinBox_6->setValue(m_pSettingStrorage->RestoreRefreshGener(5));

    ui->spinBox_7->setValue(m_pSettingStrorage->RestoreRefreshAmplif(0));
    ui->spinBox_8->setValue(m_pSettingStrorage->RestoreRefreshAmplif(1));
    ui->spinBox_9->setValue(m_pSettingStrorage->RestoreRefreshAmplif(2));
    ui->spinBox_10->setValue(m_pSettingStrorage->RestoreRefreshAmplif(3));

    ui->doubleSpinBox->setValue(m_pSettingStrorage->RestoreHighValueSignalFirst());
    ui->doubleSpinBox_2->setValue(m_pSettingStrorage->RestoreHighValueSignalSecond());
    ui->doubleSpinBox_3->setValue(m_pSettingStrorage->RestoreHighValueSignalThird());
    ui->doubleSpinBox_4->setValue(m_pSettingStrorage->RestoreHighValueSignalFourth());

    ui->doubleSpinBox_5->setValue(m_pSettingStrorage->RestoreLowValueSignalFirst());
    ui->doubleSpinBox_6->setValue(m_pSettingStrorage->RestoreLowValueSignalSecond());
    ui->doubleSpinBox_7->setValue(m_pSettingStrorage->RestoreLowValueSignalThird());
    ui->doubleSpinBox_8->setValue(m_pSettingStrorage->RestoreLowValueSignalFourth());

    if(m_pSettingStrorage->RestoreSmithPoints())
    {
        o_smith->SetNmbPoints(m_pSettingStrorage->RestoreSmithPoints());
    }

    ui->comboBox_SelectDevice->setCurrentIndex(m_pSettingStrorage->RestoreSelectedDevice());

    selectedDeviceSetAccordingSaved(ui->comboBox_SelectDevice->currentIndex());
}

void MainWindow::newDataV200(QByteArray aData)
{
    //actualize time
    timeCurrent.elapsed();
    timeCurrent.start();

    QTime timeShot(timeCurrent);

    ui->statusBar->showMessage("Data received: " + QString(aData.toHex()));
    AppendText(timeShot, QString(aData));



    QStringList myStringOnlyNumbers = adjustRowDataIntoOnlyNumber(aData);

    if(aData.at(0) == 'g')//Gener´s data
    {
        if(aData.at(1) == '3' && aData.at(2) == 'c')//ADC3 adjusted data
        {
            recognizeIfDisplayNewDataAllSignals(timeShot, &myStringOnlyNumbers, 0);
        }
        else if(aData.at(1) == '3' && aData.at(2) == 's')//ADC3 average data
        {
            recognizeIfDisplayNewDataAllSignals(timeShot, &myStringOnlyNumbers, 1);
        }
        else if(aData.at(1) == '2' && aData.at(2) == 'c')//ADC2 adjusted data
        {
            recognizeIfDisplayNewDataAllSignals(timeShot, &myStringOnlyNumbers, 2);

            COMPLEX_NUMBER_GONIO currentData, averageData, const50Data;
            COMPLEX_NUMBER_GONIO reflRatioCurrVsAvg, reflRatioCurrVs50, reflRatioAvgVs50;

            averageData.magnitude = myStringOnlyNumbers.at(1).toFloat();
            averageData.phase_rad = myStringOnlyNumbers.at(2).toFloat();
            currentData.magnitude = myStringOnlyNumbers.at(3).toFloat();
            currentData.phase_rad = myStringOnlyNumbers.at(4).toFloat();
            const50Data.magnitude = 50;
            const50Data.phase_rad = 0;

            /*COMPLEX_NUMBER_GONIO test;
            COMPLEX_NUMBER_GONIO reflret;

            test.magnitude = 110;
            test.phase_rad = 1.57;

            reflret = CalculateReflectionRatio(test,const50Data);*/

            reflRatioCurrVsAvg = CalculateReflectionRatio(currentData, averageData);
            reflRatioCurrVs50 = CalculateReflectionRatio(currentData, const50Data);
            reflRatioAvgVs50 = CalculateReflectionRatio(averageData, const50Data);
            emit SendNewImpedanceData(qreal(reflRatioCurrVsAvg.magnitude), qreal(reflRatioCurrVsAvg.phase_rad),qreal(reflRatioCurrVs50.magnitude), qreal(reflRatioCurrVs50.phase_rad), qreal(reflRatioAvgVs50.magnitude), qreal(reflRatioAvgVs50.phase_rad));

        }
        else if(aData.at(1) == '2' && aData.at(2) == 's')//ADC2 average data
        {
            recognizeIfDisplayNewDataAllSignals(timeShot, &myStringOnlyNumbers, 3);
        }
        else if(aData.at(1) == '1' && aData.at(2) == 'c')//ADC1 adjusted data
        {
            recognizeIfDisplayNewDataAllSignals(timeShot, &myStringOnlyNumbers, 4);
        }
        else if(aData.at(1) == '1' && aData.at(2) == 's')//ADC1 average data
        {
            recognizeIfDisplayNewDataAllSignals(timeShot, &myStringOnlyNumbers, 5);
        }
        else if(aData.at(1) == 'i')//Digital input readed
        {
            ui->toolButton->setStyleSheet(myStringOnlyNumbers.at(0) == "0" ? "background-color:red;color:black;font:10px" : "background-color:green;color:black;font:10px");
            ui->toolButton_2->setStyleSheet(myStringOnlyNumbers.at(1) == "0" ? "background-color:red;color:black;font:10px" : "background-color:green;color:black;font:10px");
        }
    }
    else if(aData.at(0) == 'a')//Amp´s data
    {
        if(aData.at(1) == '3' && aData.at(2) == 'c')//ADC3 adjusted data
        {
            recognizeIfDisplayNewDataAllSignals(timeShot, &myStringOnlyNumbers, NMB_ITEMS_TIMERS_GENER);
        }
        else if(aData.at(1) == '3' && aData.at(2) == 's')//ADC3 average data
        {
            recognizeIfDisplayNewDataAllSignals(timeShot, &myStringOnlyNumbers, NMB_ITEMS_TIMERS_GENER + 1);
        }
        else if(aData.at(1) == '1' && aData.at(2) == 'c')//ADC1 adjusted data
        {
            recognizeIfDisplayNewDataAllSignals(timeShot, &myStringOnlyNumbers, NMB_ITEMS_TIMERS_GENER + 2);
        }
        else if(aData.at(1) == '1' && aData.at(2) == 's')//ADC1 average data
        {
            recognizeIfDisplayNewDataAllSignals(timeShot, &myStringOnlyNumbers, NMB_ITEMS_TIMERS_GENER + 3);
        }
        else if(aData.at(1) == 'i')//Digital input readed
        {
            ui->toolButton_3->setStyleSheet(myStringOnlyNumbers.at(0) == "0" ? "background-color:green;color:black;font:10px" : "background-color:red;color:black;font:10px");
            ui->toolButton_4->setStyleSheet(myStringOnlyNumbers.at(1) == "0" ? "background-color:green;color:black;font:10px" : "background-color:red;color:black;font:10px");
            ui->toolButton_5->setStyleSheet(myStringOnlyNumbers.at(2) == "0" ? "background-color:green;color:black;font:10px" : "background-color:red;color:black;font:10px");
            ui->toolButton_6->setStyleSheet(myStringOnlyNumbers.at(3) == "0" ? "background-color:green;color:black;font:10px" : "background-color:red;color:black;font:10px");
            ui->toolButton_7->setStyleSheet(myStringOnlyNumbers.at(4) == "0" ? "background-color:green;color:black;font:10px" : "background-color:red;color:black;font:10px");
            ui->toolButton_8->setStyleSheet(myStringOnlyNumbers.at(5) == "0" ? "background-color:green;color:black;font:10px" : "background-color:red;color:black;font:10px");
        }
    }


    if(m_bSaveData)
    {
        m_oFile.write(myTimeStamp(timeShot).toUtf8() + "\t" + QString(aData).simplified().toUtf8() + "\r\n");
        m_oFile.flush();
    }

}

MainWindow::COMPLEX_NUMBER_GONIO MainWindow::CalculateReflectionRatio(COMPLEX_NUMBER_GONIO current, COMPLEX_NUMBER_GONIO average)
{
    COMPLEX_NUMBER_GONIO ReflectionRatio;

    double averageAlgebReal = average.magnitude * cos(average.phase_rad);
    double averageAlgebImag = average.magnitude * sin(average.phase_rad);

    double recentAlgebReal = current.magnitude * cos(current.phase_rad);
    double recentAlgebImag = current.magnitude * sin(current.phase_rad);

    double dividentAlgebReal = recentAlgebReal - averageAlgebReal;
    double dividentAlgebImag = recentAlgebImag - averageAlgebImag;

    double divisorAlgebReal = recentAlgebReal + averageAlgebReal;
    double divisorAlgebImag = recentAlgebImag + averageAlgebImag;



    double commonDivisor = divisorAlgebReal * divisorAlgebReal + divisorAlgebImag * divisorAlgebImag;
    double realPart = (dividentAlgebReal * divisorAlgebReal + dividentAlgebImag * divisorAlgebImag) / commonDivisor;
    double imagPart = (dividentAlgebImag * divisorAlgebReal - dividentAlgebReal * divisorAlgebImag) / commonDivisor;

    ReflectionRatio.magnitude = sqrt(realPart * realPart + imagPart * imagPart);
    ReflectionRatio.phase_rad = atan(imagPart / realPart);

    //qDebug() << ReflectionRatio.magnitude << " " << ReflectionRatio.phase_rad;


    return ReflectionRatio;
}

void MainWindow::CheckedIfIndexInQlist(int NumberComboBox, int indexInComboBox)
{
    //qDebug() << "NumberComboBox_i" << NumberComboBox << "CheckedIfIndexInQlist_i: " << indexInComboBox;

    if(indexInComboBox >= 1)
    {
        recStat[NumberComboBox] = 1;
        int absoluteIndex = indexInComboBox - 1;

        if(GetIndexFromQlist(GENERATOR_SOURCE, absoluteIndex, NumberComboBox, indexInComboBox))
        {
            return;
        }

        GetIndexFromQlist(AMPLIFIER_SOURCE, absoluteIndex, NumberComboBox, indexInComboBox);
    }
    else
    {
        sourceSignText[NumberComboBox] = "\0";
        sourceSignal[NumberComboBox] = -1;
        recStat[NumberComboBox] = 0;
        emit SendUpdateGraph(timeCurrent, recvItems[NumberComboBox], recStat[NumberComboBox], sourceSignText[NumberComboBox], NumberComboBox, sourceDataStream, 0);
    }
}

bool MainWindow::GetIndexFromQlist(SOURCE_DEVICE eSourceStream, int &dwAbsIndex, int dwNumberCmbBx, int dwIndexCmbBx)
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

                if(eSourceStream == GENERATOR_SOURCE)
                {

                    return true;
                }
                else
                {
                    return false;
                }
            }
            dwAbsIndex -= p_sStringList[iLoop].count();
        }
    }
    return false;
}

void MainWindow::SetTimerRequests(QModelIndexList &TableSelect, QString sCommand, MainWindow::SOURCE_DEVICE eSourceStream)
{
    qint32 dwVolumeItems = (eSourceStream == GENERATOR_SOURCE) ? NMB_ITEMS_TIMERS_GENER : NMB_ITEMS_TIMERS_AMPLF;
    qint32 dwPidDevice = (eSourceStream == GENERATOR_SOURCE) ? PID_TIMERS_ADCX_GENER : PID_TIMERS_ADCX_AMPLF;
    PERIODIC_REQUEST* p_sRequests = (eSourceStream == GENERATOR_SOURCE) ? eRequestsGenerAdcx : eRequestsAmplifAdcx;


    for(qint32 row = 0; row < dwVolumeItems; row++)
    {
        if(TableSelect.at(0).data().toInt() == (dwPidDevice + row))
        {
            if(TableSelect.at(2).data().toInt() == 1)
            {
                m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(sCommand.toStdString().c_str()), true);

                p_sRequests[row].timer.CurrentTime_ms = 0;
                p_sRequests[row].timer.bEnable = true;

                p_sRequests[row].assemblyMsq = QByteArray::fromHex(sCommand.toStdString().c_str());
                p_sRequests[row].respExp = true;
            }
            else if(TableSelect.at(2).data().toInt() == 0)
            {
                m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(sCommand.toStdString().c_str()), false);

                p_sRequests[row].timer.bEnable = false;
                p_sRequests[row].respExp = false;
            }

            break;
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
    if(sourceSignal[indexInSignal] >= listOfNumbers->count())
    {
        qDebug() << "!!!!!!! received messegage isn´t according template";
        return;
    }

    recvItems[indexInSignal] = listOfNumbers->at(sourceSignal[indexInSignal]).toDouble();

    QString textToShow = (sourceAd[indexInSignal] >= NMB_ITEMS_TIMERS_GENER) ? allAdxSignalsAmplf[sourceAd[indexInSignal] - NMB_ITEMS_TIMERS_GENER].at(sourceSignal[indexInSignal]) : allAdxSignalsGener[sourceAd[indexInSignal]].at(sourceSignal[indexInSignal]);

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
    ui->comboBox_SelectDevice->setEnabled(true);
    ui->comboBox->setEnabled(true);

    m_CommProt.data()->SetTargetMedium("");

    SetAvaiblePorts();
    SetLastPort();

    for(qint32 loop = 0; loop < NMB_ITEMS_TIMERS_GENER; loop++)
    {
        eRequestsGenerAdcx[loop].timer.bEnable = false;
    }

    for(qint32 loop = 0; loop < NMB_ITEMS_TIMERS_AMPLF; loop++)
    {
        eRequestsAmplifAdcx[loop].timer.bEnable = false;
    }

    eRequestGenerInput.timer.bEnable = false;
    eRequestAmplfInput.timer.bEnable = false;

    if(m_oFile.isOpen())
    {
        m_oFile.close();
        QFileInfo oFileInfo(m_oFile);
        AppendText(timeCurrent, QString("Data saved to <a href=\"%1\">%1</a>, file size is %2 kB").arg(oFileInfo.absoluteFilePath()).arg(static_cast<double>(oFileInfo.size()) / 1024, 0, 'f', 2));

        qDebug() << "Data saved";

    }

    ui->comboBox_2->clear();
    ui->comboBox_3->clear();
    ui->comboBox_4->clear();
    ui->comboBox_5->clear();

    sourceDataStream = NO_STREAM;
}

void MainWindow::on_checkBox_clicked()
{
    m_bSaveData = ui->checkBox->isChecked();
    m_pSettingStrorage->StoreSaveDataBox(m_bSaveData);
}

void MainWindow::on_openlogButton_clicked()
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

    QString savedPath = m_pSettingStrorage->RestorePathLog();
    logPath = QFileDialog::getOpenFileName(this, "Open log file", savedPath, "Log File (*.log)");
    m_pSettingStrorage->StorePathLog(logPath);

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
        }

        qDebug("sources gener: %d, %d, %d, %d, %d, %d", flagIfSourceIsLoggedGener[0], flagIfSourceIsLoggedGener[1], flagIfSourceIsLoggedGener[2], flagIfSourceIsLoggedGener[3], flagIfSourceIsLoggedGener[4], flagIfSourceIsLoggedGener[5]);
        qDebug("sources amplf: %d, %d, %d, %d", flagIfSourceIsLoggedAmplf[0], flagIfSourceIsLoggedAmplf[1], flagIfSourceIsLoggedAmplf[2], flagIfSourceIsLoggedAmplf[3]);

        prepareComboBoxesWithSignals();

        for(qint32 fillComboBoxSignal = 0; fillComboBoxSignal < NMB_ITEMS_TIMERS_GENER; fillComboBoxSignal++)
        {
            if(flagIfSourceIsLoggedGener[fillComboBoxSignal])
            {
                ui->comboBox_2->addItems(allAdxSignalsGener[fillComboBoxSignal]);
                ui->comboBox_3->addItems(allAdxSignalsGener[fillComboBoxSignal]);
                ui->comboBox_4->addItems(allAdxSignalsGener[fillComboBoxSignal]);
                ui->comboBox_5->addItems(allAdxSignalsGener[fillComboBoxSignal]);
            }
        }

        for(qint32 fillComboBoxSignal = 0; fillComboBoxSignal < NMB_ITEMS_TIMERS_AMPLF; fillComboBoxSignal++)
        {
            if(flagIfSourceIsLoggedAmplf[fillComboBoxSignal])
            {
                ui->comboBox_2->addItems(allAdxSignalsAmplf[fillComboBoxSignal]);
                ui->comboBox_3->addItems(allAdxSignalsAmplf[fillComboBoxSignal]);
                ui->comboBox_4->addItems(allAdxSignalsAmplf[fillComboBoxSignal]);
                ui->comboBox_5->addItems(allAdxSignalsAmplf[fillComboBoxSignal]);
            }
        }

        m_logFile.close();
    }
}

void MainWindow::on_textBrowser_anchorClicked(const QUrl &arg1)
{
    QDesktopServices::openUrl(arg1);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    m_pSettingStrorage->StoreGeometryMain(saveGeometry());
    m_pSettingStrorage->StoreGeometrySmith(o_smith->saveGeometry());
    m_pSettingStrorage->StoreGeometryGraph(o_graph->saveGeometry());
    m_pSettingStrorage->StoreSmithPoints(o_smith->GetNmbPoints());


    int iColumns = ui->tableWidget->columnCount();
    int iRows = ui->tableWidget->rowCount();
    QString allDataItemsGener,allDataItemsAmplifier;


    //Amp data
    for(int i = 0; i < m_NumberOfFilledRowsAmplifier; ++i)
    {
        for(int j = 2; j < iColumns; ++j)
        {
            if(ui->tableWidget->item(i, j))
            {
                allDataItemsAmplifier.append(ui->tableWidget->item(i, j)->text() + " ");
            }
        }
    }

    m_pSettingStrorage->StoreRowItemAmp(allDataItemsAmplifier);


    //Gen data
    if(m_nDeviceAddress == constGenerID)
    {
        for(int i = m_NumberOfFilledRowsAmplifier; i < iRows; ++i)
        {
            for(int j = 2; j < iColumns; ++j)
            {
                if(ui->tableWidget->item(i, j))
                {
                    allDataItemsGener.append(ui->tableWidget->item(i, j)->text() + " ");
                }
            }
        }

        m_pSettingStrorage->StoreRowItemGener(allDataItemsGener);
    }


    m_pSettingStrorage->StoreHighValueSignalFirst(ui->doubleSpinBox->value());
    m_pSettingStrorage->StoreHighValueSignalSecond(ui->doubleSpinBox_2->value());
    m_pSettingStrorage->StoreHighValueSignalThird(ui->doubleSpinBox_3->value());
    m_pSettingStrorage->StoreHighValueSignalFourth(ui->doubleSpinBox_4->value());

    m_pSettingStrorage->StoreLowValueSignalFirst(ui->doubleSpinBox_5->value());
    m_pSettingStrorage->StoreLowValueSignalSecond(ui->doubleSpinBox_6->value());
    m_pSettingStrorage->StoreLowValueSignalThird(ui->doubleSpinBox_7->value());
    m_pSettingStrorage->StoreLowValueSignalFourth(ui->doubleSpinBox_8->value());


    QWidget::closeEvent(event);
}
