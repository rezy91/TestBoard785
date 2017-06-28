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

#include "common.h"

class widgetReading : public QWidget
{
    Q_OBJECT
public:
    explicit widgetReading(QWidget *parent = 0);
    void SetQSize(QSize value) {currSize = value;}

signals:
    void SendV200Requirement(Qt::CheckState newState, int device, int indexMsg);
    void SendNewTime(int timeValue, int device, int indexMsg);

public slots:
    void showTextLog(QString showText);
    void ReadTimeRequests(bool device, int index, int value);
    void ReceiveStatusReg(STATUS_REGISTER eStatusReg);

private:
    enum{E_NMB_ITEMS_STATUS = 5};

    const QString allNamesAmp[NMB_ITEMS_TIMERS_AMPLF] = {"AMP ADC3 adjusted [ms]", "AMP ADC3 raw [ms]", "AMP ADC1 adjusted [ms]", "AMP ADC1 raw [ms]"};
    const QString allNamesGen[NMB_ITEMS_TIMERS_GENER] = {"GEN Cooling [ms]", "GEN ADC3 raw [ms]", "GEN Impedance [ms]", "GEN ADC2 raw [ms]", \
                                                         "GEN ADC1 adjusted [ms]", "GEN ADC1 raw [ms]"};
    const QString allNamesBitStatus[E_NMB_BIT_FLAGS_STATUS] = {"ChangeAcc0", "ChangeAcc1", "ChangeAcc2", "ChangeAcc3", "Empty0", "SelfTestDone", "Restart", "Error", \
                                                               "ButtonPressed", "KeyChanged", "TherapyRunning", "ParChangedByMas", "arChangedByApp", "SyncWaiting", \
                                                               "EmergencyPressed", "LogRequest", "ChangeSmartDevice0", "StateAcc0", "StateAcc1", "StateAcc2", "StateAcc3", \
                                                               "StateSmartDevice0", "BadContactPatient", "ContactNeutral", "ChoosedChannel_L", "ChoosedChannel_H", "reserve4", \
                                                               "reserve3", "reserve2", "reserve1", "StateTherapy_L", "StateTherapy_H"};
    const QString allNamesItemsStatus[E_NMB_ITEMS_STATUS] = {"Reserve", "Measured power [W]", "Set power [W]", "Measured temperature of patient [°C]", "Set temperature of patient [°C]"};

    QSize currSize;

    QVBoxLayout* vBox = new QVBoxLayout(this);
    QTextBrowser* textBrowser = new QTextBrowser(this);
    QPushButton* buttClear = new QPushButton("clear log", this);
    QSpinBox* generTimes[NMB_ITEMS_TIMERS_GENER];
    QSpinBox* amplfTimes[NMB_ITEMS_TIMERS_AMPLF];

    QGridLayout* qGridLyout = new QGridLayout(this);
    QCheckBox* chBoxAmp[NMB_ITEMS_TIMERS_AMPLF];
    QCheckBox* chBoxGen[NMB_ITEMS_TIMERS_GENER];
    QCheckBox* chBoxBitStatus[E_NMB_BIT_FLAGS_STATUS];
    QLineEdit *lineInputItemsStatus[E_NMB_ITEMS_STATUS];

protected:
    virtual void paintEvent(QPaintEvent*e);
};

#endif // widgetReading_H
