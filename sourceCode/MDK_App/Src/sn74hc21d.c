/**
 * @file    sn74hc21d.c
 * @brief   SN74HC21D driver: half-bridge control + sine wave generation
 *
 * Sine wave is approximated by modulating EPWM duty cycle across segments.
 * Upper half-bank (ION+) handles positive half-cycle.
 * Lower half-bank (ION-) handles negative half-cycle.
 * Dead-time is enforced at every transition to prevent shoot-through.
 */

#include "sn74hc21d.h"
#include "epwm.h"

/* ---- Internal state ---- */
static volatile uint8_t  g_sine_running  = 0;
static volatile uint8_t  g_sine_step     = 0;

/* Pre-computed sine duty table (16 steps, normalized to 0~100)
 * Values = |sin(step * 2*PI / 16)| * 100
 * step: 0..7 = positive half (upper bridge), 8..15 = negative half (lower bridge) */
static const uint8_t g_sine_duty[SINE_STEPS] = {
     0,  38,  71,  92, 100,  92,  71,  38,   /* positive half-cycle */
     0,  38,  71,  92, 100,  92,  71,  38    /* negative half-cycle */
};

/*****************************************************************************
 * @brief  Dead-time insertion (blocking delay)
 * @note   Simple busy-wait; replace with timer-based delay if available.
 *         At Pclk=3MHz, 150 iterations ~ 50us
 *****************************************************************************/
void SN74HC21D_DeadTime(void)
{
    volatile uint32_t i;
    for (i = 0; i < 150; i++) { __NOP(); }
}

/*****************************************************************************
 * @brief  Disable both half-bridges (gate signals off, EPWM stopped)
 *****************************************************************************/
static void SN74HC21D_DisableAll(void)
{
    /* Disable upper gate enables */
    GPIO_ResetPin(ION_ENA_PORT,  ION_ENA_PIN_MSK);     /* P31 low */
    GPIO_ResetPin(ION_ENAB_PORT, ION_ENAB_PIN_MSK);    /* P10 low */
    GPIO_ResetPin(ION_ENB_PORT,  ION_ENB_PIN_MSK);     /* P40 low */

    /* Disable lower gate enables */
    GPIO_ResetPin(ION_ENB2_PORT, ION_ENB2_PIN_MSK);    /* P12 low */

    /* Stop both EPWM outputs */
    EPWM_Stop(ION_EPWM_COMBINED_MSK);
    EPWM_DisableOutput(ION_EPWM_COMBINED_MSK);
}

/*****************************************************************************
 * @brief  Initialize GPIO pins and EPWM channels for SN74HC21D
 *****************************************************************************/
void SN74HC21D_Init(void)
{
    /* ---- Configure GPIO pins as push-pull output ---- */

    /* P10 -> GPIO output (ION_ENAB) */
    SYS_SET_IOCFG(IOP10CFG, SYS_IOCFG_P10_GPIO);
    GPIO_CONFIG_IO_MODE(ION_ENAB_PORT, GPIO_PIN_0, GPIO_MODE_OUTPUT_PUSH_PULL);
    GPIO_ResetPin(ION_ENAB_PORT, ION_ENAB_PIN_MSK);

    /* P31 -> GPIO output (ION_ENA) */
    SYS_SET_IOCFG(IOP31CFG, SYS_IOCFG_P31_GPIO);
    GPIO_CONFIG_IO_MODE(ION_ENA_PORT, GPIO_PIN_1, GPIO_MODE_OUTPUT_PUSH_PULL);
    GPIO_ResetPin(ION_ENA_PORT, ION_ENA_PIN_MSK);

    /* P40 -> GPIO output (ION_ENB) */
    SYS_SET_IOCFG(IOP40CFG, SYS_IOCFG_P40_GPIO);
    GPIO_CONFIG_IO_MODE(ION_ENB_PORT, GPIO_PIN_0, GPIO_MODE_OUTPUT_PUSH_PULL);
    GPIO_ResetPin(ION_ENB_PORT, ION_ENB_PIN_MSK);

    /* P12 -> GPIO output (ION_ENB2) */
    SYS_SET_IOCFG(IOP12CFG, SYS_IOCFG_P12_GPIO);
    GPIO_CONFIG_IO_MODE(ION_ENB2_PORT, GPIO_PIN_2, GPIO_MODE_OUTPUT_PUSH_PULL);
    GPIO_ResetPin(ION_ENB2_PORT, ION_ENB2_PIN_MSK);

    /* ---- Configure EPWM pins ---- */

    /* P00 -> EPWM2 (lower half-bridge PWM) */
    SYS_SET_IOCFG(IOP00CFG, SYS_IOCFG_P00_EPWM2);

    /* P06 -> EPWM3 (upper half-bridge PWM) */
    SYS_SET_IOCFG(IOP06CFG, SYS_IOCFG_P06_EPWM3);

    /* ---- Configure EPWM2 & EPWM3 in independent mode ---- */
    {
        INT32U freq[6];
        INT8U  div[6];
        uint8_t i;

        for (i = 0; i < 6; i++) {
            freq[i] = 3000;  /* Default 3kHz carrier */
            div[i]  = 1;
        }

        EPWM_Config_Independent_Mode(ION_EPWM_COMBINED_MSK, freq, div);

        /* Start EPWM but keep output disabled until explicitly enabled */
        EPWM_Start(ION_EPWM_COMBINED_MSK);
        EPWM_DisableOutput(ION_EPWM_COMBINED_MSK);
    }

    /* Ensure everything is in safe state */
    SN74HC21D_DisableAll();
}

