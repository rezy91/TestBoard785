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

public slots:
    void ReadAmpFreq(QString data);
    void ReadAmpPwm(QString data);
    void ReadGenPwm(QString data);
    void ReadGenDac(QString data);
    void ReadGenPwr(QString data);

private:
    enum {E_NMB_GEN_PWM_COOL = 2};
    enum {E_NMB_GEN_DAC = 2};
    enum {E_NMB_GEN_RAW_RECEIVING = 3};
    enum {E_NMB_GEN_OUTPUTS = 8};
    enum {E_NMB_GEN_APLx = 5};

    enum {E_NMB_AMP_OUTPUTS = 6};


    const int c_dw_SizeCheckBox = 30;

    const QString c_nameAmpOutputs[E_NMB_AMP_OUTPUTS] = {"PWR_Source_ON", "PWR_Source_Decharge", "ResetErr", "RESERVE_1", "RESERVE_2", "RESERVE_3"};
    const QString c_nameGenOutputs[E_NMB_GEN_OUTPUTS] = {"REL_OUT_1", "REL_OUT_2", "REL_OUT_3", "REL_OUT_4", "nAMP_RESET", "Amp_PWR_ON", "nGenErr_uP", "Fraction/Exilis"};
    const QString c_nameGenAplX[E_NMB_GEN_APLx] = {"apl_1", "apl_1_PWR_ON", "apl_2", "apl_3", "apl_4"};

    const QString c_defaultValueAmpFreq = "32000";
    const QString c_defaultValueAmpPwm = "10.0";
    const QString c_defaultValueGenPwm[E_NMB_GEN_PWM_COOL] = {"15", "50"};
    const QString c_defaultValueGenDac[E_NMB_GEN_DAC] = {"200", "2000"};
    const QString c_defaultValueGenPwr = "10";


    QVBoxLayout* MainLayout = new QVBoxLayout(this);

    QLineEdit *lineInputAmpFreq;
    QLineEdit *lineInputAmpPwm;
    QPushButton* buttSendAmpFreq;
    QPushButton* buttSendAmpPwm;

    QCheckBox* checkAmpOutput[E_NMB_AMP_OUTPUTS];

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
    QGroupBox *createSettingsAmpGroup();
    QGroupBox *createSettingsGenGroup();

    void AssemblyAplxAndPwrResetPacket(bool source);

signals:
    void SendV200specific(QString msg);
    void SaveAmpFreq(QString data);
    void SaveAmpPwm(QString data);
    void SaveGenPwm(QString data);
    void SaveGenDac(QString data);
    void SaveGenPwr(QString data);

public slots:
};

#endif // WIDGETSETTINGS_H
