/*****************************************************************************
 * File:       bsp_water.c
 * Function:   Water level detection module for P43 (GPIO4, PIN3)
 *              - Low level = sufficient water
 *              - High level = insufficient water
 *              - Software debounce with 3 consecutive samples
 *****************************************************************************/
#include "bsp_water.h"
#include "MyTypedef.h"
#include "bsp_hard.h"
#include "bsp_pid.h"

/* Debounce counter */
static uint8_t water_debounce_cnt = 0;
/* Last stable state: 0=OK, 1=low */
static uint8_t water_last_state = WATER_LEVEL_OK;

/*****************************************************************************
 * Function:   WaterLevel_Init
 * Description: Configure P43 as GPIO input with pull-up
 *****************************************************************************/
void WaterLevel_Init(void)
{
    /* Set P43 to GPIO function */
    SYS_SET_IOCFG(IOP43CFG, SYS_IOCFG_P43_GPIO);
    /* Configure P43 as input with pull-up */
    GPIO_CONFIG_IO_MODE(WATER_LEVEL_PORT, WATER_LEVEL_PIN, GPIO_MODE_INPUT_PULL_UP);

    /* Initialize state */
    water_last_state = WaterLevel_Read();
    water_debounce_cnt = 0;
    Flag.WaterLevelOK = (water_last_state == WATER_LEVEL_OK) ? 1 : 0;
    Flag.WaterLevelReport = 0;
}

/*****************************************************************************
 * Function:   WaterLevel_Read
 * Description: Read P43 pin level
 *              Returns: 0 = water level OK (low), 1 = water level low (high)
 *****************************************************************************/
uint8_t WaterLevel_Read(void)
{
    return GPIO_GET_PIN(WATER_LEVEL_PORT, WATER_LEVEL_PIN_MSK);
}

/*****************************************************************************
 * Function:   WaterLevel_Scan
 * Description: Debounce scan, call every 10ms
 *              3 consecutive identical readings to confirm state change
 *              When water level goes low:
 *              - Stop CCP0 PWM (cooling)
 *              - Disable PID
 *              - Turn off fan
 *              - Clear Flag.WorkStart
 *              - Enable report
 *              When water level recovers:
 *              - Do NOT auto-resume cooling
 *              - Disable report
 *****************************************************************************/
void WaterLevel_Scan(void)
{
    uint8_t raw;
    uint8_t water_low;

    raw = WaterLevel_Read();
    water_low = (raw == WATER_LEVEL_LOW) ? 1 : 0;

    if (water_low == water_last_state)
    {
        /* State unchanged, reset counter */
        water_debounce_cnt = 0;
    }
    else
    {
        /* State changed, count consecutive samples */
        water_debounce_cnt++;
        if (water_debounce_cnt >= WATER_DEBOUNCE_COUNT)
        {
            /* Confirmed state change */
            water_debounce_cnt = 0;
            water_last_state = water_low;

            if (water_low == 1)
            {
                /* Water level insufficient: stop cooling, enable report */
                Flag.WaterLevelOK = 0;
                Flag.WorkStart = 0;
                PID_SetEnabled(0);
                Cooling_Off();
                HeatDissipation_Off();
                Flag.WaterLevelReport = 1;
            }
            else
            {
                /* Water level recovered: disable report, do NOT auto-resume */
                Flag.WaterLevelOK = 1;
                Flag.WaterLevelReport = 0;
            }
        }
    }
}

/*****************************************************************************
 * Function:   WaterLevel_IsOK
 * Description: Return current water level status
 *              Returns: 1 = water level OK, 0 = water level insufficient
 *****************************************************************************/
uint8_t WaterLevel_IsOK(void)
{
    return Flag.WaterLevelOK;
}
