/**
 * @file    sn74hc21d.c
 * @brief   SN74HC21D half-bridge driver for microcurrent energy output
 *
 * Pins:
 *   P07(CCP1B) - PWM 1kHz, duty controls supply voltage (energy amplitude)
 *   P00(EPWM2) - Upper half-bridge signal (100Hz square wave)
 *   P06(EPWM3) - Lower half-bridge signal (100Hz square wave)
 *   P12(GPIO)  - ION_ENA (upper half-bridge enable)
 *   P40(GPIO)  - ION_ENB (lower half-bridge enable)
 *   P10(GPIO)  - ION_ENAB (master enable)
 *
 * Only P07, P00, P06 are configured as PWM. All others are GPIO.
 */

#include "sn74hc21d.h"
#include "epwm.h"

/* =========================================================================
 * Constants
 * ========================================================================= */
#define AMP_RAMP_SIZE       60      /* Steps per ramp (0->peak->0) */
#define EPWM_BASE_FREQ      100     /* EPWM signal frequency (Hz) */
#define TMR0_RELOAD         1500    /* ~0.5ms per step at Pclk=3MHz */

/* =========================================================================
 * Module state
 * ========================================================================= */
static volatile uint8_t  g_running       = 0;
static volatile uint8_t  g_channel       = 0;   /* 0=upper(A), 1=lower(B) */
static volatile uint16_t g_ramp_idx      = 0;   /* Current ramp step */
static volatile uint16_t g_ramp_peak     = 30;  /* Peak step (10+gear) */
static volatile uint8_t  g_gear          = 0;    /* Gear 0~100 */

/* =========================================================================
 * Internal helpers
 * ========================================================================= */

/** Set P07 (CCP1B) duty from ramp index. index 0=0%, index peak=100%. */
static void SetAmpDuty(uint16_t idx)
{
    uint32_t period, duty;
    if (idx > g_ramp_peak) idx = g_ramp_peak;
    period = CCP_ReadLoad(CCP1);
    duty = period * idx / g_ramp_peak;
    CCP_ConfigCompare(CCP1, CCPxB, (uint16_t)duty);
}

/** Switch to upper half-bridge (Channel A). Called only ONCE per transition. */
static void SelectA(void)
{
    GPIO_ResetPin(GPIO4, GPIO_PIN_0_MSK);     /* ION_ENB = 0 */
    GPIO_SetPin(GPIO1, GPIO_PIN_2_MSK);       /* ION_ENA = 1 */
    GPIO_SetPin(GPIO1, GPIO_PIN_0_MSK);       /* ION_ENAB = 1 */
    EPWM_Stop(EPWM_CH_3_MSK);
    EPWM_Start(EPWM_CH_2_MSK);
}

/** Switch to lower half-bridge (Channel B). Called only ONCE per transition. */
static void SelectB(void)
{
    GPIO_ResetPin(GPIO1, GPIO_PIN_2_MSK);     /* ION_ENA = 0 */
    GPIO_SetPin(GPIO4, GPIO_PIN_0_MSK);       /* ION_ENB = 1 */
    GPIO_SetPin(GPIO1, GPIO_PIN_0_MSK);       /* ION_ENAB = 1 */
    EPWM_Stop(EPWM_CH_2_MSK);
    EPWM_Start(EPWM_CH_3_MSK);
}

/* =========================================================================
 * Public API
 * ========================================================================= */

void SN74HC21D_Init(void)
{
    /* GPIO enables: P12=ION_ENA, P40=ION_ENB, P10=ION_ENAB */
    SYS_SET_IOCFG(IOP12CFG, SYS_IOCFG_P12_GPIO);
    GPIO_CONFIG_IO_MODE(GPIO1, GPIO_PIN_2, GPIO_MODE_OUTPUT_PUSH_PULL);
    GPIO_ResetPin(GPIO1, GPIO_PIN_2_MSK);

    SYS_SET_IOCFG(IOP40CFG, SYS_IOCFG_P40_GPIO);
    GPIO_CONFIG_IO_MODE(GPIO4, GPIO_PIN_0, GPIO_MODE_OUTPUT_PUSH_PULL);
    GPIO_ResetPin(GPIO4, GPIO_PIN_0_MSK);

    SYS_SET_IOCFG(IOP10CFG, SYS_IOCFG_P10_GPIO);
    GPIO_CONFIG_IO_MODE(GPIO1, GPIO_PIN_0, GPIO_MODE_OUTPUT_PUSH_PULL);
    GPIO_ResetPin(GPIO1, GPIO_PIN_0_MSK);

    /* EPWM signal pins: P00=EPWM2, P06=EPWM3 */
    SYS_SET_IOCFG(IOP00CFG, SYS_IOCFG_P00_EPWM2);
    SYS_SET_IOCFG(IOP06CFG, SYS_IOCFG_P06_EPWM3);

    /* Configure EPWM2 and EPWM3 at 100Hz */
    {
        INT32U freq[6];
        INT8U  div[6];
        uint8_t i;
        for (i = 0; i < 6; i++) { freq[i] = EPWM_BASE_FREQ; div[i] = 1; }
        EPWM_Config_Independent_Mode(EPWM_CH_2_MSK | EPWM_CH_3_MSK, freq, div);
        EPWM_ConfigChannelSymDutyScale(EPWM2, 48);
        EPWM_ConfigChannelSymDutyScale(EPWM3, 52);
        EPWM_Stop(EPWM_CH_2_MSK | EPWM_CH_3_MSK);
    }

    /* P07 as CCP1B for energy voltage (already configured by Energy_Init) */
    CCP_Start(CCP1);
    CCP_ConfigCompare(CCP1, CCPxB, 0);

    g_running = 0;
    g_gear = 0;
    g_ramp_peak = 30;
}

