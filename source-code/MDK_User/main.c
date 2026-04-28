#include "cms32f033x.h"
#include "sys.h"


/* 此版本属于测试版本，对于默认初始波形不进行设置 */



int main(void)
{
	/*关闭寄存器写保护*/
	SYS_ProtectDisable();
	/*系统时钟初始化*/
	SYS_Config();
	/*滴答定时器时钟初始化*/
    SysTick_Config();
	Dveice_Init(48000);
	/*单片机所有端口优先级初始化，不用改动*/
	All_GPIO_NVIC_Config();
	/*配置 定时器0\1定时器1*/
	TMR0_Config();
	TMR1_Config();
	/*开启硬件除法器*/
	HWDIV_Open();
	/*蜂鸣器功能初始化*/
	BEEP_Init();
	/* 按键引脚初始化 */
	KEY_GPIO_Config();
	/* LED引脚初始化 */
	LED_Init();
	/* 蓝牙引脚初始化 */
	Bluetooth_GPIO_Config();
	/* 继电器引脚初始化 */
	CurrentCtrl_Relay_GPIO_Config();
	/* 串口1 初始化  -- 暂时设置成普通GPIO口，当电源按键按下，置位系统标志位s后，进行设置 */
	#if USART1_GPIO_FF_ENABLE
	/* P34管脚配置 (MCU_TXD1/BLE_RX) */
	SYS_SET_IOCFG(IOP34CFG, SYS_IOCFG_P34_GPIO);
	GPIO_CONFIG_IO_MODE(GPIO3, GPIO_PIN_4, GPIO_MODE_INPUT_FLOAT);
	/* P35管脚配置 (MCU_RXD1/BLE_TX) */
	SYS_SET_IOCFG(IOP35CFG, SYS_IOCFG_P35_GPIO);
	GPIO_CONFIG_IO_MODE(GPIO3, GPIO_PIN_5, GPIO_MODE_INPUT_FLOAT);
	#endif
	/* 串口1工作模式配置 */
	UART1_Init(9600);
	/* 手柄识别的ADC引脚配置 */
	ADC_Config(ADC1_CH_10_MSK, ADC1_CH_10);
	/* EMS-NTC温度检测的ADC引脚配置 */
	ADC_Config(ADC1_CH_9_MSK, ADC1_CH_9);
	/* EMS加热丝引脚初始化 */
	EMS_Heat_GPIO_Config();
	/**********************以上配置为系统芯片与相关外设的初始化相关***************************************/
	/* 关闭EMS的手柄上的灯 */
	LED_Off(LED_Name_EMSLED);
	/* 系统参数初始化 */
	SystemParaInit();
	// 初始化PID
	EMS_CoolPID_Init();        

	/***/
	while(1)
	{
		
		UART1_SendScan();						// UART1 发送扫描
		UART1_RecvScan();						// UART1 接收扫描
		
		SystemWorkStatusCheck();		
		if (Flag.update == 1)
		{
			IAP_Erase_512B(0x1C000000, 0xAA);
			IAP_WriteOneWord(0x1C000000, 0x33, 0x55);//升级标志位
			SYS_ResetMCU();  //系统复位操作	
		}
		
		if (Record.Switch == 0)
		{
			CCP_Stop(CCP1);
		}


		/*****************************************************/
		/*               EMS制冷控制                         */
		/*****************************************************/
		if ((Record.EMS_HotGears != 0) && (Record.Switch == 1))
		{
			while (Record.EMS_HotCnt == 1)
			{
				EMS_Heat_On();           // 开启制冷PWM
				Record.EMS_HotCnt = 0;
			}
		}
		else
		{
			EMS_Heat_Off();
		}


		/*****************************************************/
		/*               检测蓝牙连接状态                    */
		/*****************************************************/
		Flag.BluetoothConnect = Bluetooth_GetState();
		if (Flag.BluetoothConnect == 0)
		{
			/***********************************************************/
			/*                 蓝牙断开关闭所有功能                    */
			/***********************************************************/
			/* 所有界面子开关重置 */
			Record.Switch = 0;
			/* 所有的EPWM时钟关闭 */
			EPWM_Stop(EPWM_CH_1_MSK | EPWM_CH_2_MSK | EPWM_CH_3_MSK | EPWM_CH_4_MSK | EPWM_CH_5_MSK);
			/* 关闭CCP0 和 CCP1  */
			CCP_Stop(CCP0);
			CCP_Stop(CCP1);
			TMR_Stop(TMR0);

			/* EMS 相应功能挡位恢复默认 */
			Record.K1IonStrengGears = 0;
			Record.EMS_HotGears = 0;
			Record.EMS_HotCnt = 1;
			
			/* 关灯 */
			LED_Off(LED_Name_EMSLED);

			Record.AllIonStrengGears = 0;
			Record.EMSIonStrengGears = 0;
			Record.EMS_HotGears = 0;
			Record.EPTIonStrengGears = 0;
			Record.K1IonStrengGears = 0;
			Record.SNEIonStrengGears = 0;
			Record.HandWave = 0;
			
			Refr_EMSIonStreng(0);
			CCP_ConfigDutyScale(CCP1, CCPxB, 0);
			
		}		
	}
}


