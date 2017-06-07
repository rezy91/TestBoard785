#ifndef COMMON_H
#define COMMON_H


#include <QStringList>

#define NMB_ITEMS_TIMERS_GENER              6
#define NMB_ITEMS_TIMERS_AMPLF              4
#define NMB_COEFFICIENTS_OTHERS             9

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

  // Common packet reply
  PID_REPLY_SEND_LAST_OPERATION_RESULT = 0x00,
  PID_REPLY_SEND_SELFTEST_RESULT       = 0x11,
  PID_REPLY_SEND_FW_VERSION            = 0x12,
  PID_REPLY_RUN_SELFTEST               = 0x13,
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
  PID_SET_DAC                         = 0x49,
  PID_SET_PWM_CQMS_FREQ               = 0x4A,
  PID_SET_ADC3_COEFFICIENTS_MULTIPLE  = 0x50,
  PID_SET_ADC3_COEFFICIENTS_ADDITIVE  = 0x51,
  PID_SET_ADC2_COEFFICIENTS_MULTIPLE  = 0x52,
  PID_SET_ADC2_COEFFICIENTS_ADDITIVE  = 0x53,
  PID_SET_ADC1_COEFFICIENTS_MULTIPLE  = 0x54,
  PID_SET_ADC1_COEFFICIENTS_ADDITIVE  = 0x55,
  PID_SET_ADC_OTHERS_COEFFS_MULTIPLE  = 0x56,
  PID_SET_ADC_OTHERS_COEFFS_ADDITIVE  = 0x57,
  PID_SET_OUTPUTS                     = 0x58,
  PID_SET_APLS                        = 0x59,
  PID_SET_REGULATOR                   = 0x5A,
  PID_SET_THERAPY_TEST                = 0x5B,
  PID_START_STOP_THERAPY              = 0x5C,
  PID_SET_RESERVE_3                   = 0x5D,
  PID_SET_RESERVE_2                   = 0x5E,
  PID_SET_RESERVE_1                   = 0x5F,
} PACKET_ID;

typedef enum
{
  NO_STREAM,
  RECEIVE_STREAM,
  LOG_STREAM
} SOURCE_STREAM;

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

enum{nmbCurvesInGraph = 4};

const QString c_nameGen = "Gen";
const QString c_nameAmp = "Amp";
const QString c_nameMul = "Mul";
const QString c_nameAdd = "Add";

const QStringList allAdxSignalsGener[NMB_ITEMS_TIMERS_GENER] = { \
    {"g3c_counter", "Temperature1 [°C]", "Temperature2 [°C]"}, \
    {"g3s_counter", "pump1_adc", "pump2_adc", "Temperature1_adc", "Temperature2_adc", "CoolingPF3_adc", "CoolingPF5_adc"}, \
    {"g2c_counter", "imp_avg [||]", "imp_avg [°]", "imp_max [||]", "imp_max [°]", "power_average [W]", "power_current [W]", "power_regulator [W]", "regulator_output", "regulator_error_sum", "g2c_exec", "g2c_conv"}, \
    {"g2s_counter", "Vrf_adc", "I4_adc", "Vforward_adc", "I2_adc", "Vreverse_adc", "I3_adc", "I1_adc"}, \
    {"g1c_counter", "g1c_exec", "g1c_conv"}, \
    {"g1s_counter", "Apl1_Vcc_Adc", "CQM1_adc", "CQM2_adc", "V reference [mV]", "Temperature1", "+24V_adc", "+5V_adc", "Temperature MCU [°C]", "vrefin_adc"}};
const QStringList allAdxSignalsAmplf[NMB_ITEMS_TIMERS_AMPLF] = { \
    {"at_counter", "Frequency", "/Frequency", "duty_factor_1", "duty_factor_2", "dead_time_1->2", "dead_time_2->1"}, \
    {"a3s_counter", "Temperature_1", "Temperature_2", "Temperature_3", "+24V_adc", "+10V_adc", "Vcheck", "ADC3_14", "ADC3_15"}, \
    {"ax_counter", "ax_exec", "ax_conv"}, \
    {"a1s_counter", "Vfet1Out", "Vfet2Out", "Ifet1Out", "Ifet2Out", "Vgate1Out", "Vgate2Out", "inputVolatge_ADC", "inputCurrent_ADC"}};
const QString coeffsOthersNameMultiple[NMB_COEFFICIENTS_OTHERS] = {"for power", "NOT_USED", "NOT_USED", "NOT_USED", "NOT_USED", "NOT_USED", "NOT_USED", "NOT_USED", "NOT_USED"};
const QString coeffsOthersNameAdditive[NMB_COEFFICIENTS_OTHERS] = {"for power", "for phase (0 - pi/2) [rad]", "max refl ratio (0.0 - 1.0)", "max PWM to amp (0.1 - 0.9)", "max diff power regulator (0 - 100) [%]", "refer. imp. magnitude", "refer. imp. phase (0 - pi/2) [rad]", "max refl ratio (0.0 - 1.0)", "NOT_USED"};


#endif // COMMON_H
