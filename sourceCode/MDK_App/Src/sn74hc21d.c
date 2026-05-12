/**
 * @file    sn74hc21d.c
 * @brief   SN74HC21D half-bridge driver for microcurrent energy output
 *
 * Architecture:
 *   - P07(CCP1B): PWM at 1kHz, duty controls supply voltage to half-bridge
 *                 -> This is how energy amplitude is controlled
 *   - P00(EPWM2): Upper half-bridge signal (100Hz square wave)
 *   - P06(EPWM3): Lower half-bridge signal (100Hz square wave)
 *   - P31(EPWM4): Duty modulation signal (2x base frequency)
 *   - P12(GPIO):  ION_ENA - upper half-bridge enable
 *   - P40(GPIO):  ION_ENB - lower half-bridge enable
 *   - P10(GPIO):  ION_ENAB - master enable
 *
 * Waveform generation:
 *   TMR0 ISR drives amplitude ramp (0->peak->0) on each half-wave.
 *   Half-bridge alternates (A/B) after each ramp completes.
 *   Energy intensity = P07(CCP1B) duty cycle, ramped via lookup table.
 *
 * @note    P30(CCP0A) cooling pad is NOT modified here.
 * @note    EPWM_Config_Independent_Mode library function is NOT modified.
 */

#include "sn74hc21d.h"
#include "epwm.h"
#include "bsp_hard.h"

/* =========================================================================
 * Internal constants
 * ========================================================================= */

/* Amplitude ramp lookup table size */
#define AMP_RAMP_SIZE       250

/* Default timer reload for amplitude ramp stepping.
 * Pclk = 3MHz, TMR0 reload = Pclk / (RAMP_STEPS * RAMP_SPEED_HZ)
 * With RAMP_STEPS=500 (250 up + 250 down), RAMP_SPEED=2000 half-waves/s:
 *   reload = 3000000 / (500 * 2000) = 3
 * With RAMP_SPEED=1000 half-waves/s (1s per full wave at 100Hz carrier):
 *   reload = 3000000 / (500 * 1000) = 6
 * Target: ~1ms per step -> 250 steps * 1ms = 250ms per half-wave
 * Timer reload = Pclk * 0.001 = 3000
 */
#define TMR0_DEFAULT_RELOAD 3000

/* Base EPWM frequency (Hz) */
#define EPWM_BASE_FREQ      100

/* EPWM period values for 100Hz (Pclk=3MHz, center-aligned: period = Pclk/Freq/2) */
#define EPWM_PERIOD_100HZ   (SystemAPBClock / EPWM_BASE_FREQ / 2)  /* 15000 at 3MHz */

/* EPWM4 period = double frequency (200Hz) */
#define EPWM4_PERIOD        (EPWM_PERIOD_100HZ / 2)                /* 7500 */

/* =========================================================================
 * Amplitude ramp lookup table
 * Linear ramp: 0, 1, 2, ..., 249
 * Index maps directly to duty cycle numerator (duty = period * index / 250)
 * ========================================================================= */
static const uint16_t g_amp_ramp_table[AMP_RAMP_SIZE] = {
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,
     11,  12,  13,  14,  15,  16,  17,  18,  19,  20,
     21,  22,  23,  24,  25,  26,  27,  28,  29,  30,
     31,  32,  33,  34,  35,  36,  37,  38,  39,  40,
     41,  42,  43,  44,  45,  46,  47,  48,  49,  50,
     51,  52,  53,  54,  55,  56,  57,  58,  59,  60,
     61,  62,  63,  64,  65,  66,  67,  68,  69,  70,
     71,  72,  73,  74,  75,  76,  77,  78,  79,  80,
     81,  82,  83,  84,  85,  86,  87,  88,  89,  90,
     91,  92,  93,  94,  95,  96,  97,  98,  99,
    100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
    110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
    120, 121, 122, 123, 124, 125, 126, 127, 128, 129,
    130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
    140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
    150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
    160, 161, 162, 163, 164, 165, 166, 167, 168, 169,
    170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
    180, 181, 182, 183, 184, 185, 186, 187, 188, 189,
    190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
    200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
    210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
    220, 221, 222, 223, 224, 225, 226, 227, 228, 229,
    230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249
};

/* =========================================================================
 * Module state
 * ========================================================================= */
