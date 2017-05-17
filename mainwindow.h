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

private:
    const quint32 constGenerID = 18;
    const quint32 constAmpID = 1;

    Ui::MainWindow *ui;
    QSharedPointer<CommProtV200> m_CommProt;
    quint32 m_nDeviceAddress;
    QTimer TmrMstr;
    QTime timeCurrent;


    widgetConfig *p_WidgetConfig = new widgetConfig(this);
    widgetReading *p_WidgetReading = new widgetReading(this);
    widgetGraph *p_WidgetSmith = new widgetGraph(this);
    widgetSmith *p_WidgetGraph = new widgetSmith(this);


    void newDataV200(QByteArray aData);
    void refreshPlot(void);
    void AppendText(QTime timestamp, QString strText);
    QString myTimeStamp(QTime time);

    void SetAvaiblePorts();
    void selectedDeviceSetAccordingSaved(quint32 value);


protected:
    bool eventFilter(QObject *object, QEvent *event);
};

#endif // MAINWINDOW_H
