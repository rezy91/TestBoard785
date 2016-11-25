#include "settingstorage.h"

SettingStorage::SettingStorage(QObject *parent) : QObject(parent)
{

}

void SettingStorage::StorePortName(const QString &strPortName)
{
    StoreValue(QString("portname"), strPortName);
}

QString SettingStorage::LoadPortName() const
{
    return LoadValue(QString("portname")).toString();
}

void SettingStorage::StoreValue(const QString &strKey, const QVariant &vValue)
{
    m_pAppSettings->setValue(strKey, vValue);
}

QVariant SettingStorage::LoadValue(const QString& strKey) const
{
    return m_pAppSettings->value(strKey);
}
