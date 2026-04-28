#include "cms32f033x.h"
#include "Psfr_system.h"
#include "system_cms32f033x.h"

uint32_t SystemCoreClock = __HIRC_48M; 	/*系统默认选择并开启48M高速时钟*/
uint32_t SystemAPBClock = __HIRC_48M;    
uint32_t CyclesPerUs;       

uint32_t HSIClock[4] ={__HIRC_64M, NULL, NULL, __HIRC_48M};

/*****************************************************************************
 ** 功能：	系统初始化
*****************************************************************************/
void  SystemInit(void)
{
	;
}

/*****************************************************************************
 ** 功能：	加载SystemCoreClock值
*****************************************************************************/
void  SystemCoreClockUpdate(void)
{
	uint32_t HSIClk;
	uint32_t AHBClk, AHBClkDiv;
	uint32_t APBClkDiv;
	
	HSIClk  = HSIClock[(SYS->CLKCON & SYS_CLKCON_IRCSEL_Msk)];		/*获取HSI的值*/
	
	if(SYS_CLK_SEL_HSI == (SYS->CLKSEL & SYS_CLKSEL_CLKSEL_Msk))	/*获取AHB的值*/
	{
		AHBClk = HSIClk;
	}
	else if(SYS_CLK_SEL_LSI == (SYS->CLKSEL & SYS_CLKSEL_CLKSEL_Msk))
	{
		AHBClk = __LIRC;
	}
	
	AHBClkDiv = (SYS->AHBCKDIV & SYS_APBCKDIV_APBDIV_Msk) * 2;		/*获取AHB分频*/
	if( 0 == AHBClkDiv)
	{
		SystemCoreClock = AHBClk;									/*获取SystemCoreClock*/
	}	
	else
	{
		SystemCoreClock = AHBClk / AHBClkDiv;
	}
	
	APBClkDiv = (SYS->APBCKDIV & SYS_APBCKDIV_APBDIV_Msk) * 2;		/*获取APB分频*/	
	if( 0 == APBClkDiv)
	{
		SystemAPBClock = SystemCoreClock;									/*获取SystemAPBClock*/
	}
	else
	{
		SystemAPBClock = SystemCoreClock / APBClkDiv;
	}
} 






