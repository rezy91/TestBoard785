#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore>
#include <QObject>
#include <QSettings>
#include <QCoreApplication>

class settings : public QObject
{
    QSettings* m_pAppSettings = new QSettings("configs/setting.ini", QSettings::IniFormat);
    void StoreValue(const QString& strKey, const QVariant& vValue);
    QVariant RestoreValue(const QString &strKey, const QVariant &vDefaultValue = QVariant()) const;

public:
    settings(QObject *parent);

    void StoreChannelOffSet(const quint32& nChannel, const qreal& fOffset);
    qreal RestoreChannelOffset(const quint32& nChannel) const;

    void StoreChannelMultiplier(const quint32& nChannel, const qreal& fMultiplier);
    qreal RestoreChannelMultiplier(const quint32& nChannel) const;

    void StorePortName(const QString& strPortName);
    QString RestorePortName() const;

    void StoreReferenceVoltage(const qreal& fVoltage_V);
    qreal RestoreReferenceVoltage() const;

    void StoreRefreshPeriod(const quint32& nPeriod_ms);
    qreal RestoreRefreshPeriod() const;

    void StoreActiveTab(const quint32& nTab);
    qreal RestoreActiveTab() const;

    void StoreGeometry(const QByteArray& arrGeometry);
    QByteArray RestoreGeometry() const;

    void StoreSaveDataBox(const bool& bSaveDataBox);
    bool RestoreSaveDataBox() const;
};

#endif // SETTINGS_H
