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

    void clearLineEdits(void);

signals:

public slots:
    void ReadAdcData(QString type, QString device, int index, QString data);
    void ReadConfigGenerNew(QByteArray data);

private:
    enum
    {
        E_BUTTON_LOAD_MCU,
        E_BUTTON_LOAD_FILE,
        E_BUTTON_SAVE_MCU,
        E_BUTTON_SAVE_FILE,
        E_NMB_BUTTONS,
    };
    typedef enum
    {
      CONFIG_WRITE,
      CONFIG_READ,
    } CONFIG_ACTION;

    enum GEN_ADC_X
    {
        E_GEN_ADC_1,
        E_GEN_ADC_2,
        E_GEN_ADC_3,
        E_GEN_ADC_NMB
    };

    enum {E_NMB_GEN_ADC1 = 9};
    enum {E_NMB_GEN_ADC2 = 7};
    enum {E_NMB_GEN_ADC3 = 6};
    enum {E_NMB_GEN_OTHERS = 17};
    enum {E_NMB_GEN_REGULATOR_POWER = 4};
    enum {E_NMB_GEN_REGULATOR_COOLING = 7};
    enum {E_NMB_GEN_TESTTHERAPY = 2};
    enum {E_NMB_GEN_PWM_CQM = 2};

    enum {E_NMB_AMP_ADC1 = 8};
    enum {E_NMB_AMP_ADC3 = 8};

    typedef enum
    {
      E_CFG_TYPE_RF_ADC1_MUL,
      E_CFG_TYPE_RF_ADC1_ADD,
      E_CFG_TYPE_RF_ADC2_MUL,
      E_CFG_TYPE_RF_ADC2_ADD,
      E_CFG_TYPE_RF_ADC3_MUL,
      E_CFG_TYPE_RF_ADC3_ADD,
      E_CFG_TYPE_RF_OTHERS,
      E_CFG_TYPE_RF_REG_PWR,
      E_CFG_TYPE_RF_REG_COOL,
      E_CFG_TYPE_RF_TEST_THERAPY,
      E_CFG_TYPE_RF_CQM_PWM,
      E_CFG_TYPE_RF_COUNT,
      E_CFG_TYPE_RF_ALL = 255,
    } CONFIG_TYPES_RF;

    const QString c_nameTypeFile = "RF generator 785 (*.gcf)";
    const QString c_nameGenRegulatorPower[E_NMB_GEN_REGULATOR_POWER] = {"PROPORCIAL", "INTEGRAL", "DERIVATIVE", "Period (10 - 1000 ms)"};
    const QString c_nameGenRegulatorCooling[E_NMB_GEN_REGULATOR_COOLING] = {"PROPORCIAL", "INTEGRAL", "DERIVATIVE", "Period (10 - 10000 ms)", "min PWM [%]", "t_cold [°C]", "t_hot [°C]"};
    const QString c_nameGenTestTherapy[E_NMB_GEN_TESTTHERAPY] = {"Duty factor (20 - 100 %)", "Frequency (25 - 200 Hz)"};
    const QString c_nameGenCQM[E_NMB_GEN_PWM_CQM] = {"ch_1 (1000 - 100000 Hz)", "ch_2 (1000 - 100000 Hz)"};
    const QString c_nameOthers[E_NMB_GEN_OTHERS] = {"power max", "power min", "for phase (0 - pi/2) [rad]", "max refl ratio (0.0 - 1.0)", "max PWM to amp (0.1 - 0.9)", "max diff power regulator (0 - 100) [%]", "refer. imp. magnitude", "refer. imp. phase (0 - pi/2) [rad]", "max refl ratio (0.0 - 1.0)", "R_cqm min [ohm]", "R_cqm less [ohm]", "R_cqm more [ohm]", "R_cqm max [ohm]", "Cqm_r_ser [ohm]", "Cqm_r_par [ohm]", "Cqm_u [V]", "Cqm_r_off [ohm]"};
    const QString c_nameButtonConfigGener[E_NMB_BUTTONS] = {"Load from Gener", "Load from file", "Write to Gener", "Write to file"};
    const QString c_nameButtonConfigAmplf[E_NMB_BUTTONS] = {"Load from Amplf", "Load from file", "Write to Amplf", "Write to file"};

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
    const QString c_defaultOthers[E_NMB_GEN_OTHERS] = {"1.1154", "0.0", "-0.087", "0.3", "0.5", "2000", "100", "0.1", "0.2", "-2", "20", "50", "100", "565.4867", "471.6876", "4043.999", "27.85234"};
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

    QPushButton* buttConfigGener[E_NMB_BUTTONS];
    QPushButton* buttConfigAmplf[E_NMB_BUTTONS];
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

    void loadFile(void);
    void saveFile(void);
    void saveMcuRf(void);

    float DecodeUint32ToFloat(QByteArray qByArry);
    uint32_t DecodeBytesToUint32(QByteArray qByArry);

    void ReadLineEditAndAddToMsg(QString& strPacketContent, CONFIG_TYPES_RF eType);
    void ReadAndAddToMsgAdc1Mul(QString& strPacketContent);
    void ReadAndAddToMsgAdc1Add(QString& strPacketContent);
    void ReadAndAddToMsgAdc2Mul(QString& strPacketContent);
    void ReadAndAddToMsgAdc2Add(QString& strPacketContent);
    void ReadAndAddToMsgAdc3Mul(QString& strPacketContent);
    void ReadAndAddToMsgAdc3Add(QString& strPacketContent);
    void ReadAndAddToMsgOthrers(QString& strPacketContent);
    void ReadAndAddToMsgRegCool(QString& strPacketContent);
    void ReadAndAddToMsgRegPwr(QString& strPacketContent);
    void ReadAndAddToMsgTestTher(QString& strPacketContent);
    void ReadAndAddToMsgCqmPwm(QString& strPacketContent);

signals:
    void SendV200specific(QString msg, bool bExp);
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
