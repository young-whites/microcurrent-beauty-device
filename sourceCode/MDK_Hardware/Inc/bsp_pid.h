#ifndef __BSP_PID_H
#define __BSP_PID_H
#include "sys.h"

/* Cooling temperature range */
#define COOLING_TEMP_MIN        50      // 5.0C (unit: 0.1C)
#define COOLING_TEMP_MAX        200     // 20.0C (unit: 0.1C)
#define COOLING_TEMP_DEFAULT    100     // 10.0C default target

/* PID parameters (fixed-point, scaled by 100) */
#define PID_KP                  200      // Proportional gain
#define PID_KI                  50      // Integral gain
#define PID_KD                  30      // Derivative gain
#define PID_INTEGRAL_MAX        5000    // Anti-windup limit
#define PID_OUTPUT_MIN          0       // Minimum PWM duty (0%)
#define PID_OUTPUT_MAX          100     // Maximum PWM duty (100%)

/* NTC parameters */
#define NTC_ADC_CHANNEL         9       // ADC1_CH9
#define NTC_TABLE_SIZE          51      // 0~50C
#define NTC_TABLE_START_C       0       // Table starts at 0C

/*
 * NTC lookup table: 10K NTC (B=3950) with 10K pull-up, 3.3V ref, 12-bit ADC
 * Index = temperature in Celsius (0~50)
 * Value = expected ADC reading at that temperature
 * R_ntc = 10000 * exp(3950 * (1/(T+273.15) - 1/298.15))
 * ADC = 4095 * 10000 / (R_ntc + 10000)
 */
extern const uint16_t NTC_Table[NTC_TABLE_SIZE];

typedef struct {
    int16_t target_temp;        // Target temperature (0.1C)
    int16_t current_temp;       // Current temperature (0.1C)
    int32_t integral;           // Integral accumulator
    int16_t prev_error;         // Previous error for derivative
    uint8_t output;             // PID output duty (0~100)
    uint8_t enabled;            // Cooling enable flag
} PID_Controller;

extern PID_Controller g_cooling_pid;

void PID_Init(void);
void PID_SetTarget(int16_t target_temp);
int16_t NTC_ADC_ToTemp(uint32_t adc_val);
void PID_Update(void);
void PID_SetEnabled(uint8_t enable);
uint8_t PID_GetOutput(void);

#endif /* __BSP_PID_H */
