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
/** \file app_system.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "app_system.h"
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
 ** \brief	SYS_CLK_MT_001
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_CLK_MT_001(void)
{
	/*
	(1)?????????
	*/
	SYS_ConfigHSI(SYS_CLK_HSI_48M);
	SYS_EnableHSI();
	SYS_ConfigAHBClock(SYS_CLK_SEL_HSI,SYS_CLK_DIV_1);
	SYS_ConfigAPBClock(AHB_CLK_DIV_1);
	
	SYS_ConfigClkOutput(SYS_CLK_OUT_SEL_AHB,24);		/*48M/(2*24) = 1M*/
	SYS_SET_IOCFG(IOP36CFG,SYS_IOCFG_P36_CLKO);
	
	SYS_EnableClkOutput();
}
/*****************************************************************************
 ** \brief	SYS_CLK_MT_001
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_CLK_MT_002(void)
{
	/*
	(1)?????????
	*/
	SYS_ConfigHSI(SYS_CLK_HSI_64M);
	SYS_EnableHSI();
	SYS_ConfigAHBClock(SYS_CLK_SEL_HSI,SYS_CLK_DIV_1);
	SYS_ConfigAPBClock(AHB_CLK_DIV_1);
	
	SYS_ConfigClkOutput(SYS_CLK_OUT_SEL_AHB,64);		/*64M/(2*64) = 0.5M*/
	SYS_SET_IOCFG(IOP36CFG,SYS_IOCFG_P36_CLKO);
	
	SYS_EnableClkOutput();
}
/*****************************************************************************
 ** \brief	SYS_CLK_MT_001
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_CLK_MT_003(void)
{
	/*
	(1)?????????
	*/
	SYS_ConfigAHBClock(SYS_CLK_SEL_LSI,SYS_CLK_DIV_1);
	SYS_ConfigAPBClock(AHB_CLK_DIV_1);
	
	SYS_ConfigClkOutput(SYS_CLK_OUT_SEL_AHB,AHB_CLK_DIV_1);		/*40K/1 = 40K*/
	SYS_SET_IOCFG(IOP36CFG,SYS_IOCFG_P36_CLKO);
	
	SYS_EnableClkOutput();
}

/*****************************************************************************
 ** \brief	SYS_CLK_MT_004
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_CLK_MT_004(void)
{
	/*
	(1)?????????
	*/
	SYS_ConfigAHBClock(SYS_CLK_SEL_LSI,SYS_CLK_DIV_1);
	SYS_ConfigAPBClock(AHB_CLK_DIV_1);

	SYS_ConfigClkOutput(SYS_CLK_OUT_SEL_HSI,24);		/*48M/(2*24) = 1M*/
	SYS_SET_IOCFG(IOP36CFG,SYS_IOCFG_P36_CLKO);
	
	SYS_EnableClkOutput();
}
/*****************************************************************************
 ** \brief	SYS_CLK_MT_004
 **			
 ** \param [in] none 
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_PCON_MT_001(void)
{
	/*
	(1)?????????
	*/
	SYS_ConfigHSI(SYS_CLK_HSI_48M);
	SYS_EnableHSI();
	SYS_ConfigAHBClock(SYS_CLK_SEL_HSI,SYS_CLK_DIV_1);
	SYS_ConfigAPBClock(AHB_CLK_DIV_1);

	SYS_ConfigClkOutput(SYS_CLK_OUT_SEL_HSI,24);		/*48M/(2*24) = 1M*/
	SYS_SET_IOCFG(IOP36CFG,SYS_IOCFG_P36_CLKO);	
	SYS_EnableClkOutput();
	
	SYS_EnterSleep();
}
/*****************************************************************************
 ** \brief	SYS_PCON_MT_002
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_PCON_MT_002(void)
{
	/*
	(1)?????????
	*/
	SYS_ConfigHSI(SYS_CLK_HSI_48M);
	SYS_EnableHSI();
	SYS_ConfigAHBClock(SYS_CLK_SEL_HSI,SYS_CLK_DIV_1);
	SYS_ConfigAPBClock(AHB_CLK_DIV_1);

	SYS_ConfigClkOutput(SYS_CLK_OUT_SEL_HSI,24);		/*48M/(2*24) = 1M*/
	SYS_SET_IOCFG(IOP36CFG,SYS_IOCFG_P36_CLKO);	
	SYS_EnableClkOutput();
	
	SYS_EnterDeepSleep();
}
/*****************************************************************************
 ** \brief	SYS_PCON_MT_003
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_PCON_MT_003(void)
{
	/*
	(1)?????????
	*/
	SYS_ConfigHSI(SYS_CLK_HSI_48M);
	SYS_EnableHSI();
	SYS_ConfigAHBClock(SYS_CLK_SEL_HSI,SYS_CLK_DIV_1);
	SYS_ConfigAPBClock(AHB_CLK_DIV_1);

	SYS_ConfigClkOutput(SYS_CLK_OUT_SEL_HSI,24);		/*48M/(2*24) = 1M*/
	SYS_SET_IOCFG(IOP36CFG,SYS_IOCFG_P36_CLKO);	
	SYS_EnableClkOutput();
	
	SYS_EnterStop();
}

