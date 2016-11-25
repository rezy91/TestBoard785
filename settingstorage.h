#ifndef SETTINGSTORAGE_H
#define SETTINGSTORAGE_H

#include <QtCore>
#include <QObject>
#include <QSettings>
#include <QCoreApplication>

class SettingStorage : public QObject
{
    QSettings* m_pAppSettings = new QSettings(QCoreApplication::organizationName(), QCoreApplication::applicationName(), this);
    void StoreValue(const QString& strKey, const QVariant& vValue);
    QVariant LoadValue(const QString &strKey) const;

public:
    SettingStorage(QObject *parent);

    void StorePortName(const QString& strPortName);
    QString LoadPortName() const;
};

#endif // SETTINGSTORAGE_H
