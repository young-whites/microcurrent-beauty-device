/*******************************************************************************
* Copyright (C) 2019 China Micro Semiconductor Limited Company. All Rights Reserved.
*
* This software is owned and published by:
* CMS LLC, No 2609-10, Taurus Plaza, TaoyuanRoad, NanshanDistrict, Shenzhen, China.
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with CMS
* components. This software is licensed by CMS to be adapted only
* for use in systems utilizing CMS components. CMS shall not be
* responsible for misuse or illegal use of this software for devices not
* supported herein. CMS is providing this software "AS IS" and will
* not be responsible for issues arising from incorrect user implementation
* of the software.
*
* This software may be replicated in part or whole for the licensed use,
* with the restriction that this Disclaimer and Copyright notice must be
* included with each copy of this software, whether used in part or whole,
* at all times.
*/

/****************************************************************************/
/** \file app_epwm.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "app_epwm.h"
/****************************************************************************/
/*	Local pre-processor symbols/macros('#define')
*****************************************************************************/

/****************************************************************************/
/*	Global variable definitions(declared in header file with 'extern')
*****************************************************************************/

/****************************************************************************/
/*	Local type definitions('typedef')
*****************************************************************************/

/****************************************************************************/
/*	Local variable  definitions('static')
*****************************************************************************/


/****************************************************************************/
/*	Local function prototypes('static')
*****************************************************************************/

/****************************************************************************/
/*	Function implementation - global ('extern') and local('static')
*****************************************************************************/
void EPWM_MT_000(void)
{
	unsigned int Period=150;
	/*
	(1)设置EPWM运行模式
	*/
	EPWM_ConfigRunMode(  EPWM_COUNT_DOWN 		| 			/*向下计数模式*/
						 EPWM_OCU_SYMMETRIC 	|			/*对称模式*/
						 EPWM_WFG_INDEPENDENT   |			/*独立模式*/
						 EPWM_OC_INDEPENDENT);				/*独立输出模式*/
	/*
	(2)设置EPWM时钟周期
	*/
	SYS_EnablePeripheralClk(SYS_CLK_EPWM_MSK);	
	
	EPWM_ConfigChannelClk( EPWM2, EPWM_CLK_DIV_1);
	EPWM_ConfigChannelPeriod(EPWM2,  Period);
	EPWM_ConfigChannelSymDuty(EPWM2, Period/2);	
	/*
	(3)设置EPWM反向输出
	*/
	EPWM_DisableReverseOutput( EPWM_CH_2_MSK);

	/*
	(4)设置EPWM加载方式
	*/
	EPWM_EnableAutoLoadMode(EPWM_CH_2_MSK);				/*自动加载*/
	/*
	(5)设置中断
	*/
//	EPWM_EnableZeroInt( EPWM_CH_0_MSK);
//	NVIC_EnableIRQ(EPWM_IRQn);
	/*
	(6)设置优先级
	*/	
//	NVIC_SetPriority(EPWM_IRQn,3);					/*优先级0~3， 0最高、3最低*/
	/*
	(7)设置IO口输出
	*/	
	SYS_SET_IOCFG(IOP24CFG, SYS_IOCFG_P24_EPWM2);	
	EPWM_EnableOutput(EPWM_CH_2_MSK );
	/*
	(8)开启EPWM
	*/		
	EPWM_Stop(EPWM_CH_2_MSK );
}
/*****************************************************************************
 ** \brief	EPWM_MT_001
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void EPWM_MT_001(void)
{
	/*
	(1)设置EPWM运行模式
	*/
	EPWM_ConfigRunMode(  EPWM_COUNT_DOWN 		| 			/*向下计数模式*/
						 EPWM_OCU_SYMMETRIC 	|			/*对称模式*/
						 EPWM_WFG_INDEPENDENT   |			/*独立模式*/
						 EPWM_OC_INDEPENDENT);				/*独立输出模式*/
	/*
	(2)设置EPWM时钟周期
	*/
	SYS_EnablePeripheralClk(SYS_CLK_EPWM_MSK);	
	
	EPWM_ConfigChannelClk( EPWM4, EPWM_CLK_DIV_1);
	EPWM_ConfigChannelPeriod(EPWM4,  2400);
	EPWM_ConfigChannelSymDuty(EPWM4, 0);	
	/*
	(3)设置EPWM反向输出
	*/
	EPWM_DisableReverseOutput( EPWM_CH_4_MSK);

	/*
	(4)设置EPWM加载方式
	*/
	EPWM_EnableAutoLoadMode(EPWM_CH_4_MSK);				/*自动加载*/
	/*
	(5)设置中断
	*/
//	EPWM_EnableZeroInt( EPWM_CH_0_MSK);
//	NVIC_EnableIRQ(EPWM_IRQn);
	/*
	(6)设置优先级
	*/	
