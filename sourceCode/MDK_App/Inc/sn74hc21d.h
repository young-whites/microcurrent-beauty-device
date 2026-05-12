/**
 * @file    sn74hc21d.h
 * @brief   SN74HC21D half-bridge driver for microcurrent energy output
 *
 * Only 3 pins use PWM: P07(CCP1B), P00(EPWM2), P06(EPWM3).
 * All other pins are plain GPIO.
 *
 * Hardware:
 *   P07(CCP1B) - PWM 1kHz, controls supply voltage to transformer
 *   P00(EPWM2) - 100Hz, upper half-bridge signal
 *   P06(EPWM3) - 100Hz, lower half-bridge signal
 *   P12(GPIO)  - ION_ENA (upper half-bridge gate enable)
 *   P40(GPIO)  - ION_ENB (lower half-bridge gate enable)
 *   P10(GPIO)  - ION_ENAB (master enable)
 */

#ifndef __SN74HC21D_H
#define __SN74HC21D_H

#include "sys.h"

/** Initialize GPIO, EPWM2/3, CCP1B. Safe state: all off. */
void SN74HC21D_Init(void);

/** Start energy output. gear=0 uses stored/default (50). */
void SN74HC21D_EnergyStart(uint8_t gear);

/** Stop all energy output immediately. */
void SN74HC21D_EnergyStop(void);

/** Set energy gear (0~100). Updates amplitude if already running. */
void SN74HC21D_EnergySetGear(uint8_t gear);

/** TMR0 ISR handler (called from interrupt). */
void SN74HC21D_AmpRampISR(void);

#endif /* __SN74HC21D_H */
