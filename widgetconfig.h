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

class widgetConfig : public QWidget
{
    Q_OBJECT
public:
    explicit widgetConfig(QWidget *parent = 0);

signals:

public slots:

private:
    enum {E_NMB_GEN_ADC1 = 9};
    enum {E_NMB_GEN_ADC2 = 7};
    enum {E_NMB_GEN_ADC3 = 4};
    enum {E_NMB_GEN_OTHERS = 9};
    enum {E_NMB_GEN_REGULATOR = 4};
    enum {E_NMB_GEN_TESTTHERAPY = 2};
    enum {E_NMB_GEN_PWM_CQM = 2};
    enum {E_NMB_GEN_PWM_COOL = 2};
    enum {E_NMB_GEN_DAC = 2};
    enum {E_NMB_GEN_RAW_RECEIVING = 3};
    enum {E_NMB_GEN_OUTPUTS = 8};
    enum {E_NMB_GEN_APLx = 5};

    enum {E_NMB_AMP_ADC1 = 8};
    enum {E_NMB_AMP_ADC3 = 8};
    enum {E_NMB_AMP_OUTPUTS = 6};

    enum GEN_ADC_X
    {
        E_GEN_ADC_1,
        E_GEN_ADC_2,
        E_GEN_ADC_3,
        E_GEN_ADC_OTH,
        E_GEN_ADC_NMB
    };

    enum AMP_ADC_X
    {
        E_AMP_ADC_1,
        E_AMP_ADC_3,
        E_AMP_ADC_NMB
    };

    const int c_dw_SizeCheckBox = 30;

    const QString c_nameAmpOutputs[E_NMB_AMP_OUTPUTS] = {"PWR_Source_ON", "PWR_Source_Decharge", "ResetErr", "RESERVE_1", "RESERVE_2", "RESERVE_3"};
    const QString c_nameGenOutputs[E_NMB_GEN_OUTPUTS] = {"REL_OUT_1", "REL_OUT_2", "REL_OUT_3", "REL_OUT_4", "nAMP_RESET", "Amp_PWR_ON", "nGenErr_uP", "Fraction/Exilis"};
    const QString c_nameGenAplX[E_NMB_GEN_APLx] = {"apl_1", "apl_1_PWR_ON", "apl_2", "apl_3", "apl_4"};
    const QString c_nameGenRegulator[E_NMB_GEN_REGULATOR] = {"PROPORCIAL", "INTEGRAL", "DERIVATIVE", "Period (10 - 1000 ms)"};
    const QString c_nameGenTestTherapy[E_NMB_GEN_TESTTHERAPY] = {"Duty factor (20 - 100 %)", "Frequency (25 - 200 Hz)"};
    const QString c_nameGenCQM[E_NMB_GEN_PWM_CQM] = {"ch_1 (1000 - 100000 Hz)", "ch_2 (1000 - 100000 Hz)"};


    QVBoxLayout* MainLayout = new QVBoxLayout(this);

    QLineEdit *lineInputGenMulAdcx[E_GEN_ADC_NMB][E_NMB_GEN_ADC1];
    QLineEdit *lineInputGenAddAdcx[E_GEN_ADC_NMB][E_NMB_GEN_ADC1];
    QLineEdit *lineInputGenRegulator[E_NMB_GEN_REGULATOR];
    QLineEdit *lineInputGenTestTherapy[E_NMB_GEN_TESTTHERAPY];

    QPushButton* buttSendGenMulAdcx[E_GEN_ADC_NMB];
    QPushButton* buttSendGenAddAdcx[E_GEN_ADC_NMB];
    QPushButton* buttSendGenRegulator;
    QPushButton* buttSendGenTestTherapy;

    QLineEdit *lineInputAmpMulAdcx[E_AMP_ADC_NMB][E_NMB_AMP_ADC1];
    QLineEdit *lineInputAmpAddAdcx[E_AMP_ADC_NMB][E_NMB_AMP_ADC3];
    QPushButton* buttSendAmpMulAdcx[E_AMP_ADC_NMB];
    QPushButton* buttSendAmpAddAdcx[E_AMP_ADC_NMB];

    QLineEdit *lineInputAmpFreq;
    QLineEdit *lineInputAmpPwm;
    QPushButton* buttSendAmpFreq;
    QPushButton* buttSendAmpPwm;

    QCheckBox* checkAmpOutput[E_NMB_AMP_OUTPUTS];

    QLineEdit *lineInputGenPwmCqm[E_NMB_GEN_PWM_CQM];
    QPushButton* buttSendGenPwmCqm;

    QLineEdit *lineInputGenPwmCool[E_NMB_GEN_PWM_COOL];
    QPushButton* buttSendGenPwmCool;

    QLineEdit *lineInputGenDac[E_NMB_GEN_DAC];
    QPushButton* buttSendGenDac;

    QLineEdit *lineInputGenPwrReset;
    QPushButton* buttSendGenPwrReset;

    QCheckBox* checkGenRawData[E_NMB_GEN_RAW_RECEIVING];

    QCheckBox* checkGenOutputs[E_NMB_GEN_OUTPUTS];

    QCheckBox* checkGenAplX[E_NMB_GEN_APLx];

    QCheckBox* checkGenTestTherapy;


    QLabel *createNewLabel(const QString &text);
    QGroupBox *createConfigurationGroup();
    QGroupBox *createSettingsGroup();
    QGroupBox *createConfigAmpGroup();
    QGroupBox *createConfigGenGroup();
    QGroupBox *createSettingsAmpGroup();
    QGroupBox *createSettingsGenGroup();

    void AssemblyAplxAndPwrResetPacket(bool source);

signals:
    void SendV200specific(QString msg);

protected:
    virtual void paintEvent(QPaintEvent*e);
};

#endif // widgetConfig_H
