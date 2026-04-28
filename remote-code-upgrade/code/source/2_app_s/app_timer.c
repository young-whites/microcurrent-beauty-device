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
/** \file app_timer.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "app_timer.h"
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

/*****************************************************************************
 ** \brief	TMR_TMR0_MT_001
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR_TMR0_MT_001(void)
{
	/*
	(1)设置TMR 的时钟
	*/
	SYS_EnablePeripheralClk(SYS_CLK_TIMER01_MSK);
	TMR_ConfigClk(TMR0,TMR_CLK_SEL_APB,TMR_CLK_DIV_1);		/*48MHz*/
	/*
	(2)设置TMR 运行模式
	*/	
	TMR_ConfigRunMode(TMR0,TMR_COUNT_PERIOD_MODE, TMR_BIT_16_MODE);
	TMR_DisableOneShotMode(TMR0);
	/*
	(3)设置TMR 运行周期
	*/		
	TMR_SetPeriod(TMR0,4800);			/*100us = 4800*Fclk*/
	/*
	(4)设置TMR 中断
	*/	
	TMR_EnableOverflowInt(TMR0);
	NVIC_EnableIRQ(TMR0_IRQn);	
	NVIC_SetPriority(TMR0_IRQn,3);	
	/*
	(5)开启TMR
	*/	
	TMR_Start(TMR0);
	
}
/*****************************************************************************
 ** \brief	TMR_TMR0_MT_002
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR_TMR0_MT_002(void)
{
	/*
	(1)设置TMR 的时钟
	*/
	SYS_EnablePeripheralClk(SYS_CLK_TIMER01_MSK);
	TMR_ConfigClk(TMR0,TMR_CLK_SEL_APB,TMR_CLK_DIV_1);		/*48MHz*/
	/*
	(2)设置TMR 运行模式
	*/	
	TMR_ConfigRunMode(TMR0,TMR_COUNT_PERIOD_MODE, TMR_BIT_32_MODE);
	TMR_DisableOneShotMode(TMR0);
	/*
	(3)设置TMR 运行周期
	*/		
	TMR_SetPeriod(TMR0,0x80000000);			/*100us = 4800*Fclk*/
	/*
	(4)设置TMR 中断
	*/	
	TMR_EnableOverflowInt(TMR0);
	NVIC_EnableIRQ(TMR0_IRQn);	
	NVIC_SetPriority(TMR0_IRQn,3);	
	/*
	(5)开启TMR
	*/	
	TMR_Start(TMR0);
	
}
/*****************************************************************************
 ** \brief	TMR_TMR0_MT_003
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void TMR_TMR0_MT_003(void)
{
	/*
	(1)设置TMR 的时钟
	*/
	SYS_EnablePeripheralClk(SYS_CLK_TIMER01_MSK);
	TMR_ConfigClk(TMR0,TMR_CLK_SEL_LSI,TMR_CLK_DIV_1);		/*40KHz*/
	/*
	(2)设置TMR 运行模式
	*/	
	TMR_ConfigRunMode(TMR0,TMR_COUNT_PERIOD_MODE, TMR_BIT_16_MODE);
	TMR_EnableOneShotMode(TMR0);
	/*
	(3)设置TMR 运行周期
	*/		
	TMR_SetPeriod(TMR0,4);			/*100us = 4*Fclk*/
	/*
	(4)设置TMR 中断
	*/	
	TMR_EnableOverflowInt(TMR0);
	NVIC_EnableIRQ(TMR0_IRQn);	
	NVIC_SetPriority(TMR0_IRQn,3);	
	/*
	(5)开启TMR
	*/	
	TMR_Start(TMR0);
	
}









