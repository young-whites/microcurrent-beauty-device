#include "sys.h"

/**
  * @brief  系统参数初始化函数
  * @param  None
  */
void SystemParaInit(void)
{

	/* 每个模式界面的EPWM有着不同的配置 */
	EPWM_Config();
}





/**
  * @brief  系统工作状态检测
  * @param  None
  */
void SystemWorkStatusCheck(void)
{

	/***********************************************************/
	/*                 电源按键LED状态扫描                     */
	/***********************************************************/
	if (Record.PowerKeyCnt == 0)
	{
		LED_On(LED_Name_PowerKeyOff);
		LED_Off(LED_Name_PowerKeyOn);
		Flag.PowerOn = 0;
		if (Flag.UsartConfig == 0)
		{
			Flag.UsartConfig = 1;
			/* 关闭蓝牙电源 */
			Bluetooth_PowerOff();
			/* P34管脚配置 (MCU_TXD1/BLE_RX) */
			SYS_SET_IOCFG(IOP34CFG, SYS_IOCFG_P34_GPIO);
			GPIO_CONFIG_IO_MODE(GPIO3, GPIO_PIN_4, GPIO_MODE_INPUT_FLOAT);
			/* P35管脚配置 (MCU_RXD1/BLE_TX) */
			SYS_SET_IOCFG(IOP35CFG, SYS_IOCFG_P35_GPIO);
			GPIO_CONFIG_IO_MODE(GPIO3, GPIO_PIN_5, GPIO_MODE_INPUT_FLOAT);
			/***********************************************************/
			/*                 关闭所有功能                            */
			/***********************************************************/
			/* 所有界面子开关重置 */
			Record.K1_Switch = 0;
			/* 所有的EPWM时钟关闭 */
			EPWM_Stop(EPWM_CH_1_MSK | EPWM_CH_2_MSK | EPWM_CH_3_MSK | EPWM_CH_4_MSK);
			/* 关闭CCP0 和 CCP1  */
			CCP_Stop(CCP0);
			CCP_Stop(CCP1);
			LED_Off(LED_Name_EMSLED);
			TMR_Stop(TMR0);
			/* EMS 相应功能挡位恢复默认 */
			Record.K1IonStrengGears = 0;
			Record.EMS_HotGears = 0;
			Record.EMS_HotCnt = 1;
		}
	}
	else if (Record.PowerKeyCnt == 1)
	{
		LED_Off(LED_Name_PowerKeyOff);
		LED_On(LED_Name_PowerKeyOn);
		Flag.PowerOn = 1;
		if (Flag.UsartConfig == 1)
		{
			Flag.UsartConfig = 0;
			/* 重置蓝牙开机标志 */
			btPowerOnCnt = 0;
			/* 开启蓝牙电源 */
			Bluetooth_PowerOn();
			/* 开机后配置串口1引脚复用 */
			SYS_SET_IOCFG(IOP34CFG, SYS_IOCFG_P34_TXD1);
			SYS_SET_IOCFG(IOP35CFG, SYS_IOCFG_P35_RXD1);
			/***********************************************************/
			/*                 开启功能并作相应初始化                  */
			/***********************************************************/

		}
	}
	else if (Record.PowerKeyCnt == 2)
	{
		LED_On(LED_Name_PowerKeyOff);
		LED_Off(LED_Name_PowerKeyOn);
		Flag.PowerOn = 0;
		Record.PowerKeyCnt = 0;

		if (Flag.UsartConfig == 0)
		{
			Flag.UsartConfig = 1;
			/* 关闭蓝牙电源 */
			Bluetooth_PowerOff();
			/* P34管脚配置 (MCU_TXD1/BLE_RX) */
			SYS_SET_IOCFG(IOP34CFG, SYS_IOCFG_P34_GPIO);
			GPIO_CONFIG_IO_MODE(GPIO3, GPIO_PIN_4, GPIO_MODE_INPUT_FLOAT);
			/* P35管脚配置 (MCU_RXD1/BLE_TX) */
			SYS_SET_IOCFG(IOP35CFG, SYS_IOCFG_P35_GPIO);
			GPIO_CONFIG_IO_MODE(GPIO3, GPIO_PIN_5, GPIO_MODE_INPUT_FLOAT);

			/***********************************************************/
			/*                 关闭所有功能                            */
			/***********************************************************/
			/* 所有界面子开关重置 */
			Record.K1_Switch = 0;
			/* 所有的EPWM时钟关闭 */
			EPWM_Stop(EPWM_CH_1_MSK | EPWM_CH_2_MSK | EPWM_CH_3_MSK | EPWM_CH_4_MSK);
			/* 关闭CCP0 和 CCP1  */
			CCP_Stop(CCP0);
			CCP_Stop(CCP1);
			LED_Off(LED_Name_EMSLED);
			TMR_Stop(TMR0);
			/* EMS 相应功能挡位恢复默认 */
			Record.K1IonStrengGears = 0;
			Record.EMS_HotGears = 0;
			Record.EMS_HotCnt = 1;

		}

	}
}




