/*****************************************************************************
 * @brief  Activate upper half-bank (ION+)
 * @note   Ensures lower half-bank is OFF first (dead-time enforced)
 *****************************************************************************/
void SN74HC21D_OpenUpper(void)
{
    /* Step 1: disable lower half-bridge */
    GPIO_ResetPin(ION_ENB2_PORT, ION_ENB2_PIN_MSK);    /* P12 low */
    EPWM_DisableOutput(ION_EPWM_LOWER_MSK);

    /* Step 2: dead-time */
    SN74HC21D_DeadTime();

    /* Step 3: enable upper gate signals */
    GPIO_SetPin(ION_ENA_PORT,  ION_ENA_PIN_MSK);       /* P31 high */
    GPIO_SetPin(ION_ENAB_PORT, ION_ENAB_PIN_MSK);      /* P10 high */
    GPIO_SetPin(ION_ENB_PORT,  ION_ENB_PIN_MSK);       /* P40 high */

    /* Step 4: enable upper EPWM output */
    EPWM_EnableOutput(ION_EPWM_UPPER_MSK);
}

/*****************************************************************************
 * @brief  Activate lower half-bank (ION-)
 * @note   Ensures upper half-bank is OFF first (dead-time enforced)
 *****************************************************************************/
void SN74HC21D_OpenLower(void)
{
    /* Step 1: disable upper half-bridge */
    GPIO_ResetPin(ION_ENA_PORT,  ION_ENA_PIN_MSK);     /* P31 low */
    GPIO_ResetPin(ION_ENAB_PORT, ION_ENAB_PIN_MSK);    /* P10 low */
    GPIO_ResetPin(ION_ENB_PORT,  ION_ENB_PIN_MSK);     /* P40 low */
    EPWM_DisableOutput(ION_EPWM_UPPER_MSK);

    /* Step 2: dead-time */
    SN74HC21D_DeadTime();

    /* Step 3: enable lower gate signal */
    GPIO_SetPin(ION_ENB2_PORT, ION_ENB2_PIN_MSK);      /* P12 high */

    /* Step 4: enable lower EPWM output */
    EPWM_EnableOutput(ION_EPWM_LOWER_MSK);
}

/*****************************************************************************
 * @brief  Emergency stop: all outputs off immediately
 *****************************************************************************/
void SN74HC21D_StopAll(void)
{
    SN74HC21D_DisableAll();
}

/*****************************************************************************
 * @brief  Set EPWM duty cycle for upper half-bridge
 * @param  duty: 0~100 (percentage)
 *****************************************************************************/
void SN74HC21D_SetDutyUpper(uint8_t duty)
{
    uint32_t period;
    uint32_t compare;

    if (duty > 100) duty = 100;
    if (duty == 0) {
        EPWM_SET_COMPARE(EPWM3, 0);
        return;
    }

    period  = EPWM->PERIOD[EPWM3];
    compare = (period * duty) / 100;
    if (compare == 0 && duty > 0) compare = 1;
    EPWM_SET_COMPARE(EPWM3, (uint16_t)compare);
}

