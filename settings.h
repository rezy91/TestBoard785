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

    void StoreRefreshGener(const quint32& nIndexTimer, const quint32& nPeriod_ms);
    qreal RestoreRefreshGener(const quint32& nIndexTimer) const;

    void StoreRefreshAmplif(const quint32& nIndexTimer, const quint32& nPeriod_ms);
    qreal RestoreRefreshAmplif(const quint32& nIndexTimer) const;

    void StoreGeometryMain(const QByteArray& arrGeometry);
    QByteArray RestoreGeometryMain() const;

    void StoreGeometrySmith(const QByteArray& arrGeometry);
    QByteArray RestoreGeometrySmith() const;

    void StoreGeometryGraph(const QByteArray& arrGeometry);
    QByteArray RestoreGeometryGraph() const;

    void StoreSaveDataBox(const bool& bSaveDataBox);
    bool RestoreSaveDataBox() const;

    void StoreRowItemGener(const QString& strValue);
    QString RestoreRowItemGener() const;

    void StoreRowItemAmp(const QString& strValue);
    QString RestoreRowItemAmp() const;

    void StoreHighValueSignalFirst(const qreal &value);
    qreal RestoreHighValueSignalFirst() const;

    void StoreLowValueSignalFirst(const qreal &value);
    qreal RestoreLowValueSignalFirst() const;

    void StoreHighValueSignalSecond(const qreal &value);
    qreal RestoreHighValueSignalSecond() const;

    void StoreLowValueSignalSecond(const qreal &value);
    qreal RestoreLowValueSignalSecond() const;

    void StoreHighValueSignalThird(const qreal &value);
    qreal RestoreHighValueSignalThird() const;

    void StoreLowValueSignalThird(const qreal &value);
    qreal RestoreLowValueSignalThird() const;

    void StoreHighValueSignalFourth(const qreal &value);
    qreal RestoreHighValueSignalFourth() const;

    void StoreLowValueSignalFourth(const qreal &value);
    qreal RestoreLowValueSignalFourth() const;

    void StorePathLog(const QString& strPath);
    QString RestorePathLog() const;

    void StoreSmithPoints(const quint32& nPoints);
    quint32 RestoreSmithPoints() const;

    void StoreSelectedDevice(const quint32& nDevice);
    quint32 RestoreSelectedDevice() const;
};

#endif // SETTINGS_H
