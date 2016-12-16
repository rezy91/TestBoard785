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
  \file   settingstoragetestboard785.h
  \author Pavel Hübner
  \date   16. 12. 2016
  \brief  class storing specific data for testboard785 to registers
*/

#ifndef SETTINGSTORAGETESTBOARD785_H
#define SETTINGSTORAGETESTBOARD785_H

#include <QObject>
#include <QSettings>

#include "settingstorage.h"

class SettingStorageTestBoard785 : public SettingStorage
{

public:
    SettingStorageTestBoard785(QObject *parent);

    void StorePortName(const QString& strPortName);
    QString RestorePortName() const;
};

#endif // SETTINGSTORAGETESTBOARD785_H
