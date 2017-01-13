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
    static int mMainTimer = 0;


    o_smith->setWindowIcon(QIcon(":/iconSmith.png"));
    o_smith->setWindowTitle("Smith diagram");
    o_smith->show();

    connect(this, &MainWindow::SendNewData, o_smith, &SmithMain::ReceivedNewData);
    connect(this, &MainWindow::SendStateButton, o_smith, &SmithMain::ReceivedStateButton);


    o_graph->setWindowIcon(QIcon(":/iconGraph.jpg"));
    o_graph->setWindowTitle("Graph");
    o_graph->show();

    connect(this, &MainWindow::SendUpdateGraph, o_graph, &Grapmain::refreshGraph);
    connect (o_graph, &Grapmain::SendUpdateData, this, &MainWindow::UpdateDoubleSpinBoxX);


    ui->setupUi(this);


    ui->sendButton->setEnabled(false);
    ui->tableWidget->setColumnWidth(1, 200);

    SetAvaiblePorts();
    SetLastPort();
    TmrMstr.start(1);



    restoreAllSettings();


    connect(&TmrMstr,&QTimer::timeout,[this](){
        for(qint32 loop = 0; loop < NMB_ITEMS_FOR_TIMERS + 1; loop++)
        {
            if(timerEnable[loop] == true)
            {
                CurrentTime_ms[loop]++;
                if(CurrentTime_ms[loop] >= RequirementTime_ms[loop])
                {
                    CurrentTime_ms[loop] = 0;
                    //qDebug() << "Timer" << loop << "tick";
                    m_CommProt.data()->SendData(m_nDeviceAddress, assemblyMsq[loop], respExp[loop]);
                }
            }
        }
        if(++mMainTimer >= 100)
        {
            mMainTimer = 0;
        }
    });

    connect(ui->comboBox_2,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[=](int nValue){
        getIndexInQList(0, nValue);
    });
    connect(ui->comboBox_3,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[=](int nValue){
        getIndexInQList(1, nValue);
    });
    connect(ui->comboBox_4,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[=](int nValue){
        getIndexInQList(2, nValue);
    });
    connect(ui->comboBox_5,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[=](int nValue){
        getIndexInQList(3, nValue);
    });


    connect(ui->doubleSpinBox,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),[=](double nValue){

        if(nValue >= 10)
        {
            ui->doubleSpinBox->setSingleStep(1.0);
        }
        else if(nValue >= 1)
        {
            ui->doubleSpinBox->setSingleStep(0.1);
        }
        else if(nValue >= 0.1)
        {
            ui->doubleSpinBox->setSingleStep(0.01);
        }

        coefInput[0] = nValue;
    });

    connect(ui->doubleSpinBox_2,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),[=](double nValue){

        if(nValue >= 10)
        {
            ui->doubleSpinBox_2->setSingleStep(1.0);
        }
        else if(nValue >= 1)
        {
            ui->doubleSpinBox_2->setSingleStep(0.1);
        }
        else if(nValue >= 0.1)
        {
            ui->doubleSpinBox_2->setSingleStep(0.01);
        }

        coefInput[1] = nValue;
    });

    connect(ui->doubleSpinBox_3,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),[=](double nValue){

        if(nValue >= 10)
        {
            ui->doubleSpinBox_3->setSingleStep(1.0);
        }
        else if(nValue >= 1)
        {
            ui->doubleSpinBox_3->setSingleStep(0.1);
        }
        else if(nValue >= 0.1)
        {
            ui->doubleSpinBox_3->setSingleStep(0.01);
        }

        coefInput[2] = nValue;
    });

    connect(ui->doubleSpinBox_4,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),[=](double nValue){

        if(nValue >= 10)
        {
            ui->doubleSpinBox_4->setSingleStep(1.0);
        }
        else if(nValue >= 1)
        {
            ui->doubleSpinBox_4->setSingleStep(0.1);
        }
        else if(nValue >= 0.1)
        {
            ui->doubleSpinBox_4->setSingleStep(0.01);
        }

        coefInput[3] = nValue;
    });


    connect(ui->spinBox,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int nValue){
        m_pSettingStrorage->StoreRefreshFirst(nValue);
        RequirementTime_ms[0] = ui->spinBox->value();
    });

    connect(ui->spinBox_2,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int nValue){
        m_pSettingStrorage->StoreRefreshSecond(nValue);
        RequirementTime_ms[1] = ui->spinBox_2->value();
    });

    connect(ui->spinBox_3,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int nValue){
        m_pSettingStrorage->StoreRefreshThird(nValue);
        RequirementTime_ms[2] = ui->spinBox_3->value();
    });

    connect(ui->spinBox_4,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int nValue){
        m_pSettingStrorage->StoreRefreshFourth(nValue);
        RequirementTime_ms[3] = ui->spinBox_4->value();
    });

    connect(ui->spinBox_5,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int nValue){
        m_pSettingStrorage->StoreRefreshFifth(nValue);
        RequirementTime_ms[4] = ui->spinBox_5->value();
    });

    connect(ui->spinBox_6,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[=](int nValue){
        m_pSettingStrorage->StoreRefreshSixth(nValue);
        RequirementTime_ms[5] = ui->spinBox_6->value();
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


        if(arrData.at(0) == 'a' && arrData.at(1) == 'd')//ADCx data
        {
            QString adjString = QString(arrData);
            QStringList myStringList = adjString.split(QRegExp("(\\s+| |=)"));

            QStringList myStringOnlyNumbers;


            for(int iLoop = 0; iLoop < myStringList.count(); iLoop++)
            {
                //qDebug() << "all: " + myStringList.at(iLoop) << endl;

                bool isNumber;
                myStringList.at(iLoop).toFloat(&isNumber);

                if(isNumber)
                {
                    myStringOnlyNumbers.append(myStringList.at(iLoop));
                    //qDebug() << "number: " + myStringList.at(iLoop) << endl;
                }
            }

            if(arrData.at(2) == '3' && arrData.at(3) == 'c')//ADC3 adjusted data
            {
                recognizeIfDisplayNewData(&myStringOnlyNumbers, 0);
            }
            else if(arrData.at(2) == '3' && arrData.at(3) == 's')//ADC3 average data
            {
                recognizeIfDisplayNewData(&myStringOnlyNumbers, 1);
            }
            else if(arrData.at(2) == '2' && arrData.at(3) == 'c')//ADC2 adjusted data
            {

                recognizeIfDisplayNewData(&myStringOnlyNumbers, 2);

                COMPLEX_NUMBER_GONIO currentData;
                COMPLEX_NUMBER_GONIO averageDataA;
                COMPLEX_NUMBER_GONIO averageData50;
                COMPLEX_NUMBER_GONIO reflRatioAvg;
                COMPLEX_NUMBER_GONIO reflRatio50;

                currentData.magnitude = myStringOnlyNumbers.at(1).toFloat();
                currentData.phase_rad = myStringOnlyNumbers.at(2).toFloat();
                averageDataA.magnitude = myStringOnlyNumbers.at(3).toFloat();
                averageDataA.phase_rad = myStringOnlyNumbers.at(4).toFloat();

                reflRatioAvg = CalculateReflectionRatio(currentData, averageDataA);

                averageData50.magnitude = 50;
                averageData50.phase_rad = 0;

                reflRatio50 = CalculateReflectionRatio(currentData, averageData50);
                emit SendNewData(int(reflRatioAvg.magnitude * 1000), int(reflRatioAvg.phase_rad * 1000),int(reflRatio50.magnitude * 1000), int(reflRatio50.phase_rad * 1000));
            }
            else if(arrData.at(2) == '2' && arrData.at(3) == 's')//ADC2 average data
            {
                recognizeIfDisplayNewData(&myStringOnlyNumbers, 3);
            }
            else if(arrData.at(2) == '1' && arrData.at(3) == 'c')//ADC1 adjusted data
            {
                recognizeIfDisplayNewData(&myStringOnlyNumbers, 4);
            }
            else if(arrData.at(2) == '1' && arrData.at(3) == 's')//ADC1 average data
            {
                recognizeIfDisplayNewData(&myStringOnlyNumbers, 5);
            }


        }
        else if(arrData.at(1) == '|')//Digital input readed
        {
            if(arrData.at(0) == '1')
            {
                ui->toolButton->setStyleSheet("background-color:red;color:black;font:11px");
            }
            else if(arrData.at(0) == '0')
            {
                ui->toolButton->setStyleSheet("background-color:grey;color:white;font:11px");
            }
            if(arrData.at(3) == '|')
            {
                if(arrData.at(2) == '1')
                {
                    ui->toolButton_2->setStyleSheet("background-color:red;color:black;font:11px");
                }
                else if(arrData.at(2) == '0')
                {
                    ui->toolButton_2->setStyleSheet("background-color:grey;color:white;font:11px");
                }
            }
        }

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

    RequirementTime_ms[6] = 500;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UpdateDoubleSpinBoxX(double newValue, int index)
{
    if(index == 0)
    {
     ui->doubleSpinBox->setValue(newValue);
    }
    else if(index == 1)
    {
        ui->doubleSpinBox_2->setValue(newValue);
    }
    else if(index == 2)
    {
        ui->doubleSpinBox_3->setValue(newValue);
    }
    else if(index == 3)
    {
        ui->doubleSpinBox_4->setValue(newValue);
    }
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


        for(qint32 row = 0; row < NMB_ITEMS_FOR_TIMERS; row++)
        {
            if(oTableSelection.at(0).data().toInt() == (34 + row))
            {
                if(oTableSelection.at(3).data().toInt() == 1)
                {
                    m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(strCmd.toStdString().c_str()), true);

                    CurrentTime_ms[row] = 0;
                    timerEnable[row] = true;

                    assemblyMsq[row] = QByteArray::fromHex(strCmd.toStdString().c_str());
                    respExp[row] = true;
                }
                else if(oTableSelection.at(3).data().toInt() == 0)
                {
                    m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(strCmd.toStdString().c_str()), false);

                    timerEnable[row] = false;
                    respExp[row] = false;

                }
                ui->comboBox_2->clear();
                ui->comboBox_3->clear();
                ui->comboBox_4->clear();
                ui->comboBox_5->clear();

                ui->comboBox_2->addItem("-");
                ui->comboBox_3->addItem("-");
                ui->comboBox_4->addItem("-");
                ui->comboBox_5->addItem("-");

                for(qint32 fillComboBoxSignal = 0; fillComboBoxSignal < NMB_ITEMS_FOR_TIMERS; fillComboBoxSignal++)
                {
                    if(timerEnable[fillComboBoxSignal])
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
        if(oTableSelection.at(0).data().toInt() == 32)
        {
            if(oTableSelection.at(3).data().toInt() == 1)
            {
                m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(strCmd.toStdString().c_str()), true);

                CurrentTime_ms[6] = 0;
                timerEnable[6] = true;

                assemblyMsq[6] = QByteArray::fromHex(strCmd.toStdString().c_str());
                respExp[6] = true;
            }
            else if(oTableSelection.at(3).data().toInt() == 0)
            {
                m_CommProt.data()->SendData(m_nDeviceAddress, QByteArray::fromHex(strCmd.toStdString().c_str()), false);

                timerEnable[6] = false;
                respExp[6] = false;
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

void MainWindow::AppendText(QString strText)
{
    ui->textBrowser->append(QString("%1\t%2").arg(QTime::currentTime().toString()).arg(strText));
}

void MainWindow::FillCommandTable()
{
    QString StoredItems = m_pSettingStrorage->RestoreRowItem();
    QStringList list;
    list = StoredItems.split(QRegExp("\\s+"));
    int indexInList = 0;


    //! set packet
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue1PacketID = new QTableWidgetItem(list[indexInList++]);                  // paket id
    pvalue1PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue1PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue1PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue1PacketName = new QTableWidgetItem(list[indexInList++]);     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue1PacketName); // insert item to created row to the second column

    // the third column
    QTableWidgetItem *pvalue1PacketArg0 = new QTableWidgetItem(list[indexInList++]);             // the value it contains
    pvalue1PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue1PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue1PacketArg0->setData(Qt::ToolTipRole, "(1) [0-100] in %");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue1PacketArg0); // insert item to created row to the fourth column

    // the fourth column
    QTableWidgetItem *pvalue2PacketArg1 = new QTableWidgetItem(list[indexInList++]);             // the value it contains
    pvalue2PacketArg1->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue2PacketArg1->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue2PacketArg1->setData(Qt::ToolTipRole, "(2) [0-100] in %");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, pvalue2PacketArg1); // insert item to created row to the fourth column


    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue3PacketID = new QTableWidgetItem(list[indexInList++]);                  // paket id
    pvalue3PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue3PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue3PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue3PacketName = new QTableWidgetItem(list[indexInList++]);     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue3PacketName); // insert item to created row to the second column

    // the fourth column
    QTableWidgetItem *pvalue3PacketArg0 = new QTableWidgetItem(list[indexInList++]);             // the value it contains
    pvalue3PacketArg0->setData(TableRoles::ByteCount, 2);                            // the value is 3 bytes
    pvalue3PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue3PacketArg0->setData(Qt::ToolTipRole, "(1) [0-4095]");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue3PacketArg0); // insert item to created row to the fourth column

    // the fourth column
    QTableWidgetItem *pvalue3PacketArg1 = new QTableWidgetItem(list[indexInList++]);             // the value it contains
    pvalue3PacketArg1->setData(TableRoles::ByteCount, 2);                            // the value is 3 bytes
    pvalue3PacketArg1->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue3PacketArg1->setData(Qt::ToolTipRole, "(2) [0-4095]");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, pvalue3PacketArg1); // insert item to created row to the fourth column


    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue4PacketID = new QTableWidgetItem(list[indexInList++]);                  // paket id
    pvalue4PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue4PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue4PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue4PacketName = new QTableWidgetItem(list[indexInList++]);     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue4PacketName); // insert item to created row to the second column

    // the fourth column
    QTableWidgetItem *pvalue4PacketArg0 = new QTableWidgetItem(list[indexInList++]);             // the value it contains
    pvalue4PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue4PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue4PacketArg0->setData(Qt::ToolTipRole, "[0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue4PacketArg0); // insert item to created row to the fourth column


    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue6PacketID = new QTableWidgetItem(list[indexInList++]);                  // paket id
    pvalue6PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue6PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue6PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue6PacketName = new QTableWidgetItem(list[indexInList++]);     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue6PacketName); // insert item to created row to the second column

    // the fourth column
    QTableWidgetItem *pvalue6PacketArg0 = new QTableWidgetItem(list[indexInList++]);             // the value it contains
    pvalue6PacketArg0->setData(TableRoles::ByteCount, 3);                            // the value is 3 bytes
    pvalue6PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue6PacketArg0->setData(Qt::ToolTipRole, "(1) [1000 - 100000]");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue6PacketArg0); // insert item to created row to the fourth column

    // the fourth column
    QTableWidgetItem *pvalue6PacketArg1 = new QTableWidgetItem(list[indexInList++]);             // the value it contains
    pvalue6PacketArg1->setData(TableRoles::ByteCount, 3);                            // the value is 3 bytes
    pvalue6PacketArg1->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue6PacketArg1->setData(Qt::ToolTipRole, "(2) [1000 - 100000]");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, pvalue6PacketArg1); // insert item to created row to the fourth column


    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue83PacketID = new QTableWidgetItem(list[indexInList++]);                  // paket id
    pvalue83PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue83PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue83PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue83PacketName = new QTableWidgetItem(list[indexInList++]);     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue83PacketName); // insert item to created row to the second column

    QTableWidgetItem *pvalue83PacketArg0 = new QTableWidgetItem(list[indexInList++]);             // the value it contains
    pvalue83PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue83PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue83PacketArg0->setData(Qt::ToolTipRole, "[0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue83PacketArg0); // insert item to created row to the fourth column


    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue93PacketID = new QTableWidgetItem(list[indexInList++]);                  // paket id
    pvalue93PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue93PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue93PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue93PacketName = new QTableWidgetItem(list[indexInList++]);     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue93PacketName); // insert item to created row to the second column

    QTableWidgetItem *pvalue93PacketArg0 = new QTableWidgetItem(list[indexInList++]);             // the value it contains
    pvalue93PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue93PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue93PacketArg0->setData(Qt::ToolTipRole, "[0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue93PacketArg0); // insert item to created row to the fourth column




    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue8PacketID = new QTableWidgetItem(list[indexInList++]);                  // paket id
    pvalue8PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue8PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue8PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue8PacketName = new QTableWidgetItem(list[indexInList++]);     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue8PacketName); // insert item to created row to the second column

    QTableWidgetItem *pvalue8PacketArg0 = new QTableWidgetItem(list[indexInList++]);             // the value it contains
    pvalue8PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue8PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue8PacketArg0->setData(Qt::ToolTipRole, "[0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue8PacketArg0); // insert item to created row to the fourth column


    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue9PacketID = new QTableWidgetItem(list[indexInList++]);                  // paket id
    pvalue9PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue9PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue9PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue9PacketName = new QTableWidgetItem(list[indexInList++]);     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue9PacketName); // insert item to created row to the second column

    QTableWidgetItem *pvalue9PacketArg0 = new QTableWidgetItem(list[indexInList++]);             // the value it contains
    pvalue9PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue9PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue9PacketArg0->setData(Qt::ToolTipRole, "[0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue9PacketArg0); // insert item to created row to the fourth column



    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue10PacketID = new QTableWidgetItem(list[indexInList++]);                  // paket id
    pvalue10PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue10PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue10PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue10PacketName = new QTableWidgetItem(list[indexInList++]);     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue10PacketName); // insert item to created row to the second column

    QTableWidgetItem *pvalue10PacketArg0 = new QTableWidgetItem(list[indexInList++]);             // the value it contains
    pvalue10PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue10PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue10PacketArg0->setData(Qt::ToolTipRole, "[0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue10PacketArg0); // insert item to created row to the fourth column



    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue11PacketID = new QTableWidgetItem(list[indexInList++]);                  // paket id
    pvalue11PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue11PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue11PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue11PacketName = new QTableWidgetItem(list[indexInList++]);     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue11PacketName); // insert item to created row to the second column

    QTableWidgetItem *pvalue11PacketArg0 = new QTableWidgetItem(list[indexInList++]);             // the value it contains
    pvalue11PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue11PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue11PacketArg0->setData(Qt::ToolTipRole, "[0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue11PacketArg0); // insert item to created row to the fourth column




    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue14PacketID = new QTableWidgetItem(list[indexInList++]);                  // paket id
    pvalue14PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue14PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue14PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue14PacketName = new QTableWidgetItem(list[indexInList++]);     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue14PacketName); // insert item to created row to the second column

    QTableWidgetItem *pvalue14PacketArg0 = new QTableWidgetItem(list[indexInList++]);             // the value it contains
    pvalue14PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue14PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue14PacketArg0->setData(Qt::ToolTipRole, "(3)[0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue14PacketArg0); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue14PacketArg1 = new QTableWidgetItem(list[indexInList++]);             // the value it contains
    pvalue14PacketArg1->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue14PacketArg1->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue14PacketArg1->setData(Qt::ToolTipRole, "(2)[0-1] disable/enable ");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, pvalue14PacketArg1); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue14PacketArg2 = new QTableWidgetItem(list[indexInList++]);             // the value it contains
    pvalue14PacketArg2->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue14PacketArg2->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue14PacketArg2->setData(Qt::ToolTipRole, "(1)[0-1] disable/enable ");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 5, pvalue14PacketArg2); // insert item to created row to the fourth column



    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue17PacketID = new QTableWidgetItem(list[indexInList++]);                  // paket id
    pvalue17PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue17PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue17PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue17PacketName = new QTableWidgetItem(list[indexInList++]);     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue17PacketName); // insert item to created row to the second column

    for(qint32 loop = 0; loop < 2 * 4; loop++)
    {
        QTableWidgetItem *pvalue17PacketArg = new QTableWidgetItem();             // the value it contains

        if(!(loop % 2))
        {
            pvalue17PacketArg->setText(list[indexInList++]);
            pvalue17PacketArg->setData(Qt::ToolTipRole, "Divident channel[" + QString::number(loop / 2) + "]");
        }
        else
        {
            pvalue17PacketArg->setText(list[indexInList++]);
            pvalue17PacketArg->setData(Qt::ToolTipRole, "Divisor channel[" + QString::number(loop / 2) + "]");
        }

        pvalue17PacketArg->setData(TableRoles::ByteCount, 2);
        pvalue17PacketArg->setData(TableRoles::NumeralSystem, TableRoles::Decimal);
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3 + loop, pvalue17PacketArg);
    }

    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue15PacketID = new QTableWidgetItem(list[indexInList++]);                  // paket id
    pvalue15PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue15PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue15PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue15PacketName = new QTableWidgetItem(list[indexInList++]);     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue15PacketName); // insert item to created row to the second column

    for(qint32 loop = 0; loop < 2 * 7; loop++)
    {
        QTableWidgetItem *pvalue15PacketArg = new QTableWidgetItem();             // the value it contains

        if(!(loop % 2))
        {
            pvalue15PacketArg->setText(list[indexInList++]);
            pvalue15PacketArg->setData(Qt::ToolTipRole, "Divident channel[" + QString::number(loop / 2) + "]");
        }
        else
        {
            pvalue15PacketArg->setText(list[indexInList++]);
            pvalue15PacketArg->setData(Qt::ToolTipRole, "Divisor channel[" + QString::number(loop / 2) + "]");
        }

        pvalue15PacketArg->setData(TableRoles::ByteCount, 2);
        pvalue15PacketArg->setData(TableRoles::NumeralSystem, TableRoles::Decimal);
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3 + loop, pvalue15PacketArg);
    }

    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue16PacketID = new QTableWidgetItem(list[indexInList++]);                  // paket id
    pvalue16PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue16PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue16PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue16PacketName = new QTableWidgetItem(list[indexInList++]);     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue16PacketName); // insert item to created row to the second column

    for(qint32 loop = 0; loop < 2 * 9; loop++)
    {
        QTableWidgetItem *pvalue16PacketArg = new QTableWidgetItem();             // the value it contains

        if(!(loop % 2))
        {
            pvalue16PacketArg->setText(list[indexInList++]);
            pvalue16PacketArg->setData(Qt::ToolTipRole, "Divident channel[" + QString::number(loop / 2) + "]");
        }
        else
        {
            pvalue16PacketArg->setText(list[indexInList++]);
            pvalue16PacketArg->setData(Qt::ToolTipRole, "Divisor channel[" + QString::number(loop / 2) + "]");
        }

        pvalue16PacketArg->setData(TableRoles::ByteCount, 2);
        pvalue16PacketArg->setData(TableRoles::NumeralSystem, TableRoles::Decimal);
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3 + loop, pvalue16PacketArg);
    }



    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue18PacketID = new QTableWidgetItem(list[indexInList++]);                  // paket id
    pvalue18PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue18PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue18PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue18PacketName = new QTableWidgetItem(list[indexInList++]);     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue18PacketName); // insert item to created row to the second column

    // the fourth column
    QTableWidgetItem *pvalue18PacketArg0 = new QTableWidgetItem(list[indexInList++]);             // the value it contains
    pvalue18PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg0->setData(Qt::ToolTipRole, "(REL_OUT_1) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue18PacketArg0); // insert item to created row to the fourth column

    // the fourth column
    QTableWidgetItem *pvalue18PacketArg1 = new QTableWidgetItem(list[indexInList++]);             // the value it contains
    pvalue18PacketArg1->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg1->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg1->setData(Qt::ToolTipRole, "(REL_OUT_2) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, pvalue18PacketArg1); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg2 = new QTableWidgetItem(list[indexInList++]);             // the value it contains
    pvalue18PacketArg2->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg2->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg2->setData(Qt::ToolTipRole, "(REL_OUT_3) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 5, pvalue18PacketArg2); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg4 = new QTableWidgetItem(list[indexInList++]);             // the value it contains
    pvalue18PacketArg4->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg4->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg4->setData(Qt::ToolTipRole, "(REL_OUT_4) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 6, pvalue18PacketArg4); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg5 = new QTableWidgetItem(list[indexInList++]);             // the value it contains
    pvalue18PacketArg5->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg5->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg5->setData(Qt::ToolTipRole, "(nAMP_RESET) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 7, pvalue18PacketArg5); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg6 = new QTableWidgetItem(list[indexInList++]);             // the value it contains
    pvalue18PacketArg6->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg6->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg6->setData(Qt::ToolTipRole, "(Amp_PWR_ON) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 8, pvalue18PacketArg6); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg7 = new QTableWidgetItem(list[indexInList++]);             // the value it contains
    pvalue18PacketArg7->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg7->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg7->setData(Qt::ToolTipRole, "(nAMPComm_OCP) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 9, pvalue18PacketArg7); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg8 = new QTableWidgetItem(list[indexInList++]);             // the value it contains
    pvalue18PacketArg8->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg8->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg8->setData(Qt::ToolTipRole, "(nGenErr_uP) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 10, pvalue18PacketArg8); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue18PacketArg9 = new QTableWidgetItem(list[indexInList++]);             // the value it contains
    pvalue18PacketArg9->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue18PacketArg9->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue18PacketArg9->setData(Qt::ToolTipRole, "(Fraction/Exilis) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 11, pvalue18PacketArg9); // insert item to created row to the fourth column


    //! and next packet definition
    // the first column
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());                            // create new row in table
    QTableWidgetItem *pvalue19PacketID = new QTableWidgetItem(list[indexInList++]);                  // paket id
    pvalue19PacketID->setData(TableRoles::ByteCount, 1);                              // paket id is 1 byte
    pvalue19PacketID->setData(TableRoles::NumeralSystem, TableRoles::Hex);            // packet id is displayed as hex
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, pvalue19PacketID);   // insert item to created row to the first column

    // the second column (it has no impact on data to be sent)
    QTableWidgetItem *pvalue19PacketName = new QTableWidgetItem(list[indexInList++]);     // readable description
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, pvalue19PacketName); // insert item to created row to the second column

    QTableWidgetItem *pvalue19PacketArg0 = new QTableWidgetItem(list[indexInList++]);             // the value it contains
    pvalue19PacketArg0->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue19PacketArg0->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue19PacketArg0->setData(Qt::ToolTipRole, "(RESET) [0-100] in us");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, pvalue19PacketArg0); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue19PacketArg1 = new QTableWidgetItem(list[indexInList++]);             // the value it contains
    pvalue19PacketArg1->setData(TableRoles::ByteCount, 1);                            // the value is 3 bytes
    pvalue19PacketArg1->setData(TableRoles::NumeralSystem, TableRoles::Decimal);      // packet id is displayed as decimal
    pvalue19PacketArg1->setData(Qt::ToolTipRole, "(ON) [0-1] disable/enable");     // a hint which is displayed when mouse hovers over
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 4, pvalue19PacketArg1); // insert item to created row to the fourth column

    QTableWidgetItem *pvalue19PacketArg2 = new QTableWidgetItem(list[indexInList++]);             // the value it contains
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


    RequirementTime_ms[0] = ui->spinBox->value();
    RequirementTime_ms[1] = ui->spinBox_2->value();
    RequirementTime_ms[2] = ui->spinBox_3->value();
    RequirementTime_ms[3] = ui->spinBox_4->value();
    RequirementTime_ms[4] = ui->spinBox_5->value();
    RequirementTime_ms[5] = ui->spinBox_6->value();

    m_CommProt.data()->SetTargetMedium(ui->comboBox->currentText());

    m_pSettingStrorage->StorePortName(ui->comboBox->currentText());

    if(m_bSaveData)
    {
        m_oFile.setFileName(QString("SavedData_%1.txt").arg(QDateTime::currentDateTime().toTime_t()));
        if(!m_oFile.open(QFile::WriteOnly))
        {
            qDebug() << "error: cannot open file";
            return;
        }
    }
    emit SendStateButton(true);
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

    coefInput[0] = m_pSettingStrorage->RestoreMultiplierSignalFirst();
    ui->doubleSpinBox->setValue(coefInput[0]);
    coefInput[1] = m_pSettingStrorage->RestoreMultiplierSignalSecond();
    ui->doubleSpinBox_2->setValue(coefInput[1]);
    coefInput[2] = m_pSettingStrorage->RestoreMultiplierSignalThird();
    ui->doubleSpinBox_3->setValue(coefInput[2]);
    coefInput[3] = m_pSettingStrorage->RestoreMultiplierSignalFourth();
    ui->doubleSpinBox_4->setValue(coefInput[3]);
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


    ReflectionRatio.magnitude = sqrt(dividentAlgebReal * dividentAlgebReal + dividentAlgebImag * dividentAlgebImag) / sqrt(divisorAlgebReal * divisorAlgebReal + divisorAlgebImag * divisorAlgebImag);
    if(ReflectionRatio.magnitude > 0.05)
    {
        ReflectionRatio.phase_rad = atan(divisorAlgebImag / divisorAlgebReal) - atan(dividentAlgebImag / dividentAlgebReal);

        //qDebug() << ReflectionRatio.magnitude << " " << ReflectionRatio.phase_rad;
    }
    else
    {
        ReflectionRatio.phase_rad = 0;
    }

    return ReflectionRatio;
}

void MainWindow::getIndexInQList(int NumberComboBox, int indexInComboBox)
{
    //qDebug() << "getIndexInQList: " << indexInComboBox;

    if(indexInComboBox >= 1)
    {
        recStat[NumberComboBox] = 1;
        int absoluteIndex = indexInComboBox - 1;

        for(qint32 iLoop = 0; iLoop < NMB_ITEMS_FOR_TIMERS; iLoop++)
        {
            if(timerEnable[iLoop])
            {
                if((absoluteIndex - allAdxSignals[iLoop].count()) < 0)
                {
                    //qDebug() << "has been found signal: " << allAdxSignals[iLoop].at(absoluteIndex) << endl;

                    sourceAd[NumberComboBox] = iLoop;
                    sourceSignal[NumberComboBox] = absoluteIndex;
                    sourceSignText[NumberComboBox] = allAdxSignals[iLoop].at(absoluteIndex);

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
        emit SendUpdateGraph(refreshTime, recvItems, coefInput, recStat, sourceSignText, NumberComboBox);
    }
}

void MainWindow::recognizeIfDisplayNewData(QStringList* listOfNumbers, int adx)
{
    for(int iLoop = 0; iLoop < 4; iLoop++)
    {
        if((sourceAd[iLoop] == adx) && (sourceSignal[iLoop] >= 0))
        {
            if(adx == 0)
            {
                refreshTime[iLoop] = ui->spinBox->value();
            }
            else if(adx == 1)
            {
                refreshTime[iLoop] = ui->spinBox_2->value();
            }
            else if(adx == 2)
            {
                refreshTime[iLoop] = ui->spinBox_3->value();
            }
            else if(adx == 3)
            {
                refreshTime[iLoop] = ui->spinBox_4->value();
            }
            else if(adx == 4)
            {
                refreshTime[iLoop] = ui->spinBox_5->value();
            }
            else if(adx == 5)
            {
                refreshTime[iLoop] = ui->spinBox_6->value();
            }
            recvItems[iLoop] = listOfNumbers->at(sourceSignal[iLoop]).toDouble();
            emit SendUpdateGraph(refreshTime, recvItems, coefInput, recStat, sourceSignText, iLoop);
        }
    }
}

void MainWindow::on_disconnectButton_clicked()
{
    ui->checkBox->setEnabled(true);

     m_CommProt.data()->SetTargetMedium("");

     SetAvaiblePorts();
     SetLastPort();

     for(qint32 loop = 0; loop < NMB_ITEMS_FOR_TIMERS + 1; loop++)
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

     ui->comboBox_2->clear();
     ui->comboBox_3->clear();
     ui->comboBox_4->clear();
     ui->comboBox_5->clear();

     emit SendStateButton(false);
}

void MainWindow::on_checkBox_clicked()
{
    m_bSaveData = ui->checkBox->isChecked();
    m_pSettingStrorage->StoreSaveDataBox(m_bSaveData);
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


    int iColumns = ui->tableWidget->columnCount();
    int iRows = ui->tableWidget->rowCount();
    QString myList[iRows][iColumns];
    QString allDataItems;

    for(int i = 0; i < iRows; ++i)
    {
        for(int j = 0; j < iColumns; ++j)
        {
            if(ui->tableWidget->item(i, j))
            {
                myList[i][j] = ui->tableWidget->item(i, j)->text();
                allDataItems.append(myList[i][j] + " ");
            }
        }
    }
    m_pSettingStrorage->StoreRowItem(allDataItems);

    m_pSettingStrorage->StoreMultiplierSignalFirst(coefInput[0]);
    m_pSettingStrorage->StoreMultiplierSignalSecond(coefInput[1]);
    m_pSettingStrorage->StoreMultiplierSignalThird(coefInput[2]);
    m_pSettingStrorage->StoreMultiplierSignalFourth(coefInput[3]);


    QWidget::closeEvent(event);
}