//	NVIC_SetPriority(EPWM_IRQn,3);					/*优先级0~3， 0最高、3最低*/
	/*
	(7)设置IO口输出
	*/	
	SYS_SET_IOCFG(IOP31CFG, SYS_IOCFG_P31_EPWM4);	
	EPWM_EnableOutput(EPWM_CH_4_MSK );
	/*
	(8)开启EPWM
	*/		
	EPWM_Stop(EPWM_CH_4_MSK );
}
/*****************************************************************************
 ** \brief	EPWM_MT_002
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void EPWM_MT_002(void)
{
	/*
	(1)设置EPWM运行模式
	*/
	EPWM_ConfigRunMode(  EPWM_COUNT_DOWN 		| 			/*向下计数模式*/
						 EPWM_OCU_SYMMETRIC 	|			/*对称模式*/
						 EPWM_WFG_COMPLEMENTARYK   |			/*互补模式*/
						 EPWM_OC_INDEPENDENT);				/*独立输出模式*/
	/*
	(2)设置EPWM时钟周期
	*/
	SYS_EnablePeripheralClk(SYS_CLK_EPWM_MSK);	
	
	EPWM_ConfigChannelClk( EPWM0, EPWM_CLK_DIV_1);
	EPWM_ConfigChannelPeriod(EPWM0,  2400);
	EPWM_ConfigChannelSymDuty(EPWM0, 1200);	
	
	/*
	(3)设置EPWM反向输出
	*/
	//EPWM_DisableReverseOutput( EPWM_CH_0_MSK|EPWM_CH_1_MSK);
	EPWM_EnableReverseOutput( EPWM_CH_0_MSK | EPWM_CH_1_MSK);
	/*
	(4)设置EPWM加载方式
	*/
	EPWM_EnableAutoLoadMode(EPWM_CH_0_MSK);				/*自动加载*/
	/*
	(5)设置中断
	*/
	EPWM_EnableZeroInt( EPWM_CH_0_MSK);
	NVIC_EnableIRQ(EPWM_IRQn);
	/*
	(6)设置优先级
	*/	
	NVIC_SetPriority(EPWM_IRQn,3);					/*优先级0~3， 0最高、3最低*/
	/*
	(7)设置IO口输出
	*/	
	SYS_SET_IOCFG(IOP01CFG, SYS_IOCFG_P01_EPWM0);	
	SYS_SET_IOCFG(IOP04CFG, SYS_IOCFG_P04_EPWM1);	
	EPWM_EnableOutput(EPWM_CH_0_MSK | EPWM_CH_1_MSK);
	/*
	(8)开启EPWM
	*/		
	EPWM_Start(EPWM_CH_0_MSK);
}

/*****************************************************************************
 ** \brief	EPWM_MT_003
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void EPWM_MT_003(void)
{
	/* 
	(1)设置EPWM运行模式
	*/
	EPWM_ConfigRunMode(  EPWM_COUNT_UP_DOWN  | 				/*xx计数模式*/
						 EPWM_OCU_SYMMETRIC 	  |			/*对称模式*/
						 EPWM_WFG_COMPLEMENTARYK   |		/*互补模式*/
						 EPWM_OC_INDEPENDENT);				/*独立输出模式*/
	/*
	(2)设置EPWM时钟周期
	*/
	SYS_EnablePeripheralClk(SYS_CLK_EPWM_MSK);	
	
	EPWM_ConfigChannelClk( EPWM0, EPWM_CLK_DIV_1);
	EPWM_ConfigChannelClk( EPWM2, EPWM_CLK_DIV_1);	
	EPWM_ConfigChannelClk( EPWM4, EPWM_CLK_DIV_1);	
		
	EPWM_ConfigChannelPeriod(EPWM0,  2400);
	EPWM_ConfigChannelPeriod(EPWM2,  2400);
	EPWM_ConfigChannelPeriod(EPWM4,  2400);
	EPWM_ConfigChannelSymDuty(EPWM0, 1200);	
	EPWM_ConfigChannelSymDuty(EPWM2, 1200);		
	EPWM_ConfigChannelSymDuty(EPWM4, 1200);		
	
	/*
	(3)设置EPWM反向输出
	*/
	EPWM_DisableReverseOutput( EPWM_CH_0_MSK | EPWM_CH_1_MSK |EPWM_CH_2_MSK|
								EPWM_CH_3_MSK| EPWM_CH_4_MSK|EPWM_CH_5_MSK);
	/*
	(4)设置EPWM加载方式
	*/
	EPWM_EnableAutoLoadMode(EPWM_CH_0_MSK |EPWM_CH_2_MSK|EPWM_CH_4_MSK);				/*自动加载*/
	
	/*
	(5)设置EPWM比较器0
	*/			
	/*
	(6)设置中断
	*/
	EPWM_EnableZeroInt( EPWM_CH_0_MSK);	
	NVIC_EnableIRQ(EPWM_IRQn);
	/*
	(7)设置优先级
	*/	
	NVIC_SetPriority(EPWM_IRQn,3);					/*优先级0~3， 0最高、3最低*/
	/*
	(8)设置IO口输出
	*/	
	SYS_SET_IOCFG(IOP01CFG, SYS_IOCFG_P01_EPWM0);	
	SYS_SET_IOCFG(IOP04CFG, SYS_IOCFG_P04_EPWM1);
	SYS_SET_IOCFG(IOP05CFG, SYS_IOCFG_P05_EPWM2);
	SYS_SET_IOCFG(IOP06CFG, SYS_IOCFG_P06_EPWM3);
	SYS_SET_IOCFG(IOP07CFG, SYS_IOCFG_P07_EPWM4);
	SYS_SET_IOCFG(IOP47CFG, SYS_IOCFG_P47_EPWM5);
	
	EPWM_EnableOutput(EPWM_CH_0_MSK | EPWM_CH_1_MSK|
					  EPWM_CH_2_MSK | EPWM_CH_3_MSK|
					  EPWM_CH_4_MSK | EPWM_CH_5_MSK);
	/*
	(9)开启EPWM
	*/		
	EPWM_Start(EPWM_CH_0_MSK | EPWM_CH_1_MSK|
				EPWM_CH_2_MSK | EPWM_CH_3_MSK|
				 EPWM_CH_4_MSK | EPWM_CH_5_MSK);
}
/*****************************************************************************
 ** \brief	EPWM_MT_004
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void EPWM_MT_004(void)
{
	/* 
	(1)设置EPWM运行模式
	*/
	EPWM_ConfigRunMode(  EPWM_COUNT_UP_DOWN  | 			/*XX计数模式*/
						 EPWM_OCU_SYMMETRIC 	  |			/*对称模式*/
						 EPWM_WFG_COMPLEMENTARYK   |			/*互补模式*/
						 EPWM_OC_INDEPENDENT);				/*独立输出模式*/
	/*
	(2)设置EPWM时钟周期
	*/
	SYS_EnablePeripheralClk(SYS_CLK_EPWM_MSK);	
	
	EPWM_ConfigChannelClk( EPWM0, EPWM_CLK_DIV_1);
	EPWM_ConfigChannelPeriod(EPWM0,  4800);
	EPWM_ConfigChannelSymDuty(EPWM0, 2400);	
	
	/*
	(3)设置EPWM反向输出
	*/
	EPWM_DisableReverseOutput( EPWM_CH_0_MSK | EPWM_CH_1_MSK);
	/*
	(4)设置EPWM加载方式
	*/
	EPWM_EnableAutoLoadMode(EPWM_CH_0_MSK);				/*自动加载*/
	
	/*
	(5)设置EPWM比较器
	*/	
	EPWM_ConfigCompareTiger(EPWM_CMPTG_1,EPWM_CMPTG_FALLING,EPWM_CMPTG_EPWM0,600);
	EPWM_ConfigCompareTiger(EPWM_CMPTG_0,EPWM_CMPTG_RISING,EPWM_CMPTG_EPWM0,600);		
	/*
	(6)设置中断
	*/
	EPWM_EnableZeroInt( EPWM_CH_0_MSK);	
	EPWM_EnableCountCmpInt(EPWM_CMPTG_0);
	EPWM_EnableCountCmpInt(EPWM_CMPTG_1);
	NVIC_EnableIRQ(EPWM_IRQn);
	/*
	(7)设置优先级
	*/	
	NVIC_SetPriority(EPWM_IRQn,3);					/*优先级0~3， 0最高、3最低*/
	/*
	(8)设置IO口输出
	*/	
	SYS_SET_IOCFG(IOP01CFG, SYS_IOCFG_P01_EPWM0);	
	SYS_SET_IOCFG(IOP04CFG, SYS_IOCFG_P04_EPWM1);	
	EPWM_EnableOutput(EPWM_CH_0_MSK | EPWM_CH_1_MSK);
	/*
	(9)开启EPWM
	*/		
	EPWM_Start(EPWM_CH_0_MSK);
}

