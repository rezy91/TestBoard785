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
  \file   bytearrayparser.h
  \author Pavel Hübner
  \date   22. 2. 2016
  \brief  knihovna pro parsování QByteArray na 1 B, 2 B a 4 B
*/

#ifndef BYTEARRAYPARSER_H
#define BYTEARRAYPARSER_H

#include <QObject>

class ByteArrayParser
{
public:
    static quint8 GetOneByte(const QByteArray &arrData)
    {
        return static_cast<quint8>(arrData.at(0));
    }

    static quint16 GetTwoBytes(const QByteArray &arrData)
    {
        return (quint16) (static_cast<quint8>(arrData.at(0)) << 8 ) + (static_cast<quint8>(arrData.at(1)));
    }

    static quint32 GetFourBytes(const QByteArray &arrData)
    {
        quint32 nValue = 0;

        for(int i = 0; i < 4; i++)
        {
            nValue += (quint32)(static_cast<quint8>(arrData[i]) << ((3 - i) * 8));
        }

        return nValue;
    }

};

#endif // BYTEARRAYPARSER_H
