#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore>
#include <QObject>
#include <QSettings>
#include <QCoreApplication>

class settings : public QObject
{
    QSettings* m_pAppSettings = new QSettings("settings.ini", QSettings::IniFormat);
    void StoreValue(const QString& strKey, const QVariant& vValue);
    QVariant RestoreValue(const QString &strKey, const QVariant &vDefaultValue = QVariant()) const;

public:
    settings(QObject *parent);

    void StorePortName(const QString& strPortName);
    QString RestorePortName() const;

    void StoreRefreshFirst(const quint32& nPeriod_ms);
    qreal RestoreRefreshFirst() const;

    void StoreRefreshSecond(const quint32& nPeriod_ms);
    qreal RestoreRefreshSecond() const;

    void StoreRefreshThird(const quint32& nPeriod_ms);
    qreal RestoreRefreshThird() const;

    void StoreRefreshFourth(const quint32& nPeriod_ms);
    qreal RestoreRefreshFourth() const;

    void StoreRefreshFifth(const quint32& nPeriod_ms);
    qreal RestoreRefreshFifth() const;

    void StoreRefreshSixth(const quint32& nPeriod_ms);
    qreal RestoreRefreshSixth() const;

    void StoreGeometry(const QByteArray& arrGeometry);
    QByteArray RestoreGeometry() const;

    void StoreSaveDataBox(const bool& bSaveDataBox);
    bool RestoreSaveDataBox() const;

    void StoreRowItem(const QString& strValue);
    QString RestoreRowItem() const;

    void StoreMultiplierSignalFirst(const qreal &multiplier);
    qreal RestoreMultiplierSignalFirst() const;

    void StoreMultiplierSignalSecond(const qreal &multiplier);
    qreal RestoreMultiplierSignalSecond() const;

    void StoreMultiplierSignalThird(const qreal &multiplier);
    qreal RestoreMultiplierSignalThird() const;

    void StoreMultiplierSignalFourth(const qreal &multiplier);
    qreal RestoreMultiplierSignalFourth() const;
};

#endif // SETTINGS_H
