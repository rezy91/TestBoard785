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
    void ReadAmpFreq(QString data);
    void ReadAmpPwm(QString data);
    void ReadSettingsGener(QByteArray data);

private:
    enum {E_NMB_GEN_RAW_RECEIVING = 3};
    enum {E_NMB_GEN_OUTPUTS = 8};
    enum {E_NMB_GEN_APLx = 9};

    enum {E_NMB_AMP_OUTPUTS = 6};

    typedef enum
    {
      E_SET_TYPE_RF_OUTPUTS,
      E_SET_TYPE_RF_APLS,
      E_SET_TYPE_RF_COOL_PWM,
      E_SET_TYPE_RF_TEST_THERAPY,
      E_SET_TYPE_RF_DO_PEAK,
      E_SET_TYPE_RF_COUNT,
    } SETTINGS_TYPES_RF;


    const int c_dw_SizeCheckBox = 30;

    const QString c_nameAmpOutputs[E_NMB_AMP_OUTPUTS] = {"PWR_Source_ON", "PWR_Source_Decharge", "ResetErr", "RESERVE_1", "RESERVE_2", "RESERVE_3"};
    const QString c_nameGenOutputs[E_NMB_GEN_OUTPUTS] = {"REL_OUT_1", "REL_OUT_2", "REL_OUT_3", "REL_OUT_4", "nAMP_RESET", "Amp_PWR_ON", "nGenErr_uP", "Fraction/Exilis"};
    const QString c_nameGenAplX[E_NMB_GEN_APLx] = {"apl_1", "apl_1_PWR_ON", "apl_2", "apl_3", "apl_4", "cool_pump_hot", "cool_pump_cold", "cool_fan", "cool_pelt"};

    const QString c_defaultValueAmpFreq = "32000";
    const QString c_defaultValueAmpPwm = "10.0";


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
    void SaveAmpFreq(QString data);
    void SaveAmpPwm(QString data);
    void SaveGenPwm(QString data);
    void SaveGenPwr(QString data);

public slots:
};

#endif // WIDGETSETTINGS_H
