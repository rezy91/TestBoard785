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

void settings::StoreGeometryMain(const QByteArray &arrGeometry)
{
    StoreValue("GeometryMain", arrGeometry);
}

QByteArray settings::RestoreGeometryMain() const
{
    return RestoreValue("GeometryMain").toByteArray();
}

void settings::StoreGeometrySmith(const QByteArray &arrGeometry)
{
    StoreValue("GeometrySmith", arrGeometry);
}

QByteArray settings::RestoreGeometrySmith() const
{
    return RestoreValue("GeometrySmith").toByteArray();
}

void settings::StoreGeometryGraph(const QByteArray &arrGeometry)
{
    StoreValue("GeometryGraph", arrGeometry);
}

QByteArray settings::RestoreGeometryGraph() const
{
    return RestoreValue("GeometryGraph").toByteArray();
}

void settings::StoreSaveDataBox(const bool &bSaveDataBox)
{
    StoreValue("savedatabox", bSaveDataBox);
}

bool settings::RestoreSaveDataBox() const
{
    return RestoreValue("savedatabox").toBool();
}

void settings::StoreRowItemGener(const QString& strValue)
{
    StoreValue(QString("ItemsDataGener"), strValue);
}

QString settings::RestoreRowItemGener() const
{
    return RestoreValue(QString("ItemsDataGener")).toString();
}

void settings::StoreRowItemAmp(const QString &strValue)
{
    StoreValue(QString("ItemsDataAmp"), strValue);
}

QString settings::RestoreRowItemAmp() const
{
    return RestoreValue(QString("ItemsDataAmp")).toString();
}

void settings::StoreHighValueSignalFirst(const qreal &value)
{
    StoreValue(QString("HighLevel1"), value);
}

void settings::StoreLowValueSignalFirst(const qreal &value)
{
    StoreValue(QString("LowLevel1"), value);
}

qreal settings::RestoreHighValueSignalFirst() const
{
    return RestoreValue(QString("HighLevel1")).toDouble();
}

qreal settings::RestoreLowValueSignalFirst() const
{
    return RestoreValue(QString("LowLevel1")).toDouble();
}

void settings::StoreHighValueSignalSecond(const qreal &value)
{
    StoreValue(QString("HighLevel2"), value);
}

void settings::StoreLowValueSignalSecond(const qreal &value)
{
    StoreValue(QString("LowLevel2"), value);
}

qreal settings::RestoreHighValueSignalSecond() const
{
    return RestoreValue(QString("HighLevel2")).toDouble();
}

qreal settings::RestoreLowValueSignalSecond() const
{
    return RestoreValue(QString("LowLevel2")).toDouble();
}

void settings::StoreHighValueSignalThird(const qreal &value)
{
    StoreValue(QString("HighLevel3"), value);
}

void settings::StoreLowValueSignalThird(const qreal &value)
{
    StoreValue(QString("LowLevel3"), value);
}

qreal settings::RestoreHighValueSignalThird() const
{
    return RestoreValue(QString("HighLevel3")).toDouble();
}

qreal settings::RestoreLowValueSignalThird() const
{
    return RestoreValue(QString("LowLevel3")).toDouble();
}

void settings::StoreHighValueSignalFourth(const qreal &value)
{
    StoreValue(QString("HighLevel4"), value);
}

void settings::StoreLowValueSignalFourth(const qreal &value)
{
    StoreValue(QString("LowLevel4"), value);
}

qreal settings::RestoreHighValueSignalFourth() const
{
    return RestoreValue(QString("HighLevel4")).toDouble();
}

qreal settings::RestoreLowValueSignalFourth() const
{
    return RestoreValue(QString("LowLevel4")).toDouble();
}

void settings::StorePathLog(const QString &strPath)
{
    StoreValue(QString("pathlog"), strPath);
}

QString settings::RestorePathLog() const
{
    return RestoreValue(QString("pathlog")).toString();
}

void settings::StoreSmithPoints(const quint32 &nPoints)
{
    StoreValue(QString("smithPoints"), nPoints);
}

quint32 settings::RestoreSmithPoints() const
{
    return RestoreValue(QString("smithPoints")).toUInt();
}

void settings::StoreSelectedDevice(const quint32 &nDevice)
{
    StoreValue(QString("selectedDevice"), nDevice);
}

quint32 settings::RestoreSelectedDevice() const
{
    return RestoreValue(QString("selectedDevice")).toUInt();
}

void settings::StoreValue(const QString &strKey, const QVariant &vValue)
{
    m_pAppSettings->setValue(strKey, vValue);
}

QVariant settings::RestoreValue(const QString& strKey, const QVariant& vDefaultValue) const
{
    return m_pAppSettings->value(strKey, vDefaultValue);
}
