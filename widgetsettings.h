#ifndef WIDGETSETTINGS_H
#define WIDGETSETTINGS_H

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

class widgetSettings : public QWidget
{
    Q_OBJECT
public:
    explicit widgetSettings(QWidget *parent = 0);

    void RefreshPage(void);

public slots:
    void ReadSettingsGener(QByteArray data);

private:
    enum {E_NMB_GEN_RAW_RECEIVING = 3};
    enum {E_NMB_GEN_OUTPUTS = 8};
    enum {E_NMB_GEN_APLx = 9};

    enum {E_NMB_AMP_OUTPUTS = 3};

    typedef enum
    {
      E_SET_TYPE_RF_OUTPUTS,
      E_SET_TYPE_RF_APLS,
      E_SET_TYPE_RF_COOL_PWM,
      E_SET_TYPE_RF_TEST_THERAPY,
      E_SET_TYPE_RF_DO_PEAK,
      E_SET_TYPE_RF_COUNT,
    } SETTINGS_TYPES_RF;

    typedef enum
    {
      E_SET_TYPE_AMPLF_OUTPUTS,
      E_SET_TYPE_AMPLF_FREQ,
      E_SET_TYPE_AMPLF_PWM,
      E_SET_TYPE_AMPLF_COUNT,
    } SETTINGS_TYPES_AMPLF;

    const int c_dwVolumeOfPartAmplifier[E_SET_TYPE_AMPLF_COUNT] = {3, 3, 2};
    const int c_dw_SizeCheckBox = 30;

    const QString c_nameAmpOutputs[E_NMB_AMP_OUTPUTS] = {"PWR_Source_ON", "PWR_Source_Decharge", "ResetErr"};
    const QString c_nameGenOutputs[E_NMB_GEN_OUTPUTS] = {"REL_OUT_1", "REL_OUT_2", "REL_OUT_3", "REL_OUT_4", "nAMP_RESET", "Amp_PWR_ON", "nGenErr_uP", "Fraction/Exilis"};
    const QString c_nameGenAplX[E_NMB_GEN_APLx] = {"apl_1", "apl_1_PWR_ON", "apl_2", "apl_3", "apl_4", "cool_pump_hot", "cool_pump_cold", "cool_fan", "cool_pelt"};

    QVBoxLayout* MainLayout = new QVBoxLayout(this);

    QLineEdit *lineInputAmpFreq;
    QLineEdit *lineInputAmpPwm;
    QPushButton* buttSendAmpFreq;
    QPushButton* buttSendAmpPwm;

    QCheckBox* checkAmpOutput[E_NMB_AMP_OUTPUTS];

    QLineEdit *lineInputGenPwmCool;
    QPushButton* buttSendGenPwmCool;

    QLineEdit *lineInputGenPwrReset;
    QPushButton* buttSendGenPwrReset;

    QCheckBox* checkGenRawData[E_NMB_GEN_RAW_RECEIVING];
    QCheckBox* checkGenOutputs[E_NMB_GEN_OUTPUTS];
    QCheckBox* checkGenAplX[E_NMB_GEN_APLx];
    QCheckBox* checkGenTestTherapy;


    QLabel *createNewLabel(const QString &text);
    QGroupBox *createSettingsAmpGroup();
    QGroupBox *createSettingsGenGroup();

signals:
    void SendV200specific(QString msg, bool bExp);

public slots:
};

#endif // WIDGETSETTINGS_H