/*****************************************************************************
 ** \brief	PWM_Config
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void PWM_Config(void)
{
	uint16_t period_temp = 2400;		 //1200X2=4800/48 = 100us (10kHz)
	uint16_t duty_temp = 0;				//占空比为0，所有通道输出为低电平	
	/*	
	 (1)设置EPWM为同步模式
	*/
	EPWM_ConfigRunMode(EPWM_WFG_SYNCHRONIZED      | 				/*设置为同步模式*/
						EPWM_COUNT_UP_DOWN        | 				/*设置为中心对称模式*/
						EPWM_OCU_SYMMETRIC		  |					/*设置为对称模式*/
						EPWM_OC_INDEPENDENT);						/*独立控制*/
	/*
	 (2)设置EPWM通道周期以及占空比
	*/
	SYS_EnablePeripheralClk(SYS_CLK_EPWM_MSK);	
	
	EPWM_ConfigChannelClk(EPWM0,EPWM_CLK_DIV_1);	/*SystemCoreClock=64Mhz，EPWMclk =24Mhz*/
	EPWM_ConfigChannelClk(EPWM1,EPWM_CLK_DIV_1);	/*SystemCoreClock=64Mhz，EPWMclk =24Mhz*/
	EPWM_ConfigChannelClk(EPWM2,EPWM_CLK_DIV_1);	/*SystemCoreClock=64Mhz，EPWMclk =24Mhz*/
	EPWM_ConfigChannelClk(EPWM3,EPWM_CLK_DIV_1);	/*SystemCoreClock=64Mhz，EPWMclk =24Mhz*/	
	EPWM_ConfigChannelClk(EPWM4,EPWM_CLK_DIV_1);	/*SystemCoreClock=64Mhz，EPWMclk =24Mhz*/	
	EPWM_ConfigChannelClk(EPWM5,EPWM_CLK_DIV_1);	/*SystemCoreClock=64Mhz，EPWMclk =24Mhz*/	
	
	EPWM_ConfigChannelPeriod(EPWM0,  period_temp);
	EPWM_ConfigChannelPeriod(EPWM1,  period_temp);	
	EPWM_ConfigChannelPeriod(EPWM2,  period_temp);	
	EPWM_ConfigChannelPeriod(EPWM3,  period_temp);	
	EPWM_ConfigChannelPeriod(EPWM4,  period_temp);	
	EPWM_ConfigChannelPeriod(EPWM5,  period_temp);	
	
	EPWM_ConfigChannelSymDuty(EPWM0, duty_temp);	
	EPWM_ConfigChannelSymDuty(EPWM1, duty_temp);		
	EPWM_ConfigChannelSymDuty(EPWM2, duty_temp);		
	EPWM_ConfigChannelSymDuty(EPWM3, duty_temp);	
	EPWM_ConfigChannelSymDuty(EPWM4, duty_temp);	
	EPWM_ConfigChannelSymDuty(EPWM5, duty_temp);	
	/*
	(3)设置EPWM输出极性	
	*/
	EPWM_EnableReverseOutput(	  EPWM_CH_0_MSK | EPWM_CH_1_MSK |				/*开启反向输出*/
								  EPWM_CH_2_MSK | EPWM_CH_3_MSK |
								  EPWM_CH_4_MSK | EPWM_CH_5_MSK);	
	
		/*
	(4)设置EPWM加载方式
	*/
	EPWM_EnableAutoLoadMode( EPWM_CH_0_MSK | EPWM_CH_1_MSK |
							  EPWM_CH_2_MSK | EPWM_CH_3_MSK |
							  EPWM_CH_4_MSK | EPWM_CH_5_MSK);				/*自动加载*/	
	/*
	(4)设置EPWM死区控制
	*/
	EPWM_DisableDeadZone(	  EPWM_CH_0_MSK | EPWM_CH_1_MSK |
							  EPWM_CH_2_MSK | EPWM_CH_3_MSK |
							  EPWM_CH_4_MSK | EPWM_CH_5_MSK);	
	/*
	(5)设置EPWM刹车
	*/

	/*
	(6)设置EPWM零点中断
	*/	
	EPWM_EnableZeroInt(EPWM_CH_0_MSK);									//pwm0 零点中断	
	NVIC_EnableIRQ(EPWM_IRQn);
	
	/*
	(7)设置EPWM中断优先级，开启总中断
	*/
	NVIC_SetPriority(EPWM_IRQn,0);					/*优先级0~3， 0最高、3最低*/	
	
	/*
	(8)设置EPWM io口复用
	*/
	SYS_SET_IOCFG(IOP01CFG, SYS_IOCFG_P01_EPWM0);	
	SYS_SET_IOCFG(IOP04CFG, SYS_IOCFG_P04_EPWM1);
	SYS_SET_IOCFG(IOP05CFG, SYS_IOCFG_P05_EPWM2);
	SYS_SET_IOCFG(IOP06CFG, SYS_IOCFG_P06_EPWM3);
	SYS_SET_IOCFG(IOP07CFG, SYS_IOCFG_P07_EPWM4);
	SYS_SET_IOCFG(IOP47CFG, SYS_IOCFG_P47_EPWM5);	
	
	EPWM_EnableOutput( EPWM_CH_0_MSK | EPWM_CH_1_MSK |
					   EPWM_CH_2_MSK | EPWM_CH_3_MSK |
					   EPWM_CH_4_MSK | EPWM_CH_5_MSK);	
	
	/*
	(9)使能EPWM输出
	*/
	EPWM_Start(	EPWM_CH_0_MSK | EPWM_CH_1_MSK |
				EPWM_CH_2_MSK | EPWM_CH_3_MSK |
				EPWM_CH_4_MSK | EPWM_CH_5_MSK);
}


