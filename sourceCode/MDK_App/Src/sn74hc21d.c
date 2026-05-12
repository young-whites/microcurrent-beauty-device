/**
 * @file    sn74hc21d.c
 * @brief   SN74HC21D half-bridge driver for microcurrent energy output
 *
 * Architecture (matches Version_2.1 reference):
 *   EPWM2(P00) and EPWM3(P06) always running at 1645Hz carrier.
 *   GPIO enables (P12/P40/P10) select which half-bridge is active.
 *   CCP1B(P07) duty controls supply voltage -> output amplitude.
 *   TMR0 ISR ramps CCP1B duty 0->peak->0, then toggles half-bridge.
 */

#include "sn74hc21d.h"
#include "epwm.h"

/* =========================================================================
 * Constants
 * ========================================================================= */
#define AMP_RAMP_SIZE       60
#define EPWM_CARRIER_FREQ   1645    /* Match reference Water_PwmFreq */
#define TMR0_RELOAD         1500    /* ~0.5ms per step at Pclk=3MHz */

/* =========================================================================
 * Module state
 * ========================================================================= */
static volatile uint8_t  g_running       = 0;
static volatile uint8_t  g_channel       = 0;   /* 0=upper(A), 1=lower(B) */
static volatile uint16_t g_ramp_idx      = 0;
static volatile uint16_t g_ramp_peak     = 30;
static volatile uint8_t  g_gear          = 0;

/* =========================================================================
 * Internal helpers
 * ========================================================================= */

/** Set P07 (CCP1B) duty from ramp index. */
static void SetAmpDuty(uint16_t idx)
{
    uint32_t period, duty;
    if (idx > g_ramp_peak) idx = g_ramp_peak;
    if (g_ramp_peak == 0) return;
    period = CCP_ReadLoad(CCP1);
    duty = period * idx / g_ramp_peak;
    CCP_ConfigCompare(CCP1, CCPxB, (uint16_t)duty);
}

/** Select upper half-bridge (A). GPIO only - EPWM always running. */
static void SelectA(void)
{
    GPIO_ResetPin(GPIO4, GPIO_PIN_0_MSK);     /* ION_ENB = 0 */
    GPIO_SetPin(GPIO1, GPIO_PIN_2_MSK);       /* ION_ENA = 1 */
    GPIO_SetPin(GPIO1, GPIO_PIN_0_MSK);       /* ION_ENAB = 1 */
}

/** Select lower half-bridge (B). GPIO only - EPWM always running. */
static void SelectB(void)
{
    GPIO_ResetPin(GPIO1, GPIO_PIN_2_MSK);     /* ION_ENA = 0 */
    GPIO_SetPin(GPIO4, GPIO_PIN_0_MSK);       /* ION_ENB = 1 */
    GPIO_SetPin(GPIO1, GPIO_PIN_0_MSK);       /* ION_ENAB = 1 */
}

/* =========================================================================
 * Public API
 * ========================================================================= */

