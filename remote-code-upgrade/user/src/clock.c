#include "cms32f033.h"
#include "system.h"

volatile  uint32_t TickCounter;


void Delay_Us (uint32_t delay)
{
  uint8_t i=0;
  uint32_t j=0;
  for(i=0;i<delay;i++)
  {
    for(j=0;j<8;j++);
  }
}


void Delay_Ms(uint32_t delay )
{
  uint32_t i=0;
  uint32_t j=0;
  
  for(i=0;i<delay;i++)
  {
    for(j=0;j<4540;j++);
	// for(j=0;j<100;j++);
  }
}

void HAL_Delay_nMS( uint32_t Delay )
{
//    uint32_t tickstart = 0;
//    tickstart = HAL_GetTick( );
//    while( ( HAL_GetTick( ) - tickstart ) < Delay );
}
//初始化时钟
void CLOCK_Init(void)
{
	SYS_DisableIOCFGProtect();			/*关闭IOCONFIG写保护*/
	SYS_DisableGPIO0Protect();			/*关闭GPIO0的相关寄存器写保护*/
	SYS_DisableGPIO1Protect();			/*关闭GPIO1的相关寄存器写保护*/
	SYS_DisableGPIO2Protect();			/*关闭GPIO2的相关寄存器写保护*/
	SYS_DisableGPIO3Protect();			/*关闭GPIO3的相关寄存器写保护*/
	SYS_DisableGPIO4Protect();			/*关闭GPIO4的相关寄存器写保护*/
	
	SYS_ConfigHSI(SYS_CLK_HSI_48M);		/*设置内部高速时钟为48Mhz*/
	SYS_EnableHSI();					/*开启高速时钟*/
	SYS_ConfigAHBClock(SYS_CLK_SEL_HSI,SYS_CLK_DIV_1);	/*设置AHB时钟为高速时钟的1分频*/
	SYS_ConfigAPBClock(AHB_CLK_DIV_1);					/*设置APB时钟为AHB时钟的1分频*/

}
/*****************************************************************************
 ** \brief	Systick_Config
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void Systick_Config(void)
{

    SystemInit();
	/*
	(1)设置Systick 定时时间
	*/
	SysTick_Config(48000);	/*1000us=1ms*/	
	/*
	(2)设置Systick 中断
	*/	
	NVIC_SetPriority(SysTick_IRQn,3);
	NVIC_EnableIRQ(SysTick_IRQn);	
}