/*****************************************************************************
 ** \brief	EPWM_MT_HALL
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void EPWM_MT_HALL(void)
{
	uint16_t period_temp = 2400;		 //1200X2=4800/48 = 100us (10kHz)
	uint16_t duty_temp = 0;				//占空比为0，所有通道输出为低电平	
	/*	
	 (1)设置EPWM为同步模式
	*/
	EPWM_ConfigRunMode(EPWM_WFG_SYNCHRONIZED      | 				/*设置为同步模式*/
						EPWM_COUNT_UP_DOWN        | 				/*设置为中心对称模式*/
						EPWM_OCU_SYMMETRIC		  |					/*设置为对称模式*/
						EPWM_OC_INDEPENDENT);						/*独立控制*/
	/*
	 (2)设置EPWM通道周期以及占空比
	*/
	SYS_EnablePeripheralClk(SYS_CLK_EPWM_MSK);	
	
	EPWM_ConfigChannelClk(EPWM0,EPWM_CLK_DIV_1);	/*SystemCoreClock=64Mhz，EPWMclk =24Mhz*/
	EPWM_ConfigChannelClk(EPWM1,EPWM_CLK_DIV_1);	/*SystemCoreClock=64Mhz，EPWMclk =24Mhz*/
	EPWM_ConfigChannelClk(EPWM2,EPWM_CLK_DIV_1);	/*SystemCoreClock=64Mhz，EPWMclk =24Mhz*/
	EPWM_ConfigChannelClk(EPWM3,EPWM_CLK_DIV_1);	/*SystemCoreClock=64Mhz，EPWMclk =24Mhz*/	
	EPWM_ConfigChannelClk(EPWM4,EPWM_CLK_DIV_1);	/*SystemCoreClock=64Mhz，EPWMclk =24Mhz*/	
	EPWM_ConfigChannelClk(EPWM5,EPWM_CLK_DIV_1);	/*SystemCoreClock=64Mhz，EPWMclk =24Mhz*/	
	
	EPWM_ConfigChannelPeriod(EPWM0,  period_temp);
	EPWM_ConfigChannelPeriod(EPWM1,  period_temp);	
	EPWM_ConfigChannelPeriod(EPWM2,  period_temp);	
	EPWM_ConfigChannelPeriod(EPWM3,  period_temp);	
	EPWM_ConfigChannelPeriod(EPWM4,  period_temp);	
	EPWM_ConfigChannelPeriod(EPWM5,  period_temp);	
	
	EPWM_ConfigChannelSymDuty(EPWM0, duty_temp);	
	EPWM_ConfigChannelSymDuty(EPWM1, duty_temp);		
	EPWM_ConfigChannelSymDuty(EPWM2, duty_temp);		
	EPWM_ConfigChannelSymDuty(EPWM3, duty_temp);	
	EPWM_ConfigChannelSymDuty(EPWM4, duty_temp);	
	EPWM_ConfigChannelSymDuty(EPWM5, duty_temp);	
	/*
	(3)设置EPWM输出极性	
	*/
	EPWM_EnableReverseOutput(	  EPWM_CH_0_MSK | EPWM_CH_1_MSK |				/*开启反向输出*/
								  EPWM_CH_2_MSK | EPWM_CH_3_MSK |
								  EPWM_CH_4_MSK | EPWM_CH_5_MSK);	
	
		/*
	(4)设置EPWM加载方式
	*/
	EPWM_EnableAutoLoadMode(  EPWM_CH_0_MSK | EPWM_CH_1_MSK |
							  EPWM_CH_2_MSK | EPWM_CH_3_MSK |
							  EPWM_CH_4_MSK | EPWM_CH_5_MSK);				/*自动加载*/	
	/*
	(4)设置EPWM死区控制
	*/
	EPWM_DisableDeadZone(	  EPWM_CH_0_MSK | EPWM_CH_1_MSK |
							  EPWM_CH_2_MSK | EPWM_CH_3_MSK |
							  EPWM_CH_4_MSK | EPWM_CH_5_MSK);	
	/*
	(5)设置EPWM刹车
	*/

	/*
	(6)设置EPWM零点中断
	*/	
	//EPWM_EnableZeroInt(EPWM_CH_0_MSK);									//pwm0 零点中断	
	//NVIC_EnableIRQ(EPWM_IRQn);
	
	/*
	(7)设置EPWM中断优先级，开启总中断
	*/
	//NVIC_SetPriority(EPWM_IRQn,0);					/*优先级0~3， 0最高、3最低*/	
	
	/*
	(8)设置EPWM io口复用
	*/
	SYS_SET_IOCFG(IOP01CFG, SYS_IOCFG_P01_EPWM0);	
	SYS_SET_IOCFG(IOP04CFG, SYS_IOCFG_P04_EPWM1);
	SYS_SET_IOCFG(IOP05CFG, SYS_IOCFG_P05_EPWM2);
	SYS_SET_IOCFG(IOP06CFG, SYS_IOCFG_P06_EPWM3);
	SYS_SET_IOCFG(IOP07CFG, SYS_IOCFG_P07_EPWM4);
	SYS_SET_IOCFG(IOP47CFG, SYS_IOCFG_P47_EPWM5);	
	
	EPWM_EnableOutput( EPWM_CH_0_MSK | EPWM_CH_1_MSK |
					   EPWM_CH_2_MSK | EPWM_CH_3_MSK |
					   EPWM_CH_4_MSK | EPWM_CH_5_MSK);		
	/*
	(9)使能EPWM输出
	*/
	EPWM_Start(	EPWM_CH_0_MSK | EPWM_CH_1_MSK |
				EPWM_CH_2_MSK | EPWM_CH_3_MSK |
				EPWM_CH_4_MSK | EPWM_CH_5_MSK);
		
