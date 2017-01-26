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

    void StoreGeometryMain(const QByteArray& arrGeometry);
    QByteArray RestoreGeometryMain() const;

    void StoreGeometrySmith(const QByteArray& arrGeometry);
    QByteArray RestoreGeometrySmith() const;

    void StoreGeometryGraph(const QByteArray& arrGeometry);
    QByteArray RestoreGeometryGraph() const;

    void StoreSaveDataBox(const bool& bSaveDataBox);
    bool RestoreSaveDataBox() const;

    void StoreRowItem(const QString& strValue);
    QString RestoreRowItem() const;

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
};

#endif // SETTINGS_H
