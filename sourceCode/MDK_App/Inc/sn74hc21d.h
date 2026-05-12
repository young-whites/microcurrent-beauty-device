/**
 * @file    sn74hc21d.h
 * @brief   SN74HC21D half-bridge driver for microcurrent energy output
 *
 * Hardware Connection:
 * ====================
 * SN74HC21D is a dual 4-input AND gate controlling transformer half-bridges.
 * Upper and lower half-bridges must NEVER conduct simultaneously.
 *
 * Pin Mapping:
 *   +----------+--------+-----------+---------------------------------------+
 *   | MCU IO   | Func   | Peripheral| Description                           |
 *   +----------+--------+-----------+---------------------------------------+
 *   | P07      | CCP1B  | PWM 1kHz  | Half-bridge supply voltage (energy)   |
 *   | P00      | EPWM2  | 100Hz     | Upper half-bridge frequency signal    |
 *   | P06      | EPWM3  | 100Hz     | Lower half-bridge frequency signal    |
 *   | P31      | EPWM4  | 200Hz     | Duty modulation signal                |
 *   | P12      | GPIO   | Output    | ION_ENA (upper half-bridge enable)    |
 *   | P40      | GPIO   | Output    | ION_ENB (lower half-bridge enable)    |
 *   | P10      | GPIO   | Output    | ION_ENAB (master enable)              |
 *   | P30      | CCP0A  | PWM 1kHz  | Cooling pad (NOT modified here)       |
 *   +----------+--------+-----------+---------------------------------------+
 *
 * Energy Control Principle:
 *   - P07(CCP1B) duty cycle controls supply voltage to half-bridge -> amplitude
 *   - EPWM2/3 alternate to create positive/negative half-waves
 *   - TMR0 ISR drives amplitude ramp: 0 -> peak -> 0 on each half-wave
 *   - Gear (0~100) maps to peak amplitude via ramp table
 */

#ifndef __SN74HC21D_H
#define __SN74HC21D_H

#include "sys.h"

/* ---- GPIO pin definitions (for reference) ---- */
/* P12: ION_ENA  - upper half-bridge enable (GPIO1, PIN_2) */
/* P40: ION_ENB  - lower half-bridge enable (GPIO4, PIN_0) */
/* P10: ION_ENAB - master enable            (GPIO1, PIN_0) */

/* ---- EPWM channel definitions (for reference) ---- */
/* EPWM2 (P00): upper half-bridge signal, 100Hz */
/* EPWM3 (P06): lower half-bridge signal, 100Hz */
/* EPWM4 (P31): duty modulation, 200Hz */

/* ---- API ---- */

/**
 * @brief  Initialize SN74HC21D half-bridge driver
 *         Configures P12/P40/P10 as GPIO output
 *         Configures P00=EPWM2, P06=EPWM3, P31=EPWM4 at 100Hz
 *         Ensures P07=CCP1B is ready for energy voltage control
 */
void SN74HC21D_Init(void);

/**
 * @brief  Start energy output
 * @param  gear: 0~100, amplitude level
 *         gear=0 stops output immediately
 *         Starts EPWM2, CCP1B, and TMR0 for amplitude ramping
 */
void SN74HC21D_EnergyStart(uint8_t gear);

/**
 * @brief  Stop energy output completely
 *         Stops TMR0, EPWM2/3/4, zeros CCP1B, disables enables
 */
void SN74HC21D_EnergyStop(void);

/**
 * @brief  Set energy gear level (0~100)
 * @param  gear: amplitude peak = 10 + gear
 * @note   If running, updates ramp peak for next cycle
 */
void SN74HC21D_EnergySetGear(uint8_t gear);

/**
 * @brief  TMR0 ISR amplitude ramp handler
 * @note   Call from TMR0_IRQHandler in cms32f033_it.c
 *         Drives 0->peak->0 amplitude envelope and half-bridge alternation
 */
void SN74HC21D_AmpRampISR(void);

#endif /* __SN74HC21D_H */