/*----------------------------------------------------------------------------------*/
	/*
	(1)设置掩码BUFFER
	*/
	EPWM_WriteMaskBuffer(EPWM_MASK_BUF0, 0x3f00);		/*0 全部使能MASK，输出0*/
	EPWM_WriteMaskBuffer(EPWM_MASK_BUF1, 0x3f01);		/* 1 全部使能MASK，EPWM0输出1*/
	EPWM_WriteMaskBuffer(EPWM_MASK_BUF2, 0x3f02);		/*2 全部使能MASK，EPWM1输出1*/
	EPWM_WriteMaskBuffer(EPWM_MASK_BUF3, 0x3f04);		/*3 全部使能MASK，EPWM2输出1*/
	EPWM_WriteMaskBuffer(EPWM_MASK_BUF4, 0x3f08);		/*4 全部使能MASK，EPWM3输出1*/
	EPWM_WriteMaskBuffer(EPWM_MASK_BUF5, 0x3f10);		/*5 全部使能MASK，EPWM4输出1*/
	EPWM_WriteMaskBuffer(EPWM_MASK_BUF6, 0x3f20);		/*6 全部使能MASK，EPWM5输出1*/
	EPWM_WriteMaskBuffer(EPWM_MASK_BUF7, 0x3f3f);		/*7 全部使能MASK，输出1*/
	EPWM_EnableMaskPreload(EPWM_MASK_LOAD_EPWM0);
	EPWM_EnableHall();
}

