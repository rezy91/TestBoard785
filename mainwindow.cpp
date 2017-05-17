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

    ui->verticalLayout_1->layout()->addWidget(p_WidgetConfig);
    ui->verticalLayout_2->layout()->addWidget(p_WidgetReading);
    ui->verticalLayout_3->layout()->addWidget(p_WidgetGraph);
    ui->verticalLayout_3->layout()->addWidget(p_WidgetSmith);


    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::changed_table);

    connect(&TmrMstr,&QTimer::timeout,[this](){

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
        qDebug() << "log:" << strMessage;
    });

    connect(m_CommProt.data(), &CommProtV200::Error, [this](qint32 nError, QString strError, QVariant vErrorData, QTime oErrorTime) {
        Q_UNUSED(nError);
        Q_UNUSED(vErrorData);
        Q_UNUSED(oErrorTime);

        qDebug() << "Communication error:" << strError;
        ui->statusBar->showMessage("Communication error: " + strError);
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
}

void MainWindow::on_disconnectButton_clicked()
{
    ui->checkBox->setEnabled(true);
    ui->comboBox->setEnabled(true);
    ui->comboBox_1->setEnabled(true);

    m_CommProt.data()->SetTargetMedium("");
}

void MainWindow::on_clearButton_clicked()
{
    ui->textBrowser->clear();
}

void MainWindow::newDataV200(QByteArray aData)
{
    Q_UNUSED(aData);
    //actualize time
    timeCurrent.restart();

    ui->statusBar->showMessage("Data received: " + QString(aData.toHex()));

    AppendText(timeCurrent, QString(aData));
}

void MainWindow::refreshPlot()
{
    int currentTab = ui->tabWidget->currentIndex();
    QSize currentSize;

    if(currentTab == 0)
    {
        currentSize = ui->Configuration->size();

        p_WidgetConfig->setFixedSize(currentSize);
        p_WidgetConfig->SetQSize(currentSize);
        p_WidgetConfig->repaint();
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

        /*ui->spinBox->setEnabled(true);
        ui->spinBox_2->setEnabled(true);
        ui->spinBox_3->setEnabled(true);
        ui->spinBox_4->setEnabled(true);
        ui->spinBox_5->setEnabled(true);
        ui->spinBox_6->setEnabled(true);*/
    }
    else if(value == AMPLIFIER_SOURCE)
    {
        m_nDeviceAddress = constAmpID;

        /*ui->spinBox->setEnabled(false);
        ui->spinBox_2->setEnabled(false);
        ui->spinBox_3->setEnabled(false);
        ui->spinBox_4->setEnabled(false);
        ui->spinBox_5->setEnabled(false);
        ui->spinBox_6->setEnabled(false);*/
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

