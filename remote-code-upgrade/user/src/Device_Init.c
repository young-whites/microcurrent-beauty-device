#include "Device_Init.h"

#include "key.h"
#include "gpioctl.h"
#include "work.h"
#include "app_epwm.h"
#include "app_adc1.h"
#include "bsp_timer.h"    
#include "ch455.h"    
#include "system.h"    
#include "bsp_uart.h"    
#include "string.h"    
#include "clock.h"    
#include "uart.h"


 /*delay函数初始化*/
static u8  fac_us=0;//us延时倍乘数
static u16 fac_ms=0;//ms延时倍乘数

//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864 
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           //清空计数器
	SysTick->CTRL=0x01 ;          //开始倒数  
	do
	{  
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));///等待时间到达 
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	  	    
}   
//延时nus
//nus为要延时的us数.			    								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //时间加载	  		 
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL=0x01 ;      //开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器 
}

void Dveice_Init(u32 SYSCLK) 
{
	/*delay函数*/
	//初始化延迟函数
	//SYSTICK的时钟固定为HCLK时钟的1/8
	//SYSCLK:系统时钟
	SysTick->CTRL&=0xfffffffb;//bit2清空,选择外部时钟  HCLK/8
	fac_us=SYSCLK/8;		    
	fac_ms=(u16)fac_us*1000;
}

void MCU_TIM_Init(void)
{

}

void SoftWarePara_Init(void)
{

}

void SoftWarePara_Reset(void)
{

}

void System_Device_Init(void) 
{
	gpio_init();//IO口初始化
	ADC_InitConfig();//ADC初始化
 	Timer0_init();
	Timer1_init();//定时器初始化
	EPWM_OutPut_IndependentMode();//PWM输出配置初始化
	UART_UART1_Config();
	UART_ReceiveValueInit(&USART3_QueueBuf,UART_DATALENGTH);
}

void init_mode(void)
{

}



void Power_Off(void)
{
	
}