void SN74HC21D_EnergyStart(uint8_t gear)
{
    if (gear > 0) {
        if (gear > 100) gear = 100;
        g_gear = gear;
    }
    if (g_gear == 0) g_gear = 50;

    g_ramp_idx = 0;
    g_ramp_peak = 10 + (uint16_t)g_gear;
    if (g_ramp_peak > AMP_RAMP_SIZE) g_ramp_peak = AMP_RAMP_SIZE;
    g_channel = 0;

    /* Start with channel A */
    SelectA();
    SetAmpDuty(0);

    /* Start TMR0 for amplitude ramp */
    SYS_EnablePeripheralClk(SYS_CLK_TIMER01_MSK);
    TMR_ConfigClk(TMR0, TMR_CLK_SEL_APB, TMR_CLK_DIV_1);
    TMR_ConfigRunMode(TMR0, TMR_COUNT_PERIOD_MODE, TMR_BIT_32_MODE);
    TMR_DisableOneShotMode(TMR0);
    TMR_SetPeriod(TMR0, TMR0_RELOAD);
    TMR_ClearOverflowIntFlag(TMR0);
    TMR_EnableOverflowInt(TMR0);
    NVIC_EnableIRQ(TMR0_IRQn);
    NVIC_SetPriority(TMR0_IRQn, 1);

    g_running = 1;
    TMR_Start(TMR0);
}

void SN74HC21D_EnergyStop(void)
{
    g_running = 0;
    TMR_Stop(TMR0);
    TMR_DisableOverflowInt(TMR0);
    TMR_ClearOverflowIntFlag(TMR0);

    CCP_ConfigCompare(CCP1, CCPxB, 0);
    EPWM_Stop(EPWM_CH_2_MSK | EPWM_CH_3_MSK);
    GPIO_ResetPin(GPIO1, GPIO_PIN_2_MSK);
    GPIO_ResetPin(GPIO4, GPIO_PIN_0_MSK);
    GPIO_ResetPin(GPIO1, GPIO_PIN_0_MSK);
}

void SN74HC21D_EnergySetGear(uint8_t gear)
{
    if (gear > 100) gear = 100;
    g_gear = gear;
    if (g_running) {
        g_ramp_peak = 10 + (uint16_t)gear;
        if (g_ramp_peak > AMP_RAMP_SIZE) g_ramp_peak = AMP_RAMP_SIZE;
    }
}

/* =========================================================================
 * TMR0 ISR: drives amplitude ramp 0->peak->0, then switches half-bridge
 *
 * Each call does at most ONE SetAmpDuty (CCP compare write).
 * Channel switch happens only at ramp boundaries (rare).
 * ========================================================================= */
void SN74HC21D_AmpRampISR(void)
{
    if (!g_running) return;
    if (g_gear == 0) { SetAmpDuty(0); return; }

    if (g_ramp_idx <= g_ramp_peak)
    {
        /* Rising phase: 0 -> peak */
        SetAmpDuty(g_ramp_idx);
        g_ramp_idx++;
    }
    else if (g_ramp_idx <= g_ramp_peak * 2)
    {
        /* Falling phase: peak -> 0 */
        SetAmpDuty(g_ramp_peak * 2 - g_ramp_idx);
        g_ramp_idx++;
    }
    else
    {
        /* Half-wave done: switch channel */
        g_ramp_idx = 0;
        if (g_channel == 0) {
            g_channel = 1;
            SelectB();
        } else {
            g_channel = 0;
            SelectA();
        }
    }
}
