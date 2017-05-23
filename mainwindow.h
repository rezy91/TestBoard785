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
#include <QTimer>

#include "common.h"

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
    void on_clearButton_clicked();
    void on_connectButton_clicked();
    void on_disconnectButton_clicked();
    void on_openlogButton_clicked();

    void on_textBrowser_anchorClicked(const QUrl &arg1);

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

    const QString allSignalsNameGener[NMB_ITEMS_TIMERS_GENER] = {"NOT_USED_CURRENTLY", "ADC3_RECEIVE_AVERAGE_DATA", "IMPEDANCE_RECEIVE_DATA", "ADC2_RECEIVE_AVERAGE_DATA", "NOT_USED_CURRENTLY", "ADC1_RECEIVE_AVERAGE_DATA"};
    const QString allSignalsNameAmplf[NMB_ITEMS_TIMERS_AMPLF] = {"TIMERS_RESULTS", "ADC3_RECEIVE_AVERAGE_DATA", "NOT_USED_CURRENTLY", "ADC1_RECEIVE_AVERAGE_DATA"};
    const QString allSignalsBaseOnlyGener[NMB_ITEMS_TIMERS_GENER] = {"g3c", "g3s", "g2c", "g2s", "g1c", "g1s"};
    const QString allSignalsBaseOnlyAmplf[NMB_ITEMS_TIMERS_AMPLF] = {"at", "a3s", "ax", "a1s"};
    const QStringList allAdxSignalsGener[NMB_ITEMS_TIMERS_GENER] = { \
        {"g3c_counter", "g3c_exec", "g3c_conv"}, \
        {"g3s_counter", "Cooling_ADC1", "Cooling_ADC2", "Cooling_ADC3", "Cooling_ADC4"}, \
        {"g2c_counter", "imp_avg_mag", "imp_avg_phs", "imp_max_mag", "imp_max_phs", "power_average", "power_current", "power_regulator", "regulator_output_prc", "regulator_error_sum", "g2c_exec", "g2c_conv"}, \
        {"g2s_counter", "Vrf_adc", "I4_adc", "Vforward_adc", "I2_adc", "Vreverse_adc", "I3_adc", "I1_adc"}, \
        {"g1c_counter", "g1c_exec", "g1c_conv"}, \
        {"g1s_counter", "Apl1_Vcc_Adc", "CQM1_adc", "CQM2_adc", "V reference [mV]", "Temperature1", "+24V_adc", "+5V_adc", "Temperature MCU [°C]", "vrefin_adc"}};
    const QStringList allAdxSignalsAmplf[NMB_ITEMS_TIMERS_AMPLF] = { \
        {"at_counter", "Frequency", "/Frequency", "duty_factor_1", "duty_factor_2", "dead_time_1->2", "dead_time_2->1"}, \
        {"a3s_counter", "Temperature_1", "Temperature_2", "Temperature_3", "+24V_adc", "+10V_adc", "Vcheck", "ADC3_14", "ADC3_15"}, \
        {"ax_counter", "ax_exec", "ax_conv"}, \
        {"a1s_counter", "Vfet1Out", "Vfet2Out", "Ifet1Out", "Ifet2Out", "Vgate1Out", "Vgate2Out", "inputVolatge_ADC", "inputCurrent_ADC"}};
    const QString coeffsOthersNameMultiple[NMB_COEFFICIENTS_OTHERS] = {"for power", "NOT_USED", "NOT_USED", "NOT_USED", "NOT_USED", "NOT_USED", "NOT_USED", "NOT_USED", "NOT_USED"};
    const QString coeffsOthersNameAdditive[NMB_COEFFICIENTS_OTHERS] = {"for power", "for phase (0 - pi/2) [rad]", "max refl ratio (0.0 - 1.0)", "max PWM to amp (0.1 - 0.9)", "max diff power regulator (0 - 100) [%]", "NOT_USED", "NOT_USED", "NOT_USED", "NOT_USED"};

    Ui::MainWindow *ui;
    QSharedPointer<CommProtV200> m_CommProt;
    quint32 m_nDeviceAddress = constGenerID;
    QSettings* m_pAppSettings = new QSettings(QCoreApplication::organizationName(), QCoreApplication::applicationName(), this);
    QTimer TmrMstr;


    PERIODIC_REQUEST eRequestsAmplifAdcx[NMB_ITEMS_TIMERS_AMPLF];
    PERIODIC_REQUEST eRequestsGenerAdcx[NMB_ITEMS_TIMERS_GENER];
    PERIODIC_REQUEST eRequestGenerInput;
    PERIODIC_REQUEST eRequestAmplfInput;

    bool flagIfSourceIsLoggedGener[NMB_ITEMS_TIMERS_GENER];
    bool flagIfSourceIsLoggedAmplf[NMB_ITEMS_TIMERS_AMPLF];

    bool m_bSaveData = true;
    int sourceDataStream = NO_STREAM;

    QFile m_oFile;
    QString logPath;

    //widgetConfig *p_WidgetConfig = new widgetConfig(this);
    widgetReading *p_WidgetReading = new widgetReading(this);
    widgetSmith *p_WidgetSmith = new widgetSmith(this);
    widgetGraph *p_WidgetGraph = new widgetGraph(this);

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
    COMPLEX_NUMBER_GONIO CalculateReflectionRatio(COMPLEX_NUMBER_GONIO current, COMPLEX_NUMBER_GONIO average);
    void CheckedIfIndexInQlist(int NumberComboBox, int indexInComboBox);
    void recognizeIfDisplayNewDataAllSignals(QTime timestamp, QStringList *listOfNumbers, int adx);
    void DisplayNewDataFromSignal(QTime timestamp, QStringList *listOfNumbers, int indexInSignal);
    QString myTimeStamp(QTime time);
    QStringList adjustRowDataIntoOnlyNumber(QString rowData);
    void prepareComboBoxesWithSignals();
    void adjustCoefficientSingleStep(QDoubleSpinBox* p_oubleSpinBox, double newValue);
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
    void SendNewImpedanceData(qreal magnitudeCurr2Avg, qreal phaseCurr2Avg, qreal magnitudeCurr250, qreal phaseCurr250, qreal magnitudeAvg250, qreal phaseAvg250);
    void SendUpdateGraph(QTime timestamp, double receivedValue, int recordState, QString nameSignals, int src, int srStr, int flgs);
    void SendHighLevel(double coef, int src);
    void SendLowLevel(double coef, int src);

protected:
    bool eventFilter(QObject *object, QEvent *event);
};

#endif // MAINWINDOW_H
