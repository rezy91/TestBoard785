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
#include "bytearrayparser.h"
#include <QSettings>
#include <QTimer>
#include <QDesktopServices>
#include <qspinbox.h>
#include "settings.h"

#include "common.h"
#include "smithmain.h"
#include "grapmain.h"


#define NMB_ITEMS_FOR_TIMERS    6

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
    };

    enum eNumeralSystem
    {
        Decimal,
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
    Ui::MainWindow *ui;
    QSharedPointer<CommProtV200> m_CommProt;
    const quint32 m_nDeviceAddress = 20;
    QSettings* m_pAppSettings = new QSettings(QCoreApplication::organizationName(), QCoreApplication::applicationName(), this);
    QTimer TmrMstr;

    QByteArray assemblyMsq[NMB_ITEMS_FOR_TIMERS + 1];
    bool respExp[NMB_ITEMS_FOR_TIMERS + 1];

    quint32 RequirementTime_ms[NMB_ITEMS_FOR_TIMERS + 1];
    quint32 CurrentTime_ms[NMB_ITEMS_FOR_TIMERS + 1] = {0,0,0,0,0,0,0};
    bool timerEnable[NMB_ITEMS_FOR_TIMERS + 1] = {false,false,false,false,false,false,false};

    bool m_bSaveData = true;

    int sourceDataStream = NO_STREAM;

    QFile m_oFile;
    //QFile m_logFile;
    QString logPath;

    settings* m_pSettingStrorage = new settings(this);
    SmithMain* o_smith = new SmithMain(this);
    Grapmain* o_graph = new Grapmain(this);



    void AppendText(QTime timestamp, QString strText);

    quint8 GetOneByte(QByteArray arrData);
    quint16 GetTwoBytes(QByteArray arrData);
    quint32 GetFourBytes(QByteArray arrData);

    void FillCommandTable();

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
    void fillComboBoxesWithSignals(bool* flags);

    double coefInput[nmbCurvesInGraph];
    double recvItems[nmbCurvesInGraph] = {0, 0, 0, 0};
    int recStat[nmbCurvesInGraph] = {0, 0, 0, 0};

    QString allSignalsBaseOnly[6] = {"ad3c", "ad3s", "ad2c", "ad2s", "ad1c", "ad1s"};
    QStringList allAdxSignals[6] = {{"ad3c_1", "ad3c_2", "ad3c_3"}, \
                                    {"ad3s_1", "ad3s_2", "ad3s_3", "ad3s_4", "ad3s_5"}, \
                                    {"ad2c_1", "ad2c_2", "ad2c_3", "ad2c_4", "ad2c_5", "ad2c_6", "ad2c_7", "ad2c_8", "ad2c_9", "ad2c_10"}, \
                                    {"ad2s_1", "ad2s_2", "ad2s_3", "ad2s_4", "ad2s_5", "ad2s_6", "ad2s_7", "ad2s_8"}, \
                                    {"ad1c_1", "ad1c_2", "ad1c_3"}, \
                                    {"ad1s_1", "ad1s_2", "ad1s_3", "ad1s_4", "ad1s_5", "ad1s_6", "ad1s_7", "ad1s_8", "ad1s_9", "ad1s_10"}};
    bool flagIfSourceIsLogged[6];

    int sourceSignal[nmbCurvesInGraph] = {0, 0, 0, 0};
    int sourceAd[nmbCurvesInGraph] = {0, 0, 0, 0};
    QString sourceSignText[nmbCurvesInGraph] = {"\0", "\0", "\0", "\0"};


    QTime timeCurrent;

signals:
    void SendNewData(int magnitudeA, int phaseA, int magnitude50, int phase50);
    void SendUpdateGraph(QTime timestamp, double receivedValue, int recordState, QString nameSignals, int src, int srStr, int flgs);
    void SendCoefficientSignals(double coef, int src);

    void SendStateButton(bool state);

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
