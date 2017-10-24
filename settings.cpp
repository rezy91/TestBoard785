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

void settings::StoreRefreshAplUsn(const int nIndexTimer, const int nPeriod_ms)
{
    StoreValue(QString("timerAplUsn%1").arg(nIndexTimer), nPeriod_ms);
}

int settings::RestoreRefreshAplUsn(const int nIndexTimer) const
{
    return RestoreValue(QString("timerAplUsn%1").arg(nIndexTimer)).toUInt();
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

void settings::StoreAdmin(const int nIndex, const QString strValue)
{
    StoreValue(QString("admin%1").arg(nIndex), strValue);
}

QString settings::RestoreAdmin(const int nIndex) const
{
    return RestoreValue(QString("admin%1").arg(nIndex)).toString();
}

void settings::StoreRcvMsgAmp(const QString strValue)
{
    StoreValue(QString("rcvMsgAmp"), strValue);
}

QString settings::RestoreRcvMsgAmp() const
{
    return RestoreValue(QString("rcvMsgAmp")).toString();
}

void settings::StoreRcvMsgGen(const QString strValue)
{
    StoreValue(QString("rcvMsgGen"), strValue);
}

QString settings::RestoreRcvMsgGen() const
{
    return RestoreValue(QString("rcvMsgGen")).toString();
}

void settings::StoreRcvMsgAplUsn(const QString strValue)
{
    StoreValue(QString("rcvMsgAplUsn"), strValue);
}

QString settings::RestoreRcvMsgAplUsn() const
{
    return RestoreValue(QString("rcvMsgAplUsn")).toString();
}

void settings::StorePortName(const QString &strPortName)
{
    StoreValue(QString("portname"), strPortName);
}

QString settings::RestorePortName() const
{
    return RestoreValue(QString("portname")).toString();
}
