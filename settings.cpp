#include "settings.h"


settings::settings(QObject *parent) : QObject(parent)
{

}

void settings::StorePortName(const QString &strPortName)
{
    StoreValue(QString("portname"), strPortName);
}

QString settings::RestorePortName() const
{
    return RestoreValue(QString("portname")).toString();
}

void settings::StoreRefreshFirst(const quint32 &nPeriod_ms)
{
    StoreValue(QString("timer1"), nPeriod_ms);
}

qreal settings::RestoreRefreshFirst() const
{
    return RestoreValue(QString("timer1")).toUInt();
}

void settings::StoreRefreshSecond(const quint32 &nPeriod_ms)
{
    StoreValue(QString("timer2"), nPeriod_ms);
}

qreal settings::RestoreRefreshSecond() const
{
    return RestoreValue(QString("timer2")).toUInt();
}

void settings::StoreRefreshThird(const quint32 &nPeriod_ms)
{
    StoreValue(QString("timer3"), nPeriod_ms);
}

qreal settings::RestoreRefreshThird() const
{
    return RestoreValue(QString("timer3")).toUInt();
}

void settings::StoreRefreshFourth(const quint32 &nPeriod_ms)
{
    StoreValue(QString("timer4"), nPeriod_ms);
}

qreal settings::RestoreRefreshFourth() const
{
    return RestoreValue(QString("timer4")).toUInt();
}

void settings::StoreRefreshFifth(const quint32 &nPeriod_ms)
{
    StoreValue(QString("timer5"), nPeriod_ms);
}

qreal settings::RestoreRefreshFifth() const
{
    return RestoreValue(QString("timer5")).toUInt();
}

void settings::StoreRefreshSixth(const quint32 &nPeriod_ms)
{
    StoreValue(QString("timer6"), nPeriod_ms);
}

qreal settings::RestoreRefreshSixth() const
{
    return RestoreValue(QString("timer6")).toUInt();
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

void settings::StoreRowItem(const QString& strValue)
{
    StoreValue(QString("ItemsData"), strValue);
}

QString settings::RestoreRowItem() const
{
    return RestoreValue(QString("ItemsData")).toString();
}

void settings::StoreValue(const QString &strKey, const QVariant &vValue)
{
    m_pAppSettings->setValue(strKey, vValue);
}

QVariant settings::RestoreValue(const QString& strKey, const QVariant& vDefaultValue) const
{
    return m_pAppSettings->value(strKey, vDefaultValue);
}
