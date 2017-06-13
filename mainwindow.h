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

#include "common.h"
#include "settings.h"
#include "widgetconfig.h"
#include "widgetsettings.h"
#include "widgetreading.h"
#include "widgetgraph.h"
#include "widgetsmith.h"
#include "widgettherapy.h"



#define PID_TIMERS_ADCX_GENER              40
#define PID_TIMER_INPUT_GENER              (PID_TIMERS_ADCX_GENER + NMB_ITEMS_TIMERS_GENER)
#define PID_TIMERS_ADCX_AMPLF              30
#define PID_TIMER_INPUT_AMPLF              (PID_TIMERS_ADCX_AMPLF + NMB_ITEMS_TIMERS_AMPLF)

#define COLOR_BLUE_LIGHT        QColor(0,255,255)
#define COLOR_BLUE_DARK         QColor(0,128,255)
#define COLOR_BROWN_LIGHT       QColor(200,149,121)
#define COLOR_BROWN_DARK        QColor(124,78,52)

#define COLOR_SAVED_GREEN       QColor(98,251,89)
#define COLOR_UNSAVED_GREY      QColor(207,207,207)

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    typedef struct
    {
        float real;
        float imag;
    } COMPLEX_NUMBER_ALGEB;

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
    void on_checkBox_clicked();

public slots:
    void openlogButtonPressed();
    void onNewMsgReqReceived(Qt::CheckState m_newState, int m_device, int m_indexMsg);
    void onNewTimeRequest(int valueTime, int m_device, int m_indexMsg);
    void specificMessageProtocol(QString message);

private:
    const quint32 constGenerID = 21;
    const quint32 constAmpID = 1;

    const QString allSignalsNameGener[NMB_ITEMS_TIMERS_GENER] = {"COOLING_RECEIVE_DATA", "ADC3_RECEIVE_AVERAGE_DATA", "IMPEDANCE_RECEIVE_DATA", "ADC2_RECEIVE_AVERAGE_DATA", "NOT_USED_CURRENTLY", "ADC1_RECEIVE_AVERAGE_DATA"};
    const QString allSignalsNameAmplf[NMB_ITEMS_TIMERS_AMPLF] = {"TIMERS_RESULTS", "ADC3_RECEIVE_AVERAGE_DATA", "NOT_USED_CURRENTLY", "ADC1_RECEIVE_AVERAGE_DATA"};
    const QString allSignalsBaseOnlyGener[NMB_ITEMS_TIMERS_GENER] = {"g3c", "g3s", "g2c", "g2s", "g1c", "g1s"};
    const QString allSignalsBaseOnlyAmplf[NMB_ITEMS_TIMERS_AMPLF] = {"at", "a3s", "ax", "a1s"};

    Ui::MainWindow *ui;
    QSharedPointer<CommProtV200> m_CommProt;
    quint32 m_nDeviceAddress = constGenerID;
    QTimer TmrMstr;


    PERIODIC_REQUEST eRequestsAmplifAdcx[NMB_ITEMS_TIMERS_AMPLF];
    PERIODIC_REQUEST eRequestsGenerAdcx[NMB_ITEMS_TIMERS_GENER];
    PERIODIC_REQUEST eRequestGenerInput;
    PERIODIC_REQUEST eRequestAmplfInput;

    bool flagIfSourceIsLoggedGener[NMB_ITEMS_TIMERS_GENER];
    bool flagIfSourceIsLoggedAmplf[NMB_ITEMS_TIMERS_AMPLF];

    bool m_bSaveData = false;
    int sourceDataStream = NO_STREAM;

    QFile m_oFile;
    QString logPath;

    widgetReading *p_WidgetReading = new widgetReading(this);
    widgetConfig *p_WidgetConfig = new widgetConfig(this);
    widgetSettings *p_widgetSettings = new(widgetSettings);
    widgetSmith *p_WidgetSmith = new widgetSmith(this);
    widgetGraph *p_WidgetGraph = new widgetGraph(this);
    widgetTherapy *p_WidgetTherapy = new widgetTherapy(this);
    settings* appSettings = new settings(this);

    void AppendText(QTime timestamp, QString strText);
    void FillTableContent();
    void FillCommandTableGenerator();
    void FillCommandTableAmplifier();
    void StoreValue(const QString& strKey, const QVariant& vValue);
    QVariant LoadValue(const QString& strKey);
    void SetAvaiblePorts();
    void SetLastPort();
    void restoreAllSettings(void);
    void newDataV200(QByteArray aData);
    void CheckedIfIndexInQlist(int NumberComboBox, int indexInComboBox);
    void recognizeIfDisplayNewDataAllSignals(QTime timestamp, QStringList *listOfNumbers, int adx);
    void DisplayNewDataFromSignal(QTime timestamp, QStringList *listOfNumbers, int indexInSignal);
    QString myTimeStamp(QTime time);
    QStringList adjustRowDataIntoOnlyNumber(QString rowData);
    void prepareComboBoxesWithSignals();
    void selectedDeviceSetAccordingSaved(quint32 value);

    bool GetIndexFromQlist(SOURCE_DEVICE eSourceStream, int &dwAbsIndex, int dwNumberCmbBx, int dwIndexCmbBx);
    void ShowSignalsIntoComboBox(SOURCE_STREAM eSourceStream);
    void ShowSignalsIfShould(SOURCE_STREAM eSourceStream, SOURCE_DEVICE eSourceDevice, qint32 &dwStartIndex, QColor eBackgrColor);
    void SetTimerRequests(int wIndex, bool bOnOff, QString sCommand, SOURCE_DEVICE eSourceStream);
    void SetTimerinput(bool bOnOff, QString sCommand, SOURCE_DEVICE eSourceStream);
    void HasTimerRequestsExpired(SOURCE_DEVICE eSourceStream);
    void HasTimerInputExpired(SOURCE_DEVICE eSourceStream);

    void refreshPlot(void);
    void universalRequestMessageProtocol(Qt::CheckState eState, int wIndex);


    double recvItems[nmbCurvesInGraph];
    int recStat[nmbCurvesInGraph];
    int sourceSignal[nmbCurvesInGraph];
    int sourceAd[nmbCurvesInGraph];
    QString sourceSignText[nmbCurvesInGraph];

    QTime timeCurrent;

signals:
    void SendNewImpedanceData(qreal magnitudeCurr2Avg, qreal phaseCurr2Avg, qreal magnitudeCurr250, qreal phaseCurr250);
    void SendUpdateGraph(QTime timestamp, double receivedValue, int recordState, QString nameSignals, int src, int srStr, int flgs);
    void SendTextIntoLog(QString text);
    void SendSmithPoints(int value);
    void SendTimeRequests(bool device, int index, int value);
    void SendAxisHigh(int index, double value);
    void SendAxisLow(int index, double value);
    void SendAdcData(QString type, QString device, int index, QString values);
    void SendRegulator(QString values);
    void SendTestTherapy(QString values);
    void SendTestCqmFreq(QString values);
    void SendAmpFreq(QString values);
    void SendAmpPwm(QString values);
    void SendGenPwm(QString values);
    void SendGenPwr(QString values);
    void SendStatusReg(QByteArray value);

protected:
    void closeEvent(QCloseEvent *event) override;
    bool eventFilter(QObject *object, QEvent *event);
};

#endif // MAINWINDOW_H
