#ifndef widgetReading_H
#define widgetReading_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QDebug>

#include <QTextBrowser>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QCheckBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QLabel>

#include "common.h"

class widgetReading : public QWidget
{
    Q_OBJECT
public:
    explicit widgetReading(QWidget *parent = 0);
    void SetQSize(QSize value) {currSize = value;}
    Qt::CheckState GetCheckStateAmplf(int nIndex);
    Qt::CheckState GetCheckStateGener(int nIndex);
    Qt::CheckState GetCheckStateAplUsn(int nIndex);
    bool GetEnabledAmplf(int nIndex);
    bool GetEnabledGener(int nIndex);
    bool GetEnabledAplUsn(int nIndex);
    void disableAll(void);

signals:
    void SendV200Requirement(Qt::CheckState newState, int device, int indexMsg);
    void SendNewTime(int timeValue, int device, int indexMsg);
    void SaveReadMsgsAmplf(QString data);
    void SaveReadMsgsGener(QString data);
    void SaveReadMsgsAplUsn(QString data);

public slots:
    void showTextLog(QString showText, TEXT_BROWSERS eIndexBrowser);
    void ReadTimeRequests(int device, int index, int value);
    void ReceiveStatusReg(STATUS_REGISTER eStatusReg);
    void ReceiveFirmwareVersion(int nIndex, uint nValue);
    void ReceiveRcvMsgAmp(QString data);
    void ReceiveRcvMsgGen(QString data);
    void ReceiveRcvMsgAplUsn(QString data);

private:
    enum{E_NMB_ITEMS_STATUS = 7};
    enum{E_NMB_SLAVE_DEVICES = 7};

    const QString allNamesAmp[NMB_ITEMS_TIMERS_AMPLF] = {"AMP ADC3 adjusted [ms]", "AMP ADC3 raw [ms]", "AMP ADC1 adjusted [ms]", "AMP ADC1 raw [ms]"};
    const QString allNamesGen[NMB_ITEMS_TIMERS_GENER] = {"GEN Cooling [ms]", "GEN ADC3 raw [ms]", "GEN Impedance [ms]", "GEN ADC2 raw [ms]", \
                                                         "GEN ADC1 adjusted [ms]", "GEN ADC1 raw [ms]"};
    const QString allNamesAplUsn[NMB_ITEMS_TIMERS_APLS_AND_USN] = {"USN [ms]", "APL_l [ms]", "APL_s1 [ms]", "APL_s2 [ms]", "APL_s3 [ms]"};
    const QString allNamesBitStatus[E_NMB_BIT_FLAGS_STATUS] = {"ChangeAcc0", "ChangeAcc1", "ChangeAcc2", "ChangeAcc3", "Empty0", "SelfTestDone", "Restart", "Error", \
                                                               "ButtonPressed", "KeyChanged", "TherapyRunning", "ParChangedByMas", "ParChangedByApp", "SyncWaiting", \
                                                               "EmergencyPressed", "LogRequest", "ChangeSmartDevice0", "StateAcc0", "StateAcc1", "StateAcc2", "StateAcc3", \
                                                               "StateSmartDevice0", "ContactPatient", "ContactNeutral", "ChoosedChannel_L", "ChoosedChannel_H", "CorrectChannel", \
                                                               "SynchSlavesDone", "ChangeSmartDevice1", "StateSmartDevice1", "StateTherapy_L", "StateTherapy_H"};
    const QString allNamesItemsStatus[E_NMB_ITEMS_STATUS] = {"Set duty factor RF [%]:", "Intensity USN [W/cm^2]:", "Measured power RF [W]:", "Set power RF [W]:", "Measured temperature of patient [°C]:", "Set temperature of patient [°C]:", "Extended bits [-]:"};
    const QString allNamesFirmwareVersion[E_NMB_SLAVE_DEVICES] = {"Apl_large version:", "Apl_small1 version:", "Apl_small2 version:", "Apl_small3 version:", "Amplifier version:", "RF Generator version:", "USN Generator version:"};

    QSize currSize;

    QVBoxLayout* vBox = new QVBoxLayout(this);
    QHBoxLayout* hBox = new QHBoxLayout(this);
    QTextBrowser* txtBrowsers[E_BROWSER_NMB];
    QPushButton* buttClear = new QPushButton("clear log", this);
    QSpinBox* generTimes[NMB_ITEMS_TIMERS_GENER];
    QSpinBox* amplfTimes[NMB_ITEMS_TIMERS_AMPLF];
    QSpinBox* aplUsnTimes[NMB_ITEMS_TIMERS_APLS_AND_USN];

    QGridLayout* qGridLyout = new QGridLayout(this);
    QCheckBox* chBoxAmp[NMB_ITEMS_TIMERS_AMPLF];
    QCheckBox* chBoxGen[NMB_ITEMS_TIMERS_GENER];
    QCheckBox* chBoxAplUsn[NMB_ITEMS_TIMERS_APLS_AND_USN];
    QCheckBox* chBoxBitStatus[E_NMB_BIT_FLAGS_STATUS];
    QLabel* lineInputItemsStatus[E_NMB_ITEMS_STATUS];
    QLabel* labelFirmwareVersion[E_NMB_SLAVE_DEVICES];

protected:
    virtual void paintEvent(QPaintEvent*e);
};

#endif // widgetReading_H
