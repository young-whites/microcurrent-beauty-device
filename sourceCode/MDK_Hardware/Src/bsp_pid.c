#include "bsp_pid.h"
#include "adc.h"

PID_Controller g_cooling_pid;

/*
 * NTC lookup table: 10K NTC (B=3950), 10K pull-up, 3.3V, 12-bit ADC
 * Index = temperature in Celsius (0~125)
 * ADC value DECREASES as temperature rises
 */
const uint16_t NTC_Table[NTC_TABLE_SIZE] = {
  3384, 3285, 3189, 3095, 3004, 2916, 2830, 2748, 2668, 2589, /* -2~ 7C */
  2350, 2280, 2213, 2148, 2086, 2025, 1967, 1911, 1857, 1804, /* 8~17C  */
  1661, 1615, 1571, 1528, 1487, 1448, 1409, 1373, 1338, 1304, /* 18~27C */
  1209, 1178, 1148, 1120, 1092, 1066, 1040, 1015,  991,  968, /* 28~37C */
   891,  870,  850,  832,  813,  795,  778,  761,  746,  730, /* 38~47C */
   670,  657,  644,  631,  619,  607,  596,  585,  574,  564, /* 48~57C */
   519,  509,  501,  492,  483,  475,  468,  460,  452,  446, /* 58~67C */
   411,  405,  398,  393,  386,  381,  375,  370,  364,  359, /* 68~77C */
   332,  328,  323,  320,  316,  311,  308,  304,  300,  297, /* 78~87C */
   276,  273,  271,  267,  264,  262,  259,  256,  253,  251, /* 88~97C */
   234,  232,  230,  228,  226,  223,  221,  219,  217,  215, /* 98~107C */
   201,  199,  198,  196,  194,  193,  190,  189,  187,  186, /* 108~117C */
   175,  174,  172,  171,  169,  167,  166,  165                          /* 118~125C */
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

    /* Clamp: descending table - high ADC = cold, low ADC = hot */
    if (adc >= NTC_Table[0])
    {
        return NTC_TABLE_START_C * 10;           /* Colder than 0C */
    }
    if (adc <= NTC_Table[NTC_TABLE_SIZE - 1])
    {
        return (NTC_TABLE_START_C + NTC_TABLE_SIZE - 1) * 10; /* Hotter than 125C */
    }

    /* Forward search: descending table, ADC decreases as temp rises */
    for (temp_c = 0; temp_c < NTC_TABLE_SIZE - 1; temp_c++)
    {
        if (adc <= NTC_Table[temp_c] && adc > NTC_Table[temp_c + 1])
        {
            int16_t adc_diff = NTC_Table[temp_c] - NTC_Table[temp_c + 1];
            int16_t adc_offs = NTC_Table[temp_c] - adc;
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

    /* Full power cooling when target <= 0C */
    if (g_cooling_pid.target_temp <= 0)
    {
        g_cooling_pid.output = PID_OUTPUT_MAX;
        g_cooling_pid.integral = 0;
        g_cooling_pid.prev_error = 0;
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
