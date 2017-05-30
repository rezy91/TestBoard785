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

    void StoreSelectedDevice(const quint32& nDevice);
    quint32 RestoreSelectedDevice() const;

    void StoreSmithPoints(const quint32& nPoints);
    quint32 RestoreSmithPoints() const;

    void StoreRefreshGener(const int nIndexTimer, const int nPeriod_ms);
    int RestoreRefreshGener(const int nIndexTimer) const;

    void StoreRefreshAmplif(const int nIndexTimer, const int nPeriod_ms);
    int RestoreRefreshAmplif(const int nIndexTimer) const;

    void StoreHighValueSignal(const int nIndexTimer, const double value);
    double RestoreHighValueSignal(const int nIndexTimer) const;

    void StoreLowValueSignal(const int nIndexTimer, const double value);
    double RestoreLowValueSignal(const int nIndexTimer) const;

    void StoreAdcData(const QString type, const QString device, const int nIndexAdc, const QString strValue);
    QString RestoreAdcData(const QString type, const QString device, const int nIndexAdc) const;

    void StoreRegulator(const QString strValue);
    QString RestoreRegulator(void) const;

    void StoreTestTherapy(const QString strValue);
    QString RestoreTestTherapy(void) const;

    void StoreCqmFreq(const QString strValue);
    QString RestoreCqmFreq(void) const;

    void StoreAmpFreq(const QString strValue);
    QString RestoreAmpFreq(void) const;

    void StoreAmpPwm(const QString strValue);
    QString RestoreAmpPwm(void) const;

    void StoreGenPwm(const QString strValue);
    QString RestoreGenPwm(void) const;

    void StoreGenDac(const QString strValue);
    QString RestoreGenDac(void) const;

    void StoreGenPwr(const QString strValue);
    QString RestoreGenPwr(void) const;

private:
    QSettings* m_pAppSettings = new QSettings("settingsV1.ini", QSettings::IniFormat);

    void StoreValue(const QString& strKey, const QVariant& vValue);
    QVariant RestoreValue(const QString &strKey, const QVariant &vDefaultValue = QVariant()) const;

signals:

public slots:
};

#endif // SETTINGS_H
