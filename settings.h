#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore>
#include <QObject>
#include <QWidget>

class settings : public QObject
{
public:
    explicit settings(QObject *parent = 0);

    void StorePortName(const QString& strPortName);
    QString RestorePortName() const;

    void StoreGeometryMain(const QByteArray& arrGeometry);
    QByteArray RestoreGeometryMain() const;

    void StoreSaveDataBox(const bool& bSaveDataBox);
    bool RestoreSaveDataBox() const;

    void StoreSmithPoints(const quint32& nPoints);
    quint32 RestoreSmithPoints() const;

    void StoreRefreshGener(const int nIndexTimer, const int nPeriod_ms);
    int RestoreRefreshGener(const int nIndexTimer) const;

    void StoreRefreshAmplif(const int nIndexTimer, const int nPeriod_ms);
    int RestoreRefreshAmplif(const int nIndexTimer) const;

    void StoreRefreshAplUsn(const int nIndexTimer, const int nPeriod_ms);
    int RestoreRefreshAplUsn(const int nIndexTimer) const;

    void StoreHighValueSignal(const int nIndexTimer, const double value);
    double RestoreHighValueSignal(const int nIndexTimer) const;

    void StoreLowValueSignal(const int nIndexTimer, const double value);
    double RestoreLowValueSignal(const int nIndexTimer) const;

    void StoreAdmin(const int nIndex, const QString strValue);
    QString RestoreAdmin(const int nIndex) const;

    void StoreRcvMsgAmp(const QString strValue);
    QString RestoreRcvMsgAmp(void) const;

    void StoreRcvMsgGen(const QString strValue);
    QString RestoreRcvMsgGen(void) const;

    void StoreRcvMsgAplUsn(const QString strValue);
    QString RestoreRcvMsgAplUsn(void) const;

private:
    QSettings* m_pAppSettings = new QSettings("settingsV1.ini", QSettings::IniFormat);

    void StoreValue(const QString& strKey, const QVariant& vValue);
    QVariant RestoreValue(const QString &strKey, const QVariant &vDefaultValue = QVariant()) const;

signals:

public slots:
};

#endif // SETTINGS_H
