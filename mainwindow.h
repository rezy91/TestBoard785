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
  \file   mainwindow.h
  \author Pavel Hübner
  \date   22. 2. 2016
  \brief  GUI zasílající a přijímající zprávy generu
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort>
#include "../../../Applications/088/CommProtV200/commprotv200_global.h"
#include <QSettings>
#include <QDesktopServices>

#include <QtGui>
#include <QtCore>
#include <QPushButton>


#include "widgetconfig.h"
#include "widgetreading.h"
#include "widgetgraph.h"
#include "widgetsmith.h"


#define NMB_ITEMS_TIMERS_GENER              6
#define NMB_ITEMS_TIMERS_AMPLF              4
#define NMB_COEFFICIENTS_OTHERS             9

#define PID_TIMERS_ADCX_GENER              40
#define PID_TIMER_INPUT_GENER              (PID_TIMERS_ADCX_GENER + NMB_ITEMS_TIMERS_GENER)
#define PID_TIMERS_ADCX_AMPLF              30
#define PID_TIMER_INPUT_AMPLF              (PID_TIMERS_ADCX_AMPLF + NMB_ITEMS_TIMERS_AMPLF)

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    typedef struct
    {
        float magnitude;
        float phase_rad;
    } COMPLEX_NUMBER_GONIO;

    typedef struct
    {
        quint32 RequirementTime_ms;
        quint32 CurrentTime_ms;
        bool bEnable;
    } APP_TIMER;

    typedef struct
    {
        APP_TIMER timer;
        QByteArray assemblyMsq;
        bool respExp;
        bool isInProgress;
        int internalIdClass;

    } PERIODIC_REQUEST;

    typedef enum
    {
        GENERATOR_SOURCE,
        AMPLIFIER_SOURCE
    } SOURCE_DEVICE;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void changed_table(int index);
    void on_connectButton_clicked();
    void on_disconnectButton_clicked();
    void on_clearButton_clicked();

    void on_checkBox_clicked();
    void on_checkBox_2_clicked();
    void on_checkBox_3_clicked();
    void on_checkBox_4_clicked();
    void on_checkBox_5_clicked();
    void on_checkBox_6_clicked();
    void on_checkBox_7_clicked();
    void on_checkBox_8_clicked();
    void on_checkBox_9_clicked();
    void on_checkBox_10_clicked();
    void on_checkBox_11_clicked();
    void on_checkBox_12_clicked();
    void on_checkBox_13_clicked();


private:
    const quint32 constGenerID = 18;
    const quint32 constAmpID = 1;

    Ui::MainWindow *ui;
    QSharedPointer<CommProtV200> m_CommProt;
    quint32 m_nDeviceAddress;
    QTimer TmrMstr;
    QTime timeCurrent;


    //widgetConfig *p_WidgetConfig = new widgetConfig(this);
    widgetReading *p_WidgetReading = new widgetReading(this);
    widgetGraph *p_WidgetSmith = new widgetGraph(this);
    widgetSmith *p_WidgetGraph = new widgetSmith(this);


    PERIODIC_REQUEST eRequestsAmplifAdcx[NMB_ITEMS_TIMERS_AMPLF];
    PERIODIC_REQUEST eRequestsGenerAdcx[NMB_ITEMS_TIMERS_GENER];
    PERIODIC_REQUEST eRequestGenerInput;
    PERIODIC_REQUEST eRequestAmplfInput;

    bool flagIfSourceIsLoggedGener[NMB_ITEMS_TIMERS_GENER];
    bool flagIfSourceIsLoggedAmplf[NMB_ITEMS_TIMERS_AMPLF];


    void newDataV200(QByteArray aData);
    void refreshPlot(void);
    void AppendText(QTime timestamp, QString strText);
    QString myTimeStamp(QTime time);

    void SetAvaiblePorts();
    void selectedDeviceSetAccordingSaved(quint32 value);
    void universalRequestMessageProtocol(Qt::CheckState eState, int wIndex);

    void SetTimerRequests(int wIndex, bool bOnOff, QString sCommand, SOURCE_DEVICE eSourceStream);
    void SetTimerinput(bool bOnOff, QString sCommand, SOURCE_DEVICE eSourceStream);
    void HasTimerRequestsExpired(SOURCE_DEVICE eSourceStream);
    void HasTimerInputExpired(SOURCE_DEVICE eSourceStream);

protected:
    bool eventFilter(QObject *object, QEvent *event);
};

#endif // MAINWINDOW_H