/*****************************************************************************
 ** \brief	EPWM_MT_007
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void EPWM_MT_007(void)//20khz
{
	/*
	(1)设置EPWM运行模式
	*/
	EPWM_ConfigRunMode(  EPWM_COUNT_DOWN 		| 			/*向下计数模式*/
						 EPWM_OCU_SYMMETRIC 	|			/*对称模式*/
						 EPWM_WFG_COMPLEMENTARYK   |			/*互补模式*/
						 EPWM_OC_INDEPENDENT);				/*独立输出模式*/
	/*
	(2)设置EPWM时钟周期
	*/
	SYS_EnablePeripheralClk(SYS_CLK_EPWM_MSK);	
	
	EPWM_ConfigChannelClk( EPWM0, EPWM_CLK_DIV_1);
	EPWM_ConfigChannelPeriod(EPWM0,  1200);
	EPWM_ConfigChannelSymDuty(EPWM0, 600);	
	/*
	(3)设置EPWM死区
	*/
	EPWM_EnableDeadZone(EPWM_CH_0_MSK, 48);
		
	/*
	(4)设置EPWM反向输出
	*/
	EPWM_DisableReverseOutput( EPWM_CH_0_MSK|EPWM_CH_1_MSK);
	/*
	(5)设置EPWM加载方式
	*/
	EPWM_EnableAutoLoadMode(EPWM_CH_0_MSK);				/*自动加载*/
	
	/*
	(6)设置EPWM 刹车
	*/
	EPWM_EnableFaultBrake(EPWM_BRK_SOFT);
	EPWM_ConfigFaultBrakeLevel(EPWM_CH_0_MSK,0);
	EPWM_ConfigFaultBrakeLevel(EPWM_CH_1_MSK,1);	
	EPWM_AllBrakeEnable();
	
	/*
	(5)设置中断
	*/
	//EPWM_EnableZeroInt( EPWM_CH_0_MSK);
	//EPWM_EnableBrakeInt();
	//NVIC_EnableIRQ(EPWM_IRQn);
	/*
	(6)设置优先级
	*/	
	NVIC_SetPriority(EPWM_IRQn,3);					/*优先级0~3， 0最高、3最低*/
	/*
	(7)设置IO口输出
	*/	
	SYS_SET_IOCFG(IOP12CFG, SYS_IOCFG_P12_EPWM0);	
	SYS_SET_IOCFG(IOP44CFG, SYS_IOCFG_P44_EPWM1);	
	//EPWM_EnableOutput(EPWM_CH_0_MSK | EPWM_CH_1_MSK);
	EPWM_DisableOutput(EPWM_CH_0_MSK | EPWM_CH_1_MSK);//初始化先不输出

	/*
	(8)开启EPWM
	*/		
	EPWM_Start(EPWM_CH_0_MSK);	
}
/*****************************************************************************
 ** \brief	EPWM_MT_008
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void EPWM_MT_008(void)
{
	/*
	(1)设置EPWM运行模式
	*/
	EPWM_ConfigRunMode(  EPWM_COUNT_DOWN 		| 			/*向下计数模式*/
						 EPWM_OCU_SYMMETRIC 	|			/*对称模式*/
						 EPWM_WFG_COMPLEMENTARYK   |			/*互补模式*/
						 EPWM_OC_INDEPENDENT);				/*独立输出模式*/
	/*
	(2)设置EPWM时钟周期
	*/
	SYS_EnablePeripheralClk(SYS_CLK_EPWM_MSK);	
	
	EPWM_ConfigChannelClk( EPWM0, EPWM_CLK_DIV_1);
	EPWM_ConfigChannelPeriod(EPWM0,  2400);
	EPWM_ConfigChannelSymDuty(EPWM0, 1200);	
	/*
	(3)设置EPWM死区
	*/
	EPWM_EnableDeadZone(EPWM_CH_0_MSK, 48);
		
	/*
	(4)设置EPWM反向输出
	*/
	EPWM_DisableReverseOutput( EPWM_CH_0_MSK|EPWM_CH_1_MSK);
	/*
	(5)设置EPWM加载方式
	*/
	EPWM_EnableAutoLoadMode(EPWM_CH_0_MSK);				/*自动加载*/
	
	/*
	(6)设置EPWM 刹车
	*/
	EPWM_EnableFaultBrake(EPWM_BRK_EXT);			/*开启硬件刹车*/
	EPWM_EnableEXTBrake(EPWM_EXTBK_EDGE_RISING);	/*开启硬件上升沿刹车模式*/
	SYS_SET_IOCFG(IOP46CFG, SYS_IOCFG_P46_BKIN);			
	
	EPWM_ConfigFaultBrakeLevel(EPWM_CH_0_MSK,0);
	EPWM_ConfigFaultBrakeLevel(EPWM_CH_1_MSK,1);	
	EPWM_AllBrakeEnable();
	
	/*
	(7)设置中断
	*/
	EPWM_EnableZeroInt( EPWM_CH_0_MSK);
	EPWM_EnableBrakeInt();
	NVIC_EnableIRQ(EPWM_IRQn);
	/*
	(8)设置优先级
	*/	
	NVIC_SetPriority(EPWM_IRQn,3);					/*优先级0~3， 0最高、3最低*/
	/*
	(9)设置IO口输出
	*/	
	SYS_SET_IOCFG(IOP01CFG, SYS_IOCFG_P01_EPWM0);	
	SYS_SET_IOCFG(IOP04CFG, SYS_IOCFG_P04_EPWM1);	
	EPWM_EnableOutput(EPWM_CH_0_MSK | EPWM_CH_1_MSK);
	/*
	(10)开启EPWM
	*/		
	EPWM_Start(EPWM_CH_0_MSK);	

}

