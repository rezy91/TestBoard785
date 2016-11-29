#include "settingstorage.h"

SettingStorage::SettingStorage(QObject *parent) : QObject(parent)
{

}

void SettingStorage::StorePortName(const QString &strPortName)
{
    StoreValue(QString("portname"), strPortName);
}

QString SettingStorage::RestorePortName() const
{
    return RestoreValue(QString("portname")).toString();
}

void SettingStorage::StoreValue(const QString &strKey, const QVariant &vValue)
{
    m_pAppSettings->setValue(strKey, vValue);
}

QVariant SettingStorage::RestoreValue(const QString& strKey) const
{
    return m_pAppSettings->value(strKey);
}

void SettingStorage::StoreGeometry(const QByteArray &arrGeometry)
{
    StoreValue("geometry", arrGeometry);
}

QByteArray SettingStorage::RestoreGeometry() const
{
    return RestoreValue("geometry").toByteArray();
}
