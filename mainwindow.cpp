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

    for(int iLoop = 0; iLoop < NMB_ITEMS_FOR_TIMERS; iLoop++)
    {
        eRequestsGenerAdcx[iLoop].timer.CurrentTime_ms = 0;
        eRequestsGenerAdcx[iLoop].timer.bEnable = false;
    }

    eRequestGenerInput.timer.CurrentTime_ms = 0;
    eRequestGenerInput.timer.bEnable = false;

    restoreAllSettings();
    timeCurrent.start();


    connect(&TmrMstr,&QTimer::timeout,[this](){
        for(qint32 loop = 0; loop < NMB_ITEMS_FOR_TIMERS; loop++)
        {
            if(eRequestsGenerAdcx[loop].timer.bEnable == true)
            {
                eRequestsGenerAdcx[loop].timer.CurrentTime_ms++;
                if(eRequestsGenerAdcx[loop].timer.CurrentTime_ms >= eRequestsGenerAdcx[loop].timer.RequirementTime_ms)
                {
                    eRequestsGenerAdcx[loop].timer.CurrentTime_ms = 0;
                    //qDebug() << "Timer" << loop << "tick";
                    m_CommProt.data()->SendData(m_nDeviceAddress, eRequestsGenerAdcx[loop].assemblyMsq, eRequestsGenerAdcx[loop].respExp);
                }
            }
        }


        if(eRequestGenerInput.timer.bEnable == true)
        {
            eRequestGenerInput.timer.CurrentTime_ms++;
            if(eRequestGenerInput.timer.CurrentTime_ms >= eRequestGenerInput.timer.RequirementTime_ms)
            {
                eRequestGenerInput.timer.CurrentTime_ms = 0;
                //qDebug() << "Timer input tick";
                m_CommProt.data()->SendData(m_nDeviceAddress, eRequestGenerInput.assemblyMsq, eRequestGenerInput.respExp);
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
            getIndexInQList(0, 0);//send fake in order to clear history signal
        }
        getIndexInQList(0, nValue);
        emit SendHighLevel(ui->doubleSpinBox->value(), 0);
        emit SendLowLevel(ui->doubleSpinBox_5->value(), 0);
    });
    connect(ui->comboBox_3,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[=](int nValue){
        if(sourceDataStream == LOG_STREAM && nValue >= 0)
        {
            getIndexInQList(1, 0);//send fake in order to clear history signal
        }
        getIndexInQList(1, nValue);
        emit SendHighLevel(ui->doubleSpinBox_2->value(), 1);
        emit SendLowLevel(ui->doubleSpinBox_6->value(), 1);
    });
    connect(ui->comboBox_4,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[=](int nValue){
        if(sourceDataStream == LOG_STREAM && nValue >= 0)
        {
            getIndexInQList(2, 0);//send fake in order to clear history signal
        }
        getIndexInQList(2, nValue);
        emit SendHighLevel(ui->doubleSpinBox_3->value(), 2);
        emit SendLowLevel(ui->doubleSpinBox_7->value(), 2);
    });
    connect(ui->comboBox_5,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[=](int nValue){
        if(sourceDataStream == LOG_STREAM && nValue >= 0)
        {
            getIndexInQList(3, 0);//send fake in order to clear history signal
        }
        getIndexInQList(3, nValue);
        emit SendHighLevel(ui->doubleSpinBox_4->value(), 3);
        emit SendLowLevel(ui->doubleSpinBox_8->value(), 3);
    });
    connect(ui->comboBox_SelectDevice,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[=](int nValue){
        if(nValue == 0)
        {
            m_nDeviceAddress = constGenerID;
        }
        else if(nValue == 1)
        {
            m_nDeviceAddress = constAmpID;
        }

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
        m_pSettingStrorage->StoreRefreshFirst(nValue);
        eRequestsGenerAdcx[0].timer.RequirementTime_ms = ui->spinBox->value();
    });

    connect(ui->spinBox_2,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int nValue){
        m_pSettingStrorage->StoreRefreshSecond(nValue);
        eRequestsGenerAdcx[1].timer.RequirementTime_ms = ui->spinBox_2->value();
    });

    connect(ui->spinBox_3,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int nValue){
        m_pSettingStrorage->StoreRefreshThird(nValue);
        eRequestsGenerAdcx[2].timer.RequirementTime_ms = ui->spinBox_3->value();
    });

    connect(ui->spinBox_4,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int nValue){
        m_pSettingStrorage->StoreRefreshFourth(nValue);
        eRequestsGenerAdcx[3].timer.RequirementTime_ms = ui->spinBox_4->value();
    });

    connect(ui->spinBox_5,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int nValue){
        m_pSettingStrorage->StoreRefreshFifth(nValue);
        eRequestsGenerAdcx[4].timer.RequirementTime_ms = ui->spinBox_5->value();
    });

    connect(ui->spinBox_6,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int nValue){
        m_pSettingStrorage->StoreRefreshSixth(nValue);
        eRequestsGenerAdcx[5].timer.RequirementTime_ms = ui->spinBox_6->value();
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


        for(qint32 row = 0; row < NMB_ITEMS_FOR_TIMERS; row++)
        {
            if(oTableSelection.at(0).data().toInt() == (40 + row))
            {
                if(oTableSelection.at(2).data().toInt() == 1)
                {
                    m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(strCmd.toStdString().c_str()), true);

                    eRequestsGenerAdcx[row].timer.CurrentTime_ms = 0;
                    eRequestsGenerAdcx[row].timer.bEnable = true;

                    eRequestsGenerAdcx[row].assemblyMsq = QByteArray::fromHex(strCmd.toStdString().c_str());
                    eRequestsGenerAdcx[row].respExp = true;
                }
                else if(oTableSelection.at(2).data().toInt() == 0)
                {
                    m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(strCmd.toStdString().c_str()), false);

                    eRequestsGenerAdcx[row].timer.bEnable = false;
                    eRequestsGenerAdcx[row].respExp = false;

                }

                prepareComboBoxesWithSignals();

                for(qint32 fillComboBoxSignal = 0; fillComboBoxSignal < NMB_ITEMS_FOR_TIMERS; fillComboBoxSignal++)
                {
                    if(eRequestsGenerAdcx[fillComboBoxSignal].timer.bEnable)
                    {
                        ui->comboBox_2->addItems(allAdxSignals[fillComboBoxSignal]);
                        ui->comboBox_3->addItems(allAdxSignals[fillComboBoxSignal]);
                        ui->comboBox_4->addItems(allAdxSignals[fillComboBoxSignal]);
                        ui->comboBox_5->addItems(allAdxSignals[fillComboBoxSignal]);
                    }
                }

                return;
            }
        }
        if(oTableSelection.at(0).data().toInt() == 49)
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
            }
            return;
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


    //! set packet
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue83PacketID = new QTableWidgetItem("40");                  // paket id
    pvalue83PacketID->setBackground(COLOR_BLUE_DARK);
    pvalue83PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue83PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue83PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue83PacketName = new QTableWidgetItem("NOT_USED_CURRENTLY");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue83PacketName); // insert item to created row to the second column

    QTableWidgetItem *pvalue83PacketArg0 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue83PacketArg0->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue83PacketArg0->setText("1");
    }
    pvalue83PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue83PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue83PacketArg0->setData(Qt::ToolTipRole, "[0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue83PacketArg0); // insert item to created row to the fourth column


    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue93PacketID = new QTableWidgetItem("41");                  // paket id
    pvalue93PacketID->setBackground(COLOR_BLUE_DARK);
    pvalue93PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue93PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue93PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue93PacketName = new QTableWidgetItem("ADC3_RECEIVE_AVERAGE_DATA");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue93PacketName); // insert item to created row to the second column

    QTableWidgetItem *pvalue93PacketArg0 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue93PacketArg0->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue93PacketArg0->setText("1");
    }
    pvalue93PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue93PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue93PacketArg0->setData(Qt::ToolTipRole, "[0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue93PacketArg0); // insert item to created row to the fourth column




    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue8PacketID = new QTableWidgetItem("42");                  // paket id
    pvalue8PacketID->setBackground(COLOR_BLUE_DARK);
    pvalue8PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue8PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue8PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue8PacketName = new QTableWidgetItem("IMPEDANCE_RECEIVE_DATA");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue8PacketName); // insert item to created row to the second column

    QTableWidgetItem *pvalue8PacketArg0 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue8PacketArg0->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue8PacketArg0->setText("1");
    }
    pvalue8PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue8PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue8PacketArg0->setData(Qt::ToolTipRole, "[0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue8PacketArg0); // insert item to created row to the fourth column


    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue9PacketID = new QTableWidgetItem("43");                  // paket id
    pvalue9PacketID->setBackground(COLOR_BLUE_DARK);
    pvalue9PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue9PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue9PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue9PacketName = new QTableWidgetItem("ADC2_RECEIVE_AVERAGE_DATA");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue9PacketName); // insert item to created row to the second column

    QTableWidgetItem *pvalue9PacketArg0 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue9PacketArg0->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue9PacketArg0->setText("1");
    }
    pvalue9PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue9PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue9PacketArg0->setData(Qt::ToolTipRole, "[0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue9PacketArg0); // insert item to created row to the fourth column



    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue10PacketID = new QTableWidgetItem("44");                  // paket id
    pvalue10PacketID->setBackground(COLOR_BLUE_DARK);
    pvalue10PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue10PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue10PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue10PacketName = new QTableWidgetItem("NOT_USED_CURRENTLY");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue10PacketName); // insert item to created row to the second column

    QTableWidgetItem *pvalue10PacketArg0 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue10PacketArg0->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue10PacketArg0->setText("1");
    }
    pvalue10PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue10PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue10PacketArg0->setData(Qt::ToolTipRole, "[0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue10PacketArg0); // insert item to created row to the fourth column



    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue11PacketID = new QTableWidgetItem("45");                  // paket id
    pvalue11PacketID->setBackground(COLOR_BLUE_DARK);
    pvalue11PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue11PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue11PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue11PacketName = new QTableWidgetItem("ADC1_RECEIVE_AVERAGE_DATA");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue11PacketName); // insert item to created row to the second column

    QTableWidgetItem *pvalue11PacketArg0 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue11PacketArg0->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue11PacketArg0->setText("1");
    }
    pvalue11PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue11PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue11PacketArg0->setData(Qt::ToolTipRole, "[0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue11PacketArg0); // insert item to created row to the fourth column




    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue14PacketID = new QTableWidgetItem("46");                  // paket id
    pvalue14PacketID->setBackground(COLOR_BLUE_DARK);
    pvalue14PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue14PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue14PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue14PacketName = new QTableWidgetItem("ADC(X)_RECEIVE_RAW_DATA");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue14PacketName); // insert item to created row to the second column

    QTableWidgetItem *pvalue14PacketArg0 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue14PacketArg0->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue14PacketArg0->setText("0");
    }
    pvalue14PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue14PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue14PacketArg0->setData(Qt::ToolTipRole, "(3)[0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue14PacketArg0); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue14PacketArg1 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue14PacketArg1->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue14PacketArg1->setText("0");
    }
    pvalue14PacketArg1->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue14PacketArg1->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue14PacketArg1->setData(Qt::ToolTipRole, "(2)[0-1] disable/enable ");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue14PacketArg1); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue14PacketArg2 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue14PacketArg2->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue14PacketArg2->setText("0");
    }
    pvalue14PacketArg2->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue14PacketArg2->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue14PacketArg2->setData(Qt::ToolTipRole, "(1)[0-1] disable/enable ");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, pvalue14PacketArg2); // insert item to created row to the fourth column



    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue1PacketID = new QTableWidgetItem("47");                  // paket id
    pvalue1PacketID->setBackground(COLOR_BLUE_LIGHT);
    pvalue1PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue1PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue1PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue1PacketName = new QTableWidgetItem("SET_PWM_COOL_(X)_DUTY");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue1PacketName); // insert item to created row to the second column

    // the third column
    QTableWidgetItem *pvalue1PacketArg0 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue1PacketArg0->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue1PacketArg0->setText("15");
    }

    pvalue1PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue1PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue1PacketArg0->setData(Qt::ToolTipRole, "(1) [0-100] in %");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue1PacketArg0); // insert item to created row to the fourth column

    // the fourth column
    QTableWidgetItem *pvalue2PacketArg1 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue2PacketArg1->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue2PacketArg1->setText("50");
    }
    pvalue2PacketArg1->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue2PacketArg1->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue2PacketArg1->setData(Qt::ToolTipRole, "(2) [0-100] in %");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue2PacketArg1); // insert item to created row to the fourth column


    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue3PacketID = new QTableWidgetItem("48");                  // paket id
    pvalue3PacketID->setBackground(COLOR_BLUE_LIGHT);
    pvalue3PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue3PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue3PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue3PacketName = new QTableWidgetItem("SET_DAC_(X)");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue3PacketName); // insert item to created row to the second column

    // the fourth column
    QTableWidgetItem *pvalue3PacketArg0 = new QTableWidgetItem();             // the value it contains
    if(b_dataSaved)
    {
        pvalue3PacketArg0->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue3PacketArg0->setText("200");
    }
    pvalue3PacketArg0->setData(TableRoles::ByteCount, 2);                            // the value is 3 bytes
    pvalue3PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue3PacketArg0->setData(Qt::ToolTipRole, "(1) [0-4095]");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue3PacketArg0); // insert item to created row to the fourth column

    // the fourth column
    QTableWidgetItem *pvalue3PacketArg1 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue3PacketArg1->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue3PacketArg1->setText("2000");
    }
    pvalue3PacketArg1->setData(TableRoles::ByteCount, 2);                            // the value is 3 bytes
    pvalue3PacketArg1->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue3PacketArg1->setData(Qt::ToolTipRole, "(2) [0-4095]");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue3PacketArg1); // insert item to created row to the fourth column


    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue4PacketID = new QTableWidgetItem("49");                  // paket id
    pvalue4PacketID->setBackground(COLOR_BLUE_DARK);
    pvalue4PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue4PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue4PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue4PacketName = new QTableWidgetItem("READ INPUT");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue4PacketName); // insert item to created row to the second column

    // the fourth column
    QTableWidgetItem *pvalue4PacketArg0 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue4PacketArg0->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue4PacketArg0->setText("1");
    }
    pvalue4PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue4PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue4PacketArg0->setData(Qt::ToolTipRole, "[0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue4PacketArg0); // insert item to created row to the fourth column


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
    if(b_dataSaved)
    {
        pvalue6PacketArg0->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue6PacketArg0->setText("10000");
    }
    pvalue6PacketArg0->setData(TableRoles::ByteCount, 3);                            // the value is 3 bytes
    pvalue6PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue6PacketArg0->setData(Qt::ToolTipRole, "(1) [1000 - 100000]");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue6PacketArg0); // insert item to created row to the fourth column

    // the fourth column
    QTableWidgetItem *pvalue6PacketArg1 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue6PacketArg1->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue6PacketArg1->setText("5000");
    }
    pvalue6PacketArg1->setData(TableRoles::ByteCount, 3);                            // the value is 3 bytes
    pvalue6PacketArg1->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue6PacketArg1->setData(Qt::ToolTipRole, "(2) [1000 - 100000]");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue6PacketArg1); // insert item to created row to the fourth column



    // the first column
    for(int iLoop = NMB_ITEMS_FOR_TIMERS; iLoop > 0; iLoop--)
    {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
        int numberPID = 50 + NMB_ITEMS_FOR_TIMERS - iLoop;
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
    if(b_dataSaved)
    {
        pvalue18PacketArg0->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue18PacketArg0->setText("0");
    }
    pvalue18PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg0->setData(Qt::ToolTipRole, "(REL_OUT_1) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue18PacketArg0); // insert item to created row to the fourth column

    // the fourth column
    QTableWidgetItem *pvalue18PacketArg1 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue18PacketArg1->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue18PacketArg1->setText("0");
    }
    pvalue18PacketArg1->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg1->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg1->setData(Qt::ToolTipRole, "(REL_OUT_2) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue18PacketArg1); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg2 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue18PacketArg2->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue18PacketArg2->setText("0");
    }
    pvalue18PacketArg2->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg2->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg2->setData(Qt::ToolTipRole, "(REL_OUT_3) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, pvalue18PacketArg2); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg4 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue18PacketArg4->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue18PacketArg4->setText("0");
    }
    pvalue18PacketArg4->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg4->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg4->setData(Qt::ToolTipRole, "(REL_OUT_4) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 5, pvalue18PacketArg4); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg5 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue18PacketArg5->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue18PacketArg5->setText("0");
    }
    pvalue18PacketArg5->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg5->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg5->setData(Qt::ToolTipRole, "(nAMP_RESET) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 6, pvalue18PacketArg5); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg6 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue18PacketArg6->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue18PacketArg6->setText("0");
    }
    pvalue18PacketArg6->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg6->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg6->setData(Qt::ToolTipRole, "(Amp_PWR_ON) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 7, pvalue18PacketArg6); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg8 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue18PacketArg8->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue18PacketArg8->setText("0");
    }
    pvalue18PacketArg8->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg8->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg8->setData(Qt::ToolTipRole, "(nGenErr_uP) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 8, pvalue18PacketArg8); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg9 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue18PacketArg9->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue18PacketArg9->setText("0");
    }
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
    if(b_dataSaved)
    {
        pvalue19PacketArg0->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue19PacketArg0->setText("0");
    }
    pvalue19PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue19PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue19PacketArg0->setData(Qt::ToolTipRole, "(1_ON) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue19PacketArg0); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue19PacketArg1 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue19PacketArg1->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue19PacketArg1->setText("10");
    }
    pvalue19PacketArg1->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue19PacketArg1->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue19PacketArg1->setData(Qt::ToolTipRole, "(1_PWR_RESET) [0-100] in us");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue19PacketArg1); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue19PacketArg2 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue19PacketArg2->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue19PacketArg2->setText("0");
    }
    pvalue19PacketArg2->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue19PacketArg2->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue19PacketArg2->setData(Qt::ToolTipRole, "(1_PWR_ON) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, pvalue19PacketArg2); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue19PacketArg3 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue19PacketArg3->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue19PacketArg3->setText("0");
    }
    pvalue19PacketArg3->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue19PacketArg3->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue19PacketArg3->setData(Qt::ToolTipRole, "(2_ON) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 5, pvalue19PacketArg3); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue19PacketArg4 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue19PacketArg4->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue19PacketArg4->setText("0");
    }
    pvalue19PacketArg4->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue19PacketArg4->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue19PacketArg4->setData(Qt::ToolTipRole, "(3_ON) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 6, pvalue19PacketArg4); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue19PacketArg5 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue19PacketArg5->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue19PacketArg5->setText("0");
    }
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
    if(b_dataSaved)
    {
        pvalue20PacketArg0->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue20PacketArg0->setText("1.0");
    }
    pvalue20PacketArg0->setData(TableRoles::ByteCount, 4);                            // the value is 3 bytes
    pvalue20PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Float);      // packet id is displayed as decimal
    pvalue20PacketArg0->setData(Qt::ToolTipRole, "PROPORCIAL");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue20PacketArg0); // insert item to created row to the fourth column


    QTableWidgetItem *pvalue20PacketArg1 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue20PacketArg1->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue20PacketArg1->setText("1.0");
    }
    pvalue20PacketArg1->setData(TableRoles::ByteCount, 4);                            // the value is 3 bytes
    pvalue20PacketArg1->setData(TableRoles::NumeralSystem, TableRoles::Float);      // packet id is displayed as decimal
    pvalue20PacketArg1->setData(Qt::ToolTipRole, "INTEGRAL");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue20PacketArg1); // insert item to created row to the fourth column


    QTableWidgetItem *pvalue20PacketArg2 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue20PacketArg2->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue20PacketArg2->setText("1.0");
    }
    pvalue20PacketArg2->setData(TableRoles::ByteCount, 4);                            // the value is 3 bytes
    pvalue20PacketArg2->setData(TableRoles::NumeralSystem, TableRoles::Float);      // packet id is displayed as decimal
    pvalue20PacketArg2->setData(Qt::ToolTipRole, "DERIVATIVE");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, pvalue20PacketArg2); // insert item to created row to the fourth column


    QTableWidgetItem *pvalue20PacketArg3 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pvalue20PacketArg3->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue20PacketArg3->setText("100");
    }
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
    if(b_dataSaved)
    {
        pvalue21PacketArg0->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pvalue21PacketArg0->setText("5");
    }

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
    if(b_dataSaved)
    {
        pFrequencyPacketArg0->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pFrequencyPacketArg0->setText("30000");
    }
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
    QTableWidgetItem *pPacketName = new QTableWidgetItem("SET_PWM");                    // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pPacketName);          // insert item to created row to the second column


    // the fourth column
    QTableWidgetItem *pPacketArg0 = new QTableWidgetItem();
    if(b_dataSaved)
    {
        pPacketArg0->setText(arrListSaved.at(w_IndexInList++));
    }
    else
    {
        pPacketArg0->setText("50.0");
    }
    pPacketArg0->setData(TableRoles::ByteCount, 2);                                     // the value is 3 bytes
    pPacketArg0->setData(TableRoles::NumeralSystem, TableRoles::DecimalFloat);          // packet id is displayed as float in decimal
    pPacketArg0->setData(TableRoles::DivisorPosition, 10);
    pPacketArg0->setData(Qt::ToolTipRole, "from 0.0 to 100.0 %");                     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pPacketArg0);          // insert item to created row to the third column

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

    m_CommProt.data()->SetTargetMedium(ui->comboBox->currentText());

    m_pSettingStrorage->StorePortName(ui->comboBox->currentText());


    if(ui->comboBox_SelectDevice->currentIndex() == 0)
    {
        m_nDeviceAddress = constGenerID;
    }
    else
    {
        m_nDeviceAddress = constAmpID;
    }


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

    ui->spinBox->setValue(m_pSettingStrorage->RestoreRefreshFirst());
    ui->spinBox_2->setValue(m_pSettingStrorage->RestoreRefreshSecond());
    ui->spinBox_3->setValue(m_pSettingStrorage->RestoreRefreshThird());
    ui->spinBox_4->setValue(m_pSettingStrorage->RestoreRefreshFourth());
    ui->spinBox_5->setValue(m_pSettingStrorage->RestoreRefreshFifth());
    ui->spinBox_6->setValue(m_pSettingStrorage->RestoreRefreshSixth());

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
    if(m_pSettingStrorage->RestoreSelectedDevice() == 0)
    {
        m_nDeviceAddress = constGenerID;
    }
    else
    {
        m_nDeviceAddress = constAmpID;
    }
}

