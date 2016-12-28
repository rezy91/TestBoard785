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
#include "settingstorage.h"
#include "settingstoragetestboard785.h"

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
        Hex
    };
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_sendButton_clicked();
    void on_clearButton_clicked();
    void on_connectButton_clicked();

    void on_disconnectButton_clicked();

private:
    Ui::MainWindow *ui;
    QSharedPointer<CommProtV200> m_CommProt;
    const quint32 m_nDeviceAddress = 20;

    QTimer m_oPortsRefresh;
    SettingStorageTestBoard785* m_pSettingStrorage = new SettingStorageTestBoard785(this);

//    void EventsList(QByteArray arrData);
//    void StatusRegister(QByteArray arrData);

    void AppendText(QString strText);

    quint8 GetOneByte(QByteArray arrData);
    quint16 GetTwoBytes(QByteArray arrData);
    quint32 GetFourBytes(QByteArray arrData);

    void FillCommandTable();

    void SetAvaiblePorts();

    void FillTable_SetFrequencyPacket();
    void FillTable_SetPwmPacket();
    void FillTable_GetFrequencyPacket();
    void FillTable_GetPulsesWidthsPackets();

protected:
    // QWidget interface
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
