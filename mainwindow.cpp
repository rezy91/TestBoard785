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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sendButton->setEnabled(false);
    ui->tableWidget->setColumnWidth(1, 150);
    ui->tableWidget->setColumnWidth(2, 60);

    SetAvaiblePorts();
    SetLastPort();
    TmrMstr.start(1);

    connect(&TmrMstr,&QTimer::timeout,[this](){

        for(qint32 loop = 0; loop < NMB_ITEMS_FOR_TIMERS; loop++)
        {
            if(timerEnable[loop] == true)
            {
                CurrentTime_ms[loop]++;
                if(CurrentTime_ms[loop] == RequirementTime_ms[loop])
                {
                    CurrentTime_ms[loop] = 0;
                    qDebug() << "Timer" << loop << "tick";
                    m_CommProt.data()->SendData(m_nDeviceAddress, assemblyMsq[loop], respExp[loop]);
                }
            }
        }
    });

    // vytvoříme instanci CommProt200
    m_CommProt.reset(CommProtV200_Create(this));
    Q_ASSERT(!m_CommProt.isNull());

    // napojení signálů a slotů
    connect(m_CommProt.data(), &CommProtV200::DataReceived, [this](qint32 nTransactionID, qint32 nDeviceID, QByteArray arrData) {
        // parse received data here
        Q_UNUSED(nTransactionID);
        Q_UNUSED(nDeviceID);

        ui->statusBar->showMessage("Data received: " + QString(arrData.toHex()));
        AppendText(QString(arrData));

        if(m_bSaveData)
        {
            m_oFile.write(arrData + "\r\n");
            m_oFile.flush();
        }
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

    ui->deviceIdLabel->setText(QString("Device Id: %1").arg(m_nDeviceAddress));
    FillCommandTable();
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
            if(nItemIndex == 1 || nItemIndex == 2) continue;

            qint32 nAlignment = oTableSelection.at(nItemIndex).data(TableRoles::ByteCount).toInt() * 2;
            if(oTableSelection.at(nItemIndex).data(TableRoles::NumeralSystem) == TableRoles::Hex)
            {
                strCmd += oTableSelection.at(nItemIndex).data().toString().rightJustified(nAlignment, '0');
            }
            else if(oTableSelection.at(nItemIndex).data(TableRoles::NumeralSystem) == TableRoles::Decimal)
            {
                QString strNumber = oTableSelection.at(nItemIndex).data().toString();
                QString strHexNumber = strNumber.contains(".") ? QString::number(strNumber.toDouble(), 'f') : QString::number(strNumber.toInt(), 16);

                strCmd += strHexNumber.rightJustified(nAlignment, '0');
            }
            qDebug() << strCmd;

        }



        bool bResponseExpected = false;
        if(oTableSelection.count() > 3)
        {
            bResponseExpected = oTableSelection.at(2).data(Qt::CheckStateRole) == Qt::Checked;
        }
        m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(strCmd.toStdString().c_str()), bResponseExpected);


        for(qint32 row = 0; row < NMB_ITEMS_FOR_TIMERS; row++)
        {
            if(oTableSelection.at(0).data().toInt() == (34 + row))
            {
                if(oTableSelection.at(3).data().toInt() == 1)
                {
                    CurrentTime_ms[row] = 0;
                    timerEnable[row] = true;

                    assemblyMsq[row] = QByteArray::fromHex(strCmd.toStdString().c_str());
                    respExp[row] = bResponseExpected;
                }
                else if(oTableSelection.at(3).data().toInt() == 0)
                {
                    timerEnable[row] = false;
                }
            }
        }


        //AppendText(QString("Data sent: %1").arg(strCmd));
    }
    else
    {
        ui->statusBar->showMessage("Empty selection, select row to send.");
    }
}

//void MainWindow::StatusRegister(QByteArray arrData)
//{
//    QByteArray arrStatusReg(arrData.mid(1, 4));

//    quint32 nStatusReg = ByteArrayParser::GetFourBytes(arrStatusReg);