static volatile uint8_t  g_energy_running   = 0;    /* Energy output active flag */
static volatile uint8_t  g_halfwave_flag_a  = 1;    /* 1=channel A active, 0=channel B */
static volatile uint16_t g_ramp_index_up    = 0;    /* Rising ramp counter */
static volatile uint16_t g_ramp_index_down  = 0;    /* Falling ramp counter */
static volatile uint16_t g_ramp_peak       = 0;    /* Target peak index (gear+base) */
static volatile uint8_t  g_energy_gear      = 0;    /* Current gear setting 0~100 */

/* =========================================================================
 * Internal helpers
 * ========================================================================= */

/**
 * @brief  Select upper half-bridge (Channel A) for positive half-wave
 *         ENA=1, ENB=0, ENAB=1, EPWM2 on, EPWM3 off
 */
static void SN74HC21D_SelectChannelA(void)
{
    GPIO_SetPin(GPIO1, GPIO_PIN_2_MSK);       /* ION_ENA  = 1 */
    GPIO_ResetPin(GPIO4, GPIO_PIN_0_MSK);     /* ION_ENB  = 0 */
    GPIO_SetPin(GPIO1, GPIO_PIN_0_MSK);       /* ION_ENAB = 1 */
    EPWM_Start(EPWM_CH_2_MSK);               /* EPWM2 on */
    EPWM_Stop(EPWM_CH_3_MSK);                /* EPWM3 off */
}

/**
 * @brief  Select lower half-bridge (Channel B) for negative half-wave
 *         ENA=0, ENB=1, ENAB=1, EPWM2 off, EPWM3 on
 */
static void SN74HC21D_SelectChannelB(void)
{
    GPIO_ResetPin(GPIO1, GPIO_PIN_2_MSK);     /* ION_ENA  = 0 */
    GPIO_SetPin(GPIO4, GPIO_PIN_0_MSK);       /* ION_ENB  = 1 */
    GPIO_SetPin(GPIO1, GPIO_PIN_0_MSK);       /* ION_ENAB = 1 */
    EPWM_Stop(EPWM_CH_2_MSK);                /* EPWM2 off */
    EPWM_Start(EPWM_CH_3_MSK);               /* EPWM3 on */
}

/**
 * @brief  Set P07(CCP1B) duty cycle from ramp table index
 *         duty = period * table[index] / 250
 * @param  index: 0~249 (clamped to table range)
 */
static void SN74HC21D_SetAmpDuty(uint16_t index)
{
    uint16_t period;
    uint16_t duty;
    uint16_t table_val;

    if (index >= AMP_RAMP_SIZE) {
        index = AMP_RAMP_SIZE - 1;
    }

    table_val = g_amp_ramp_table[index];
    period = CCP_ReadLoad(CCP1);
    duty = (uint32_t)period * table_val / 250;
    CCP_ConfigCompare(CCP1, CCPxB, duty);
}

/* =========================================================================
 * Public API
 * ========================================================================= */

/**
 * @brief  Initialize SN74HC21D half-bridge driver
 *         Configures GPIO enables, EPWM channels, and CCP1B energy control
 */
