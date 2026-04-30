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
/** \file app_uart.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "app_uart.h"
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
void putch(unsigned char ch)
{
  //while(!(UART1->LSR&UART_LSR_THRE_Msk));
  UART1->THR = ch;
  while(!(UART1->LSR&UART_LSR_THRE_Msk));
}

/****************************************************************************/
/*	Local function prototypes('static')
*****************************************************************************/

/****************************************************************************/
/*	Function implementation - global ('extern') and local('static')
*****************************************************************************/

/*****************************************************************************
 ** \brief	UART_MT_UART1
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void UART_MT_UART1(void)
{
	uint32_t  BuadRate = 115200;//9600;
	/*
	(1)����UARTxģʽ
	*/
	UART_ConfigRunMode(UART1, BuadRate, UART_WLS_8, UART_PARITY_NONE,UART_STOP_BIT_1);
	/*
	(2)����UARTxʱ��
	*/	
	SYS_EnablePeripheralClk(SYS_CLK_UART1_MSK);
	/*
	(3)����UARTx���
	*/		
	//SYS_SET_IOCFG(IOP15CFG, SYS_IOCFG_P15_TXD1);
	SYS_SET_IOCFG(IOP22CFG, SYS_IOCFG_P22_TXD1);
	
}