/*****************************************************************************
 ** \brief	SYS_LVD_MT_001
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_LVD_MT_001(void)
{
	SYS_ConfigLVDLevel(SYS_LVD_3V);
	SYS_EnableLVDInt();	
	NVIC_EnableIRQ(LVD_IRQn);
	
	NVIC_SetPriority(LVD_IRQn,3);		
}
/*****************************************************************************
 ** \brief	SYS_LVD_MT_002
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_LVD_MT_002(void)
{
	SYS_ConfigLVDLevel(SYS_LVD_4P3V);
	SYS_EnableLVDInt();	
	NVIC_EnableIRQ(LVD_IRQn);	
	NVIC_SetPriority(LVD_IRQn,3);		
}

/*****************************************************************************
 ** \brief	SYS_RESET_MT_001
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_RESET_MT_001(void)
{
	SYS_ResetCPU();
}
/*****************************************************************************
 ** \brief	SYS_RESET_MT_002
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_RESET_MT_002(void)
{
	SYS_ResetMCU();
}
/*****************************************************************************
 ** \brief	SYS_WAKEUP_MT_001
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_WAKEUP_MT_001(void)
{
	SYS_SET_IOCFG(IOP15CFG, SYS_IOCFG_P15_GPIO);
	GPIO_ConfigRunMode(GPIO1,GPIO_PIN_5_MSK, GPIO_MODE_INPUT_WITH_PULL_UP,RESET);				
	GPIO_EnableFilter(GPIO1, GPIO_PIN_5_MSK, GPIO_FILCLK_DIV_10);	
	GPIO_EnableInt(GPIO1, GPIO_PIN_5_MSK,GPIO_INT_EDGE_FALLING);	
	NVIC_EnableIRQ(GPIO1_IRQn);	
	NVIC_SetPriority(GPIO1_IRQn,3);		
	SYS_EnterSleep();	
}

/*****************************************************************************
 ** \brief	SYS_WAKEUP_MT_002
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_WAKEUP_MT_002(void)
{
	SYS_SET_IOCFG(IOP15CFG, SYS_IOCFG_P15_GPIO);
	GPIO_ConfigRunMode(GPIO1,GPIO_PIN_5_MSK, GPIO_MODE_INPUT_WITH_PULL_UP,RESET);			
	GPIO_EnableFilter(GPIO1, GPIO_PIN_5_MSK, GPIO_FILCLK_DIV_10);	
	GPIO_EnableInt(GPIO1, GPIO_PIN_5_MSK,GPIO_INT_EDGE_FALLING);	
	NVIC_EnableIRQ(GPIO1_IRQn);	
	NVIC_SetPriority(GPIO1_IRQn,3);		
	SYS_EnterDeepSleep();	
}
/*****************************************************************************
 ** \brief	SYS_WAKEUP_MT_003
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_WAKEUP_MT_003(void)
{
	SYS_SET_IOCFG(IOP15CFG, SYS_IOCFG_P15_GPIO);
	GPIO_ConfigRunMode(GPIO1,GPIO_PIN_5_MSK, GPIO_MODE_INPUT_WITH_PULL_UP,RESET);			
	GPIO_EnableFilter(GPIO1, GPIO_PIN_5_MSK, GPIO_FILCLK_DIV_10);	
	GPIO_EnableInt(GPIO1, GPIO_PIN_5_MSK,GPIO_INT_EDGE_FALLING);	
	NVIC_EnableIRQ(GPIO1_IRQn);	
	NVIC_SetPriority(GPIO1_IRQn,3);		
	SYS_EnterStop();	
}

/*****************************************************************************
 ** \brief	SYS_WAKEUP_MT_004
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_WAKEUP_MT_004(void)
{
	SYS_EnablePeripheralClk(SYS_CLK_WDT_MSK);
	WDT_DisableRestSystem();
	WDT_ConfigClk(WDT_CLK_DIV_1);
	WDT_ConfigPeriod(400);		 /*25us * 400 =10000 us*/
	WDT_EnableOverflowInt();
	NVIC_EnableIRQ(WDT_IRQn);	
	NVIC_SetPriority(WDT_IRQn,3);
	SYS_EnterDeepSleep();	
}