void EPWM_OutPut_IndependentMode(void)
{
	/*
	(1)设置EPWM运行模式
	*/
	EPWM_ConfigRunMode   //( EPWM_COUNT_DOWN);// 		| 			/*向下计数模式(边沿对齐)*/
	// 					 (EPWM_OCU_SYMMETRIC );	//|			/*对称模式*/
	// 					 (EPWM_WFG_INDEPENDENT ); // |			/*独立模式*/
	 					 (EPWM_OC_INDEPENDENT);	//			/*独立输出模式*/
 
	/*
	(2)设置EPWM时钟周期
	*/
	SYS_EnablePeripheralClk(SYS_CLK_EPWM_MSK);				/*开启EPWM时钟*/
	
	EPWM_ConfigChannelClk( EPWM0, EPWM_CLK_DIV_1);			/*设置EPWM0时钟为APB时钟的1分频*/
	EPWM_ConfigChannelClk( EPWM1, EPWM_CLK_DIV_1);			/*设置EPWM1时钟为APB时钟的1分频*/
	EPWM_ConfigChannelClk( EPWM2, EPWM_CLK_DIV_1);			/*设置EPWM2时钟为APB时钟的1分频*/
	EPWM_ConfigChannelClk( EPWM3, EPWM_CLK_DIV_1);			/*设置EPWM3时钟为APB时钟的1分频*/
	EPWM_ConfigChannelClk( EPWM4, EPWM_CLK_DIV_1);			/*设置EPWM4时钟为APB时钟的1分频*/
	EPWM_ConfigChannelClk( EPWM5, EPWM_CLK_DIV_1);			/*设置EPWM5时钟为APB时钟的1分频*/

	EPWM_ConfigChannelPeriod(EPWM0,  2400);					/*EPWM0通道的周期 = 2400/Fepwm = 50us*/
	EPWM_ConfigChannelPeriod(EPWM1,  2400);					/*EPWM1通道的周期 = 2400/Fepwm = 50us*/
	EPWM_ConfigChannelPeriod(EPWM2,  2400);					/*EPWM2通道的周期 = 2400/Fepwm = 50us*/
	EPWM_ConfigChannelPeriod(EPWM3,  2400);					/*EPWM3通道的周期 = 2400/Fepwm = 50us*/
	EPWM_ConfigChannelPeriod(EPWM4,  2400);					/*EPWM4通道的周期 = 2400/Fepwm = 50us*/
	EPWM_ConfigChannelPeriod(EPWM5,  2400);					/*EPWM5通道的周期 = 2400/Fepwm = 50us*/

	EPWM_ConfigChannelSymDuty(EPWM0, 0);					/*EPWM0通道的占空比 = 0%       0*/
	EPWM_ConfigChannelSymDuty(EPWM1, 0);					/*EPWM1通道的占空比 = 20%    480*/
	EPWM_ConfigChannelSymDuty(EPWM2, 960);					/*EPWM2通道的占空比 = 40%    960*/
	EPWM_ConfigChannelSymDuty(EPWM3, 1440);					/*EPWM3通道的占空比 = 60%    1400*/	
	EPWM_ConfigChannelSymDuty(EPWM4, 1920);					/*EPWM4通道的占空比 = 80%    1920*/	
	EPWM_ConfigChannelSymDuty(EPWM5, 2400);					/*EPWM5通道的占空比 = 100%   2400*/	
		
	/*
	(3)设置EPWM反向输出
	*/
	EPWM_DisableReverseOutput( EPWM_CH_0_MSK | EPWM_CH_1_MSK |
							   EPWM_CH_2_MSK | EPWM_CH_3_MSK |
							   EPWM_CH_4_MSK | EPWM_CH_5_MSK );				/*关闭反相输出*/

	/*
	(4)设置EPWM加载方式
	*/
	EPWM_EnableAutoLoadMode(EPWM_CH_0_MSK | EPWM_CH_1_MSK |
							EPWM_CH_2_MSK | EPWM_CH_3_MSK |
							EPWM_CH_4_MSK | EPWM_CH_5_MSK);				/*设置为自动加载模式*/
	EPWM_ConfigLoadAndIntMode(EPWM0, EPWM_EACH_PERIOD_ZERO);			
	EPWM_ConfigLoadAndIntMode(EPWM1, EPWM_EACH_PERIOD_ZERO);
	EPWM_ConfigLoadAndIntMode(EPWM2, EPWM_EACH_PERIOD_ZERO);
	EPWM_ConfigLoadAndIntMode(EPWM3, EPWM_EACH_PERIOD_ZERO);
	EPWM_ConfigLoadAndIntMode(EPWM4, EPWM_EACH_PERIOD_ZERO);
	EPWM_ConfigLoadAndIntMode(EPWM5, EPWM_EACH_PERIOD_ZERO);			/*加载点设置为每个周期点和零点*/

/*
	(5)设置中断
	*/
	// EPWM_EnablePeriodInt( EPWM_CH_0_MSK);				/*开启周期点中断*/
	// EPWM_EnableDownCmpInt( EPWM_CH_0_MSK);				/*开启向下比较点中断*/
	// EPWM_EnableZeroInt( EPWM_CH_0_MSK);					/*开启零点中断*/
	NVIC_EnableIRQ(EPWM_IRQn);
	/*
	(6)设置优先级
	*/	
	NVIC_SetPriority(EPWM_IRQn,3);					/*优先级0~3， 0最高、3最低*/
	
	/*
	(7)设置IO口输出
	*/	
	// SYS_SET_IOCFG(IOP12CFG, SYS_IOCFG_P01_EPWM0);		
	SYS_SET_IOCFG(IOP10CFG, SYS_IOCFG_P10_EPWM1);	
	// SYS_SET_IOCFG(IOP05CFG, SYS_IOCFG_P05_EPWM2);	
	// SYS_SET_IOCFG(IOP06CFG, SYS_IOCFG_P06_EPWM3);	
	// SYS_SET_IOCFG(IOP07CFG, SYS_IOCFG_P07_EPWM4);	
	// SYS_SET_IOCFG(IOP21CFG, SYS_IOCFG_P21_EPWM5);	
	
	EPWM_EnableOutput(EPWM_CH_0_MSK | EPWM_CH_1_MSK |
					  EPWM_CH_2_MSK | EPWM_CH_3_MSK |
					  EPWM_CH_4_MSK | EPWM_CH_5_MSK );

	/*
	(8)开启EPWM
	*/		
	EPWM_Start(EPWM_CH_0_MSK | EPWM_CH_1_MSK |
			   EPWM_CH_2_MSK | EPWM_CH_3_MSK |
			   EPWM_CH_4_MSK | EPWM_CH_5_MSK );
	
	
	/*
	(8)设置GPIO输出
	*/
	// SYS_SET_IOCFG(IOP12CFG, SYS_IOCFG_P12_GPIO);	
	// GPIO_CONFIG_IO_MODE(GPIO1,GPIO_PIN_2,GPIO_MODE_OUTPUT);		/*指示周期中断*/
	// GPIO1->DO_f.P2 = 0;	
	
	// SYS_SET_IOCFG(IOP13CFG, SYS_IOCFG_P13_GPIO);	
	// GPIO_CONFIG_IO_MODE(GPIO1,GPIO_PIN_3,GPIO_MODE_OUTPUT);		/*指示比较点中断*/
	// GPIO1->DO_f.P3 = 0;		
	
	// SYS_SET_IOCFG(IOP14CFG, SYS_IOCFG_P14_GPIO);	
	// GPIO_CONFIG_IO_MODE(GPIO1,GPIO_PIN_4,GPIO_MODE_OUTPUT);		/*指示比较点中断*/
	// GPIO1->DO_f.P4 = 0;		
	
}
