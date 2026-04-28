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
/** \file app_acmp.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "app_acmp.h"
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
 ** \brief	ACMP0_Config
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void ACMP0_Config(void)
{
	uint32_t AdjResult;
	
	SYS_EnablePeripheralClk(SYS_CLK_ACMP_MSK);
	/*
	(1)设置ACMP0 正端输入
	 */
	ACMP_ConfigPositive(ACMP0,ACMP_POSSEL_P2);
	/*
	 (2)设置ACMP0 负端输入 
	 */	
	ACMP_ConfigNegative(ACMP0,ACMP_NEGSEL_N, 0);
	/*
	(3)关闭电压调节							
	 */
	AdjResult = ACMP_EnableAutoAdjust(ACMP0);
	
	ACMP_EnableCADJAdjust(ACMP0, AdjResult);	
	
	/*
	 (4)开启滤波输出
	 */
	 ACMP_EnableFilter(ACMP0, ACMP_NFCLK_3_TSYS);
	 /*
	 (5)关闭反相输出
	 */
	 ACMP_DisableReverseOutput(ACMP0);
	 /*
	 (6)开启刹车
	 */


	/*
	(7)关闭中断
	*/
	 ACMP_DisableInt(ACMP0);
	 ACMP_ClearIntFlag(ACMP0);

	/*
	(8)设置迟滞
	*/	 	 
	ACMP_ConfigHYS(ACMP0,ACMP_HYS_0_TO_1,ACMP_HYS_S_60); 	 
	/*
	(9)配置ACMP IO口
	*/
	SYS_SET_IOCFG(IOP13CFG,SYS_IOCFG_P13_C0P0);
	SYS_SET_IOCFG(IOP14CFG,SYS_IOCFG_P14_C0P1);	
	SYS_SET_IOCFG(IOP15CFG,SYS_IOCFG_P15_C0P2);	
	SYS_SET_IOCFG(IOP43CFG,SYS_IOCFG_P43_C0N);		
	/*
	(10)开启ACMP0
	*/
	ACMP_Start(ACMP0);
}

