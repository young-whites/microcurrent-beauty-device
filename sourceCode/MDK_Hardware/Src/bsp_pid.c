#include "bsp_pid.h"
#include "adc.h"

PID_Controller g_cooling_pid;

/*
 * NTC lookup table: 10K NTC (B=3950), 10K pull-up, 3.3V, 12-bit ADC
 * Index = temperature in Celsius (0~50)
 * ADC value increases as temperature rises (NTC resistance drops)
 */
const uint16_t NTC_Table[NTC_TABLE_SIZE] = {
  3699, 3604, 3513, 3424, 3337, 3253, 3172, 3092, 3015, 2940, /*  0~ 9C */
  2721, 2677, 2633, 2589, 2544, 2499, 2454, 2409, 2363, 2318, /* 10~19C */
  2272, 2227, 2182, 2136, 2091, 2048, 2002, 1957, 1913, 1870, /* 20~29C */
  1826, 1783, 1741, 1699, 1658, 1617, 1577, 1537, 1498, 1460, /* 30~39C */
  1422, 1385, 1348, 1313, 1277, 1243, 1209, 1176, 1144, 1112, /* 40~49C */
  1082, 1051, 1022,  993,  965,  937,  911,  884,  859,  834, /* 50~59C */
   810,  787,  764,  742,  720,  699,  679,  659,  639,  621, /* 60~69C */
   603,  585,  568,  551,  535,  519,  504,  490,  475,  462, /* 70~79C */
   448,  435,  422,  410,  399,  387,  376,  365,  355,  345, /* 80~89C */
   335,  325,  316,  307,  299,  290,  282,  274,  267,  259, /* 90~99C */
   253,  245,  239,  232,  226,  220,  214,  208,  202,  197, /* 100~109C */
   192,  187,  182,  177,  172,  168,  163,  159,  155,  151, /* 110~119C */
   147,  143,  139,  135,  132,  129                          /* 120~125C */
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

    /* Always read temperature (for debug display even when PID is off) */
    uint32_t adc_val = ADC_Read(NTC_ADC_CHANNEL);
    g_cooling_pid.current_temp = NTC_ADC_ToTemp(adc_val);

    if (!g_cooling_pid.enabled)
    {
        g_cooling_pid.output = 0;
        g_cooling_pid.integral = 0;
        return;
    }

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
