#include "bsp_hard.h"


/**
  * @brief  散热控制使能引脚初始化 (P25)
  * @param  None
  * @retval None
  */
void HeatDissipation_GPIO_Config(void)
{
    /* P25 配置为普通 GPIO 模式 */
    SYS_SET_IOCFG(IOP25CFG, SYS_IOCFG_P25_GPIO);
    
    /* 配置为推挽输出模式 */
    GPIO_CONFIG_IO_MODE(GPIO2, GPIO_PIN_5, GPIO_MODE_OUTPUT_PUSH_PULL);
    
    /* 初始化为关闭状态（低电平）—— 安全第一 */
    GPIO_ResetPin(GPIO2, GPIO_PIN_5_MSK);
}

/**
  * @brief  开启散热（P25 输出高电平）
  * @param  None
  * @retval None
  */
void HeatDissipation_On(void)
{
    GPIO_SetPin(GPIO2, GPIO_PIN_5_MSK);
}

/**
  * @brief  关闭散热（P25 输出低电平）
  * @param  None
  * @retval None
  */
void HeatDissipation_Off(void)
{
    GPIO_ResetPin(GPIO2, GPIO_PIN_5_MSK);
}




/**************************************************************************
* 功能:	发热丝 端口初始化
***************************************************************************/
void Cold_Enable_GPIO_Config(void)
{
	SYS_SET_IOCFG(IOP30CFG, SYS_IOCFG_P30_CCP0A);
}








