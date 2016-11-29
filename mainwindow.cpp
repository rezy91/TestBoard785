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
    this->restoreGeometry(m_pSettingStrorage->RestoreGeometry());
    ui->sendButton->setEnabled(false);
    ui->tableWidget->setColumnWidth(1, 150);
    ui->tableWidget->setColumnWidth(2, 60);

    m_oPortsRefresh.start(500);

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
            m_pSettingStrorage->StorePortName(m_CommProt.data()->GetLastSetTargetMedium());
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

    connect(&m_oPortsRefresh, &QTimer::timeout, [this]() {
        if(m_CommProt.data()->GetState() == 0)
        {
            static quint32 nLastPortCount = 0;
            quint32 nPortsCount = QSerialPortInfo::availablePorts().size();
            if(nLastPortCount != nPortsCount)
            {
                nLastPortCount = nPortsCount;
                SetAvaiblePorts();
            }
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
    //! set frequency packet
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pFrequencyPacketID = new QTableWidgetItem("30");                  // paket id
    pFrequencyPacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pFrequencyPacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pFrequencyPacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pFrequencyPacketName = new QTableWidgetItem("SET_FREQUENCY");     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pFrequencyPacketName); // insert item to created row to the second column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pFrequencyPacketResponseExpected = new QTableWidgetItem();        // is response expected?
    pFrequencyPacketResponseExpected->setCheckState(Qt::Unchecked);                     // Qt::Unchecked or Qt::Checked
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, pFrequencyPacketResponseExpected); // insert item to created row to the third column

    // the third column
    QTableWidgetItem *pFrequencyPacketArg0 = new QTableWidgetItem("30000");             // the value it contains
    pFrequencyPacketArg0->setData(TableRoles::ByteCount, 3);                            // the value is 3 bytes
    pFrequencyPacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pFrequencyPacketArg0->setData(Qt::ToolTipRole, "minimal resolution is 100 Hz");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pFrequencyPacketArg0); // insert item to created row to the fourth column

    //! and next packet definition
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
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    m_pSettingStrorage->StoreGeometry(saveGeometry());
    QWidget::closeEvent(event);
}
