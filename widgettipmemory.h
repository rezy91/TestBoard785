#ifndef WIDGETTIPMEMORY_H
#define WIDGETTIPMEMORY_H

#include <QObject>
#include <QWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "common.h"

class widgetTipMemory : public QWidget
{
    Q_OBJECT
public:
    explicit widgetTipMemory(QWidget *parent = 0);
    void clearAll(void);

private:
    enum
    {
        E_VERSION,
        E_SERIAL,
        E_EXPIRATION,
        E_DATE_MANUFACT,
        E_DATE_ACTIVE,
        E_TIP_TYPE,
        E_USN_FREQ_CRYSTAL,
        E_USN_CONTROL_VARIABLE,
        E_USN_VOLTAGEV100,
        E_USN_VALUE_AIR,
        E_USN_VALUE_WATER,
        E_USN_CONTROL_TYPE,
        E_RF_POWER_MAX,
        E_REFER_IMPEDANCE_MODUL,
        E_REFER_IMPEDANCE_PHASE,
        E_REFER_IMPEDANCE_REFL,
        E_CURRN_IMPEDANCE_REFL,
        E_NMB_PARAMETERS_IN_MEMORY,
    };
    enum
    {
        E_BUTTON_LOAD_MCU,
        E_BUTTON_LOAD_FILE,
        E_BUTTON_SAVE_MCU,
        E_BUTTON_SAVE_FILE,
        E_NMB_BUTTONS,
    };
    enum {MAX_TIP_MEMORY_STRUCT_SIZE = 128};
    enum {TIP_MEMORY_VERSION = 100};

    const QString c_nameTypeFile = "Tip memory 285 (*.tip)";
    const QString c_nameParametersInMemory[E_NMB_PARAMETERS_IN_MEMORY] =
    {"VERSION", "SERIAL", "EXPIRATION", "DATE_MANUFACT", "DATE_ACTIVE", "TIP_TYPE", "USN_FREQ_CRYSTAL", "USN_CONTROL_VARIABLE", "USN_VOLTAGEV100",
     "USN_VALUE_AIR", "USN_VALUE_WATER", "USN_CONTROL_TYPE", "RF_POWER_MAX", "REFER_IMPEDANCE_MODUL", "REFER_IMPEDANCE_PHASE", "REFER_IMPEDANCE_REFL", "CURRENT_IMPEDANCE_REFL"};
    const QString c_nameUnitInMemory[E_NMB_PARAMETERS_IN_MEMORY] =
    {"-", "-", "-", "-", "-", "-", "Hz", "mA|mV", "mV", "mV", "mV", "-", "W", "||", "°", "%", "%"};
    const QString c_nameButtonConfig[E_NMB_BUTTONS] = {"Load from Tip", "Load from file", "Write to Tip", "Write to file"};
    const int c_VolumeParameters[E_NMB_PARAMETERS_IN_MEMORY] = {1, 30, 4, 8, 14, 1, 2, 2, 2, 2, 2, 1, 2, 1, 1, 1, 1};
    const int dwChannelDefault = -1;

    int c_AddressParameters[E_NMB_PARAMETERS_IN_MEMORY];

    QLineEdit *lineInputParameter[E_NMB_PARAMETERS_IN_MEMORY];
    QPushButton* buttSend[E_NMB_PARAMETERS_IN_MEMORY];
    QPushButton* buttConfig[E_NMB_BUTTONS];
    QVBoxLayout* vBox = new QVBoxLayout(this);
    QHBoxLayout* hBox = new QHBoxLayout();
    QGridLayout* gridParams = new QGridLayout();
    QComboBox* listOfChannels = new QComboBox(this);

    uint8_t arr_byTipContent[nmbChannelsAppls][MAX_TIP_MEMORY_STRUCT_SIZE];
    int dwCurrentChannel;


    QLabel *createNewLabel(const QString &text);
    void decodeTipMemory(uint8_t byChannel);
    bool IsStringNumber(QString strInput);
    bool IsPrintable(QString strInput);

    bool CollectStringAscii(int iIndex, QString strInputString, QString &text);
    bool CollectStringParameter(int iIndex, QString strInputString, QString &text);

    void clearComboBox(int iIndex);
    void clearLineInput(void);


signals:
    void SendV200specific(QString msg, bool bExp);
    void SendReferenceImpedance(float mag, float phase, float ratioRef, float ratioCur);
    void SendDefaultReferenceImpedance(void);
    void SendMaximalPower(uint16_t sMaxP);
    void SendDefaultMaximalPower();

public slots:
    void ReceiveStatusReg(STATUS_REGISTER eStatusReg);
    void ReceiveTipMemory(uint8_t byChannel, uint8_t byBytes, QByteArray byBuffer);
    void DecodeChoosedChannel(int dwChannel);
};

#endif // WIDGETTIPMEMORY_H