//    if(nStatusReg & 0x20)
//    {
//        AppendText("selftest finished");
//    }
//    if(nStatusReg & 0x40)
//    {
//        AppendText("the device has been restarted");
//    }
//    if(nStatusReg & 0x80)
//    {
//        AppendText("error occured");
//    }
//    if(nStatusReg & 0x8000)
//    {
//        AppendText("new log");
//    }
//    if(nStatusReg & 0x10000)
//    {
//        AppendText("new event");
//    }
//    if(nStatusReg & 0xE7F1F)
//    {
//        AppendText("undefined event");
//    }
//}

void MainWindow::AppendText(QString strText)
{
    ui->textBrowser->append(QString("%1\t%2").arg(QTime::currentTime().toString()).arg(strText));
}

void MainWindow::FillCommandTable()
{
    //! set packet
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue1PacketID = new QTableWidgetItem("30");                  // paket id
    pvalue1PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue1PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue1PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue1PacketName = new QTableWidgetItem("SET_PWM_COOL_(X)_DUTY");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue1PacketName); // insert item to created row to the second column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue1PacketResponseExpected = new QTableWidgetItem();        // is response expected?
    pvalue1PacketResponseExpected->setCheckState(Qt::Unchecked);                     // Qt::Unchecked or Qt::Checked
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue1PacketResponseExpected); // insert item to created row to the third column

    // the third column
    QTableWidgetItem *pvalue1PacketArg0 = new QTableWidgetItem("15");             // the value it contains
    pvalue1PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue1PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue1PacketArg0->setData(Qt::ToolTipRole, "(1) [0-100] in %");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue1PacketArg0); // insert item to created row to the fourth column

    // the fourth column
    QTableWidgetItem *pvalue2PacketArg1 = new QTableWidgetItem("50");             // the value it contains
    pvalue2PacketArg1->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue2PacketArg1->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue2PacketArg1->setData(Qt::ToolTipRole, "(2) [0-100] in %");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, pvalue2PacketArg1); // insert item to created row to the fourth column




    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue3PacketID = new QTableWidgetItem("31");                  // paket id
    pvalue3PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue3PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue3PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue3PacketName = new QTableWidgetItem("SET_DAC_(X)");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue3PacketName); // insert item to created row to the second column

    // the third column
    QTableWidgetItem *pvalue3PacketResponseExpected = new QTableWidgetItem();        // is response expected?
    pvalue3PacketResponseExpected->setCheckState(Qt::Unchecked);                     // Qt::Unchecked or Qt::Checked
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue3PacketResponseExpected); // insert item to created row to the third column

    // the fourth column
    QTableWidgetItem *pvalue3PacketArg0 = new QTableWidgetItem("200");             // the value it contains
    pvalue3PacketArg0->setData(TableRoles::ByteCount, 2);                            // the value is 3 bytes
    pvalue3PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue3PacketArg0->setData(Qt::ToolTipRole, "(1) [0-4095]");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue3PacketArg0); // insert item to created row to the fourth column

    // the fourth column
    QTableWidgetItem *pvalue3PacketArg1 = new QTableWidgetItem("2000");             // the value it contains
    pvalue3PacketArg1->setData(TableRoles::ByteCount, 2);                            // the value is 3 bytes
    pvalue3PacketArg1->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue3PacketArg1->setData(Qt::ToolTipRole, "(2) [0-4095]");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, pvalue3PacketArg1); // insert item to created row to the fourth column


    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue6PacketID = new QTableWidgetItem("33");                  // paket id
    pvalue6PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue6PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue6PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue6PacketName = new QTableWidgetItem("SET_PWM_CQM_(X)_FREQ");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue6PacketName); // insert item to created row to the second column

    // the third column
    QTableWidgetItem *pvalue6PacketResponseExpected = new QTableWidgetItem();        // is response expected?
    pvalue6PacketResponseExpected->setCheckState(Qt::Unchecked);                     // Qt::Unchecked or Qt::Checked
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue6PacketResponseExpected); // insert item to created row to the third column

    // the fourth column
    QTableWidgetItem *pvalue6PacketArg0 = new QTableWidgetItem("10000");             // the value it contains
    pvalue6PacketArg0->setData(TableRoles::ByteCount, 3);                            // the value is 3 bytes
    pvalue6PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue6PacketArg0->setData(Qt::ToolTipRole, "(1) [1000 - 100000]");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue6PacketArg0); // insert item to created row to the fourth column

    // the fourth column
    QTableWidgetItem *pvalue6PacketArg1 = new QTableWidgetItem("5000");             // the value it contains
    pvalue6PacketArg1->setData(TableRoles::ByteCount, 3);                            // the value is 3 bytes
    pvalue6PacketArg1->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue6PacketArg1->setData(Qt::ToolTipRole, "(2) [1000 - 100000]");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, pvalue6PacketArg1); // insert item to created row to the fourth column


    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue83PacketID = new QTableWidgetItem("34");                  // paket id
    pvalue83PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue83PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue83PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue83PacketName = new QTableWidgetItem("ADC3_SEND_ADJUSTED_DATA");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue83PacketName); // insert item to created row to the second column

    // the third column
    QTableWidgetItem *pvalue83PacketResponseExpected = new QTableWidgetItem();        // is response expected?
    pvalue83PacketResponseExpected->setCheckState(Qt::Checked);                     // Qt::Unchecked or Qt::Checked
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue83PacketResponseExpected); // insert item to created row to the third column

    QTableWidgetItem *pvalue83PacketArg0 = new QTableWidgetItem("1");             // the value it contains
    pvalue83PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue83PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue83PacketArg0->setData(Qt::ToolTipRole, "[0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue83PacketArg0); // insert item to created row to the fourth column


    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue93PacketID = new QTableWidgetItem("35");                  // paket id
    pvalue93PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue93PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue93PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue93PacketName = new QTableWidgetItem("ADC3_SEND_AVERAGE_DATA");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue93PacketName); // insert item to created row to the second column

    // the third column
    QTableWidgetItem *pvalue93PacketResponseExpected = new QTableWidgetItem();        // is response expected?
    pvalue93PacketResponseExpected->setCheckState(Qt::Checked);                     // Qt::Unchecked or Qt::Checked
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue93PacketResponseExpected); // insert item to created row to the third column

    QTableWidgetItem *pvalue93PacketArg0 = new QTableWidgetItem("1");             // the value it contains
    pvalue93PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue93PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue93PacketArg0->setData(Qt::ToolTipRole, "[0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue93PacketArg0); // insert item to created row to the fourth column




    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue8PacketID = new QTableWidgetItem("36");                  // paket id
    pvalue8PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue8PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue8PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue8PacketName = new QTableWidgetItem("ADC2_SEND_ADJUSTED_DATA");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue8PacketName); // insert item to created row to the second column

    // the third column
    QTableWidgetItem *pvalue8PacketResponseExpected = new QTableWidgetItem();        // is response expected?
    pvalue8PacketResponseExpected->setCheckState(Qt::Checked);                     // Qt::Unchecked or Qt::Checked
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue8PacketResponseExpected); // insert item to created row to the third column

    QTableWidgetItem *pvalue8PacketArg0 = new QTableWidgetItem("1");             // the value it contains
    pvalue8PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue8PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue8PacketArg0->setData(Qt::ToolTipRole, "[0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue8PacketArg0); // insert item to created row to the fourth column


    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue9PacketID = new QTableWidgetItem("37");                  // paket id
    pvalue9PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue9PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue9PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue9PacketName = new QTableWidgetItem("ADC2_SEND_AVERAGE_DATA");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue9PacketName); // insert item to created row to the second column

    // the third column
    QTableWidgetItem *pvalue9PacketResponseExpected = new QTableWidgetItem();        // is response expected?
    pvalue9PacketResponseExpected->setCheckState(Qt::Checked);                     // Qt::Unchecked or Qt::Checked
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue9PacketResponseExpected); // insert item to created row to the third column

    QTableWidgetItem *pvalue9PacketArg0 = new QTableWidgetItem("1");             // the value it contains
    pvalue9PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue9PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue9PacketArg0->setData(Qt::ToolTipRole, "[0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue9PacketArg0); // insert item to created row to the fourth column



    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue10PacketID = new QTableWidgetItem("38");                  // paket id
    pvalue10PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue10PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue10PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue10PacketName = new QTableWidgetItem("ADC1_SEND_ADJUSTED_DATA");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue10PacketName); // insert item to created row to the second column

    // the third column
    QTableWidgetItem *pvalue10PacketResponseExpected = new QTableWidgetItem();        // is response expected?
    pvalue10PacketResponseExpected->setCheckState(Qt::Checked);                     // Qt::Unchecked or Qt::Checked
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue10PacketResponseExpected); // insert item to created row to the third column

    QTableWidgetItem *pvalue10PacketArg0 = new QTableWidgetItem("1");             // the value it contains
    pvalue10PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue10PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue10PacketArg0->setData(Qt::ToolTipRole, "[0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue10PacketArg0); // insert item to created row to the fourth column



    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue11PacketID = new QTableWidgetItem("39");                  // paket id
    pvalue11PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue11PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue11PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue11PacketName = new QTableWidgetItem("ADC1_SEND_AVERAGE_DATA");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue11PacketName); // insert item to created row to the second column

    // the third column
    QTableWidgetItem *pvalue11PacketResponseExpected = new QTableWidgetItem();        // is response expected?
    pvalue11PacketResponseExpected->setCheckState(Qt::Checked);                     // Qt::Unchecked or Qt::Checked
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue11PacketResponseExpected); // insert item to created row to the third column

    QTableWidgetItem *pvalue11PacketArg0 = new QTableWidgetItem("1");             // the value it contains
    pvalue11PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue11PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue11PacketArg0->setData(Qt::ToolTipRole, "[0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue11PacketArg0); // insert item to created row to the fourth column




    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue14PacketID = new QTableWidgetItem("40");                  // paket id
    pvalue14PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue14PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue14PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue14PacketName = new QTableWidgetItem("ADC(X)_SEND_ROW_DATA");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue14PacketName); // insert item to created row to the second column

    // the third column
    QTableWidgetItem *pvalue14PacketResponseExpected = new QTableWidgetItem();        // is response expected?
    pvalue14PacketResponseExpected->setCheckState(Qt::Unchecked);                     // Qt::Unchecked or Qt::Checked
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue14PacketResponseExpected); // insert item to created row to the third column

    QTableWidgetItem *pvalue14PacketArg0 = new QTableWidgetItem("0");             // the value it contains
    pvalue14PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue14PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue14PacketArg0->setData(Qt::ToolTipRole, "(3)[0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue14PacketArg0); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue14PacketArg1 = new QTableWidgetItem("0");             // the value it contains
    pvalue14PacketArg1->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue14PacketArg1->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue14PacketArg1->setData(Qt::ToolTipRole, "(2)[0-1] disable/enable ");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, pvalue14PacketArg1); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue14PacketArg2 = new QTableWidgetItem("0");             // the value it contains
    pvalue14PacketArg2->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue14PacketArg2->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue14PacketArg2->setData(Qt::ToolTipRole, "(1)[0-1] disable/enable ");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 5, pvalue14PacketArg2); // insert item to created row to the fourth column



    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue17PacketID = new QTableWidgetItem("41");                  // paket id
    pvalue17PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue17PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue17PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue17PacketName = new QTableWidgetItem("ADC3_SEND_COEFFICIENT");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue17PacketName); // insert item to created row to the second column

    // the third column
    QTableWidgetItem *pvalue17PacketResponseExpected = new QTableWidgetItem();        // is response expected?
    pvalue17PacketResponseExpected->setCheckState(Qt::Unchecked);                     // Qt::Unchecked or Qt::Checked
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue17PacketResponseExpected); // insert item to created row to the third column

    for(qint32 loop = 0; loop < 2 * 4; loop++)
    {
        QTableWidgetItem *pvalue17PacketArg = new QTableWidgetItem();             // the value it contains

        if(!(loop % 2))
        {
            pvalue17PacketArg->setText("3300");
            pvalue17PacketArg->setData(Qt::ToolTipRole, "Divident channel[" + QString::number(loop / 2) + "]");
        }
        else
        {
            pvalue17PacketArg->setText("4096");
            pvalue17PacketArg->setData(Qt::ToolTipRole, "Divisor channel[" + QString::number(loop / 2) + "]");
        }

        pvalue17PacketArg->setData(TableRoles::ByteCount, 2);
        pvalue17PacketArg->setData(TableRoles::NumeralSystem, TableRoles::Decimal);
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3 + loop, pvalue17PacketArg);
    }

    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue15PacketID = new QTableWidgetItem("42");                  // paket id
    pvalue15PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue15PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue15PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue15PacketName = new QTableWidgetItem("ADC2_SET_COEFFICIENTS");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue15PacketName); // insert item to created row to the second column

    // the third column
    QTableWidgetItem *pvalue15PacketResponseExpected = new QTableWidgetItem();        // is response expected?
    pvalue15PacketResponseExpected->setCheckState(Qt::Unchecked);                     // Qt::Unchecked or Qt::Checked
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue15PacketResponseExpected); // insert item to created row to the third column

    for(qint32 loop = 0; loop < 2 * 7; loop++)
    {
        QTableWidgetItem *pvalue15PacketArg = new QTableWidgetItem();             // the value it contains

        if(!(loop % 2))
        {
            pvalue15PacketArg->setText("3300");
            pvalue15PacketArg->setData(Qt::ToolTipRole, "Divident channel[" + QString::number(loop / 2) + "]");
        }
        else
        {
            pvalue15PacketArg->setText("4096");
            pvalue15PacketArg->setData(Qt::ToolTipRole, "Divisor channel[" + QString::number(loop / 2) + "]");
        }

        pvalue15PacketArg->setData(TableRoles::ByteCount, 2);
        pvalue15PacketArg->setData(TableRoles::NumeralSystem, TableRoles::Decimal);
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3 + loop, pvalue15PacketArg);
    }

    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue16PacketID = new QTableWidgetItem("43");                  // paket id
    pvalue16PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue16PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue16PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue16PacketName = new QTableWidgetItem("ADC1_SEND_COEFFICIENT");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue16PacketName); // insert item to created row to the second column

    // the third column
    QTableWidgetItem *pvalue16PacketResponseExpected = new QTableWidgetItem();        // is response expected?
    pvalue16PacketResponseExpected->setCheckState(Qt::Unchecked);                     // Qt::Unchecked or Qt::Checked
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue16PacketResponseExpected); // insert item to created row to the third column

    for(qint32 loop = 0; loop < 2 * 9; loop++)
    {
        QTableWidgetItem *pvalue16PacketArg = new QTableWidgetItem();             // the value it contains

        if(!(loop % 2))
        {
            pvalue16PacketArg->setText("3300");
            pvalue16PacketArg->setData(Qt::ToolTipRole, "Divident channel[" + QString::number(loop / 2) + "]");
        }
        else
        {
            pvalue16PacketArg->setText("4096");
            pvalue16PacketArg->setData(Qt::ToolTipRole, "Divisor channel[" + QString::number(loop / 2) + "]");
        }

        pvalue16PacketArg->setData(TableRoles::ByteCount, 2);
        pvalue16PacketArg->setData(TableRoles::NumeralSystem, TableRoles::Decimal);
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3 + loop, pvalue16PacketArg);
    }



    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue18PacketID = new QTableWidgetItem("44");                  // paket id
    pvalue18PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue18PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue18PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue18PacketName = new QTableWidgetItem("(RE)SET OUTPUT (X)");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue18PacketName); // insert item to created row to the second column

    // the third column
    QTableWidgetItem *pvalue18PacketResponseExpected = new QTableWidgetItem();        // is response expected?
    pvalue18PacketResponseExpected->setCheckState(Qt::Unchecked);                     // Qt::Unchecked or Qt::Checked
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue18PacketResponseExpected); // insert item to created row to the third column

    // the fourth column
    QTableWidgetItem *pvalue18PacketArg0 = new QTableWidgetItem("0");             // the value it contains
    pvalue18PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg0->setData(Qt::ToolTipRole, "(REL_OUT_1) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue18PacketArg0); // insert item to created row to the fourth column

    // the fourth column
    QTableWidgetItem *pvalue18PacketArg1 = new QTableWidgetItem("0");             // the value it contains
    pvalue18PacketArg1->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg1->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg1->setData(Qt::ToolTipRole, "(REL_OUT_2) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, pvalue18PacketArg1); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg2 = new QTableWidgetItem("0");             // the value it contains
    pvalue18PacketArg2->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg2->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg2->setData(Qt::ToolTipRole, "(REL_OUT_3) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 5, pvalue18PacketArg2); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg4 = new QTableWidgetItem("0");             // the value it contains
    pvalue18PacketArg4->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg4->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg4->setData(Qt::ToolTipRole, "(REL_OUT_4) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 6, pvalue18PacketArg4); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg5 = new QTableWidgetItem("0");             // the value it contains
    pvalue18PacketArg5->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg5->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg5->setData(Qt::ToolTipRole, "(nAMP_RESET) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 7, pvalue18PacketArg5); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg6 = new QTableWidgetItem("0");             // the value it contains
    pvalue18PacketArg6->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg6->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg6->setData(Qt::ToolTipRole, "(Amp_PWR_ON) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 8, pvalue18PacketArg6); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg7 = new QTableWidgetItem("0");             // the value it contains
    pvalue18PacketArg7->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg7->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg7->setData(Qt::ToolTipRole, "(nAMPComm_OCP) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 9, pvalue18PacketArg7); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg8 = new QTableWidgetItem("0");             // the value it contains
    pvalue18PacketArg8->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg8->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg8->setData(Qt::ToolTipRole, "(nGenErr_uP) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 10, pvalue18PacketArg8); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg9 = new QTableWidgetItem("0");             // the value it contains
    pvalue18PacketArg9->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg9->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg9->setData(Qt::ToolTipRole, "(Fraction/Exilis) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 11, pvalue18PacketArg9); // insert item to created row to the fourth column


    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue19PacketID = new QTableWidgetItem("45");                  // paket id
    pvalue19PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue19PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue19PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue19PacketName = new QTableWidgetItem("apl_PWR(X)");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue19PacketName); // insert item to created row to the second column

    // the third column
    QTableWidgetItem *pvalue19PacketResponseExpected = new QTableWidgetItem();        // is response expected?
    pvalue19PacketResponseExpected->setCheckState(Qt::Unchecked);                     // Qt::Unchecked or Qt::Checked
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue19PacketResponseExpected); // insert item to created row to the third column

    // the fourth column
    QTableWidgetItem *pvalue19PacketArg0 = new QTableWidgetItem("10");             // the value it contains
    pvalue19PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue19PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue19PacketArg0->setData(Qt::ToolTipRole, "(RESET) [0-100] in us");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue19PacketArg0); // insert item to created row to the fourth column

    // the fourth column
    QTableWidgetItem *pvalue19PacketArg1 = new QTableWidgetItem("0");             // the value it contains
    pvalue19PacketArg1->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue19PacketArg1->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue19PacketArg1->setData(Qt::ToolTipRole, "(ON) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, pvalue19PacketArg1); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue19PacketArg2 = new QTableWidgetItem("0");             // the value it contains
    pvalue19PacketArg2->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue19PacketArg2->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue19PacketArg2->setData(Qt::ToolTipRole, "(OCP) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 5, pvalue19PacketArg2); // insert item to created row to the fourth column

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

     m_CommProt.data()->SetTargetMedium(ui->comboBox->currentText());

     if(m_bSaveData)
     {
         m_oFile.setFileName(QString("SavedData_%1.txt").arg(QDateTime::currentDateTime().toTime_t()));
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

    for(auto comPort : QSerialPortInfo::availablePorts())
    {
        ui->comboBox->addItem(comPort.portName());
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

void MainWindow::on_disconnectButton_clicked()
{
    ui->checkBox->setEnabled(true);

     m_CommProt.data()->SetTargetMedium("");

     SetAvaiblePorts();
     SetLastPort();

     for(qint32 loop = 0; loop < NMB_ITEMS_FOR_TIMERS; loop++)
     {
         timerEnable[loop] = false;
     }

     if(m_oFile.isOpen())
     {
         m_oFile.close();
         QFileInfo oFileInfo(m_oFile);
         AppendText(QString("Data saved to <a href=\"%1\">%1</a>, file size is %2 kB").arg(oFileInfo.absoluteFilePath()).arg(static_cast<double>(oFileInfo.size()) / 1024, 0, 'f', 2));

         qDebug() << "Data saved";

     }
}

void MainWindow::on_checkBox_clicked()
{
    m_bSaveData = ui->checkBox->isChecked();
}

void MainWindow::on_textBrowser_anchorClicked(const QUrl &arg1)
{
    QDesktopServices::openUrl(arg1);
}
