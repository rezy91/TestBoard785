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
#include <QComboBox>
#include <QtSerialPort>
#include "../../../Applications/088/CommProtV200/commprotv200_global.h"
#include <QSettings>
#include <QTimer>
#include <QDesktopServices>
#include <qspinbox.h>
#include "settings.h"

#include "common.h"
#include "smithmain.h"
#include "grapmain.h"


#define NMB_ITEMS_FOR_TIMERS         6
#define NMB_COEFFICIENTS_OTHERS        9


#define COLOR_BLUE_LIGHT        QColor(0,255,255)
#define COLOR_BLUE_DARK         QColor(0,128,255)
#define COLOR_BROWN_LIGHT       QColor(200,149,121)
#define COLOR_BROWN_DARK        QColor(124,78,52)

namespace Ui {
class MainWindow;
}

class TableRoles
{
public:
    enum eTableRoles
    {
        NumeralSystem = Qt::UserRole + 1,
        ByteCount,
        DivisorPosition,
    };

    enum eNumeralSystem
    {
        Decimal,
        DecimalFloat,
        Float,
        Hex
    };
};

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


    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_sendButton_clicked();
    void on_clearButton_clicked();
    void on_connectButton_clicked();
    void on_disconnectButton_clicked();
    void on_checkBox_clicked();
    void on_openlogButton_clicked();

    void on_textBrowser_anchorClicked(const QUrl &arg1);

private:
    const quint32 constGenerID = 18;
    const quint32 constAmpID = 20;


    const int m_NumberOfFilledTablesGenerator = 2 + 2 + 1 + 2 + 6 + 3 + 4 * 2 + 7 * 2 + 9 * 2 + 9 * 2 + 8 + 6 + 3 + 2;
    const int m_NumberOfFilledTablesAmplifier = 1 + 1;

    const QString allSignalsBaseOnly[NMB_ITEMS_FOR_TIMERS] = {"ad3c", "ad3s", "ad2c", "ad2s", "ad1c", "ad1s"};
    const QStringList allAdxSignals[NMB_ITEMS_FOR_TIMERS] = { \
                                    {"ad3c_counter", "ad3c_exec", "ad3c_conv"}, \
                                    {"ad3s_counter", "Cooling_ADC1", "Cooling_ADC2", "Cooling_ADC3", "Cooling_ADC4"}, \
                                    {"ad2c_counter", "imp_avg_mag", "imp_avg_phs", "imp_max_mag", "imp_max_phs", "power_average", "power_current", "power_regulator", "ad2c_exec", "ad2c_conv"}, \
                                    {"ad2s_counter", "Vrf_adc", "I4_adc", "Vforward_adc", "I2_adc", "Vreverse_adc", "I3_adc", "I1_adc"}, \
                                    {"ad1c_counter", "ad1c_exec", "ad1c_conv"}, \
                                    {"ad1s_counter", "Apl1_Vcc_Adc", "CQM1_adc", "CQM2_adc", "Vref", "Temperature1", "+24V_adc", "+5V_adc", "temper_mcu_adc", "vrefin_adc"}};
    const QString coeffsOthersName[NMB_COEFFICIENTS_OTHERS] = {"for power", "NOT_USED", "NOT_USED", "NOT_USED", "NOT_USED", "NOT_USED", "NOT_USED", "NOT_USED", "NOT_USED"};

    const QString coeffsRegulatorName[3] = {"PROPORCIAL", "INTEGRAL", "DERIVATIVE"};


    Ui::MainWindow *ui;
    QSharedPointer<CommProtV200> m_CommProt;
    quint32 m_nDeviceAddress;
    QSettings* m_pAppSettings = new QSettings(QCoreApplication::organizationName(), QCoreApplication::applicationName(), this);
    QTimer TmrMstr;



    PERIODIC_REQUEST eRequestsGenerAdcx[NMB_ITEMS_FOR_TIMERS];
    PERIODIC_REQUEST eRequestGenerInput;

    bool flagIfSourceIsLogged[NMB_ITEMS_FOR_TIMERS];

    bool m_bSaveData = true;
    int sourceDataStream = NO_STREAM;

    QFile m_oFile;
    QString logPath;

    settings* m_pSettingStrorage = new settings(this);
    SmithMain* o_smith = new SmithMain(this);
    Grapmain* o_graph = new Grapmain(this);

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
    void getIndexInQList(int NumberComboBox, int indexInComboBox);
    void recognizeIfDisplayNewDataAllSignals(QTime timestamp, QStringList *listOfNumbers, int adx, int flg);
    void recognizeIfDisplayNewDataInSignal(QTime timestamp, QStringList *listOfNumbers, int indexInSignal, int flg);
    QString myTimeStamp(QTime time);
    QStringList adjustRowDataIntoOnlyNumber(QString rowData);
    void prepareComboBoxesWithSignals();
    void adjustCoefficientSingleStep(QDoubleSpinBox* p_oubleSpinBox, double newValue);

    double recvItems[nmbCurvesInGraph] = {0, 0, 0, 0};
    int recStat[nmbCurvesInGraph] = {0, 0, 0, 0};


    int sourceSignal[nmbCurvesInGraph] = {0, 0, 0, 0};
    int sourceAd[nmbCurvesInGraph] = {0, 0, 0, 0};
    QString sourceSignText[nmbCurvesInGraph] = {"\0", "\0", "\0", "\0"};


    QTime timeCurrent;

signals:
    void SendNewImpedanceData(qreal magnitudeCurr2Avg, qreal phaseCurr2Avg, qreal magnitudeCurr250, qreal phaseCurr250, qreal magnitudeAvg250, qreal phaseAvg250);
    void SendUpdateGraph(QTime timestamp, double receivedValue, int recordState, QString nameSignals, int src, int srStr, int flgs);
    void SendHighLevel(double coef, int src);
    void SendLowLevel(double coef, int src);

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
