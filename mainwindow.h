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
#include "widgetadmin.h"
#include "widgettipmemory.h"


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
        AMPLIFIER_SOURCE,
        APL_LARGE_SOURCE,
        APL_SMALL_1_SOURCE,
        APL_SMALL_2_SOURCE,
        APL_SMALL_3_SOURCE,
        USN_GENER_SOURCE
    } SOURCE_DEVICE;

    typedef enum
    {
      E_READ_TYPE_ADC1_CONVERTED,
      E_READ_TYPE_ADC1_ADJUSTED,
      E_READ_TYPE_ADC2_CONVERTED,
      E_READ_TYPE_ADC2_ADJUSTED,
      E_READ_TYPE_ADC3_CONVERTED,
      E_READ_TYPE_ADC3_ADJUSTED,
      E_READ_TYPE_DIGITAL_INPUTS,
      E_READ_TYPE_RF_COUNT,
    } CONVERTED_CALCULATED_TYPES_RF;

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
    void specificMessageProtocol(QString message, bool bExpectedResponse);

private:
    const QColor c_BackgrColorGenerRf = QColor(0,128,255);
    const QColor c_BackgrColorAmplif = QColor(128,64,64);
    const QColor c_BackgrColorGreen[NMB_ITEMS_TIMERS_APLS_AND_USN] = {QColor(255,128,0), QColor(100,255,100), QColor(0,255,0), QColor(0,200,0), QColor(0,150,0)};
    const quint32 m_nDeviceAddress = 21;

    const QString allSignalsBaseOnlyGener[NMB_ITEMS_TIMERS_GENER] = {"g3c", "g3s", "g2c", "g2s", "g1c", "g1s"};
    const QString allSignalsBaseOnlyAmplf[NMB_ITEMS_TIMERS_AMPLF] = {"at", "a3s", "ax", "a1s"};
    const QString allSignalsBaseOnlyAplUsn[NMB_ITEMS_TIMERS_APLS_AND_USN] = {"u", "l", "s1", "s2", "s3"};
    const QString allNamedFiles[E_BROWSER_NMB] = {"SavedData", "LoggedData"};

    Ui::MainWindow *ui;
    QSharedPointer<CommProtV200> m_CommProt;
    QTimer TmrMstr;


    PERIODIC_REQUEST eRequestsAmplifAdcx[NMB_ITEMS_TIMERS_AMPLF];
    PERIODIC_REQUEST eRequestsGenerAdcx[E_READ_TYPE_RF_COUNT];
    PERIODIC_REQUEST eRequestsAplUsn[NMB_ITEMS_TIMERS_APLS_AND_USN];
    PERIODIC_REQUEST eRequestAmplfInput;

    bool flagIfSourceIsLoggedGener[NMB_ITEMS_TIMERS_GENER];
    bool flagIfSourceIsLoggedAmplf[NMB_ITEMS_TIMERS_AMPLF];
    bool flagIfSourceIsLoggedAplUsn[NMB_ITEMS_TIMERS_APLS_AND_USN];

    bool m_bSaveData = false;
    bool m_bGeneratorConnected = false;
    int sourceDataStream = NO_STREAM;

    QFile m_oFile[E_BROWSER_NMB];
    QString logPath;

    widgetReading *p_WidgetReading = new widgetReading(this);
    widgetConfig *p_WidgetConfig = new widgetConfig(this);
    widgetSettings *p_widgetSettings = new(widgetSettings);
    widgetSmith *p_WidgetSmith = new widgetSmith(this);
    widgetGraph *p_WidgetGraph = new widgetGraph(this);
    widgetTherapy *p_WidgetTherapy = new widgetTherapy(this);
    widgetAdmin *p_WidgetAdmin = new widgetAdmin(this);
    widgetTipMemory *p_WidgetTipMemory = new widgetTipMemory(this);
    settings* appSettings = new settings(this);

    void AppendText(QTime timestamp, QString strText, TEXT_BROWSERS eIndex);
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

    bool GetIndexFromQlistGenAmp(SOURCE_DEVICE eSourceStream, int &dwAbsIndex, int dwNumberCmbBx, int dwIndexCmbBx);
    bool GetIndexFromQlistAplUsn(int &dwAbsIndex, int dwNumberCmbBx, int dwIndexCmbBx);
    void ShowSignalsIntoComboBox(SOURCE_STREAM eSourceStream);
    void ShowSignalsGenAmplIfShould(SOURCE_STREAM eSourceStream, SOURCE_DEVICE eSourceDevice, qint32 &dwStartIndex, QColor eBackgrColor);
    void ShowSignalsAplUsnIfShould(SOURCE_STREAM eSourceStream, qint32 &dwStartIndex);
    void SetTimerRequestsAmpTemporary(int wIndex, bool bOnOff, QString sCommand);

    void SetTimerRequestsGenReadValues(bool bOnOff, CONVERTED_CALCULATED_TYPES_RF eType);

    void SetTimerRequestsAplUsn(bool bOnOff, QString sCommand, SOURCE_DEVICE eSourceStream);
    void SetTimerinputAmplf(bool bOnOff, QString sCommand);
    void HasTimerRequestsExpiredGenAmpl(SOURCE_DEVICE eSourceStream);
    void HasTimerRequestsExpiredAplUsn();
    void HasTimerInputExpired(void);

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
    void SendTextIntoLog(QString text, TEXT_BROWSERS eIndex);
    void SendSmithPoints(int value);
    void SendTimeRequests(int device, int index, int value);
    void SendAxisHigh(int index, double value);
    void SendAxisLow(int index, double value);
    void SendConfigGener(QByteArray values);
    void SendSettingsGener(QByteArray values);
    void SendAmpFreq(QString values);
    void SendAmpPwm(QString values);
    void SendStatusReg(STATUS_REGISTER eStatusReg);
    void SendFirmwareVersion(int nIndex, uint nValue);
    void SendAdmin(int index, QString values);
    void SendRcvMsgAmp(QString values);
    void SendRcvMsgGen(QString values);
    void SendRcvMsgAplUsn(QString values);
    void SendTipMemory(uint8_t byChannel, uint8_t byBytes, QByteArray byBuffer);

protected:
    void closeEvent(QCloseEvent *event) override;
    bool eventFilter(QObject *object, QEvent *event);
};

#endif // MAINWINDOW_H