void SN74HC21D_Init(void)
{
    /* ---- GPIO enable pins: P12, P40, P10 as push-pull output, default low ---- */

    /* P12 -> ION_ENA (upper half-bridge enable) */
    SYS_SET_IOCFG(IOP12CFG, SYS_IOCFG_P12_GPIO);
    GPIO_CONFIG_IO_MODE(GPIO1, GPIO_PIN_2, GPIO_MODE_OUTPUT_PUSH_PULL);
    GPIO_ResetPin(GPIO1, GPIO_PIN_2_MSK);

    /* P40 -> ION_ENB (lower half-bridge enable) */
    SYS_SET_IOCFG(IOP40CFG, SYS_IOCFG_P40_GPIO);
    GPIO_CONFIG_IO_MODE(GPIO4, GPIO_PIN_0, GPIO_MODE_OUTPUT_PUSH_PULL);
    GPIO_ResetPin(GPIO4, GPIO_PIN_0_MSK);

    /* P10 -> ION_ENAB (master enable) */
    SYS_SET_IOCFG(IOP10CFG, SYS_IOCFG_P10_GPIO);
    GPIO_CONFIG_IO_MODE(GPIO1, GPIO_PIN_0, GPIO_MODE_OUTPUT_PUSH_PULL);
    GPIO_ResetPin(GPIO1, GPIO_PIN_0_MSK);

    /* ---- EPWM signal pins ---- */

    /* P00 -> EPWM2 (upper half-bridge frequency signal) */
    SYS_SET_IOCFG(IOP00CFG, SYS_IOCFG_P00_EPWM2);

    /* P06 -> EPWM3 (lower half-bridge frequency signal) */
    SYS_SET_IOCFG(IOP06CFG, SYS_IOCFG_P06_EPWM3);

    /* P31 -> EPWM4 (duty modulation signal) */
    SYS_SET_IOCFG(IOP31CFG, SYS_IOCFG_P31_EPWM4);

    /* ---- Configure EPWM2/3/4 via independent mode ---- */
    {
        INT32U freq[6];
        INT8U  div[6];
        uint8_t i;

        for (i = 0; i < 6; i++) {
            freq[i] = EPWM_BASE_FREQ;
            div[i]  = 1;
        }
        /* EPWM4 at double frequency for duty modulation */
        freq[4] = EPWM_BASE_FREQ * 2;

        EPWM_Config_Independent_Mode(
            EPWM_CH_2_MSK | EPWM_CH_3_MSK | EPWM_CH_4_MSK,
            freq, div);

        /* Set duty cycles */
        EPWM_ConfigChannelSymDutyScale(EPWM2, 48);   /* Upper: 48% */
        EPWM_ConfigChannelSymDutyScale(EPWM3, 52);   /* Lower: 52% */
        EPWM_ConfigChannelSymDutyScale(EPWM4, 20);   /* Modulation: 20% */

        /* All EPWM channels stopped initially */
        EPWM_Stop(EPWM_CH_2_MSK | EPWM_CH_3_MSK | EPWM_CH_4_MSK);
    }

    /* ---- P07 as CCP1B for energy voltage control (1kHz) ----
     * Energy_Init() from bsp_hard.c already configures CCP1:
     *   - Pclk=3MHz, divider=1, reload=3000 -> 1kHz
     *   - CCP1, CCPxB channel, 0% duty
     * We only need to ensure it is started here.
     */
    CCP_Start(CCP1);

    /* ---- Default state: everything off ---- */
    g_energy_running = 0;
    g_halfwave_flag_a = 1;
    g_ramp_index_up = 0;
    g_ramp_index_down = 0;
    g_ramp_peak = 0;
    g_energy_gear = 0;
}

/**
 * @brief  Start energy output at specified gear level
 * @param  gear: 0~100, maps to amplitude peak (10 + gear)
 *         gear=0 stops output immediately
 * @note   Starts EPWM2 (channel A), CCP1B, and TMR0 for amplitude ramping
 */
void SN74HC21D_EnergyStart(uint8_t gear)
{
    if (gear > 0)
    {
        if (gear > 100) gear = 100;
        g_energy_gear = gear;
    }

    /* Use stored gear if parameter is 0 */
    if (g_energy_gear == 0)
    {
        g_energy_gear = 50;  /* Default to 50% if never set */
    }

    /* Reset ramp state */
    g_ramp_index_up = 0;
    g_ramp_index_down = 0;
    g_ramp_peak = 10 + (uint16_t)g_energy_gear;
    g_halfwave_flag_a = 1;

    /* Start with channel A (upper half-bridge) */
    SN74HC21D_SelectChannelA();

    /* Start EPWM4 for duty modulation */
    EPWM_Start(EPWM_CH_4_MSK);

    /* Ensure CCP1B is running (Energy_Init already configured it) */
    CCP_Start(CCP1);
    SN74HC21D_SetAmpDuty(0);

    /* Configure and start TMR0 for amplitude ramp stepping.
     * Timer drives the 0->peak->0 amplitude envelope on each half-wave.
     * Reload = Pclk / (2 * RAMP_STEPS * desired_halfwave_freq)
     * Default: 250ms per half-wave -> reload = 3000000 / (2 * 250 * 2000) = 3
     */
    SYS_EnablePeripheralClk(SYS_CLK_TIMER01_MSK);
    TMR_ConfigClk(TMR0, TMR_CLK_SEL_APB, TMR_CLK_DIV_1);
    TMR_ConfigRunMode(TMR0, TMR_COUNT_PERIOD_MODE, TMR_BIT_32_MODE);
    TMR_DisableOneShotMode(TMR0);
    TMR_SetPeriod(TMR0, TMR0_DEFAULT_RELOAD);
    TMR_ClearOverflowIntFlag(TMR0);
    TMR_EnableOverflowInt(TMR0);
    NVIC_EnableIRQ(TMR0_IRQn);
    NVIC_SetPriority(TMR0_IRQn, 1);

    g_energy_running = 1;
    TMR_Start(TMR0);
}

