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

void settings::StoreAdcData(const QString type, const QString device, const int nIndexAdc, const QString strValue)
{
    StoreValue(QString("adc%1%2%3").arg(type).arg(device).arg(nIndexAdc), strValue);
}

QString settings::RestoreAdcData(const QString type, const QString device, const int nIndexAdc) const
{
    return RestoreValue(QString("adc%1%2%3").arg(type).arg(device).arg(nIndexAdc)).toString();
}

void settings::StoreRegulator(const QString strValue)
{
    StoreValue(QString("regulator"), strValue);
}

QString settings::RestoreRegulator() const
{
    return RestoreValue(QString("regulator")).toString();
}

void settings::StoreTestTherapy(const QString strValue)
{
    StoreValue(QString("testTherapy"), strValue);
}

QString settings::RestoreTestTherapy() const
{
    return RestoreValue(QString("testTherapy")).toString();
}

void settings::StoreCqmFreq(const QString strValue)
{
    StoreValue(QString("cqmFreq"), strValue);
}

QString settings::RestoreCqmFreq() const
{
    return RestoreValue(QString("cqmFreq")).toString();
}

void settings::StoreAmpFreq(const QString strValue)
{
    StoreValue(QString("ampFreq"), strValue);
}

QString settings::RestoreAmpFreq() const
{
    return RestoreValue(QString("ampFreq")).toString();
}

void settings::StoreAmpPwm(const QString strValue)
{
    StoreValue(QString("ampPwm"), strValue);
}

QString settings::RestoreAmpPwm() const
{
    return RestoreValue(QString("ampPwm")).toString();
}

void settings::StoreGenPwm(const QString strValue)
{
    StoreValue(QString("genPwm"), strValue);
}

QString settings::RestoreGenPwm() const
{
    return RestoreValue(QString("genPwm")).toString();
}

void settings::StoreGenDac(const QString strValue)
{
    StoreValue(QString("genAdc"), strValue);
}

QString settings::RestoreGenDac() const
{
    return RestoreValue(QString("genAdc")).toString();
}

void settings::StoreGenPwr(const QString strValue)
{
    StoreValue(QString("genPwr"), strValue);
}

QString settings::RestoreGenPwr() const
{
    return RestoreValue(QString("genPwr")).toString();
}

void settings::StorePortName(const QString &strPortName)
{
    StoreValue(QString("portname"), strPortName);
}

QString settings::RestorePortName() const
{
    return RestoreValue(QString("portname")).toString();
}
