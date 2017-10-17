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
    void ReadConfigGener(QByteArray data);

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


    enum {E_NMB_MAX_VALUE = 17};

    const int c_dwVolumeOfPart[E_CFG_TYPE_RF_COUNT] = {9, 9, 7, 7, 6, 6, 17, 4, 7, 2, 2};
    const QStringList c_qstrName[E_CFG_TYPE_RF_COUNT] = { \
        {"Apl1_Vcc_Adc", "CQM1_adc", "CQM2_adc", "V reference [mV]", "Temperature board [°C]", "+24V_adc", "+5V_adc", "Temperature MCU [°C]", "vrefin_adc"}, \
        {"Apl1_Vcc_Adc", "CQM1_adc", "CQM2_adc", "V reference [mV]", "Temperature board [°C]", "+24V_adc", "+5V_adc", "Temperature MCU [°C]", "vrefin_adc"}, \
        {"Vrf_adc", "I4_adc", "Vforward_adc", "I2_adc", "Vreverse_adc", "I3_adc", "I1_adc"}, \
        {"Vrf_adc", "I4_adc", "Vforward_adc", "I2_adc", "Vreverse_adc", "I3_adc", "I1_adc"}, \
        {"Cold_pump_adc", "Hot_pump_adc", "Temperature_cold_adc", "Temperature_hot_adc", "fan_adc", "pelt_adc"}, \
        {"Cold_pump_adc", "Hot_pump_adc", "Temperature_cold_adc", "Temperature_hot_adc", "fan_adc", "pelt_adc"}, \
        {"power max", "power min", "for phase (0 - pi/2) [rad]", "max refl ratio (0.0 - 1.0)", "max PWM to amp (0.1 - 0.9)", "max diff power regulator (0 - 100) [%]", "refer. imp. magnitude", "refer. imp. phase (0 - pi/2) [rad]", "max refl ratio (0.0 - 1.0)", "R_cqm min [ohm]", "R_cqm less [ohm]", "R_cqm more [ohm]", "R_cqm max [ohm]", "Cqm_r_ser [ohm]", "Cqm_r_par [ohm]", "Cqm_u [V]", "Cqm_r_off [ohm]"}, \
        {"PROPORCIAL", "INTEGRAL", "DERIVATIVE", "Period (10 - 1000 ms)"}, \
        {"PROPORCIAL", "INTEGRAL", "DERIVATIVE", "Period (10 - 10000 ms)", "min PWM [%]", "t_cold [°C]", "t_hot [°C]"}, \
        {"Duty factor (20 - 100 %)", "Frequency (25 - 200 Hz)"}, \
        {"ch_1 (1000 - 100000 Hz)", "ch_2 (1000 - 100000 Hz)"}, \
    };
    const QString c_nameTypeFile = "RF generator 785 (*.gcf)";
    const QString c_nameVolumeOfPart[E_CFG_TYPE_RF_COUNT] = {"ADC1 multiple", "ADC1 additive", "ADC2 multiple", "ADC2 additive", "ADC3 multiple", "ADC3 additive", "Others", "Regulator power", "Regulator cooling", "Test therapy", "Cqm PWM"};
    const QString c_nameButtonConfigGener[E_NMB_BUTTONS] = {"Load from Gener", "Load from file", "Write to Gener", "Write to file"};
    const QString c_nameButtonConfigAmplf[E_NMB_BUTTONS] = {"Load from Amplf", "Load from file", "Write to Amplf", "Write to file"};

    const QStringList c_defaultValueAmpAdcMul[E_AMP_ADC_NMB] = { \
        {"10", "191.27", "1.777", "1.777", "3.27", "3.27", "36.666", "3.05"}, \
        {"1.0", "1.0", "1.0", "1.0", "1.0", "1.0", "1.0", "1.0"}};
    const QStringList c_defaultValueAmpAdcAdd[E_AMP_ADC_NMB] = { \
        {"0.0", "15.34", "0.0", "0.0", "0.0", "0.0", "0.0", "0.0"}, \
        {"0.0", "188.18", "188.18", "0.0", "0.0", "0.0", "0.0", "0.0"}};


    QVBoxLayout* MainLayout = new QVBoxLayout(this);

    QPushButton* buttConfigGener[E_NMB_BUTTONS];
    QPushButton* buttConfigAmplf[E_NMB_BUTTONS];
    QLineEdit *lineInputPart[E_CFG_TYPE_RF_COUNT][E_NMB_MAX_VALUE];
    QPushButton *buttSendPart[E_CFG_TYPE_RF_COUNT];

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

    void SendReferenceImpedance(float mag, float phase, float ratioRef, float ratioCur);

protected:
    virtual void paintEvent(QPaintEvent*e);
};

#endif // widgetConfig_H
