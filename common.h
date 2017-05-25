#ifndef COMMON_H
#define COMMON_H


#include <QStringList>

#define NMB_ITEMS_TIMERS_GENER              6
#define NMB_ITEMS_TIMERS_AMPLF              4
#define NMB_COEFFICIENTS_OTHERS             9

typedef enum
{
  NO_STREAM,
  RECEIVE_STREAM,
  LOG_STREAM
} SOURCE_STREAM;


enum{nmbCurvesInGraph = 4};


const QStringList allAdxSignalsGener[NMB_ITEMS_TIMERS_GENER] = { \
    {"g3c_counter", "g3c_exec", "g3c_conv"}, \
    {"g3s_counter", "Cooling_ADC1", "Cooling_ADC2", "Cooling_ADC3", "Cooling_ADC4"}, \
    {"g2c_counter", "imp_avg_mag", "imp_avg_phs", "imp_max_mag", "imp_max_phs", "power_average", "power_current", "power_regulator", "regulator_output_prc", "regulator_error_sum", "g2c_exec", "g2c_conv"}, \
    {"g2s_counter", "Vrf_adc", "I4_adc", "Vforward_adc", "I2_adc", "Vreverse_adc", "I3_adc", "I1_adc"}, \
    {"g1c_counter", "g1c_exec", "g1c_conv"}, \
    {"g1s_counter", "Apl1_Vcc_Adc", "CQM1_adc", "CQM2_adc", "V reference [mV]", "Temperature1", "+24V_adc", "+5V_adc", "Temperature MCU [°C]", "vrefin_adc"}};
const QStringList allAdxSignalsAmplf[NMB_ITEMS_TIMERS_AMPLF] = { \
    {"at_counter", "Frequency", "/Frequency", "duty_factor_1", "duty_factor_2", "dead_time_1->2", "dead_time_2->1"}, \
    {"a3s_counter", "Temperature_1", "Temperature_2", "Temperature_3", "+24V_adc", "+10V_adc", "Vcheck", "ADC3_14", "ADC3_15"}, \
    {"ax_counter", "ax_exec", "ax_conv"}, \
    {"a1s_counter", "Vfet1Out", "Vfet2Out", "Ifet1Out", "Ifet2Out", "Vgate1Out", "Vgate2Out", "inputVolatge_ADC", "inputCurrent_ADC"}};
const QString coeffsOthersNameMultiple[NMB_COEFFICIENTS_OTHERS] = {"for power", "NOT_USED", "NOT_USED", "NOT_USED", "NOT_USED", "NOT_USED", "NOT_USED", "NOT_USED", "NOT_USED"};
const QString coeffsOthersNameAdditive[NMB_COEFFICIENTS_OTHERS] = {"for power", "for phase (0 - pi/2) [rad]", "max refl ratio (0.0 - 1.0)", "max PWM to amp (0.1 - 0.9)", "max diff power regulator (0 - 100) [%]", "NOT_USED", "NOT_USED", "NOT_USED", "NOT_USED"};


#endif // COMMON_H
