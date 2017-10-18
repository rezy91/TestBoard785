#ifndef COMMON_H
#define COMMON_H


#include <QStringList>

#define NMB_ITEMS_TIMERS_GENER              6
#define NMB_ITEMS_TIMERS_AMPLF              4

typedef enum packet_id
{
  // Common packets
  PID_SEND_LAST_OPERATION_RESULT       = 0x00,
  PID_RUN_SELFTEST                     = 0x01,
  PID_DO_SELF_RESTART                  = 0x02,
  PID_SEND_FW_VERSION                  = 0x03,
  PID_SEND_SELFTEST_RESULT             = 0x04,
  PID_ENTRY_SLEEP_MODE                 = 0x05,
  PID_RESET_COMM_WD                    = 0x06,
  PID_TOUCH_MEMORY                     = 0x08,
  PID_SEND_STATUS_REGISTER             = 0x0A,
  PID_SEND_RX_TX_BUFFER_SIZE           = 0x0B,
  PID_SEND_TEMPERATURE                 = 0x0C,
  PID_SEND_HW_CONFIG                   = 0x0D,
  PID_WRITE_TO_MEMORY                  = 0x0F,
  PID_SWITCH_BRIDGE_MODE               = 0x81,
  PID_SEND_CHANNEL_COUNT               = 0x82,
  PID_SET_LOGGING                      = 0x83,
  PID_SEND_LOGGING_INFORMATION         = 0x84,
  PID_SEND_BINARY_DISC                 = 0x85,

  PID_SET_STATE_OF_THERAPY             = 0x20,
  PID_SET_CHOOSE_CURRENT_CHANNEL       = 0x22,
  PID_SET_THERAPY_SETTINGS             = 0x23,
  PID_SET_THERAPY_TIME                 = 0x24,
  PID_SET_CONFIGURATION_OF_ACCESSORY   = 0x26,
  PID_SET_SEND_LAST_ERROR              = 0x28,
  PID_CONFIGURATION_DEVICE             = 0x29,

  // Common packet reply
  PID_REPLY_SEND_LAST_OPERATION_RESULT = 0x00,
  PID_REPLY_SEND_SELFTEST_RESULT       = 0x11,
  PID_REPLY_SEND_FW_VERSION            = 0x12,
  PID_REPLY_RUN_SELFTEST               = 0x13,
  PID_REPLY_TOUCH_MEMORY_OK            = 0x15,
  PID_REPLY_TOUCH_MEMORY               = 0x18,
  PID_REPLY_SEND_STATUS_REGISTER       = 0x1A,
  PID_REPLY_SEND_RX_TX_BUFFER_SIZE     = 0x1B,
  PID_REPLY_SEND_TEMPERATURE           = 0x1C,
  PID_REPLY_SEND_HW_CONFIG             = 0x1D,
  PID_REPLY_SEND_CHANNEL_COUNT         = 0x92,
  PID_REPLY_SEND_LOGGING_ASCII         = 0X93,
  PID_REPLY_SEND_LOGGING_BINARY        = 0X94,
  PID_REPLY_SEND_LOGGING_INFORMATION   = 0x95,
  PID_REPLY_SEND_BINARY_DISC           = 0x96,
  PID_REPLY_SEND_LOGGING_ERROR         = 0xE0,

  PID_REPLY_SET_THERAPY_SETTINGS       = 0x43,
  PID_REPLY_SEND_LAST_ERROR            = 0x48,
  PID_REPLY_CONFIGURATION_DEVICE       = 0x49,


  // test(service) packets
  PID_SEND_AMP_TIMERS_RESULTS         = 0x30,
  PID_SEND_AMP_ADC3_AVERAGE_DATA      = 0x31,
  PID_SEND_AMP_ADC1_ADJUSTED_DATA     = 0x32,
  PID_SEND_AMP_ADC1_AVERAGE_DATA      = 0x33,
  PID_READ_AMP_INPUT                  = 0x34,
  PID_SET_AMP_ADC3_COEFF_MULTIPLE     = 0x35,
  PID_SET_AMP_ADC3_COEFF_ADDITIVE     = 0x36,
  PID_SET_AMP_ADC1_COEFF_MULTIPLE     = 0x37,
  PID_SET_AMP_ADC1_COEFF_ADDITIVE     = 0x38,
  PID_SET_AMP_OUTPUTS                 = 0x39,
  PID_SEND_AMP_SET_FREQUENCY          = 0x3A,
  PID_SEND_AMP_SET_PWM                = 0x3B,
  PID_SET_AMP_THERAPY                 = 0x3C,
  PID_SEND_AMP_ISSUE                  = 0x3D,

  PID_SEND_ADC3_ADJUSTED_DATA_GENER   = 0x40,
  PID_SEND_ADC3_AVERAGE_DATA_GENER    = 0x41,
  PID_SEND_ADC2_ADJUSTED_DATA_GENER   = 0x42,
  PID_SEND_ADC2_AVERAGE_DATA_GENER    = 0x43,
  PID_SEND_ADC1_ADJUSTED_DATA_GENER   = 0x44,
  PID_SEND_ADC1_AVERAGE_DATA_GENER    = 0x45,
  PID_READ_INPUT                      = 0x46,
  PID_SEND_ADCS_RAW_DATA              = 0x47,
  PID_SET_PWM_COOLS_DUTY              = 0x48,

  PID_SET_RESERVE_14                  = 0x4A,
  PID_SET_RESERVE_13                  = 0x50,
  PID_SET_RESERVE_12                  = 0x51,
  PID_SET_RESERVE_11                  = 0x52,
  PID_SET_RESERVE_10                  = 0x53,
  PID_SET_RESERVE_9                   = 0x54,
  PID_SET_RESERVE_8                   = 0x55,
  PID_SET_RESERVE_7                   = 0x56,
  PID_SET_RESERVE_6                   = 0x57,
  PID_SET_OUTPUTS                     = 0x58,
  PID_SET_APLS                        = 0x59,
  PID_SET_RESERVE_5                   = 0x5A,
  PID_SET_RESERVE_4                   = 0x5B,
  PID_START_STOP_TEST_THERAPY         = 0x5C,
  PID_SET_RESERVE_3                   = 0x5D,
  PID_SET_RESERVE_2                   = 0x5E,
  PID_SET_RESERVE_1                   = 0x5F,

  PID_READ_MEASURE_DATA_USN           = 0x2F,
  PID_READ_MEASURE_DATA_APL_L         = 0x3E,
  PID_READ_MEASURE_DATA_APL_S         = 0x3F,

  PID_READ_MEAS_APL_S_1_DEVEL         = 0x60,
  PID_READ_MEAS_APL_S_2_DEVEL         = 0x61,
  PID_READ_MEAS_APL_S_3_DEVEL         = 0x62,
} PACKET_ID;