/*****************************************************************************
 ** \brief	ACMP0_MT_001
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void ACMP0_MT_001(void)
{
	uint32_t AdjResult;
	
	SYS_EnablePeripheralClk(SYS_CLK_ACMP_MSK);
	/*
	(1)设置ACMP0 正端输入
	 */
	ACMP_ConfigPositive(ACMP0,ACMP_POSSEL_P2);
	/*
	 (2)设置ACMP0 负端输入 
	 */	
	ACMP_ConfigNegative(ACMP0,ACMP_NEGSEL_N, 0);
	/*
	(3)设置电压调节							
	 */
	AdjResult = ACMP_EnableAutoAdjust(ACMP0);
	
	ACMP_EnableCADJAdjust(ACMP0, AdjResult);
	/*
	 (4)开启滤波输出
	 */
	 ACMP_EnableFilter(ACMP0, ACMP_NFCLK_3_TSYS);
	 /*
	 (5)关闭反相输出
	 */
	 ACMP_DisableReverseOutput(ACMP0);
	 /*
	 (6)开启刹车
	 */

	/*
	(7)中断
	*/
	//ACMP_ConfigEventAndIntMode(ACMP0,ACMP_EVENT_INT_BOTH_EDGE);
	
	// ACMP_EnableInt(ACMP0);
	// NVIC_EnableIRQ(ACMP_IRQn);
	// NVIC_SetPriority(ACMP_IRQn,3);
	/*
	(8)设置迟滞
	*/	 
	 
	ACMP_ConfigHYS(ACMP0,ACMP_HYS_0_TO_1,ACMP_HYS_S_60);  
	/*
	(9)配置ACMP IO口
	*/
	SYS_SET_IOCFG(IOP13CFG,SYS_IOCFG_P13_C0P0);
	SYS_SET_IOCFG(IOP14CFG,SYS_IOCFG_P14_C0P1);	
	SYS_SET_IOCFG(IOP15CFG,SYS_IOCFG_P15_C0P2);	
	SYS_SET_IOCFG(IOP43CFG,SYS_IOCFG_P43_C0N);		
	/*
	(10)开启ACMP0
	*/
	ACMP_Start(ACMP0);
}
/*****************************************************************************
 ** \brief	ACMP0_MT_EPWM
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void ACMP0_MT_EPWM(void)
{
	uint32_t AdjResult;
	
	SYS_EnablePeripheralClk(SYS_CLK_ACMP_MSK);
	
	AdjResult = ACMP_EnableAutoAdjust(ACMP0);	
	
	
	/*
	(1)设置ACMP0 正端输入
	 */
	ACMP_ConfigPositive(ACMP0,ACMP_POSSEL_P2);
	/*
	 (2)设置ACMP0 负端输入 
	 */	
	ACMP_ConfigNegative(ACMP0,ACMP_NEGSEL_N, 0);
	/*
	(3)设置电压调节							
	 */	
	ACMP_EnableCADJAdjust(ACMP0, AdjResult);
	/*
	 (4)开启滤波输出
	 */
	 ACMP_EnableFilter(ACMP0, ACMP_NFCLK_3_TSYS);
	 /*
	 (5)关闭反相输出
	 */
	 ACMP_DisableReverseOutput(ACMP0);
	 /*
	 (6)开启事件输出
	 */
	 ACMP_ConfigEventAndIntMode(ACMP0,ACMP_EVENT_INT_RISING);
	 ACMP_EnableEventOut(ACMP0);
	
	/*
	(7)中断
	*/
	//ACMP_ConfigEventAndIntMode(ACMP0,ACMP_EVENT_INT_BOTH_EDGE);
	
	// ACMP_EnableInt(ACMP0);
	// NVIC_EnableIRQ(ACMP_IRQn);
	// NVIC_SetPriority(ACMP_IRQn,3);
	/*
	(8)设置迟滞
	*/	 
	 
	ACMP_ConfigHYS(ACMP0,ACMP_HYS_0_TO_1,ACMP_HYS_S_60);  
	/*
	(9)配置ACMP IO口
	*/
	SYS_SET_IOCFG(IOP15CFG,SYS_IOCFG_P15_C0P2);	
	SYS_SET_IOCFG(IOP43CFG,SYS_IOCFG_P43_C0N);		
	/*
	(10)开启ACMP0
	*/
	ACMP_Start(ACMP0);
}
/*****************************************************************************
 ** \brief	ACMP1_MT_EPWM
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void ACMP1_MT_EPWM(void)
{
	uint32_t AdjResult;
	
	SYS_EnablePeripheralClk(SYS_CLK_ACMP_MSK);
	
	AdjResult = ACMP_EnableAutoAdjust(ACMP1);	
	
	
	/*
	(1)设置ACMP0 正端输入
	 */
	ACMP_ConfigPositive(ACMP1,ACMP_POSSEL_P2);
	/*
	 (2)设置ACMP0 负端输入 
	 */	
	ACMP_ConfigNegative(ACMP1,ACMP_NEGSEL_VREF_BG, 0);
	/*
	(3)设置电压调节							
	 */	
	ACMP_EnableCADJAdjust(ACMP1, AdjResult);
	/*
	 (4)开启滤波输出
	 */
	 ACMP_EnableFilter(ACMP1, ACMP_NFCLK_3_TSYS);
	 /*
	 (5)关闭反相输出
	 */
	 ACMP_DisableReverseOutput(ACMP1);
	 /*
	 (6)开启事件输出
	 */
	 ACMP_ConfigEventAndIntMode(ACMP1,ACMP_EVENT_INT_RISING);
	 ACMP_EnableEventOut(ACMP1);
	
	/*
	(7)中断
	*/
	//ACMP_ConfigEventAndIntMode(ACMP0,ACMP_EVENT_INT_BOTH_EDGE);
	
	// ACMP_EnableInt(ACMP0);
	// NVIC_EnableIRQ(ACMP_IRQn);
	// NVIC_SetPriority(ACMP_IRQn,3);
	/*
	(8)设置迟滞
	*/	 
	 
	ACMP_ConfigHYS(ACMP1,ACMP_HYS_0_TO_1,ACMP_HYS_S_60);  
	/*
	(9)配置ACMP IO口
	*/
	SYS_SET_IOCFG(IOP34CFG,SYS_IOCFG_P34_C1P2);	
	/*
	(10)开启ACMP0
	*/
	ACMP_Start(ACMP1);
}

