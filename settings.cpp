#include "settings.h"


settings::settings(QObject *parent) : QObject(parent)
{

}

void settings::StoreChannelOffSet(const quint32 &nChannel, const qreal &fOffset)
{
    StoreValue(QString("offset%1").arg(nChannel), fOffset);
}

qreal settings::RestoreChannelOffset(const quint32 &nChannel) const
{
    return RestoreValue(QString("offset%1").arg(nChannel)).toReal();
}

void settings::StoreChannelMultiplier(const quint32 &nChannel, const qreal &fMultiplier)
{
    StoreValue(QString("multiplier%1").arg(nChannel), fMultiplier);
}

qreal settings::RestoreChannelMultiplier(const quint32 &nChannel) const
{
    return RestoreValue(QString("multiplier%1").arg(nChannel), 1).toReal();
}

void settings::StorePortName(const QString &strPortName)
{
    StoreValue(QString("portname"), strPortName);
}

QString settings::RestorePortName() const
{
    return RestoreValue(QString("portname")).toString();
}

void settings::StoreReferenceVoltage(const qreal &fVoltage_V)
{
    StoreValue(QString("Vref"), fVoltage_V);
}

qreal settings::RestoreReferenceVoltage() const
{
    return RestoreValue(QString("Vref")).toReal();
}

void settings::StoreRefreshPeriod(const quint32 &nPeriod_ms)
{
    StoreValue(QString("refreshperiod"), nPeriod_ms);
}

qreal settings::RestoreRefreshPeriod() const
{
    return RestoreValue(QString("refreshperiod")).toUInt();
}

void settings::StoreActiveTab(const quint32 &nTab)
{
    StoreValue(QString("activetab"), nTab);
}

qreal settings::RestoreActiveTab() const
{
    return RestoreValue(QString("activetab")).toUInt();
}

void settings::StoreGeometry(const QByteArray &arrGeometry)
{
    StoreValue("geometry", arrGeometry);
}

QByteArray settings::RestoreGeometry() const
{
    return RestoreValue("geometry").toByteArray();
}

void settings::StoreSaveDataBox(const bool &bSaveDataBox)
{
    StoreValue("savedatabox", bSaveDataBox);
}

bool settings::RestoreSaveDataBox() const
{
    return RestoreValue("savedatabox").toBool();
}

void settings::StoreValue(const QString &strKey, const QVariant &vValue)
{
    m_pAppSettings->setValue(strKey, vValue);
}

QVariant settings::RestoreValue(const QString& strKey, const QVariant& vDefaultValue) const
{
    return m_pAppSettings->value(strKey, vDefaultValue);
}
