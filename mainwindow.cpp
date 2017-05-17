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


    ui->horizontalLayout_1->layout()->addWidget(p_WidgetConfig);
    ui->horizontalLayout_2->layout()->addWidget(p_WidgetReading);
    ui->verticalLayout_1->layout()->addWidget(p_WidgetGraph);
    ui->verticalLayout_1->layout()->addWidget(p_WidgetSmith);


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
    Q_UNUSED(index);

    refreshPlot();
}

void MainWindow::newDataV200(QByteArray aData)
{
    Q_UNUSED(aData);
    //actualize time
    timeCurrent.elapsed();
    timeCurrent.start();
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

