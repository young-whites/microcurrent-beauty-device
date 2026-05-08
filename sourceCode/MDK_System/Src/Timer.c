#include "Timer.h"

/*****************************************************************************
* 功能:		timer0配置
*****************************************************************************/
void TMR0_Config(void)
{
	// (1)设置Timer0 的时钟
	SYS_EnablePeripheralClk(SYS_CLK_TIMER01_MSK);						// 打开Timer0/1的时钟
	TMR_ConfigClk(TMR0, TMR_CLK_SEL_APB, TMR_CLK_DIV_1);				// TMR0clk = Pclk/1 = 3MHz

	// (2)设置Timer0 运行模式
	TMR_ConfigRunMode(TMR0, TMR_COUNT_PERIOD_MODE, TMR_BIT_32_MODE);	// 32位、周期计数模式
	TMR_DisableOneShotMode(TMR0);										// 关闭单次模式

	// (3)设置Timer0 运行周期
	TMR_SetPeriod(TMR0, 3000);											// Ftmr0 = TMR0clk/3000 = 1000hz,  Ttmr0 = 1/Ftmr0 = 1ms

	// (4)设置Timer0 中断
	TMR_EnableOverflowInt(TMR0);
	NVIC_EnableIRQ(TMR0_IRQn);

	// (5)设置Timer0 中断优先级
	NVIC_SetPriority(TMR0_IRQn, 0);										// 最高优先级

	// (6)关闭Timer0
	TMR_Stop(TMR0);
}

/*****************************************************************************
* 功能:		timer1配置
*****************************************************************************/
void TMR1_Config(void)
{
	// (1)设置Timer1 的时钟
	SYS_EnablePeripheralClk(SYS_CLK_TIMER01_MSK);						// 打开Timer0/1的时钟
	TMR_ConfigClk(TMR1, TMR_CLK_SEL_APB, TMR_CLK_DIV_1);				// TMR1clk = Pclk/1 = 3MHz

	// (2)设置Timer1 运行模式
	TMR_ConfigRunMode(TMR1, TMR_COUNT_PERIOD_MODE, TMR_BIT_32_MODE);	// 32位、周期计数模式
	TMR_DisableOneShotMode(TMR1);										// 关闭单次模式
	//	TMR_EnableOneShotMode(TMR1);									// 开启单次模式

	// (3)设置Timer1 运行周期
	TMR_SetPeriod(TMR1, 3000);											// Ftmr1 = TMR1clk/3000 = 1000Hz,  Ttmr1 = 1/Ftmr1 = 1ms

	// (4)设置Timer1 中断
	TMR_EnableOverflowInt(TMR1);
	NVIC_EnableIRQ(TMR1_IRQn);

	// (5)设置Timer1 中断优先级
	NVIC_SetPriority(TMR1_IRQn, 1);

	// (6)开启Timer1
	TMR_Start(TMR1);
}




static uint8_t  fac_us=0;//us延时倍乘数
static uint16_t fac_ms=0;//ms延时倍乘数

void delay_ms(uint16_t nms)
{
	uint32_t temp;
	SysTick->LOAD = (uint32_t)nms * fac_ms;//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL = 0x00;           //清空计数器
	SysTick->CTRL = 0x01;          //开始倒数  
	do
	{
		temp = SysTick->CTRL;
	} while (temp & 0x01 && !(temp & (1 << 16)));///等待时间到达 
	SysTick->CTRL = 0x00;       //关闭计数器
	SysTick->VAL = 0X00;       //清空计数器	  	    
}






void Dveice_Init(uint32_t SYSCLK)
{
	/*delay函数*/
	//初始化延迟函数
	//SYSTICK的时钟固定为HCLK时钟的1/8
	//SYSCLK:系统时钟
	SysTick->CTRL &= 0xfffffffb;//bit2清空,选择外部时钟  HCLK/8
	fac_us = SYSCLK / 8;
	fac_ms = (uint16_t)fac_us * 1000;
}