typedef enum
{
  NO_STREAM,
  RECEIVE_STREAM,
  LOG_STREAM
} SOURCE_STREAM;

enum AMP_ADC_X
{
    E_AMP_ADC_1,
    E_AMP_ADC_3,
    E_AMP_ADC_NMB
};

enum USN_APL_MSGS
{
    E_USN,
    E_APL_LARGE,
    E_APL_SMALL_1,
    E_APL_SMALL_2,
    E_APL_SMALL_3,
    NMB_ITEMS_TIMERS_APLS_AND_USN
};

enum TEXT_BROWSERS
{
    E_BROWSER_FOR_OSCILLOSCOPE,
    E_BROWSER_LOG_ASCII,
    E_BROWSER_NMB
};

enum {nmbCurvesInGraph = 4};
enum {nmbChannelsAppls = 4};
enum {E_NMB_BIT_FLAGS_STATUS = 32};
enum {E_NMB_ITEMS_ADMIN = 3};

const QString c_nameGen = "Gen";
const QString c_nameAmp = "Amp";
const QString c_nameMul = "Mul";
const QString c_nameAdd = "Add";

const QStringList allAdxSignalsGener[NMB_ITEMS_TIMERS_GENER] = { \
    {"g3c_counter", "Temperature cold [°C]", "Temperature hot [°C]", "Temperature tip [°C]", "regulator_output [%]", "regulator_error_sum"}, \
    {"g3s_counter", "Cold_pump_adc", "Hot_pump_adc", "Temperature_cold_adc", "Temperature_hot_adc", "fan_adc", "pelt_adc"}, \
    {"g2c_counter", "imp_avg [||]", "imp_avg [°]", "imp_max [||]", "imp_max [°]", "power_average [W]", "power_current [W]", "power_regulator [W]", "regulator_output",  \
     "regulator_error_sum", "refl_ratio", "effectiveness [%]", "R_cqm [ohm]",  "g2c_exec", "g2c_conv"}, \
    {"g2s_counter", "Vrf_adc", "I4_adc", "Vforward_adc", "I2_adc", "Vreverse_adc", "I3_adc", "I1_adc"}, \
    {"g1c_counter", "g1c_exec", "g1c_conv"}, \
    {"g1s_counter", "Apl1_Vcc_Adc", "CQM1_adc", "CQM2_adc", "V reference [mV]", "Temperature board [°C]", "+24V_adc", "+5V_adc", "Temperature MCU [°C]", "vrefin_adc"}};
const QStringList allAdxSignalsAmplf[NMB_ITEMS_TIMERS_AMPLF] = { \
    {"at_counter", "Frequency", "/Frequency", "duty_factor_1", "duty_factor_2", "dead_time_1->2", "dead_time_2->1"}, \
    {"a3s_counter", "Temperature_1", "Temperature_2", "Temperature_3", "+24V_adc", "+10V_adc", "Vcheck", "ADC3_14", "ADC3_15"}, \
    {"ax_counter", "ax_exec", "ax_conv"}, \
    {"a1s_counter", "Vfet1Out", "Vfet2Out", "Ifet1Out", "Ifet2Out", "Vgate1Out", "Vgate2Out", "inputVolatge_ADC", "inputCurrent_ADC"}};
