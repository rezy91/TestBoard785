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
#include "widgetone.h"
#include "widgettwo.h"
#include "widgetthree.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    static int mMainTimer = 0;

    ui->setupUi(this);

    ui->statusBar->showMessage("Start app");

    qDebug() << "Start of application.";


    widgetOne *p_WidgetConfig = new widgetOne(this);
    widgetTwo *p_WidgetReading = new widgetTwo(this);
    widgetThree *p_WidgetSettings = new widgetThree(this);


    //ui->horizontalLayout_3->setGeometry(QRect(0,0,500,400));
    //p_WidgetSettings->setGeometry(QRect(0,0,500,400));


    ui->horizontalLayout_1->layout()->addWidget(p_WidgetConfig);
    ui->horizontalLayout_2->layout()->addWidget(p_WidgetReading);
    ui->horizontalLayout_3->layout()->addWidget(p_WidgetSettings);



    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::changed_table);

    connect(&TmrMstr,&QTimer::timeout,[this](){

        if(++mMainTimer >= 500)//only for some tests
        {
            mMainTimer = 0;

        }
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
    qDebug() << "current index:" << index;
}

void MainWindow::newDataV200(QByteArray aData)
{
    Q_UNUSED(aData);
    //actualize time
    timeCurrent.elapsed();
    timeCurrent.start();
}

