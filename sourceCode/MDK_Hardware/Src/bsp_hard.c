#include "bsp_hard.h"


/**
  * @brief  Heat dissipation fan GPIO init (P25)
  * @param  None
  * @retval None
  */
void HeatDissipation_GPIO_Config(void)
{
    /* P25 configured as GPIO */
    SYS_SET_IOCFG(IOP25CFG, SYS_IOCFG_P25_GPIO);

    /* Push-pull output mode */
    GPIO_CONFIG_IO_MODE(GPIO2, GPIO_PIN_5, GPIO_MODE_OUTPUT_PUSH_PULL);

    /* Default off (low) -- safety first */
    GPIO_ResetPin(GPIO2, GPIO_PIN_5_MSK);
}

/**
  * @brief  Turn on heat dissipation fan (P25 high)
  * @param  None
  * @retval None
  */
void HeatDissipation_On(void)
{
    GPIO_SetPin(GPIO2, GPIO_PIN_5_MSK);
}

/**
  * @brief  Turn off heat dissipation fan (P25 low)
  * @param  None
  * @retval None
  */
void HeatDissipation_Off(void)
{
    GPIO_ResetPin(GPIO2, GPIO_PIN_5_MSK);
}


/**
  * @brief  Cooling pad GPIO init (P30 as CCP0A for PWM)
  * @param  None
  * @retval None
  */
void Cold_Enable_GPIO_Config(void)
{
    SYS_SET_IOCFG(IOP30CFG, SYS_IOCFG_P30_CCP0A);
}

/**
  * @brief  Initialize cooling subsystem
  *         Configures CCP0 for PWM output on P30 (CCP0A)
  */
void Cooling_Init(void)
{
    Cold_Enable_GPIO_Config();

    /* Configure CCP0 for cooling PWM at 1kHz */
    SYS_EnablePeripheralClk(SYS_CLK_CCP_MSK);
    CCP_ConfigCLK(CCP0, CCP_CLK_DIV_1, CCP_RELOAD_CCPLOAD, 3000);  // Pclk=3MHz/3000=1kHz
    CCP_EnablePWMMode(CCP0);
    CCP_ConfigDutyScale(CCP0, CCPxA, 0);   // Start at 0%
    CCP_DisableReverseOutput(CCP0, CCPxA);
    CCP_Start(CCP0);       /* Enable CCP0 module */
    CCP_EnableRun(CCP0);   /* Start timer counting */
}

/**
  * @brief  Set cooling pad power via PWM duty cycle
  * @param  duty: 0~100 (0%~100%)
  */
void Cooling_SetPower(uint8_t duty)
{
    uint16_t period, compare;
    if (duty > 100) duty = 100;
    period = CCP_ReadLoad(CCP0);
    compare = (uint32_t)period * duty / 100;
    CCP_ConfigCompare(CCP0, CCPxA, compare);
}

/**
  * @brief  Turn off cooling pad
  */
void Cooling_Off(void)
{
    Cooling_SetPower(0);
    CCP_Stop(CCP0);
}
