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

    QFile m_oFile;

    settings* m_pSettingStrorage = new settings(this);
    SmithMain *s;
    Grapmain *g;


    //    void EventsList(QByteArray arrData);
//    void StatusRegister(QByteArray arrData);

    void AppendText(QString strText);

    quint8 GetOneByte(QByteArray arrData);
    quint16 GetTwoBytes(QByteArray arrData);
    quint32 GetFourBytes(QByteArray arrData);

    void FillCommandTable();

    void StoreValue(const QString& strKey, const QVariant& vValue);
    QVariant LoadValue(const QString& strKey);
    void SetAvaiblePorts();
    void SetLastPort();

    COMPLEX_NUMBER_GONIO CalculateReflectionRatio(COMPLEX_NUMBER_GONIO current, COMPLEX_NUMBER_GONIO average);

signals:
    void SendNewData(int magnitudeA, int phaseA, int magnitude50, int phase50);
    void SendUpdateGraph(int value, int val1);

    void SendStateButton(bool state);

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
