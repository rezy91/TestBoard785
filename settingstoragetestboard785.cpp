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
  \file   settingstoragetestboard785.cpp
  \author Pavel Hübner
  \date   16. 12. 2016
  \brief  class storing specific data for testboard785 to registers
*/

#include "settingstoragetestboard785.h"

SettingStorageTestBoard785::SettingStorageTestBoard785(QObject *parent) : SettingStorage(parent)
{

}

void SettingStorageTestBoard785::StorePortName(const QString &strPortName)
{
    StoreValue(QString("portname"), strPortName);
}

QString SettingStorageTestBoard785::RestorePortName() const
{
    return RestoreValue(QString("portname")).toString();
}
