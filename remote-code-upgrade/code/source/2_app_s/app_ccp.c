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
/** \file app_ccp.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "app_ccp.h"
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
 ** \brief	CCP_MT_001
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_MT_001(void)
{
	/*
	(1)设置CCP的时钟
	*/
	SYS_EnablePeripheralClk(SYS_CLK_CCP_MSK);
	CCP_ConfigCLK(CCP0,CCP_CLK_DIV_1,CCP_RELOAD_CCPLOAD,4800);			/*Fahb=48M, Pahb = 100us*/
	CCP_ConfigCLK(CCP1,CCP_CLK_DIV_1,CCP_RELOAD_CCPLOAD,4800);			/*Fahb=48M, Pahb = 100us*/	
	/*
	(2)设置CCP运行模式
	*/
	CCP_EnablePWMMode(CCP0);
	CCP_EnablePWMMode(CCP1);	
	/*
	(3)设置CCP 比较值
	*/	
	CCP_ConfigCompare(CCP0, CCPxA, 960); 	/*20us*/
	CCP_ConfigCompare(CCP0, CCPxB, 1920); 	/*40us*/	
	CCP_ConfigCompare(CCP1, CCPxA, 2880); 	/*60us*/
	CCP_ConfigCompare(CCP1, CCPxB, 3840); 	/*80us*/		
	/*
	(4)设置CCP 反相输出
	*/		
	CCP_EnableReverseOutput(CCP0,CCPxA);
	CCP_DisableReverseOutput(CCP0,CCPxB);
	CCP_EnableReverseOutput(CCP1,CCPxA);
	CCP_DisableReverseOutput(CCP1,CCPxB);
	/*
	(5)设置CCP 中断`
	*/	
	CCP_EnableOverflowInt(CCP0);
	CCP_EnableOverflowInt(CCP1);
	CCP_EnableCompareInt(CCP0);	
	CCP_EnableCompareInt(CCP1);
	
	NVIC_EnableIRQ(CCP_IRQn);
	/*
	(6)设置CCP 中断优先级
	*/	
	NVIC_SetPriority(CCP_IRQn,3);					/*优先级0~3， 0最高、3最低*/	

	/*
	(7)设置CCP IO口复用
	*/	
	SYS_SET_IOCFG(IOP22CFG, SYS_IOCFG_P22_CCP0A);	
	SYS_SET_IOCFG(IOP23CFG, SYS_IOCFG_P23_CCP0B);	
	SYS_SET_IOCFG(IOP24CFG, SYS_IOCFG_P24_CCP1A);	
	SYS_SET_IOCFG(IOP47CFG, SYS_IOCFG_P47_CCP1B);	
	/*
	(8)开启CCP模块
	*/	
	CCP_EnableRun(CCP0);		
	CCP_EnableRun(CCP1);		
	CCP_Start(CCP0);			/*CCP0模块使能*/
	CCP_Start(CCP1);			/*CCP1模块使能*/
}
/*****************************************************************************
 ** \brief	CCP_MT_002
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_MT_002(void)
{
/*
	(1)设置CCP的时钟
	*/
	SYS_EnablePeripheralClk(SYS_CLK_CCP_MSK);
	CCP_ConfigCLK(CCP0,CCP_CLK_DIV_64,CCP_RELOAD_CCPLOAD,4800);			/*Fahb=48M, Pahb = 100us*/
	/*
	(2)设置CCP运行模式
	*/
	CCP_EnableCAPMode0(CCP0);
	CCP_ConfigCAPMode0(CCP0,CCP_CAP_MODE0_RISING);
//	CCP_ConfigCAPMode0(CCP0,CCP_CAP_MODE0_FALLING);	
//	CCP_ConfigCAPMode0(CCP0,CCP_CAP_MODE0_RISING_TO_FALLING);	
//	CCP_ConfigCAPMode0(CCP0,CCP_CAP_MODE0_FALLING_TO_RISING);		
	/*
	(3)设置CCP捕获通道
	*/
	CCP_SelCAPMode0Channel(CCP0,CCPxA);
	SYS_SET_IOCFG(IOP22CFG, SYS_IOCFG_P22_CCP0A);	
	/*
	(4)设置CCP捕获中断
	*/	
	CCP_EnableOverflowInt(CCP0);
	CCP_EnableCAPMode0Int(CCP0);
	NVIC_EnableIRQ(CCP_IRQn);	
	/*
	(5)设置CCP 中断优先级
	*/	
	NVIC_SetPriority(CCP_IRQn,3);					/*优先级0~3， 0最高、3最低*/		
	/*
	(6)开启CCP模块
	*/	
	CCP_EnableRun(CCP0);			
	CCP_Start(CCP0);			/*CCP0模块使能*/
}

