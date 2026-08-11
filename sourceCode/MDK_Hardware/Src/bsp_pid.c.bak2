#include "bsp_pid.h"
#include "adc.h"

PID_Controller g_cooling_pid;

/*
 * NTC lookup table: 10K NTC (B=3950), 10K pull-up, 3.3V, 12-bit ADC
 * Index = temperature in Celsius (0~125)
 * ADC value DECREASES as temperature rises
 */
const uint16_t NTC_Table[NTC_TABLE_SIZE] = {
  3076, 2986, 2899, 2814, 2731, 2651, 2573, 2498, 2425, 2354, /* -2~ 7C */
  2136, 2073, 2012, 1953, 1896, 1841, 1788, 1737, 1688, 1640, /* 8~17C  */
  1510, 1468, 1428, 1389, 1352, 1316, 1281, 1248, 1216, 1185, /* 18~27C */
  1099, 1071, 1044, 1018,  993,  969,  945,  923,  901,  880, /* 28~37C */
   810,  791,  773,  756,  739,  723,  707,  692,  678,  664, /* 38~47C */
   609,  597,  585,  574,  563,  552,  542,  532,  522,  513, /* 48~57C */
   472,  463,  455,  447,  439,  432,  425,  418,  411,  405, /* 58~67C */
   374,  368,  362,  357,  351,  346,  341,  336,  331,  326, /* 68~77C */
   302,  298,  294,  291,  287,  283,  280,  276,  273,  270, /* 78~87C */
   251,  248,  246,  243,  240,  238,  235,  233,  230,  228, /* 88~97C */
   213,  211,  209,  207,  205,  203,  201,  199,  197,  195, /* 98~107C */
   183,  181,  180,  178,  176,  175,  173,  172,  170,  169, /* 108~117C */
   159,  158,  156,  155,  154,  152,  151,  150                          /* 118~125C */
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
