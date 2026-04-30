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
/** \file app_adc1.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "app_adc1.h"
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
/********************************************************************************
 ** \brief	ADC配置
 **
 ** \param [in]  none
 **
 ** \return none
 ******************************************************************************/
void ADC_Config(void)
{
	/*
	(1)设置ADC1时钟
	*/
	SYS_EnablePeripheralClk(SYS_CLK_ADC1_MSK);
	ADC1_ConfigRunMode(ADC1_CONVERT_SINGLE,ADC1_CLK_DIV_4,ADC1_HOLD_3P5_CLK);
	/*
	(2)设置ADC1通道使能
	*/
	ADC1_EnableScanChannel(ADC1_CH_20_MSK);	
	
	ADC1_EnableHardwareTigger(ADC1_TG_EPWM0_ZERO);
	ADC1_EnableEPWMTiggerChannel(ADC1_CH_20_MSK);
	SYS_SET_IOCFG(IOP31CFG,SYS_IOCFG_P31_AN20);
	/*
	(3)设置ADC1中断
	*/			
	ADC1_SetTiggerDelayTime(0);	
	ADC1_EnableChannelInt(ADC1_CH_20_MSK);
	NVIC_EnableIRQ(ADC1_IRQn);
	/*
	(4)设置优先级
	*/	
	NVIC_SetPriority(ADC1_IRQn,3);	
	/*
	(5)开启ADC1
	*/		
	ADC1_Start();
	
	/*
	(6)开启ADC1校准
	*/		
	ADC1_StartAdjust();
	/*
	(7)使用ADC1校准值的ADC转换
	*/	
	ADC1_EnableAdjust();
}


/*****************************************************************************
 ** \brief	ADC1_MT_001
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC1_MT_000(void)
{
	/*
	(1)设置ADC1时钟
	*/
	SYS_EnablePeripheralClk(SYS_CLK_ADC1_MSK);
	ADC1_ConfigRunMode(ADC1_CONVERT_CONTINUOUS,ADC1_CLK_DIV_4,ADC1_HOLD_3P5_CLK);
	/*
	(2)设置ADC1通道使能
	*/	
//		ADC1_EnableScanChannel(ADC1_CH_20_MSK);	
//	SYS_SET_IOCFG(IOP31CFG,SYS_IOCFG_P31_AN20);
//	
	ADC1_EnableScanChannel(ADC1_CH_19_MSK);	
	ADC1_EnableScanChannel(ADC1_CH_25_MSK);	
	SYS_SET_IOCFG(IOP30CFG,SYS_IOCFG_P30_AN19);
	SYS_SET_IOCFG(IOP40CFG,SYS_IOCFG_P40_AN25);
	/*
	(3)设置ADC1中断
	*/	
	//ADC1_EnableChannelInt(ADC1_CH_14_MSK);
	//NVIC_EnableIRQ(ADC1_IRQn);
	/*
	(4)设置优先级
	*/	
	//NVIC_SetPriority(ADC1_IRQn,3);	
	/*
	(5)开启ADC1
	*/		
	ADC1_Start();
	
	/*
	(6)开启ADC1校准
	*/		
	ADC1_StartAdjust();
	/*
	(7)使用ADC1校准值的ADC转换
	*/	
	ADC1_EnableAdjust();

}

void ADC1_MT_001(void)
{
	/*
	(1)设置ADC1时钟
	*/
	SYS_EnablePeripheralClk(SYS_CLK_ADC1_MSK);
	ADC1_ConfigRunMode(ADC1_CONVERT_SINGLE,ADC1_CLK_DIV_4,ADC1_HOLD_3P5_CLK);
	/*
	(2)设置ADC1通道使能
	*/	
//		ADC1_EnableScanChannel(ADC1_CH_20_MSK);	
//	SYS_SET_IOCFG(IOP31CFG,SYS_IOCFG_P31_AN20);
//	
//	ADC1_EnableScanChannel(ADC1_CH_19_MSK);	
	ADC1_EnableScanChannel(ADC1_CH_25_MSK);	
//	SYS_SET_IOCFG(IOP30CFG,SYS_IOCFG_P30_AN19);
	SYS_SET_IOCFG(IOP40CFG,SYS_IOCFG_P40_AN25);
	/*
	(3)设置ADC1中断
	*/	
	//ADC1_EnableChannelInt(ADC1_CH_14_MSK);
	//NVIC_EnableIRQ(ADC1_IRQn);
	/*
	(4)设置优先级
	*/	
	//NVIC_SetPriority(ADC1_IRQn,3);	
	/*
	(5)开启ADC1
	*/		
	ADC1_Start();
	
	/*
	(6)开启ADC1校准
	*/		
	ADC1_StartAdjust();
	/*
	(7)使用ADC1校准值的ADC转换
	*/	
	ADC1_EnableAdjust();

}

/*****************************************************************************
 ** \brief	ADC1_CMP0_MT_EPWM
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC1_CMP0_MT_EPWM(void)
{
	/*
	(1)设置ADC1时钟
	*/
	SYS_EnablePeripheralClk(SYS_CLK_ADC1_MSK);
	ADC1_ConfigRunMode(ADC1_CONVERT_SINGLE,ADC1_CLK_DIV_2,ADC1_HOLD_3P5_CLK);
	/*
	(2)设置ADC1通道使能
	*/	
	ADC1_EnableScanChannel(ADC1_CH_20_MSK);	
	SYS_SET_IOCFG(IOP31CFG,SYS_IOCFG_P31_AN20);
	
	ADC1_ConfigCompare(ADC1_CMP0, ADC1_CMP_NOT_LESS_THAN_RESULT, 10, ADC1_CH_20,2048);
	ADC1_EnableCompare(ADC1_CMP0);
	/*
	(3)设置ADC1中断
	*/	
	/*
	(4)设置优先级
	*/	
	/*
	(5)开启ADC1
	*/		
	ADC1_Start();
	
	/*
	(6)开启ADC1校准
	*/		
	ADC1_StartAdjust();
	/*
	(7)使用ADC1校准值的ADC转换
	*/	
	ADC1_EnableAdjust();
}
/*****************************************************************************
 ** \brief	ADC1_CMP1_MT_EPWM
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC1_CMP1_MT_EPWM(void)
{
	/*
	(1)设置ADC1时钟
	*/
	SYS_EnablePeripheralClk(SYS_CLK_ADC1_MSK);
	ADC1_ConfigRunMode(ADC1_CONVERT_SINGLE,ADC1_CLK_DIV_2,ADC1_HOLD_3P5_CLK);
	/*
	(2)设置ADC1通道使能
	*/	
	ADC1_EnableScanChannel(ADC1_CH_20_MSK);	
	SYS_SET_IOCFG(IOP31CFG,SYS_IOCFG_P31_AN20);
	
	ADC1_ConfigCompare(ADC1_CMP1, ADC1_CMP_NOT_LESS_THAN_RESULT, 10, ADC1_CH_20,2048);
	ADC1_EnableCompare(ADC1_CMP1);
	/*
	(3)设置ADC1中断
	*/	
	/*
	(4)设置优先级
	*/	
	/*
	(5)开启ADC1
	*/		
	ADC1_Start();
	
	/*
	(6)开启ADC1校准
	*/		
	ADC1_StartAdjust();
	/*
	(7)使用ADC1校准值的ADC转换
	*/	
	ADC1_EnableAdjust();
}