void SN74HC21D_Init(void)
{
    /* GPIO enables */
    SYS_SET_IOCFG(IOP12CFG, SYS_IOCFG_P12_GPIO);
    GPIO_CONFIG_IO_MODE(GPIO1, GPIO_PIN_2, GPIO_MODE_OUTPUT_PUSH_PULL);
    GPIO_ResetPin(GPIO1, GPIO_PIN_2_MSK);

    SYS_SET_IOCFG(IOP40CFG, SYS_IOCFG_P40_GPIO);
    GPIO_CONFIG_IO_MODE(GPIO4, GPIO_PIN_0, GPIO_MODE_OUTPUT_PUSH_PULL);
    GPIO_ResetPin(GPIO4, GPIO_PIN_0_MSK);

    SYS_SET_IOCFG(IOP10CFG, SYS_IOCFG_P10_GPIO);
    GPIO_CONFIG_IO_MODE(GPIO1, GPIO_PIN_0, GPIO_MODE_OUTPUT_PUSH_PULL);
    GPIO_ResetPin(GPIO1, GPIO_PIN_0_MSK);

    /* EPWM signal pins */
    SYS_SET_IOCFG(IOP00CFG, SYS_IOCFG_P00_EPWM2);
    SYS_SET_IOCFG(IOP06CFG, SYS_IOCFG_P06_EPWM3);

    /* Configure EPWM2 and EPWM3 at carrier frequency */
    {
        INT32U freq[6];
        INT8U  div[6];
        uint8_t i;
        for (i = 0; i < 6; i++) { freq[i] = EPWM_CARRIER_FREQ; div[i] = 1; }
        EPWM_Config_Independent_Mode(EPWM_CH_2_MSK | EPWM_CH_3_MSK, freq, div);
        EPWM_ConfigChannelSymDutyScale(EPWM2, 48);
        EPWM_ConfigChannelSymDutyScale(EPWM3, 52);
    }

    /* ---- CCP PWM init (match reference CCP_Config_PWM_Mode) ---- */
    /* P07 as CCP1B for energy voltage control */
    SYS_SET_IOCFG(IOP07CFG, SYS_IOCFG_P07_CCP1B);

    /* P30 as CCP0A for cooling (already done by Cooling_Init, but ensure) */
    SYS_SET_IOCFG(IOP30CFG, SYS_IOCFG_P30_CCP0A);

    SYS_EnablePeripheralClk(SYS_CLK_CCP_MSK);

    /* Configure both CCP0 and CCP1 clocks */
    CCP_ConfigCLK(CCP0, CCP_CLK_DIV_1, CCP_RELOAD_CCPLOAD, 15000); /* 200Hz */
    CCP_ConfigCLK(CCP1, CCP_CLK_DIV_1, CCP_RELOAD_CCPLOAD, 3000);  /* 1kHz */

    /* PWM mode for both */
    CCP_EnablePWMMode(CCP0);
    CCP_EnablePWMMode(CCP1);

    /* Set all 4 channels to 0% duty */
    CCP_ConfigDutyScale(CCP0, CCPxA, 0);
    CCP_ConfigDutyScale(CCP0, CCPxB, 0);
    CCP_ConfigDutyScale(CCP1, CCPxA, 0);
    CCP_ConfigDutyScale(CCP1, CCPxB, 0);

    /* Disable reverse output for all channels */
    CCP_DisableReverseOutput(CCP0, CCPxA);
    CCP_DisableReverseOutput(CCP0, CCPxB);
    CCP_DisableReverseOutput(CCP1, CCPxA);
    CCP_DisableReverseOutput(CCP1, CCPxB);

    /* CCP interrupt */
    NVIC_EnableIRQ(CCP_IRQn);
    NVIC_SetPriority(CCP_IRQn, 0);

    /* Enable then stop (module ready, start on demand) */
    CCP_EnableRun(CCP0);
    CCP_EnableRun(CCP1);
    CCP_Stop(CCP0);
    CCP_Stop(CCP1);

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

    /* Ensure P07 pin mux and CCP1 are running */
    SYS_SET_IOCFG(IOP07CFG, SYS_IOCFG_P07_CCP1B);
    CCP_Start(CCP1);
    CCP_EnableRun(CCP1);
    SetAmpDuty(0);

    /* Start EPWM2 and EPWM3 - they stay running forever */
    EPWM_ClearDownCmpIntFlag(EPWM2);
    EPWM_EnableDownCmpInt(EPWM_CH_2_MSK);
    EPWM_Start(EPWM_CH_2_MSK);

    EPWM_ClearDownCmpIntFlag(EPWM3);
    EPWM_EnableDownCmpInt(EPWM_CH_3_MSK);
    EPWM_Start(EPWM_CH_3_MSK);

    /* Select channel A to start */
    SelectA();

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

    /* Stop EPWM */
    EPWM_Stop(EPWM_CH_2_MSK | EPWM_CH_3_MSK);

    /* Disable GPIOs */
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

/* TMR0 ISR: ramp CCP1B duty, toggle half-bridge at boundaries */
void SN74HC21D_AmpRampISR(void)
{
    if (!g_running) return;
    if (g_gear == 0) { SetAmpDuty(0); return; }

    if (g_ramp_idx <= g_ramp_peak)
    {
        SetAmpDuty(g_ramp_idx);
        g_ramp_idx++;
    }
    else if (g_ramp_idx <= g_ramp_peak * 2)
    {
        SetAmpDuty(g_ramp_peak * 2 - g_ramp_idx);
        g_ramp_idx++;
    }
    else
    {
        g_ramp_idx = 0;
        if (g_channel == 0) { g_channel = 1; SelectB(); }
        else                { g_channel = 0; SelectA(); }
    }
}