/*****************************************************************************
 * @brief  Set EPWM duty cycle for lower half-bridge
 * @param  duty: 0~100 (percentage)
 *****************************************************************************/
void SN74HC21D_SetDutyLower(uint8_t duty)
{
    uint32_t period;
    uint32_t compare;

    if (duty > 100) duty = 100;
    if (duty == 0) {
        EPWM_SET_COMPARE(EPWM2, 0);
        return;
    }

    period  = EPWM->PERIOD[EPWM2];
    compare = (period * duty) / 100;
    if (compare == 0 && duty > 0) compare = 1;
    EPWM_SET_COMPARE(EPWM2, (uint16_t)compare);
}

/*****************************************************************************
 * @brief  Execute one step of the sine wave generation
 * @param  step: 0 .. (SINE_STEPS-1)
 *
 * Step 0~7:  positive half-cycle -> upper half-bridge active
 * Step 8~15: negative half-cycle -> lower half-bridge active
 *****************************************************************************/
void SN74HC21D_SineWaveStep(uint8_t step)
{
    uint8_t duty;

    step %= SINE_STEPS;
    duty  = g_sine_duty[step];

    if (step < (SINE_STEPS / 2))
    {
        /* Positive half-cycle: use upper half-bridge (ION+) */
        SN74HC21D_SetDutyUpper(duty);
        SN74HC21D_OpenUpper();
    }
    else
    {
        /* Negative half-cycle: use lower half-bridge (ION-) */
        SN74HC21D_SetDutyLower(duty);
        SN74HC21D_OpenLower();
    }
}

/*****************************************************************************
 * @brief  Timer interrupt handler for sine wave stepping
 * @note   Call this from the timer ISR (e.g. TIM0 or TIM1)
 *         Each call advances the sine wave by one step
 *****************************************************************************/
void SN74HC21D_SineWaveISR(void)
{
    if (!g_sine_running) return;

    SN74HC21D_SineWaveStep(g_sine_step);

    g_sine_step++;
    if (g_sine_step >= SINE_STEPS) {
        g_sine_step = 0;
    }
}

/*****************************************************************************
 * @brief  Enable sine wave auto-generation
 * @param  freq_hz: desired sine wave frequency in Hz
 * @note   Timer interrupt rate = freq_hz * SINE_STEPS
 *         Example: 100Hz sine -> timer runs at 1600Hz
 *****************************************************************************/
void SN74HC21D_SineWaveEnable(uint16_t freq_hz)
{
    uint32_t timer_reload;

    if (freq_hz == 0) freq_hz = 1;
    if (freq_hz > 500) freq_hz = 500;

    g_sine_step = 0;
    g_sine_running = 1;

    /*
     * Configure TIM0 for sine wave stepping.
     * Timer clock = Pclk = 3MHz (assuming APB divider = 1)
     * Timer interrupt rate = freq_hz * SINE_STEPS
     * Timer reload = Pclk / (freq_hz * SINE_STEPS)
     */
    timer_reload = 3000000UL / ((uint32_t)freq_hz * SINE_STEPS);

    SYS_EnablePeripheralClk(SYS_CLK_TIMER01_MSK);
    TMR_ConfigClk(TMR0, TMR_CLK_SEL_APB, TMR_CLK_DIV_1);
    TMR_ConfigRunMode(TMR0, TMR_COUNT_PERIOD_MODE, TMR_BIT_16_MODE);
    TMR_DisableOneShotMode(TMR0);
    TMR_SetPeriod(TMR0, timer_reload);
    TMR_EnableOverflowInt(TMR0);
    NVIC_EnableIRQ(TMR0_IRQn);
    NVIC_SetPriority(TMR0_IRQn, 1);
    TMR_Start(TMR0);
}

/*****************************************************************************
 * @brief  Disable sine wave auto-generation and stop all outputs
 *****************************************************************************/
void SN74HC21D_SineWaveDisable(void)
{
    g_sine_running = 0;
    TMR_Stop(TMR0);
    TMR_DisableOverflowInt(TMR0);
    SN74HC21D_StopAll();
}
