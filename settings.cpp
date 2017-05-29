#include "settings.h"

settings::settings(QObject *parent) : QObject(parent)
{

}

void settings::StoreValue(const QString &strKey, const QVariant &vValue)
{
    m_pAppSettings->setValue(strKey, vValue);
}

QVariant settings::RestoreValue(const QString &strKey, const QVariant &vDefaultValue) const
{
    return m_pAppSettings->value(strKey, vDefaultValue);
}

void settings::StoreGeometryMain(const QByteArray &arrGeometry)
{
    StoreValue("GeometryMain", arrGeometry);
}

QByteArray settings::RestoreGeometryMain() const
{
    return RestoreValue("GeometryMain").toByteArray();
}

void settings::StoreSaveDataBox(const bool &bSaveDataBox)
{
    StoreValue("savedatabox", bSaveDataBox);
}

bool settings::RestoreSaveDataBox() const
{
    return RestoreValue("savedatabox").toBool();
}

void settings::StoreSelectedDevice(const quint32 &nDevice)
{
    StoreValue(QString("selectedDevice"), nDevice);
}

quint32 settings::RestoreSelectedDevice() const
{
    return RestoreValue(QString("selectedDevice")).toUInt();
}

void settings::StoreSmithPoints(const quint32 &nPoints)
{
    StoreValue(QString("smithPoints"), nPoints);
}

quint32 settings::RestoreSmithPoints() const
{
    return RestoreValue(QString("smithPoints")).toUInt();
}

void settings::StoreRefreshGener(const int nIndexTimer, const int nPeriod_ms)
{
    StoreValue(QString("timerGener%1").arg(nIndexTimer), nPeriod_ms);
}

int settings::RestoreRefreshGener(const int nIndexTimer) const
{
    return RestoreValue(QString("timerGener%1").arg(nIndexTimer)).toUInt();
}

void settings::StoreRefreshAmplif(const int nIndexTimer, const int nPeriod_ms)
{
    StoreValue(QString("timerAmp%1").arg(nIndexTimer), nPeriod_ms);
}

int settings::RestoreRefreshAmplif(const int nIndexTimer) const
{
    return RestoreValue(QString("timerAmp%1").arg(nIndexTimer)).toUInt();
}

void settings::StoreHighValueSignal(const int nIndexTimer, const double value)
{
    StoreValue(QString("HighLevel1%1").arg(nIndexTimer), value);
}

double settings::RestoreHighValueSignal(const int nIndexTimer) const
{
    return RestoreValue(QString("HighLevel1%1").arg(nIndexTimer)).toDouble();
}

void settings::StoreLowValueSignal(const int nIndexTimer, const double value)
{
    StoreValue(QString("LowLevel1%1").arg(nIndexTimer), value);
}

double settings::RestoreLowValueSignal(const int nIndexTimer) const
{
    return RestoreValue(QString("LowLevel1%1").arg(nIndexTimer)).toDouble();
}

void settings::StorePortName(const QString &strPortName)
{
    StoreValue(QString("portname"), strPortName);
}

QString settings::RestorePortName() const
{
    return RestoreValue(QString("portname")).toString();
}
