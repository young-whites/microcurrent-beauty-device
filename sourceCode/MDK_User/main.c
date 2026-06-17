#include "cms32f033x.h"
#include "sys.h"
#include "bsp_pid.h"
#include "bsp_hard.h"
#include "adc.h"
#include "debug_uart.h"
#include "sn74hc21d.h"



int main(void)
{
	/*关闭寄存器写保护*/
	SYS_ProtectDisable();
	/*系统时钟初始化*/
	SYS_Config();
	/*滴答定时器时钟初始化*/
	/*SysTick_Config();*/
	Dveice_Init(48000);
	/*单片机所有端口优先级初始化，不用改动*/
	All_GPIO_NVIC_Config();
	/*配置 定时器0\1定时器1*/
	// TMR0_Config();
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
	/* EMS-NTC温度检测的ADC引脚配置 */
	ADC_Config(ADC1_CH_9_MSK, ADC1_CH_9);
	/* Cooling subsystem initialized on-demand by PID control */
	/* Initialize cooling PWM subsystem (CCP0) */
	Cooling_Init();
	/* Heat dissipation fan GPIO init (P25) */
	HeatDissipation_GPIO_Config();
	/* Energy output PWM init (P07 as CCP1B for microcurrent) */
	Energy_Init();
	/* SN74HC21D half-bridge driver init (EPWM2/3 + GPIO) */
	SN74HC21D_Init();
	/* Water level detection init (P43) */
	WaterLevel_Init();
	/**********************以上配置为系统芯片与相关外设的初始化相关*+.0**************************************/
	/* 系统参数初始化 */
	/* Debug UART0 init for temperature data output */
	Debug_UART_Init();
	SystemParaInit();
	
	while(1)
	{
		
		UART1_SendScan();						// UART1 发送扫描
		UART1_RecvScan();						// UART1 接收扫描
		ADC_DrvScan();							// Trigger ADC scan for NTC temperature sampling
		WaterLevel_Scan();					// Water level debounce scan
		
		SystemWorkStatusCheck();		
		if (Flag.update == 1)
		{
			IAP_Erase_512B(0x1C000000, 0xAA);
			IAP_WriteOneWord(0x1C000000, 0x33, 0x55);//升级标志位
			SYS_ResetMCU();  //系统复位操作	
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
			/* Stop cooling on bluetooth disconnect */
			PID_SetEnabled(0);
			Cooling_Off();
			HeatDissipation_Off();
			/* Stop energy output on bluetooth disconnect */
			Flag.EnergyOutput = 0;
			Energy_Off();
			SN74HC21D_EnergyStop();
		}
	}
}



