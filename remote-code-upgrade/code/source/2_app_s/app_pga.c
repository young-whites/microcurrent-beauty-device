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
/** \file app_pga.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "app_pga.h"
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
 ** \brief	PGA_MT_001
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void PGA_MT_001(void)
{
	/*
	(1)设置PGA 时钟
	*/
	SYS_EnablePeripheralClk(SYS_CLK_OPA_PGA_MSK);
	/*
	(2)设置PGA 正端输入
	*/	
	PGA_ConfigPositive(PGA0,PGA_POSSEL_P2);
	SYS_SET_IOCFG(IOP31CFG, SYS_IOCFG_P31_A0P2);	
	
	//SYS_SET_IOCFG(IOP36CFG, SYS_IOCFG_P36_A0P0);
	/*
	(3)设置PGA 增益
	*/	
	PGA_ConfigGain(PGA0,PGA_GAIN_10);
	/*
	(4)设置PGA 反馈电阻接地端
	*/	
	PGA_ConfigResistance(PGA0, PGA_R_GND);
	/*
	(5)设置PGA输出
	*/	
	PGA_ConfigOutput(PGA0,PGA_OUTSEL_O);
	SYS_SET_IOCFG(IOP34CFG, SYS_IOCFG_P34_A0O);	
	/*
	(6)开启PGA
	*/
	PGA_Start(PGA0);
}



