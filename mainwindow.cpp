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

    for(auto comPort : QSerialPortInfo::availablePorts())
    {
        ui->comboBox->addItem(comPort.portName());
    }

    // vytvoříme instanci CommProt200
    m_CommProt.reset(CommProtV200_Create(this));
    Q_ASSERT(!m_CommProt.isNull());

    // napojení signálů a slotů
    connect(m_CommProt.data(), &CommProtV200::DataReceived, [this](qint32 nTransactionID, qint32 nDeviceID, QByteArray arrData) {
        // parse received data here
        Q_UNUSED(nTransactionID);
        Q_UNUSED(nDeviceID);

        ui->statusBar->showMessage("Data received: " + QString(arrData.toHex()));

        quint8 nReplyId = ByteArrayParser::GetOneByte(arrData.mid(0, 1));

        switch (nReplyId)
        {
//        case 0x1a:
//            // status register
//            StatusRegister(arrData);
//            break;
        default:
            qDebug() << "undefined id";
            break;
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
                QString strHexNumber = QString::number(oTableSelection.at(nItemIndex).data().toString().toInt(), 16);
                strCmd += strHexNumber.rightJustified(nAlignment, '0');
            }
            qDebug() << strCmd;

        }
        bool bResponseExpected = false;
        if(oTableSelection.count() > 3)
        {
            bResponseExpected = oTableSelection.at(2).data(Qt::CheckStateRole) == Qt::Checked;
        }

        qint32 nMsgCode = m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(strCmd.toStdString().c_str()), bResponseExpected);
        qDebug() << "data sending result:" << nMsgCode << "response expected" << bResponseExpected;
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
    QTableWidgetItem *pvalue1PacketName = new QTableWidgetItem("SET PWM_COOL_1 DUTY[0-100]");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue1PacketName); // insert item to created row to the second column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue1PacketResponseExpected = new QTableWidgetItem();        // is response expected?
    pvalue1PacketResponseExpected->setCheckState(Qt::Unchecked);                     // Qt::Unchecked or Qt::Checked
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue1PacketResponseExpected); // insert item to created row to the third column

    // the third column
    QTableWidgetItem *pvalue1PacketArg0 = new QTableWidgetItem("1.5");             // the value it contains
    pvalue1PacketArg0->setData(TableRoles::ByteCount, 3);                            // the value is 3 bytes
    pvalue1PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue1PacketArg0->setData(Qt::ToolTipRole, "minimal resolution is 100 Hz");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue1PacketArg0); // insert item to created row to the fourth column

    // the third column
    QTableWidgetItem *pvalue1PacketArg1 = new QTableWidgetItem("25");             // the value it contains
    pvalue1PacketArg1->setData(TableRoles::ByteCount, 3);                            // the value is 3 bytes
    pvalue1PacketArg1->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue1PacketArg1->setData(Qt::ToolTipRole, "minimal resolution is 100 Hz");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, pvalue1PacketArg1); // insert item to created row to the fourth column

    // the third column
    QTableWidgetItem *pvalue1PacketArg2 = new QTableWidgetItem("35");             // the value it contains
    pvalue1PacketArg2->setData(TableRoles::ByteCount, 3);                            // the value is 3 bytes
    pvalue1PacketArg2->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue1PacketArg2->setData(Qt::ToolTipRole, "minimal resolution is 100 Hz");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 5, pvalue1PacketArg2); // insert item to created row to the fourth column


    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue2PacketID = new QTableWidgetItem("31");                  // paket id
    pvalue2PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue2PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue2PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue2PacketName = new QTableWidgetItem("SET PWM_COOL_2 DUTY[0-100]");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue2PacketName); // insert item to created row to the second column

    // the third column
    QTableWidgetItem *pvalue2PacketResponseExpected = new QTableWidgetItem();        // is response expected?
    pvalue2PacketResponseExpected->setCheckState(Qt::Unchecked);                     // Qt::Unchecked or Qt::Checked
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue2PacketResponseExpected); // insert item to created row to the third column

    // the fourth column
    QTableWidgetItem *pvalue2PacketArg0 = new QTableWidgetItem("50");             // the value it contains
    pvalue2PacketArg0->setData(TableRoles::ByteCount, 3);                            // the value is 3 bytes
    pvalue2PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue2PacketArg0->setData(Qt::ToolTipRole, "no help");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue2PacketArg0); // insert item to created row to the fourth column


    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue3PacketID = new QTableWidgetItem("32");                  // paket id
    pvalue3PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue3PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue3PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue3PacketName = new QTableWidgetItem("SET DAC_1 [0-4095]");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue3PacketName); // insert item to created row to the second column

    // the third column
    QTableWidgetItem *pvalue3PacketResponseExpected = new QTableWidgetItem();        // is response expected?
    pvalue3PacketResponseExpected->setCheckState(Qt::Unchecked);                     // Qt::Unchecked or Qt::Checked
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue3PacketResponseExpected); // insert item to created row to the third column

    // the fourth column
    QTableWidgetItem *pvalue3PacketArg0 = new QTableWidgetItem("200");             // the value it contains
    pvalue3PacketArg0->setData(TableRoles::ByteCount, 3);                            // the value is 3 bytes
    pvalue3PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue3PacketArg0->setData(Qt::ToolTipRole, "no help");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue3PacketArg0); // insert item to created row to the fourth column


    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue4PacketID = new QTableWidgetItem("33");                  // paket id
    pvalue4PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue4PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue4PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue4PacketName = new QTableWidgetItem("SET DAC_2 [0-4095]");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue4PacketName); // insert item to created row to the second column

    // the third column
    QTableWidgetItem *pvalue4PacketResponseExpected = new QTableWidgetItem();        // is response expected?
    pvalue4PacketResponseExpected->setCheckState(Qt::Unchecked);                     // Qt::Unchecked or Qt::Checked
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue4PacketResponseExpected); // insert item to created row to the third column

    // the fourth column
    QTableWidgetItem *pvalue4PacketArg0 = new QTableWidgetItem("1000");             // the value it contains
    pvalue4PacketArg0->setData(TableRoles::ByteCount, 3);                            // the value is 3 bytes
    pvalue4PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue4PacketArg0->setData(Qt::ToolTipRole, "no help");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue4PacketArg0); // insert item to created row to the fourth column


    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue5PacketID = new QTableWidgetItem("34");                  // paket id
    pvalue5PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue5PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue5PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue5PacketName = new QTableWidgetItem("DO_PEAK_US [1-100]");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue5PacketName); // insert item to created row to the second column

    // the third column
    QTableWidgetItem *pvalue5PacketResponseExpected = new QTableWidgetItem();        // is response expected?
    pvalue5PacketResponseExpected->setCheckState(Qt::Unchecked);                     // Qt::Unchecked or Qt::Checked
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue5PacketResponseExpected); // insert item to created row to the third column

    // the fourth column
    QTableWidgetItem *pvalue5PacketArg0 = new QTableWidgetItem("10");             // the value it contains
    pvalue5PacketArg0->setData(TableRoles::ByteCount, 3);                            // the value is 3 bytes
    pvalue5PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue5PacketArg0->setData(Qt::ToolTipRole, "no help");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue5PacketArg0); // insert item to created row to the fourth column


    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue6PacketID = new QTableWidgetItem("35");                  // paket id
    pvalue6PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue6PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue6PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue6PacketName = new QTableWidgetItem("SET_PWM_CQM_1_FREQ [1000 - 10000]");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue6PacketName); // insert item to created row to the second column

    // the third column
    QTableWidgetItem *pvalue6PacketResponseExpected = new QTableWidgetItem();        // is response expected?
    pvalue6PacketResponseExpected->setCheckState(Qt::Unchecked);                     // Qt::Unchecked or Qt::Checked
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue6PacketResponseExpected); // insert item to created row to the third column

    // the fourth column
    QTableWidgetItem *pvalue6PacketArg0 = new QTableWidgetItem("10000");             // the value it contains
    pvalue6PacketArg0->setData(TableRoles::ByteCount, 3);                            // the value is 3 bytes
    pvalue6PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue6PacketArg0->setData(Qt::ToolTipRole, "no help");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue6PacketArg0); // insert item to created row to the fourth column


    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue7PacketID = new QTableWidgetItem("36");                  // paket id
    pvalue7PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue7PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue7PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue7PacketName = new QTableWidgetItem("SET_PWM_CQM_2_FREQ [1000 - 10000]");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue7PacketName); // insert item to created row to the second column

    // the third column
    QTableWidgetItem *pvalue7PacketResponseExpected = new QTableWidgetItem();        // is response expected?
    pvalue7PacketResponseExpected->setCheckState(Qt::Unchecked);                     // Qt::Unchecked or Qt::Checked
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue7PacketResponseExpected); // insert item to created row to the third column

    // the fourth column
    QTableWidgetItem *pvalue7PacketArg0 = new QTableWidgetItem("25000");             // the value it contains
    pvalue7PacketArg0->setData(TableRoles::ByteCount, 3);                            // the value is 3 bytes
    pvalue7PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue7PacketArg0->setData(Qt::ToolTipRole, "no help");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue7PacketArg0); // insert item to created row to the fourth column


    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue8PacketID = new QTableWidgetItem("37");                  // paket id
    pvalue8PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue8PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue8PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue8PacketName = new QTableWidgetItem("ADC2_SEND_DATA");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue8PacketName); // insert item to created row to the second column

    // the third column
    QTableWidgetItem *pvalue8PacketResponseExpected = new QTableWidgetItem();        // is response expected?
    pvalue8PacketResponseExpected->setCheckState(Qt::Unchecked);                     // Qt::Unchecked or Qt::Checked
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue8PacketResponseExpected); // insert item to created row to the third column

    // the fourth column
    QTableWidgetItem *pvalue8PacketArg0 = new QTableWidgetItem("1");             // the value it contains
    pvalue8PacketArg0->setData(TableRoles::ByteCount, 3);                            // the value is 3 bytes
    pvalue8PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue8PacketArg0->setData(Qt::ToolTipRole, "1 - adjusted; 2 - row average; 3 - row all");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue8PacketArg0); // insert item to created row to the fourth column


    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue9PacketID = new QTableWidgetItem("38");                  // paket id
    pvalue9PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue9PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue9PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue9PacketName = new QTableWidgetItem("ADC1_SEND_DATA");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue9PacketName); // insert item to created row to the second column

    // the third column
    QTableWidgetItem *pvalue9PacketResponseExpected = new QTableWidgetItem();        // is response expected?
    pvalue9PacketResponseExpected->setCheckState(Qt::Unchecked);                     // Qt::Unchecked or Qt::Checked
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pvalue9PacketResponseExpected); // insert item to created row to the third column

    // the fourth column
    QTableWidgetItem *pvalue9PacketArg0 = new QTableWidgetItem("1");             // the value it contains
    pvalue9PacketArg0->setData(TableRoles::ByteCount, 3);                            // the value is 3 bytes
    pvalue9PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue9PacketArg0->setData(Qt::ToolTipRole, "1 - adjusted; 2 - row average; 3 - row all");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue9PacketArg0); // insert item to created row to the fourth column
}

void MainWindow::on_clearButton_clicked()
{
    ui->textBrowser->clear();
}

void MainWindow::on_connectButton_clicked()
{
     m_CommProt.data()->SetTargetMedium(ui->comboBox->currentText());
}

void MainWindow::on_disconnectButton_clicked()
{
     m_CommProt.data()->SetTargetMedium("");

     ui->comboBox->clear();

     for(auto comPort : QSerialPortInfo::availablePorts())
     {
         ui->comboBox->addItem(comPort.portName());
     }
}
