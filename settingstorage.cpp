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
  \file   settingstorage.cpp
  \author Pavel Hübner
  \date   2. 12. 2016
  \brief  Třída ukládající nastavení aplikace do registrů
*/

#include "settingstorage.h"

SettingStorage::SettingStorage(QObject *parent) : QObject(parent)
{

}

void SettingStorage::StoreGeometry(const QByteArray &arrGeometry)
{
    StoreValue("geometry", arrGeometry);
}

QByteArray SettingStorage::RestoreGeometry() const
{
    return RestoreValue("geometry").toByteArray();
}

void SettingStorage::StoreValue(const QString &strKey, const QVariant &vValue)
{
    m_pAppSettings->setValue(strKey, vValue);
}

QVariant SettingStorage::RestoreValue(const QString& strKey, const QVariant& vDefaultValue) const
{
    return m_pAppSettings->value(strKey, vDefaultValue);
}
