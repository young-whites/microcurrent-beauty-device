/**
 * @file    sn74hc21d.h
 * @brief   SN74HC21D half-bridge driver for microcurrent energy output
 *
 * Only 3 pins use PWM: P07(CCP1B), P00(EPWM2), P06(EPWM3).
 * All other pins are plain GPIO.
 *
 * Hardware:
 *   P07(CCP1B) - PWM 1kHz, controls supply voltage to transformer
 *   P00(EPWM2) - carrier freq, upper half-bridge signal
 *   P06(EPWM3) - carrier freq, lower half-bridge signal
 *   P12(GPIO)  - ION_ENA (upper half-bridge gate enable)
 *   P40(GPIO)  - ION_ENB (lower half-bridge gate enable)
 *   P10(GPIO)  - ION_ENAB (master enable)
 */

#ifndef __SN74HC21D_H
#define __SN74HC21D_H

#include "sys.h"

/* ---- Output mode definitions ---- */
#define OUTPUT_MODE_1           0   /* Mode 1: 100Hz carrier, ~1Hz output */
#define OUTPUT_MODE_2           1   /* Mode 2: 200Hz carrier, ~2Hz output */
#define OUTPUT_MODE_COUNT       2
#define OUTPUT_MODE_DEFAULT     OUTPUT_MODE_1

/* ---- Mode configuration structure ---- */
typedef struct {
    uint32_t epwm_carrier_freq;     /* EPWM2/3 carrier frequency (Hz) */
    uint32_t epwm4_freq;            /* EPWM4 modulation frequency (Hz) */
    uint32_t tmr0_reload;           /* TMR0 reload value (~step duration) */
    uint8_t  epwm2_duty;            /* EPWM2 symmetric duty scale */
    uint8_t  epwm3_duty;            /* EPWM3 symmetric duty scale */
    uint8_t  epwm4_duty;            /* EPWM4 symmetric duty scale */
} OutputModeConfig;

/** Initialize GPIO, EPWM2/3, CCP1B. Safe state: all off. */
void SN74HC21D_Init(void);

/** Start energy output. gear=0 uses stored/default (50). */
void SN74HC21D_EnergyStart(uint8_t gear);

/** Stop all energy output immediately. */
void SN74HC21D_EnergyStop(void);

/** Set energy gear (0~100). Updates amplitude if already running. */
void SN74HC21D_EnergySetGear(uint8_t gear);

/** Set output mode (OUTPUT_MODE_1 or OUTPUT_MODE_2). */
void SN74HC21D_SetMode(uint8_t mode);

/** Get current output mode. */
uint8_t SN74HC21D_GetMode(void);

/** TMR0 ISR handler (called from interrupt). */
void SN74HC21D_AmpRampISR(void);

#endif /* __SN74HC21D_H */
