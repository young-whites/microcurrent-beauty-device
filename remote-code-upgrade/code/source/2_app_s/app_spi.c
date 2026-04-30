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
/** \file app_spi.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "app_spi.h"
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
/*****************************************************************************
 ** \brief	SPI_W25Q128_Start		
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SPI_W25Q128_Start(void)
{
	SSP_MasterClearCS();
}
/*****************************************************************************
 ** \brief	SPI_W25Q128_Stop
 ** \param [in] none
 ** \return  none
 ** \note	 
*****************************************************************************/
void SPI_W25Q128_Stop(void)
{
	SSP_MasterSetCS();
}
/*****************************************************************************
 ** \brief	SPI_Transmit
 **			
** \param [in] SendData: 发送的值
 ** \return  16bit 获取的值
 ** \note	
*****************************************************************************/
uint32_t  SPI_Transmit(uint32_t  Data)
{
	while(SSP_GetBusyFlag());			
	while(!SSP_GetTFEFlag());
	SSP_SendData(Data);
	while(!SSP_GetRNEFlag());
	return (SSP_GetData());		
}

/***************************************************************************
 ** \brief	 SPI_W25Q128_Write
 **			 D′2ù×÷ 
 ** \param [in]  addr: μ??·
 **				 buf￡oêy?Y
 ** \return 
 ** \note
***************************************************************************/
void SPI_W25Q128_Write(uint32_t addr, uint32_t buf)
{
	uint32_t temp;
	
	SPI_W25Q128_Start();
	SPI_Transmit(W25Q128_W_EN);
	SPI_W25Q128_Stop();			
	for(temp=2;temp>0;temp--);	
	
	SPI_W25Q128_Start();
	SPI_Transmit(W25Q128_PAGE_PROGRAM);	
	SPI_Transmit(addr>>16);
	SPI_Transmit(addr>>8);
	SPI_Transmit(addr);		
	SPI_Transmit(buf);	
	SPI_W25Q128_Stop();	

	for(temp=2;temp>0;temp--);	
	SPI_W25Q128_Start();
	SPI_Transmit(W25Q128_W_DIS);
	SPI_W25Q128_Stop();			
}
/***************************************************************************
 ** \brief	 SPI_W25Q128_Read_Data
 **			
** \param [in]  addr :   
 ** \return 8bit Data
 ** \note
***************************************************************************/
uint32_t SPI_W25Q128_Read_Data(uint32_t addr)
{
	uint32_t temp;
	SPI_W25Q128_Start();
	SPI_Transmit(W25Q128_R_DAT);
	SPI_Transmit(addr>>16);	
	SPI_Transmit(addr>>8);
	SPI_Transmit(addr);
	temp = SPI_Transmit(0x00);		
	SPI_W25Q128_Stop();
	return temp;
}
/***************************************************************************
 ** \brief	 SPI_W25Q128_Reset
 **			
** \param [in]  none  
 ** \return none
 ** \note
***************************************************************************/
void SPI_W25Q128_Reset(void)
{
	uint8_t temp;
	SPI_W25Q128_Start();
	SPI_Transmit(W25Q128_RESET_EN);	
	SPI_W25Q128_Stop();	
	for(temp=2;temp>0;temp--);	
	SPI_W25Q128_Start();
	SPI_Transmit(W25Q128_RESET);		
	SPI_W25Q128_Stop();
}

/***************************************************************************
 ** \brief	 SPI_W25Q128_Read_SFR
 **			
 ** \param [in]  cmd  
 ** \return 8bit Data
 ** \note
***************************************************************************/
uint32_t  SPI_W25Q128_Read_SFR(uint32_t cmd)
{
	uint8_t temp;
	SPI_W25Q128_Start();
	SPI_Transmit(cmd);
	temp = SPI_Transmit(0x00);
	SPI_W25Q128_Stop();	
	return temp;	
}



/****************************************************************************/
/*	Function implementation - global ('extern') and local('static')
*****************************************************************************/
/***************************************************************************
 ** \brief	 SPI_Master_Mode
 **
 ** \param [in]  none   
 ** \return none
 ** \note
***************************************************************************/
void SPI_Master_Mode(void)
{
	/*
	(1)设置SSP的时钟
	*/
	SYS_EnablePeripheralClk(SYS_CLK_SSP_MSK);	
	SSP_ConfigClk(23,2);					/*Fapb = 48Mhz,  sclk = 1Mhz*/								
	/*
	(2)设置SSP 为SPI模式
	*/							
	SSP_ConfigRunMode(SSP_FRAME_SPI,SSP_CPO_0,SSP_CPHA_0,SSP_DAT_LENGTH_8);																			
	/*
	(3)设置SPI 控制模式
	*/
	SSP_EnableMasterMode();
	SSP_DisableMasterAutoControlCS();
	/*
	(4)设置SPI IO
	*/
	SYS_SET_IOCFG(IOP04CFG, SYS_IOCFG_P04_SPI0SS);
	SYS_SET_IOCFG(IOP05CFG, SYS_IOCFG_P05_SPI0MOSI);
	SYS_SET_IOCFG(IOP06CFG, SYS_IOCFG_P06_SPI0MISO);
	SYS_SET_IOCFG(IOP07CFG, SYS_IOCFG_P07_SPI0CLK);	
	/*
	(5)开启SPI
	*/	
	SPI_W25Q128_Stop();	
	SSP_Start();
	
}















