/**
 * @file    sn74hc21d.h
 * @brief   SN74HC21D dual 4-input AND gate driver for transformer half-bridge control
 * @note    Generates sine wave approximation via segmented square-wave switching
 *
 * Hardware Connection:
 * ====================
 * SN74HC21D is a dual 4-input AND gate. Two groups control transformer half-bridges.
 *
 *   Upper Half-Bridge (ION+): SN74HC21D pins 9, 10, 12, 13
 *   Lower Half-Bridge (ION-): SN74HC21D pins 1, 2, 4, 5
 *   CRITICAL: Upper and lower half-bridges must NEVER conduct simultaneously.
 *
 * Pin Mapping (SN74HC21D -> MCU):
 *   +----------+--------+-----------+----------------------------------+
 *   | HC21 Pin | MCU IO | Function  | Description                      |
 *   +----------+--------+-----------+----------------------------------+
 *   |    9,5   |  P10   | GPIO      | Enable (upper & lower gate ctrl) |
 *   |    2,12  |  P31   | GPIO      | Enable (upper & lower gate ctrl) |
 *   |    10    |  P06   | EPWM3     | PWM output to upper half-bridge  |
 *   |    13    |  P40   | GPIO      | Enable to upper half-bridge      |
 *   |    1     |  P12   | GPIO      | Enable to lower half-bridge      |
 *   |    4     |  P00   | EPWM2     | PWM output to lower half-bridge  |
 *   +----------+--------+-----------+----------------------------------+
 *
 * Sine Wave Generation:
 *   One sine period is divided into SINE_STEPS segments.
 *   Positive half-cycle: upper half-bridge active, lower off
 *   Negative half-cycle: lower half-bridge active, upper off
 *   EPWM duty cycle is modulated per segment to approximate sine amplitude.
 *   Dead-time is inserted at every half-bridge transition.
 */

#ifndef __SN74HC21D_H
#define __SN74HC21D_H

#include "sys.h"

/* ---- Sine wave parameters ---- */
#define SINE_STEPS              16      /* Segments per sine period          */
#define SINE_DEADTIME_US        50      /* Dead-time between transitions(us) */

/* ---- GPIO pin aliases ---- */
/* P10: controls HC21 pins 9 & 5 (series connection) */
#define ION_ENAB_PORT           GPIO1
#define ION_ENAB_PIN_MSK        GPIO_PIN_0_MSK

/* P31: controls HC21 pins 2 & 12 (series connection) */
#define ION_ENA_PORT            GPIO3
#define ION_ENA_PIN_MSK         GPIO_PIN_1_MSK

/* P40: controls HC21 pin 13 */
#define ION_ENB_PORT            GPIO4
#define ION_ENB_PIN_MSK         GPIO_PIN_0_MSK

/* P12: controls HC21 pin 1 */
#define ION_ENB2_PORT           GPIO1
#define ION_ENB2_PIN_MSK        GPIO_PIN_2_MSK

/* ---- EPWM channel aliases ---- */
#define ION_EPWM_UPPER_MSK      EPWM_CH_3_MSK   /* P06 -> EPWM3 */
#define ION_EPWM_LOWER_MSK      EPWM_CH_2_MSK   /* P00 -> EPWM2 */
#define ION_EPWM_COMBINED_MSK   (ION_EPWM_UPPER_MSK | ION_EPWM_LOWER_MSK)

/* ---- API ---- */

/** Initialize all SN74HC21D GPIO pins and EPWM channels */
void SN74HC21D_Init(void);

/** Start upper half-bridge (ION+), ensure lower is off */
void SN74HC21D_OpenUpper(void);

/** Start lower half-bridge (ION-), ensure upper is off */
void SN74HC21D_OpenLower(void);

/** Emergency stop: disable all outputs immediately */
void SN74HC21D_StopAll(void);

/** Set EPWM duty cycle for a specific half-bridge (0~100) */
void SN74HC21D_SetDutyUpper(uint8_t duty);
void SN74HC21D_SetDutyLower(uint8_t duty);

/** Execute one step of sine wave generation */
void SN74HC21D_SineWaveStep(uint8_t step);

/** Enable/disable sine wave auto-generation via timer interrupt */
void SN74HC21D_SineWaveEnable(uint16_t freq_hz);
void SN74HC21D_SineWaveDisable(void);

/** Set sine wave amplitude scale (0~100%) */
void SN74HC21D_SetAmplitude(uint8_t percent);

/** Low-level dead-time insertion (blocking delay) */
void SN74HC21D_DeadTime(void);

void SN74HC21D_SineWaveISR(void);
#endif /* __SN74HC21D_H */