const QStringList allAdxSignalsAplUsn[NMB_ITEMS_TIMERS_APLS_AND_USN] = { \
    {"output frequency [Hz]", "current head [mA]", "output voltage [mV]", "power voltage [mV]", "generated voltage [mV]", "voltage at 10V [mV]", "voltage at 5V [mV]",  \
     "voltage at 3.3V [mV]", "temp. gener [K]", "temp head_A [0.1K]", "temp head_B [0.1K]", "contact [%]", "temp. board [K]"}, \
    {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"}, \
    {"s1 counter", "RF voltage [V]", "USN voltage [V]", "TIP voltage [V]", "temp. tip [°C]", "nmb meas. tip", "err meas. tip", "err fatal meas. tip", "temp. board [°C]",  \
     "nmb meas. board", "err meas. board", "err fatal meas. board", "temp. IR", "temp. IR int.", "nmb IR", "err nmb IR", "nmb BNO", "err nmb BNO", "nmb CAP", "err nmb CAP",  \
     "nmb TIP", "err nmb TIP", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50"},
    {"s2 counter", "RF voltage [V]", "USN voltage [V]", "TIP voltage [V]", "temp. tip [°C]", "nmb meas. tip", "err meas. tip", "err fatal meas. tip", "temp. board [°C]",  \
     "nmb meas. board", "err meas. board", "err fatal meas. board", "temp. IR", "temp. IR int.", "nmb IR", "err nmb IR", "nmb BNO", "err nmb BNO", "nmb CAP", "err nmb CAP",  \
     "nmb TIP", "err nmb TIP", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50"},
    {"s3 counter", "RF voltage [V]", "USN voltage [V]", "TIP voltage [V]", "temp. tip [°C]", "nmb meas. tip", "err meas. tip", "err fatal meas. tip", "temp. board [°C]",  \
     "nmb meas. board", "err meas. board", "err fatal meas. board", "temp. IR", "temp. IR int.", "nmb IR", "err nmb IR", "nmb BNO", "err nmb BNO", "nmb CAP", "err nmb CAP",  \
     "nmb TIP", "err nmb TIP", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50"}};



typedef struct status_register
{
  union M_REG
  {
    uint32_t m_dwLong;
    struct M_BIT
    {
      // byte 0
      uint8_t ChangeAcc0          :1;   // bit 0
      uint8_t ChangeAcc1          :1;   // bit 1
      uint8_t ChangeAcc2          :1;   // bit 2
      uint8_t ChangeAcc3          :1;   // bit 3
      uint8_t Empty0              :1;   // bit 4
      uint8_t SelfTestDone        :1;   // bit 5
      uint8_t Restart             :1;   // bit 6
      uint8_t Error               :1;   // bit 7
      // byte 1
      uint8_t ButtonPressed       :1;   // bit 8
      uint8_t KeyChanged          :1;   // bit 9
      uint8_t TherapyRunning      :1;   // bit 10
      uint8_t ParChangedByMas     :1;   // bit 11
      uint8_t ParChangedByApp     :1;   // bit 12
      uint8_t SyncWaiting         :1;   // bit 13
      uint8_t EmergencyPressed    :1;   // bit 14
      uint8_t LogRequest          :1;   // bit 15
      // byte 2
      uint8_t ChangeSmartDevice0  :1;   // bit 16
      uint8_t StateAcc0           :1;   // bit 17
      uint8_t StateAcc1           :1;   // bit 18
      uint8_t StateAcc2           :1;   // bit 19
      uint8_t StateAcc3           :1;   // bit 20
      uint8_t StateSmartDevice0   :1;   // bit 21
      uint8_t ContactPatient      :1;   // bit 22
      uint8_t ContactNeutral      :1;   // bit 23
      // byte 3
      uint8_t ChoosedChannel      :2;   // bit 24 - 25
      uint8_t ChsdChannelCorrect  :1;   // bit 26
      uint8_t SlavesSynchOnDone   :1;   // bit 27
      uint8_t ChangeSmartDevice1  :1;   // bit 28
      uint8_t StateSmartDevice1   :1;   // bit 29
      uint8_t StateTherapy        :2;   // bit 30 - 31
    } m_Bit;
  } m_Reg;
  uint8_t m_bySetTemperaturePatient;
  uint8_t m_bySetIntensityUsn;
  uint16_t m_wMeasuredPower;
  uint16_t m_wSetPower;
  int16_t m_wMeasuredTemperaturePatient;
  uint8_t m_wSetDutyFactor;

  union
  {
    uint8_t mByExtended;
    struct
    {
      uint8_t measuredCqm      :3;   // bit 0 - 2
      uint8_t Extreserve5      :1;   // bit 3
      uint8_t Extreserve4      :1;   // bit 4
      uint8_t Extreserve3      :1;   // bit 5
      uint8_t Extreserve2      :1;   // bit 6
      uint8_t Extreserve1      :1;   // bit 7
    } m_BitExtended;
  } m_ByteExtended;
} STATUS_REGISTER;

#endif // COMMON_H
