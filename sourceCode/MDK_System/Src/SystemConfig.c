#include "SystemConfig.h"




/*****************************************************************************
* 功能:		开启写保护
*****************************************************************************/
void SYS_ProtectEnable(void)
{
	SYS_EnableSARMProtect(SYS_SRAM_PORTECT_2K_4K_MSK);		// 开启SRAM（2K~4K地址范围）写保护
	SYS_EnableSARMProtect(SYS_SRAM_PORTECT_4K_6K_MSK);		// 开启SRAM（4K~6K地址范围）写保护
	SYS_EnableSARMProtect(SYS_SRAM_PORTECT_6K_8K_MSK);		// 开启SRAM（6K~8K地址范围）写保护
	SYS_EnableIOCFGProtect();								// 开启IOCONFIG写保护
	SYS_EnableGPIO0Protect();								// 开启GPIO0的相关寄存器写保护
	SYS_EnableGPIO1Protect();								// 开启GPIO1的相关寄存器写保护
	SYS_EnableGPIO2Protect();								// 开启GPIO2的相关寄存器写保护
	SYS_EnableGPIO3Protect();								// 开启GPIO3的相关寄存器写保护
	SYS_EnableGPIO4Protect();								// 开启GPIO4的相关寄存器写保护
}

/*****************************************************************************
* 功能:		关闭写保护
*****************************************************************************/
void SYS_ProtectDisable(void)
{
	SYS_DisableSARMProtect(SYS_SRAM_PORTECT_2K_4K_MSK);		// 关闭SRAM（2K~4K地址范围）写保护
	SYS_DisableSARMProtect(SYS_SRAM_PORTECT_4K_6K_MSK);		// 关闭SRAM（4K~6K地址范围）写保护
	SYS_DisableSARMProtect(SYS_SRAM_PORTECT_6K_8K_MSK);		// 关闭SRAM（6K~8K地址范围）写保护
	SYS_DisableIOCFGProtect();								// 关闭IOCONFIG写保护
	SYS_DisableGPIO0Protect();								// 关闭GPIO0的相关寄存器写保护
	SYS_DisableGPIO1Protect();								// 关闭GPIO1的相关寄存器写保护
	SYS_DisableGPIO2Protect();								// 关闭GPIO2的相关寄存器写保护
	SYS_DisableGPIO3Protect();								// 关闭GPIO3的相关寄存器写保护
	SYS_DisableGPIO4Protect();								// 关闭GPIO4的相关寄存器写保护
}




/*****************************************************************************
* 功能:		system配置
*****************************************************************************/
void SYS_Config(void)
{
	// (1)设置HSI（频率选择/时钟使能）
	SYS_ConfigHSI(SYS_CLK_HSI_48M);							// 选择内部高速时钟频率为48Mhz
	SYS_EnableHSI();										// 开启内部高速时钟

	// (2)设置AHB（时钟选择/时钟分频）
	SYS_ConfigAHBClock(SYS_CLK_SEL_HSI, SYS_CLK_DIV_2);		// HSIclk = 48MHz, Hclk = HSIclk/2 = 24MHz

	// (3)设置APB（分频）
	SYS_ConfigAPBClock(AHB_CLK_DIV_8);						// Pclk = Hclk/8 = 3MHz

	// (4)设置时钟输出（选择/分频/使能）
	SYS_DisableClkOutput();									// 禁止时钟输出

	// (5)设置LVD（监测电压选择/中断使能）
	SYS_ConfigLVDLevel(SYS_LVD_2V);							// LVD监测电压选择 2V
	SYS_EnableLVDInt();										// 开启LVD监测中断

	// (6)设置寄存器保护类型
	SYS_ProtectDisable();
}





/*****************************************************************************
* 功能:		系统滴答定时器配置
*****************************************************************************/
void SysTick_Config(void)
{
	/*HCLK = 24Mhz*/
	TICK_SetPeriod(2400);						// 设置系统滴答定时器的定时周期(Ftick = HCLK/24 = 100000Hz, Ttick = 100us)
	TICK_ClearTimingData();						// 清除系统滴答定时器当前计数值
	NVIC_SetPriority(SysTick_IRQn, 1);			// 设置系统滴答定时器的中断优先级
	TICK_EnableOverflowInt();					// 开启系统滴答定时器中断
	TICK_Start();								// 开启系统滴答定时器
}





/*****************************************************************************
* 功能:		配置所有的GPIO端口的优先级以及SWD下载口
*****************************************************************************/
void All_GPIO_NVIC_Config(void)
{

	// SWD管脚配置
	SYS_SET_IOCFG(IOP47CFG, SYS_IOCFG_P47_SWDDAT0);
	SYS_SET_IOCFG(IOP46CFG, SYS_IOCFG_P46_SWDCLK0);

	NVIC_SetPriority(GPIO0_IRQn, 0);									// 优先级0~3， 0最高、3最低
	NVIC_SetPriority(GPIO1_IRQn, 0);									// 优先级0~3， 0最高、3最低
	NVIC_SetPriority(GPIO2_IRQn, 0);									// 优先级0~3， 0最高、3最低
	NVIC_SetPriority(GPIO3_IRQn, 0);									// 优先级0~3， 0最高、3最低
	NVIC_SetPriority(GPIO4_IRQn, 0);									// 优先级0~3， 0最高、3最低
	NVIC_EnableIRQ(GPIO0_IRQn);
	NVIC_EnableIRQ(GPIO1_IRQn);
	NVIC_EnableIRQ(GPIO2_IRQn);
	NVIC_EnableIRQ(GPIO3_IRQn);
	NVIC_EnableIRQ(GPIO4_IRQn);
}





/*****************************************************************************
* 功能:		开启除法器 （默认无符号模式）
*****************************************************************************/
void HWDIV_Open(void)
{
	SYS_EnablePeripheralClk(SYS_CLK_HWDIVCE_MSK);		// 打开HWDIV模块时钟
	HWDIV_EnableUnsingedMode();							// 无符号模式
}

/*****************************************************************************
* 功能:		关闭除法器
*****************************************************************************/
void HWDIV_Close(void)
{
	SYS_DisablePeripheralClk(SYS_CLK_HWDIVCE_MSK);		// 关闭HWDIV模块时钟
}