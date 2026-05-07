#include "bsp_pid.h"
#include "adc.h"

PID_Controller g_cooling_pid;

/*
 * NTC lookup table: 10K NTC (B=3950), 10K pull-up, 3.3V, 12-bit ADC
 * Index = temperature in Celsius (0~50)
 * ADC value increases as temperature rises (NTC resistance drops)
 */
const uint16_t NTC_Table[NTC_TABLE_SIZE] = {
  939, 977, 1017, 1057, 1098, 1140, 1182, 1225, 1269, 1313,
  1357, 1402, 1447, 1493, 1539, 1585, 1631, 1678, 1724, 1771,
  1817, 1864, 1910, 1956, 2002, 2048, 2093, 2138, 2182, 2227,
  2270, 2314, 2356, 2398, 2440, 2481, 2521, 2561, 2600, 2639,
  2676, 2713, 2749, 2785, 2820, 2854, 2887, 2920, 2952, 2983,
  3014
};

/**
  * @brief  Initialize PID controller with default values
  */
void PID_Init(void)
{
    g_cooling_pid.target_temp  = COOLING_TEMP_DEFAULT;
    g_cooling_pid.current_temp = 250;   // Assume room temp 25C
    g_cooling_pid.integral     = 0;
    g_cooling_pid.prev_error   = 0;
    g_cooling_pid.output       = 0;
    g_cooling_pid.enabled      = 0;
}

/**
  * @brief  Set target cooling temperature
  * @param  target_temp: Target in 0.1C, clamped to [COOLING_TEMP_MIN, COOLING_TEMP_MAX]
  */
void PID_SetTarget(int16_t target_temp)
{
    if (target_temp < COOLING_TEMP_MIN) target_temp = COOLING_TEMP_MIN;
    if (target_temp > COOLING_TEMP_MAX) target_temp = COOLING_TEMP_MAX;
    g_cooling_pid.target_temp = target_temp;
}

/**
  * @brief  Convert NTC ADC raw value to temperature using lookup table
  * @param  adc_val: 12-bit ADC reading
  * @retval Temperature in 0.1C units
  * @note   10K NTC (B=3950) with 10K pull-up resistor
  *         Uses table lookup with linear interpolation between entries
  *         Returns 0.1C resolution for smoother PID control
  */
int16_t NTC_ADC_ToTemp(uint32_t adc_val)
{
    uint16_t adc = (uint16_t)adc_val;
    int16_t temp_c;

    /* Clamp: above table maximum */
    if (adc >= NTC_Table[NTC_TABLE_SIZE - 1])
    {
        return (NTC_TABLE_START_C + NTC_TABLE_SIZE - 1) * 10;
    }

    /* Clamp: below table minimum */
    if (adc <= NTC_Table[0])
    {
        return NTC_TABLE_START_C * 10;
    }

    /* Search for the interval containing adc_val */
    for (temp_c = 0; temp_c < NTC_TABLE_SIZE - 1; temp_c++)
    {
        if (adc >= NTC_Table[temp_c] && adc < NTC_Table[temp_c + 1])
        {
            /* Linear interpolation: temp = base + (adc - table[base]) / (table[base+1] - table[base]) */
            int16_t adc_diff = NTC_Table[temp_c + 1] - NTC_Table[temp_c];
            int16_t adc_offs = adc - NTC_Table[temp_c];
            /* Return in 0.1C: temp_c * 10 + fractional * 10 */
            return (NTC_TABLE_START_C + temp_c) * 10 + (adc_offs * 10) / adc_diff;
        }
    }

    /* Fallback (should not reach here) */
    return (NTC_TABLE_START_C + NTC_TABLE_SIZE - 1) * 10;
}

/**
  * @brief  PID control update - call periodically (e.g. every 100ms)
  * @note   For cooling: error = current_temp - target_temp
  *         Positive error means too hot -> increase cooling power
  */
void PID_Update(void)
{
    int16_t error;
    int32_t p_term, i_term, d_term;
    int32_t output_raw;

    if (!g_cooling_pid.enabled)
    {
        g_cooling_pid.output = 0;
        g_cooling_pid.integral = 0;
        return;
    }

    /* Read current temperature from NTC ADC */
    uint32_t adc_val = ADC_Read(NTC_ADC_CHANNEL);
    g_cooling_pid.current_temp = NTC_ADC_ToTemp(adc_val);

    /* Calculate error: positive = need more cooling */
    error = g_cooling_pid.current_temp - g_cooling_pid.target_temp;

    /* Proportional term */
    p_term = (int32_t)error * PID_KP / 100;

    /* Integral term with anti-windup */
    g_cooling_pid.integral += error;
    if (g_cooling_pid.integral > PID_INTEGRAL_MAX)
        g_cooling_pid.integral = PID_INTEGRAL_MAX;
    if (g_cooling_pid.integral < -PID_INTEGRAL_MAX)
        g_cooling_pid.integral = -PID_INTEGRAL_MAX;
    i_term = g_cooling_pid.integral * PID_KI / 100;

    /* Derivative term */
    d_term = (int32_t)(error - g_cooling_pid.prev_error) * PID_KD / 100;
    g_cooling_pid.prev_error = error;

    /* Sum output */
    output_raw = p_term + i_term + d_term;

    /* Clamp output */
    if (output_raw > PID_OUTPUT_MAX) output_raw = PID_OUTPUT_MAX;
    if (output_raw < PID_OUTPUT_MIN) output_raw = PID_OUTPUT_MIN;

    g_cooling_pid.output = (uint8_t)output_raw;
}

/**
  * @brief  Enable or disable cooling PID
  */
void PID_SetEnabled(uint8_t enable)
{
    g_cooling_pid.enabled = enable;
    if (!enable)
    {
        g_cooling_pid.output = 0;
        g_cooling_pid.integral = 0;
        g_cooling_pid.prev_error = 0;
    }
}

/**
  * @brief  Get current PID output duty cycle (0~100)
  */
uint8_t PID_GetOutput(void)
{
    return g_cooling_pid.output;
}
