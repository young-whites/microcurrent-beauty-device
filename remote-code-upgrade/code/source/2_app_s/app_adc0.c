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
/** \file app_adc0.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "app_adc0.h"
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
 ** \brief	ADC1_CMP1_MT_EPWM
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void ADC0_CMP0_MT_EPWM(void)
{
	/*
	(1)设置ADC1时钟
	*/
	SYS_EnablePeripheralClk(SYS_CLK_ADC0_MSK);
	ADC0_ConfigRunMode(ADC0_CONVERT_SINGLE,ADC0_CLK_DIV_2);
	/*
	(2)设置ADC1通道使能
	*/	
	ADC0_EnableChannel(ADC0_CH_20_MSK);	
	SYS_SET_IOCFG(IOP31CFG,SYS_IOCFG_P31_AN20);
	
	ADC0_ConfigCompare(ADC0_CMP_NOT_LESS_THAN_RESULT, 10, ADC0_CH_20,2048);
	ADC0_EnableCompare();
	/*
	(3)设置ADC1中断
	*/	
	/*
	(4)设置优先级
	*/	
	/*
	(5)开启ADC1
	*/		
	ADC0_Start();
	
}