/**
 * @brief  Stop energy output completely
 *         Stops TMR0, EPWM2/3/4, zeros CCP1B duty
 */
void SN74HC21D_EnergyStop(void)
{
    /* Stop timer first */
    g_energy_running = 0;
    TMR_Stop(TMR0);
    TMR_DisableOverflowInt(TMR0);
    TMR_ClearOverflowIntFlag(TMR0);

    /* Reset state */
    g_halfwave_flag_a = 1;
    g_ramp_index_up = 0;
    g_ramp_index_down = 0;
    g_ramp_peak = 0;

    /* Zero CCP1B duty (energy voltage = 0) */
    CCP_ConfigCompare(CCP1, CCPxB, 0);

    /* Stop all EPWM channels */
    EPWM_Stop(EPWM_CH_2_MSK | EPWM_CH_3_MSK | EPWM_CH_4_MSK);

    /* Disable all GPIO enables */
    GPIO_ResetPin(GPIO1, GPIO_PIN_2_MSK);     /* ION_ENA  = 0 */
    GPIO_ResetPin(GPIO4, GPIO_PIN_0_MSK);     /* ION_ENB  = 0 */
    GPIO_ResetPin(GPIO1, GPIO_PIN_0_MSK);     /* ION_ENAB = 0 */
}

/**
 * @brief  Set energy gear level (0~100)
 * @param  gear: 0~100, maps to amplitude peak (10 + gear)
 * @note   If energy is already running, updates ramp peak immediately.
 *         If not running, just stores the value for next start.
 */
void SN74HC21D_EnergySetGear(uint8_t gear)
{
    if (gear > 100) gear = 100;
    g_energy_gear = gear;

    if (g_energy_running)
    {
        /* Update ramp peak for next cycle */
        g_ramp_peak = 10 + (uint16_t)gear;
    }
}

/* =========================================================================
 * TMR0 ISR handler (called from cms32f033_it.c TMR0_IRQHandler)
 *
 * Drives amplitude ramp on P07(CCP1B):
 *   1. Ramp UP:   index 0 -> peak (gear + 10)
 *   2. Ramp DOWN: index peak -> 0
 *   3. Toggle half-bridge (A <-> B)
 *   4. Repeat
 * ========================================================================= */
void SN74HC21D_AmpRampISR(void)
{
    if (!g_energy_running) return;

    if (g_energy_gear == 0)
    {
        SN74HC21D_SetAmpDuty(0);
        return;
    }

    if (g_halfwave_flag_a)
    {
        /* ---- Channel A (upper half-bridge, positive half-wave) ---- */
        SN74HC21D_SelectChannelA();

        if (g_ramp_index_up < g_ramp_peak)
        {
            /* Rising edge of envelope */
            SN74HC21D_SetAmpDuty(g_ramp_index_up);
            g_ramp_index_up++;
        }
        else if (g_ramp_index_up == g_ramp_peak)
        {
            /* Peak reached, prepare to descend */
            g_ramp_index_down = g_ramp_index_up;
        }

        if (g_ramp_index_down > 0)
        {
            /* Falling edge of envelope */
            g_ramp_index_down--;
            SN74HC21D_SetAmpDuty(g_ramp_index_down);

            if (g_ramp_index_down == 0)
            {
                /* Half-wave complete, switch to channel B */
                g_ramp_index_up = 0;
                g_halfwave_flag_a = 0;
            }
        }
    }
    else
    {
        /* ---- Channel B (lower half-bridge, negative half-wave) ---- */
        SN74HC21D_SelectChannelB();

        if (g_ramp_index_up < g_ramp_peak)
        {
            SN74HC21D_SetAmpDuty(g_ramp_index_up);
            g_ramp_index_up++;
        }
        else if (g_ramp_index_up == g_ramp_peak)
        {
            g_ramp_index_down = g_ramp_index_up;
        }

        if (g_ramp_index_down > 0)
        {
            g_ramp_index_down--;
            SN74HC21D_SetAmpDuty(g_ramp_index_down);

            if (g_ramp_index_down == 0)
            {
                g_ramp_index_up = 0;
                g_halfwave_flag_a = 1;
            }
        }
    }
}
