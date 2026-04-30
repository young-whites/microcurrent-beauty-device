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
/** \file app_opa.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "app_opa.h"
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
 ** \brief	OPA0_Config
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void  OPA1_Config(void)
{
	
	uint32_t AdjData;
	
	SYS_EnablePeripheralClk(SYS_CLK_OPA_PGA_MSK);
	
	AdjData = OPA_GetAutoAdjustResult(OPA1);				/*获取OPA调节值*/	
	
	
	OPA_ConfigRunMode(OPA1,OPA_FIL_OPA);
	/*
	(1)配置OP1正端输入
	*/
	OPA_ConfigPositive(OPA1, OPA_POSSEL_P);
	
	/*
	(2)配置OP1负端输入
	*/
	OPA_ConfigNegative(OPA1, OPA_NEGSEL_N);	
	/*
	(3)配置OP1输出端
	*/
	OPA_ConfigOutput(OPA1, OPA_OUTSEL_O);		
	/*
	(4)配置电压调节模式
	*/

	OPA_EnableOPADJAdjust(OPA1,AdjData);
	
	/*
	(5)配置OP IO口
	*/
	SYS_SET_IOCFG(IOP21CFG,SYS_IOCFG_P21_OP1P);
	SYS_SET_IOCFG(IOP17CFG,SYS_IOCFG_P17_OP1N);	
	SYS_SET_IOCFG(IOP16CFG,SYS_IOCFG_P16_OP1O);	
	
	/*
	(6)开启OP1
	*/
	OPA_Start(OPA1);
}


