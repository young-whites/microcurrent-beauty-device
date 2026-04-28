#include "bsp_timer.h"
#include "system.h"
// #include "work.h"

volatile u16 SysMsCntForDela1 = 0;
volatile u32 SysMsCntForDela2 = 0;

void Timer0_init(void)
{
   	/*
	(1)设置TMR 的时钟
	*/
	SYS_EnablePeripheralClk(SYS_CLK_TIMER01_MSK);
	TMR_ConfigClk(TMR0,TMR_CLK_SEL_APB,TMR_CLK_DIV_256);		/*48MHz*/
	/*
	(2)设置TMR 运行模式
	*/	
	TMR_ConfigRunMode(TMR0,TMR_COUNT_PERIOD_MODE, TMR_BIT_16_MODE);
	TMR_DisableOneShotMode(TMR0);
	/*
	(3)设置TMR 运行周期
	*/		
	TMR_SetPeriod(TMR0,15000);			/*1000us = 1ms = 48000*Fclk*/
	/*
	(4)设置TMR 中断
	*/	
	TMR_EnableOverflowInt(TMR0);
	NVIC_EnableIRQ(TMR0_IRQn);	
	NVIC_SetPriority(TMR0_IRQn,2);	//优先级2
	/*
	(5)开启TMR
	*/	
	TMR_Start(TMR0); 
}

void Timer1_init(void)
{
   	/*
	(1)设置TMR 的时钟
	*/
	SYS_EnablePeripheralClk(SYS_CLK_TIMER01_MSK);
	TMR_ConfigClk(TMR1,TMR_CLK_SEL_APB,TMR_CLK_DIV_256);		/*48MHz*3/
	/*
	(2)设置TMR 运行模式
	*/	
	TMR_ConfigRunMode(TMR1,TMR_COUNT_PERIOD_MODE, TMR_BIT_16_MODE);
	TMR_DisableOneShotMode(TMR1);
	/*
	(3)设置TMR 运行周期
	*/		
	TMR_SetPeriod(TMR1,30000);			/*1000us = 1ms = 48000*Fclk*/
	/*
	(4)设置TMR 中断
	*/	
	TMR_EnableOverflowInt(TMR1);
	NVIC_EnableIRQ(TMR1_IRQn);	
	NVIC_SetPriority(TMR1_IRQn,2);	//优先级2
	/*
	(5)开启TMR
	*/	
	TMR_Start(TMR1); 
}
void SetLongDelay2( u32 TimeMs )//1800
{
    SysMsCntForDela2 = TimeMs;
    while(SysMsCntForDela2!=0);
}

u8 TimerOutLongDelay2( void )
{
    if (SysMsCntForDela2!=0 )
    {
        SysMsCntForDela2--;
    }
}