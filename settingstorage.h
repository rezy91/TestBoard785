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
  \file   settingstorage.h
  \author Pavel Hübner
  \date   2. 12. 2016
  \brief  Třída ukládající nastavení aplikace do registrů
*/

#ifndef SETTINGSTORAGE_H
#define SETTINGSTORAGE_H

#include <QtCore>
#include <QObject>
#include <QSettings>
#include <QCoreApplication>

class SettingStorage : public QObject
{
protected:
    QSettings* m_pAppSettings = new QSettings(QCoreApplication::organizationName(), QCoreApplication::applicationName(), this);
    void StoreValue(const QString& strKey, const QVariant& vValue);
    QVariant RestoreValue(const QString &strKey, const QVariant &vDefaultValue = QVariant()) const;

public:
    SettingStorage(QObject *parent);

    void StoreGeometry(const QByteArray& arrGeometry);
    QByteArray RestoreGeometry() const;
};

#endif // SETTINGSTORAGE_H