void MainWindow::newDataV200(QByteArray aData)
{
    //actualize time
    timeCurrent.elapsed();
    timeCurrent.start();

    QTime timeShot(timeCurrent);

    ui->statusBar->showMessage("Data received: " + QString(aData.toHex()));
    AppendText(timeShot, QString(aData));


    if(aData.at(0) == 'a' && aData.at(1) == 'd')//ADCx data
    {
        QStringList myStringOnlyNumbers = adjustRowDataIntoOnlyNumber(aData);

        if(aData.at(2) == '3' && aData.at(3) == 'c')//ADC3 adjusted data
        {
            recognizeIfDisplayNewDataAllSignals(timeShot, &myStringOnlyNumbers, 0, 0);
        }
        else if(aData.at(2) == '3' && aData.at(3) == 's')//ADC3 average data
        {
            recognizeIfDisplayNewDataAllSignals(timeShot, &myStringOnlyNumbers, 1, 0);
        }
        else if(aData.at(2) == '2' && aData.at(3) == 'c')//ADC2 adjusted data
        {

            recognizeIfDisplayNewDataAllSignals(timeShot, &myStringOnlyNumbers, 2, 0);

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
        else if(aData.at(2) == '2' && aData.at(3) == 's')//ADC2 average data
        {
            recognizeIfDisplayNewDataAllSignals(timeShot, &myStringOnlyNumbers, 3, 0);
        }
        else if(aData.at(2) == '1' && aData.at(3) == 'c')//ADC1 adjusted data
        {
            recognizeIfDisplayNewDataAllSignals(timeShot, &myStringOnlyNumbers, 4, 0);
        }
        else if(aData.at(2) == '1' && aData.at(3) == 's')//ADC1 average data
        {
            recognizeIfDisplayNewDataAllSignals(timeShot, &myStringOnlyNumbers, 5, 0);
        }


    }
    else if(aData.at(1) == '|')//Digital input readed
    {
        if(aData.at(0) == '0')
        {
            ui->toolButton->setStyleSheet("background-color:red;color:black;font:11px");
        }
        else if(aData.at(0) == '1')
        {
            ui->toolButton->setStyleSheet("background-color:green;color:black;font:11px");
        }
        if(aData.at(3) == '|')
        {
            if(aData.at(2) == '0')
            {
                ui->toolButton_2->setStyleSheet("background-color:red;color:black;font:11px");
            }
            else if(aData.at(2) == '1')
            {
                ui->toolButton_2->setStyleSheet("background-color:green;color:black;font:11px");
            }
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

void MainWindow::getIndexInQList(int NumberComboBox, int indexInComboBox)
{
    //qDebug() << "NumberComboBox_i" << NumberComboBox << "getIndexInQList_i: " << indexInComboBox;

    if(indexInComboBox >= 1)
    {
        recStat[NumberComboBox] = 1;
        int absoluteIndex = indexInComboBox - 1;

        for(qint32 iLoop = 0; iLoop < NMB_ITEMS_FOR_TIMERS; iLoop++)
        {
            if((sourceDataStream == RECEIVE_STREAM && eRequestsGenerAdcx[iLoop].timer.bEnable) || (sourceDataStream == LOG_STREAM && flagIfSourceIsLogged[iLoop]))
            {
                if((absoluteIndex - allAdxSignals[iLoop].count()) < 0)
                {
                    sourceAd[NumberComboBox] = iLoop;
                    sourceSignal[NumberComboBox] = absoluteIndex;
                    sourceSignText[NumberComboBox] = allSignalsBaseOnly[iLoop];

                    if(sourceDataStream == LOG_STREAM)
                    {
                        //qDebug() << "has been found signal: " << allAdxSignals[iLoop].at(absoluteIndex) << endl;
                        //qDebug() << sourceSignText[NumberComboBox];

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


                                if(stringsSplitted[1] == sourceSignText[NumberComboBox])//founded row appeared
                                {
                                    QTime timeLog = QTime::fromString(stringsSplitted[0], "hh:mm:ss,zzz");
                                    QStringList myStringOnlyNumbers = adjustRowDataIntoOnlyNumber(newLinereaded);

                                    recognizeIfDisplayNewDataInSignal(timeLog, &myStringOnlyNumbers, NumberComboBox, 0);
                                }

                            }

                            //send flag, that has been sending last one
                            emit SendUpdateGraph(QTime(0,0,0), 0, 0, " ", indexInComboBox, sourceDataStream, 1);
                        }

                        m_logFile.close();
                    }

                    return;
                }

                absoluteIndex -= allAdxSignals[iLoop].count();
            }
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

void MainWindow::recognizeIfDisplayNewDataAllSignals(QTime timestamp, QStringList* listOfNumbers, int adx, int flg)
{
    for(int iLoop = 0; iLoop < nmbCurvesInGraph; iLoop++)
    {
        if((sourceAd[iLoop] == adx) && (sourceSignal[iLoop] >= 0))
        {
            recognizeIfDisplayNewDataInSignal(timestamp, listOfNumbers, iLoop, flg);
        }
    }
}

void MainWindow::recognizeIfDisplayNewDataInSignal(QTime timestamp, QStringList *listOfNumbers, int indexInSignal, int flg)
{
    recvItems[indexInSignal] = listOfNumbers->at(sourceSignal[indexInSignal]).toDouble();

    QString textToShow = allAdxSignals[sourceAd[indexInSignal]].at(sourceSignal[indexInSignal]);

    emit SendUpdateGraph(timestamp, recvItems[indexInSignal], recStat[indexInSignal], textToShow, indexInSignal, sourceDataStream, flg);
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

    for(qint32 loop = 0; loop < NMB_ITEMS_FOR_TIMERS; loop++)
    {
        eRequestsGenerAdcx[loop].timer.bEnable = false;
    }

    eRequestGenerInput.timer.bEnable = false;

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

    for(int iLoop = 0; iLoop < NMB_ITEMS_FOR_TIMERS; iLoop++)
    {
        flagIfSourceIsLogged[iLoop] = false;
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

            for(int iLoop = 0; iLoop < NMB_ITEMS_FOR_TIMERS; iLoop++)
            {
                if(stringsSplitted[1] == allSignalsBaseOnly[iLoop])
                {
                    flagIfSourceIsLogged[iLoop] = true;
                }
            }

        }

        qDebug("sources: %d, %d, %d, %d, %d, %d", flagIfSourceIsLogged[0], flagIfSourceIsLogged[1], flagIfSourceIsLogged[2], flagIfSourceIsLogged[3], flagIfSourceIsLogged[4], flagIfSourceIsLogged[5]);

        prepareComboBoxesWithSignals();

        for(qint32 fillComboBoxSignal = 0; fillComboBoxSignal < NMB_ITEMS_FOR_TIMERS; fillComboBoxSignal++)
        {
            if(flagIfSourceIsLogged[fillComboBoxSignal])
            {
                ui->comboBox_2->addItems(allAdxSignals[fillComboBoxSignal]);
                ui->comboBox_3->addItems(allAdxSignals[fillComboBoxSignal]);
                ui->comboBox_4->addItems(allAdxSignals[fillComboBoxSignal]);
                ui->comboBox_5->addItems(allAdxSignals[fillComboBoxSignal]);
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
    for(int i = 0; i < m_NumberOfFilledTablesAmplifier; ++i)
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
        for(int i = m_NumberOfFilledTablesAmplifier; i < iRows; ++i)
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