/*****************************************************************************
 ** \brief	CCP_MT_003
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_MT_003(void)
{
	/*
	(1)设置CCP的时钟
	*/
	SYS_EnablePeripheralClk(SYS_CLK_CCP_MSK);
	CCP_ConfigCLK(CCP0,CCP_CLK_DIV_1,CCP_RELOAD_CCPLOAD,4800);			/*Fahb=48M, Pahb = 100us*/
	CCP_ConfigCLK(CCP1,CCP_CLK_DIV_1,CCP_RELOAD_CCPLOAD,4800);			/*Fahb=48M, Pahb = 100us*/	
	
	/*
	(2)设置CCP运行模式
	*/
	CCP_EnableCAPMode1();
	CCP_ConfigCAPMode1(CAP0,CCP_CAP_MODE1_RISING);
	//CCP_ConfigCAPMode1(CAP0,CCP_CAP_MODE1_FALLING);
	//CCP_ConfigCAPMode1(CAP0,CCP_CAP_MODE1_BOTH_EDGE);
	/*
	(3)设置CCP捕获通道
	*/
	CCP_SelCAPMode1Channel(CAP0,ECAP04);
	SYS_SET_IOCFG(IOP16CFG, SYS_IOCFG_P16_ECAP04);
	/*
	(4)设置CCP捕获中断
	*/	
	CCP_ConfigCompare(CCP1, CCPxA, 2880); 	/*60us*/
	CCP_EnableCompareInt(CCP1);
	
	
	CCP_EnableOverflowInt(CCP1);
	CCP_EnableCAPMode1Int(CAP0);
	NVIC_EnableIRQ(CCP_IRQn);	
	/*
	(5)设置CCP 中断优先级
	*/	
	NVIC_SetPriority(CCP_IRQn,3);					/*优先级0~3， 0最高、3最低*/		
	/*
	(6)开启CCP模块
	*/	
	CCP_EnableRun(CCP1);			
	CCP_Start(CCP1);								/*CCP1模块使能*/
}
/*****************************************************************************
 ** \brief	CCP_EPWM_HALL
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_EPWM_HALL(void)
{
	/*
	(1)设置CCP的时钟
	*/
	SYS_EnablePeripheralClk(SYS_CLK_CCP_MSK);
	CCP_ConfigCLK(CCP0,CCP_CLK_DIV_64,CCP_RELOAD_CCPLOAD,4800);			/*Fahb=48M, Pahb = 6400us*/
	CCP_ConfigCLK(CCP1,CCP_CLK_DIV_64,CCP_RELOAD_CCPLOAD,4800);			/*Fahb=48M, Pahb = 6400us*/	
	
	/*
	(2)设置CCP运行模式
	*/
	CCP_EnableCAPMode1();
//	CCP_ConfigCAPMode1(CAP0,CCP_CAP_MODE1_BOTH_EDGE);
//	CCP_ConfigCAPMode1(CAP1,CCP_CAP_MODE1_BOTH_EDGE);
//	CCP_ConfigCAPMode1(CAP2,CCP_CAP_MODE1_BOTH_EDGE);	
		
	/*
	(3)设置CCP捕获通道
	*/
	CCP_SelCAPMode1Channel(CAP0,CAP0_CCP0A);
	CCP_SelCAPMode1Channel(CAP1,CAP1_CCP0B);	
	CCP_SelCAPMode1Channel(CAP2,CAP2_CCP1A);
	
	SYS_SET_IOCFG(IOP22CFG, SYS_IOCFG_P22_CCP0A);		
	SYS_SET_IOCFG(IOP23CFG, SYS_IOCFG_P23_CCP0B);	
	SYS_SET_IOCFG(IOP24CFG, SYS_IOCFG_P24_CCP1A);	
	/*
	(4)设置CCP捕获中断
	*/	
	
	CCP_EnableOverflowInt(CCP1);
//	CCP_EnableCAPMode1Int(CAP0);
//	CCP_EnableCAPMode1Int(CAP1);	
//	CCP_EnableCAPMode1Int(CAP2);
	
	CCP_ClearCAPMode1IntFlag(CAP0);				/*CCP捕获口悬空可能会误触发，开中断前清一次中断标志位*/
	CCP_ClearCAPMode1IntFlag(CAP1);	
	CCP_ClearCAPMode1IntFlag(CAP2);
	
	NVIC_EnableIRQ(CCP_IRQn);	
	/*
	(5)设置CCP 中断优先级
	*/	
	NVIC_SetPriority(CCP_IRQn,3);					/*优先级0~3， 0最高、3最低*/		
	/*
	(6)开启CCP模块
	*/	
	CCP_EnableRun(CCP1);			
//	CCP_Start(CCP1);								/*CCP1模块使能*/
}
