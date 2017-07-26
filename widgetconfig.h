#ifndef widgetConfig_H
#define widgetConfig_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QStringList>

#include "common.h"

class widgetConfig : public QWidget
{
    Q_OBJECT
public:
    explicit widgetConfig(QWidget *parent = 0);

signals:

public slots:
    void ReadAdcData(QString type, QString device, int index, QString data);
    void ReadOthers(QString data);
    void ReadRegulatorPower(QString data);
    void ReadRegulatorCooling(QString data);
    void ReadTestTherapy(QString data);
    void ReadTestCqmFreq(QString data);

private:
    enum {E_NMB_GEN_ADC1 = 9};
    enum {E_NMB_GEN_ADC2 = 7};
    enum {E_NMB_GEN_ADC3 = 6};
    enum {E_NMB_GEN_OTHERS = 15};
    enum {E_NMB_GEN_REGULATOR_POWER = 4};
    enum {E_NMB_GEN_REGULATOR_COOLING = 7};
    enum {E_NMB_GEN_TESTTHERAPY = 2};
    enum {E_NMB_GEN_PWM_CQM = 2};

    enum {E_NMB_AMP_ADC1 = 8};
    enum {E_NMB_AMP_ADC3 = 8};

    const QString c_nameGenRegulatorPower[E_NMB_GEN_REGULATOR_POWER] = {"PROPORCIAL", "INTEGRAL", "DERIVATIVE", "Period (10 - 1000 ms)"};
    const QString c_nameGenRegulatorCooling[E_NMB_GEN_REGULATOR_COOLING] = {"PROPORCIAL", "INTEGRAL", "DERIVATIVE", "Period (10 - 10000 ms)", "min PWM [%]", "t_cold [°C]", "t_hot [°C]"};
    const QString c_nameGenTestTherapy[E_NMB_GEN_TESTTHERAPY] = {"Duty factor (20 - 100 %)", "Frequency (25 - 200 Hz)"};
    const QString c_nameGenCQM[E_NMB_GEN_PWM_CQM] = {"ch_1 (1000 - 100000 Hz)", "ch_2 (1000 - 100000 Hz)"};
    const QString c_nameOthers[E_NMB_GEN_OTHERS] = {"power max", "power min", "for phase (0 - pi/2) [rad]", "max refl ratio (0.0 - 1.0)", "max PWM to amp (0.1 - 0.9)", "max diff power regulator (0 - 100) [%]", "refer. imp. magnitude", "refer. imp. phase (0 - pi/2) [rad]", "max refl ratio (0.0 - 1.0)", "R_cqm max [ohm]", "R_cqm min [ohm]", "Cqm_r_ser [ohm]", "Cqm_r_par [ohm]", "Cqm_u [V]", "Cqm_r_off [ohm]"};


    const QStringList c_defaultValueGenAdcMul[E_GEN_ADC_NMB] = { \
        {"1.0", "1.0", "1.0", "1.0", "1.0", "1.0", "1.0", "1.0", "1.0"}, \
        {"52.2082", "1.0", "5.4", "1.0", "5.4359", "1.0", "0.4384"}, \
        {"1.0", "1.0", "1.0", "1.0", "1.0", "1.0"}};
    const QStringList c_defaultValueGenAdcAdd[E_GEN_ADC_NMB] = { \
        {"0.0", "0.0", "0.0", "0.0", "0.0", "0.0", "0.0", "0.0", "0.0"}, \
        {"2422", "0.0", "316", "0.0", "274.35", "0.0", "22.733"}, \
        {"0.0", "0.0", "0.0", "0.0", "0.0", "0.0"}};
    const QStringList c_defaultValueAmpAdcMul[E_AMP_ADC_NMB] = { \
        {"10", "191.27", "1.777", "1.777", "3.27", "3.27", "36.666", "3.05"}, \
        {"1.0", "1.0", "1.0", "1.0", "1.0", "1.0", "1.0", "1.0"}};
    const QStringList c_defaultValueAmpAdcAdd[E_AMP_ADC_NMB] = { \
        {"0.0", "15.34", "0.0", "0.0", "0.0", "0.0", "0.0", "0.0"}, \
        {"0.0", "188.18", "188.18", "0.0", "0.0", "0.0", "0.0", "0.0"}};
    const QString c_defaultOthers[E_NMB_GEN_OTHERS] = {"1.1154", "0.0", "-0.087", "0.3", "0.5", "2000", "100", "0.1", "0.2", "2", "-2", "565.4867", "471.6876", "4043.999", "27.85234"};
    const QString c_defaultRegulatorPower[E_NMB_GEN_REGULATOR_POWER] = {"0.002", "0.001", "1", "10"};
    const QString c_defaultRegulatorCooling[E_NMB_GEN_REGULATOR_COOLING] = {"0.15", "0.0043", "1", "30", "15", "20", "40"};
    const QString c_defaultTestTherapy[E_NMB_GEN_TESTTHERAPY] = {"2", "20"};
    const QString c_defaultValuePwmCqm[E_NMB_GEN_PWM_CQM] = {"35000", "35000"};


    QVBoxLayout* MainLayout = new QVBoxLayout(this);

    QLineEdit *lineInputGenMulAdcx[E_GEN_ADC_NMB][E_NMB_GEN_ADC1];
    QLineEdit *lineInputGenAddAdcx[E_GEN_ADC_NMB][E_NMB_GEN_ADC1];
    QLineEdit *lineInputGenOthers[E_NMB_GEN_OTHERS];
    QLineEdit *lineInputGenPwmCqm[E_NMB_GEN_PWM_CQM];
    QLineEdit *lineInputGenRegulatorPower[E_NMB_GEN_REGULATOR_POWER];
    QLineEdit *lineInputGenRegulatorCooling[E_NMB_GEN_REGULATOR_COOLING];
    QLineEdit *lineInputGenTestTherapy[E_NMB_GEN_TESTTHERAPY];

    QPushButton* buttSendGenMulAdcx[E_GEN_ADC_NMB];
    QPushButton* buttSendGenAddAdcx[E_GEN_ADC_NMB];
    QPushButton* buttSendGenOther;
    QPushButton* buttSendGenPwmCqm;
    QPushButton* buttSendGenRegulatorPower;
    QPushButton* buttSendGenRegulatorCooling;
    QPushButton* buttSendGenTestTherapy;

    QLineEdit *lineInputAmpMulAdcx[E_AMP_ADC_NMB][E_NMB_AMP_ADC1];
    QLineEdit *lineInputAmpAddAdcx[E_AMP_ADC_NMB][E_NMB_AMP_ADC1];
    QPushButton* buttSendAmpMulAdcx[E_AMP_ADC_NMB];
    QPushButton* buttSendAmpAddAdcx[E_AMP_ADC_NMB];


    QLabel *createNewLabel(const QString &text);
    QGroupBox *createConfigAmpGroup();
    QGroupBox *createConfigGenGroup();

signals:
    void SendV200specific(QString msg);
    void SaveAdcData(QString type, QString device, int index, QString data);
    void SaveOthers(QString data);
    void SaveRegulatorPower(QString data);
    void SaveRegulatorCooling(QString data);
    void SaveTestTherapy(QString data);
    void SaveTestCqmFreq(QString data);

    void SendReferenceImpedance(float mag, float phase, float ratioRef, float ratioCur);

protected:
    virtual void paintEvent(QPaintEvent*e);
};

#endif // widgetConfig_H