/*****************************************************************************
 ** \brief	EPWM_MT_009
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void EPWM_MT_009(void)
{
	/*
	(1)设置EPWM运行模式
	*/
	EPWM_ConfigRunMode(  EPWM_COUNT_DOWN 		| 			/*向下计数模式*/
						 EPWM_OCU_SYMMETRIC 	|			/*对称模式*/
						 EPWM_WFG_COMPLEMENTARYK   |			/*互补模式*/
						 EPWM_OC_INDEPENDENT);				/*独立输出模式*/
	/*
	(2)设置EPWM时钟周期
	*/
	SYS_EnablePeripheralClk(SYS_CLK_EPWM_MSK);	
	
	EPWM_ConfigChannelClk( EPWM0, EPWM_CLK_DIV_1);
	EPWM_ConfigChannelPeriod(EPWM0,  2400);
	EPWM_ConfigChannelSymDuty(EPWM0, 1200);	
	/*
	(3)设置EPWM死区
	*/
	EPWM_EnableDeadZone(EPWM_CH_0_MSK, 48);
		
	/*
	(4)设置EPWM反向输出
	*/
	EPWM_DisableReverseOutput( EPWM_CH_0_MSK|EPWM_CH_1_MSK);
	/*
	(5)设置EPWM加载方式
	*/
	EPWM_EnableAutoLoadMode(EPWM_CH_0_MSK);				/*自动加载*/
	
	/*
	(6)设置EPWM 刹车
	*/
	EPWM_EnableFaultBrake(EPWM_BRK_ACMP0);				
	EPWM_ConfigFaultBrakeLevel(EPWM_CH_0_MSK,0);
	EPWM_ConfigFaultBrakeLevel(EPWM_CH_1_MSK,1);	
	EPWM_AllBrakeEnable();
	
	/*
	(7)设置中断
	*/
	EPWM_EnableZeroInt( EPWM_CH_0_MSK);
	EPWM_EnableBrakeInt();
	NVIC_EnableIRQ(EPWM_IRQn);
	/*
	(8)设置优先级
	*/	
	NVIC_SetPriority(EPWM_IRQn,3);					/*优先级0~3， 0最高、3最低*/
	/*
	(9)设置IO口输出
	*/	
	SYS_SET_IOCFG(IOP01CFG, SYS_IOCFG_P01_EPWM0);	
	SYS_SET_IOCFG(IOP04CFG, SYS_IOCFG_P04_EPWM1);	
	EPWM_EnableOutput(EPWM_CH_0_MSK | EPWM_CH_1_MSK);
	/*
	(10)开启EPWM
	*/		
	EPWM_Start(EPWM_CH_0_MSK);	

}
/*****************************************************************************
 ** \brief	EPWM_MT_010
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void EPWM_MT_010(void)
{
	/*
	(1)设置EPWM运行模式
	*/
	EPWM_ConfigRunMode(  EPWM_COUNT_DOWN 		| 			/*向下计数模式*/
						 EPWM_OCU_SYMMETRIC 	|			/*对称模式*/
						 EPWM_WFG_COMPLEMENTARYK   |			/*互补模式*/
						 EPWM_OC_INDEPENDENT);				/*独立输出模式*/
	/*
	(2)设置EPWM时钟周期
	*/
	SYS_EnablePeripheralClk(SYS_CLK_EPWM_MSK);	
	
	EPWM_ConfigChannelClk( EPWM0, EPWM_CLK_DIV_1);
	EPWM_ConfigChannelPeriod(EPWM0,  2400);
	EPWM_ConfigChannelSymDuty(EPWM0, 1200);	
	/*
	(3)设置EPWM死区
	*/
	EPWM_EnableDeadZone(EPWM_CH_0_MSK, 48);
		
	/*
	(4)设置EPWM反向输出
	*/
	EPWM_DisableReverseOutput( EPWM_CH_0_MSK|EPWM_CH_1_MSK);
	/*
	(5)设置EPWM加载方式
	*/
	EPWM_EnableAutoLoadMode(EPWM_CH_0_MSK);				/*自动加载*/
	
	/*
	(6)设置EPWM 刹车
	*/
	EPWM_EnableFaultBrake(EPWM_BRK_ACMP1);				
	EPWM_ConfigFaultBrakeLevel(EPWM_CH_0_MSK,0);
	EPWM_ConfigFaultBrakeLevel(EPWM_CH_1_MSK,1);	
	EPWM_AllBrakeEnable();
	
	/*
	(7)设置中断
	*/
	EPWM_EnableZeroInt( EPWM_CH_0_MSK);
	EPWM_EnableBrakeInt();
	NVIC_EnableIRQ(EPWM_IRQn);
	/*
	(8)设置优先级
	*/	
	NVIC_SetPriority(EPWM_IRQn,3);					/*优先级0~3， 0最高、3最低*/
	/*
	(9)设置IO口输出
	*/	
	SYS_SET_IOCFG(IOP01CFG, SYS_IOCFG_P01_EPWM0);	
	SYS_SET_IOCFG(IOP04CFG, SYS_IOCFG_P04_EPWM1);	
	EPWM_EnableOutput(EPWM_CH_0_MSK | EPWM_CH_1_MSK);
	/*
	(10)开启EPWM
	*/		
	EPWM_Start(EPWM_CH_0_MSK);	

}
/*****************************************************************************
 ** \brief	EPWM_MT_011
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void EPWM_MT_011(void)
{
	/*
	(1)设置EPWM运行模式
	*/
	EPWM_ConfigRunMode(  EPWM_COUNT_DOWN 		| 			/*向下计数模式*/
						 EPWM_OCU_SYMMETRIC 	|			/*对称模式*/
						 EPWM_WFG_COMPLEMENTARYK   |			/*互补模式*/
						 EPWM_OC_INDEPENDENT);				/*独立输出模式*/
	/*
	(2)设置EPWM时钟周期
	*/
	SYS_EnablePeripheralClk(SYS_CLK_EPWM_MSK);	
	
	EPWM_ConfigChannelClk( EPWM0, EPWM_CLK_DIV_1);
	EPWM_ConfigChannelPeriod(EPWM0,  2400);
	EPWM_ConfigChannelSymDuty(EPWM0, 1200);	
	/*
	(3)设置EPWM死区
	*/
	EPWM_EnableDeadZone(EPWM_CH_0_MSK, 48);
		
	/*
	(4)设置EPWM反向输出
	*/
	EPWM_DisableReverseOutput( EPWM_CH_0_MSK|EPWM_CH_1_MSK);
	/*
	(5)设置EPWM加载方式
	*/
	EPWM_EnableAutoLoadMode(EPWM_CH_0_MSK);				/*自动加载*/
	
	/*
	(6)设置EPWM 刹车
	*/
	EPWM_EnableFaultBrake(EPWM_BRK_ADC1CMP0);				
	EPWM_ConfigFaultBrakeLevel(EPWM_CH_0_MSK,0);
	EPWM_ConfigFaultBrakeLevel(EPWM_CH_1_MSK,1);	
	EPWM_AllBrakeEnable();
	
	/*
	(7)设置中断
	*/
	EPWM_EnableZeroInt( EPWM_CH_0_MSK);
	EPWM_EnableBrakeInt();
	NVIC_EnableIRQ(EPWM_IRQn);
	/*
	(8)设置优先级
	*/	
	NVIC_SetPriority(EPWM_IRQn,3);					/*优先级0~3， 0最高、3最低*/
	/*
	(9)设置IO口输出
	*/	
	SYS_SET_IOCFG(IOP01CFG, SYS_IOCFG_P01_EPWM0);	
	SYS_SET_IOCFG(IOP04CFG, SYS_IOCFG_P04_EPWM1);	
	EPWM_EnableOutput(EPWM_CH_0_MSK | EPWM_CH_1_MSK);
	/*
	(10)开启EPWM
	*/		
	EPWM_Start(EPWM_CH_0_MSK);	

}
/*****************************************************************************
 ** \brief	EPWM_MT_013
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void EPWM_MT_013(void)
{
	/*
	(1)设置EPWM运行模式
	*/
	EPWM_ConfigRunMode(  EPWM_COUNT_DOWN 		| 			/*向下计数模式*/
						 EPWM_OCU_SYMMETRIC 	|			/*对称模式*/
						 EPWM_WFG_COMPLEMENTARYK   |			/*互补模式*/
						 EPWM_OC_INDEPENDENT);				/*独立输出模式*/
	/*
	(2)设置EPWM时钟周期
	*/
	SYS_EnablePeripheralClk(SYS_CLK_EPWM_MSK);	
	
	EPWM_ConfigChannelClk( EPWM0, EPWM_CLK_DIV_1);
	EPWM_ConfigChannelPeriod(EPWM0,  2400);
	EPWM_ConfigChannelSymDuty(EPWM0, 1200);	
	/*
	(3)设置EPWM死区
	*/
	EPWM_EnableDeadZone(EPWM_CH_0_MSK, 48);
		
	/*
	(4)设置EPWM反向输出
	*/
	EPWM_DisableReverseOutput( EPWM_CH_0_MSK|EPWM_CH_1_MSK);
	/*
	(5)设置EPWM加载方式
	*/
	EPWM_EnableAutoLoadMode(EPWM_CH_0_MSK);				/*自动加载*/
	
	/*
	(6)设置EPWM 刹车
	*/
	EPWM_EnableFaultBrake(EPWM_BRK_ADC0CMP0);				
	EPWM_ConfigFaultBrakeLevel(EPWM_CH_0_MSK,0);
	EPWM_ConfigFaultBrakeLevel(EPWM_CH_1_MSK,1);	
	EPWM_AllBrakeEnable();
	
	/*
	(7)设置中断
	*/
	EPWM_EnableZeroInt( EPWM_CH_0_MSK);
	EPWM_EnableBrakeInt();
	NVIC_EnableIRQ(EPWM_IRQn);
	/*
	(8)设置优先级
	*/	
	NVIC_SetPriority(EPWM_IRQn,3);					/*优先级0~3， 0最高、3最低*/
	/*
	(9)设置IO口输出
	*/	
	SYS_SET_IOCFG(IOP01CFG, SYS_IOCFG_P01_EPWM0);	
	SYS_SET_IOCFG(IOP04CFG, SYS_IOCFG_P04_EPWM1);	
	EPWM_EnableOutput(EPWM_CH_0_MSK | EPWM_CH_1_MSK);
	/*
	(10)开启EPWM
	*/		
	EPWM_Start(EPWM_CH_0_MSK);	

}

