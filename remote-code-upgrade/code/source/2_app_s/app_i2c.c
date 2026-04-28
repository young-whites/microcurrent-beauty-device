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
/** \file app_i2c.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "app_i2c.h"
/****************************************************************************/
/*	Local pre-processor symbols/macros('#define')
*****************************************************************************/
/*----AT24C256地址+读写-----------------------------------------------------*/
#define  AT24C256_WRITE		0xA0				/*Write Cmd*/
#define  AT24C256_READ      0xA1				/*Read  Cmd*/
#define  AT24C256_MAX		0x7fff				/*address  max*/




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
 ** \brief	I2C_MasterSendStart
 **			主控发送启动位
 ** \param [in] none
 ** \return  none
 ** \note	 
*****************************************************************************/
void  I2C_MasterSendStart(void)
{
	I2C_SendStart();
}
/*****************************************************************************
 ** \brief	I2C_MasterWriteAddr
 **			主控写从机地址
 ** \param [in] SlaveAddr ：从机地址+读写位R/W
 ** \return  none
 ** \note	 读写位在从机地址的第0位，即7位地址+R/W  
*****************************************************************************/
void  I2C_MasterWriteAddr(uint32_t SlaveAddr)
{
	I2C_SendData(SlaveAddr);	
}
/*****************************************************************************
 ** \brief	I2C_MasterWriteBuffer
 **			主控写数据
 ** \param [in] buffer
 ** \return  none
 ** \note	 读写位在从机地址的第0位，即7位地址+R/W  
*****************************************************************************/
void  I2C_MasterWriteBuffer(uint32_t buffer)
{
	I2C_SendData(buffer);
}

/*****************************************************************************
 ** \brief	I2C_MasterSendStop
 **			主控发送停止位
 ** \param [in] none
 ** \return  none
 ** \note	 
*****************************************************************************/
void  I2C_MasterSendStop(void)
{
	I2C_SendStop();
}

/*****************************************************************************
 ** \brief	I2C_MasterSendRestart
 **			主控发送重启位
 ** \param [in] none
 ** \return  none
 ** \note	 
*****************************************************************************/
void  I2C_MasterSendRestart(void)
{
	I2C_SendRestart();
}
/*****************************************************************************
 ** \brief	I2C_MasterReadBuffer
 **			主控读缓存
 ** \param [in] none
 ** \return  data
 ** \note	 
*****************************************************************************/
uint32_t I2C_MasterReadBuffer(void)
{
	return (I2C_GetData());
}

/*****************************************************************************
 ** \brief	I2C_MasterSendACK
 **			主控发送ACK
 ** \param [in] none
 ** \return  data
 ** \note	 
*****************************************************************************/
void  I2C_MasterSendACK(void)
{
	I2C_SendACK();
}
/*****************************************************************************
 ** \brief	I2C_MasterNotSendACK
 **			主控不发送ACK
 ** \param [in] none
 ** \return  data
 ** \note	 
*****************************************************************************/
void  I2C_MasterNotSendACK(void)
{
	I2C_NotSendACK();
}


/****************************************************************************/
/*	Function implementation - global ('extern') and local('static')
*****************************************************************************/
/*****************************************************************************
 ** \brief	 At24c256_write_byte
 **			 写数据到At24C256
 ** \param [in] addr ：地址
**				ch   : 数据
 **            	
 ** \return  -1：超出地址范围 0：写完成
 ** \note  
 *****************************************************************************/
int16_t  At24c256_write_byte(uint32_t addr , uint32_t ch)
{
	int32_t i;
	if(addr >AT24C256_MAX)
		return -1;
	else
	{			
		I2C_ClearIntFlag();
		I2C_MasterSendStart();
		while(!(I2C_MS_START_OK ==I2C_GetStatus()));
		I2C_ClearIntFlag();
		
		I2C_MasterWriteAddr(AT24C256_WRITE);			/*写从机地址+写*/
		while(!(I2C_MS_SEND_ADDR_W_ACK ==I2C_GetStatus()));
		I2C_ClearIntFlag();	
		
		I2C_MasterWriteBuffer((addr>>8) & 0xff);		/*高位ROM 地址*/
		while(!(I2C_MS_SEND_DAT_ACK ==I2C_GetStatus()));	
		I2C_ClearIntFlag();		
		
		I2C_MasterWriteBuffer(addr & 0xff);				/*低位ROM 地址*/
		while(!(I2C_MS_SEND_DAT_ACK ==I2C_GetStatus()));		
		I2C_ClearIntFlag();	
		
		I2C_MasterWriteBuffer(ch);							/*写数据*/
		while(!(I2C_MS_SEND_DAT_ACK ==I2C_GetStatus()));	
		I2C_ClearIntFlag();	
		
		I2C_MasterSendStop();								/*发送停止位*/
		for(i=50000;i>0;i--);								/*延时确保AT24C256写数据完成*/
		return 0;
	}
}

/********************************************************************************
 ** \brief	 At24c256_read_byte
 **			 从At24C256中读数据
 ** \param [in]  addr : 地址
 **            	
 ** \return  返回8位数据
 ** \note   
 ******************************************************************************/
uint8_t  At24c256_read_byte(uint32_t addr)
{
	unsigned char buffer;
	if(addr > AT24C256_MAX)
		return 1;
	
	I2C_ClearIntFlag();	
	I2C_MasterSendStart();
	while(!(I2C_MS_START_OK ==I2C_GetStatus()));
	I2C_ClearIntFlag();	
	
	I2C_MasterWriteAddr(AT24C256_WRITE);			/*写从机地址+写*/
	while(!(I2C_MS_SEND_ADDR_W_ACK ==I2C_GetStatus()));
	I2C_ClearIntFlag();
	
	I2C_MasterWriteBuffer((addr>>8) & 0xff);		/*高位ROM 地址*/
	while(!(I2C_MS_SEND_DAT_ACK ==I2C_GetStatus()));
	I2C_ClearIntFlag();	
	I2C_MasterWriteBuffer(addr & 0xff);				/*低位ROM 地址*/
	while(!(I2C_MS_SEND_DAT_ACK ==I2C_GetStatus()));	
	I2C_ClearIntFlag();
	
	I2C_MasterSendRestart();	
	while(!(I2C_MS_RESTART_OK ==I2C_GetStatus()));	
	I2C_ClearIntFlag();
	
	I2C_MasterWriteAddr(AT24C256_READ);					/*写从机地址+读*/	
	while(!(I2C_MS_SEND_ADDR_R_ACK ==I2C_GetStatus()));	
	I2C_ClearIntFlag();
	
	I2C_MasterNotSendACK();
	I2C_ClearIntFlag();
	while(!(I2C_MS_READ_DAT_NO_ACK ==I2C_GetStatus()));	/*等待数据接收完成*/

	buffer=I2C_MasterReadBuffer();						/*读取数据*/
	I2C_ClearIntFlag();	
	
	I2C_MasterSendStop();								/*发送停止位*/		
	return buffer;
}
/********************************************************************************
 ** \brief	 At24c256_read_str
 **			 连续读取At24c256数据
 ** \param [in] addr ：起始地址
 **            	ch	 : 数据保存的容器(数组)  
**				sizevalue: 读取的数据的个数
 ** \return  0/1
 ** \note  	 例：  unsigned char  array[10];			//定义一个10字节的容器
 **	  At24c256_read_str(0x0010, Buffer, 5); //从地址0x0010开始读取5个字节的数据放到数组array中
 ******************************************************************************/
int16_t At24c256_read_str(uint32_t addr,  uint32_t *ch, uint32_t sizevalue)
{
	int16_t i;
	if(addr > AT24C256_MAX)
		return 0;
	
	I2C_ClearIntFlag();		
	I2C_MasterSendStart();
	while(!(I2C_MS_START_OK ==I2C_GetStatus()));
	I2C_ClearIntFlag();	
	
	I2C_MasterWriteAddr(AT24C256_WRITE);			/*写从机地址+写*/
	while(!(I2C_MS_SEND_ADDR_W_ACK ==I2C_GetStatus()));
	I2C_ClearIntFlag();		
	
	I2C_MasterWriteBuffer((addr>>8) & 0xff);		/*高位ROM 地址*/
	while(!(I2C_MS_SEND_DAT_ACK ==I2C_GetStatus()));	
	I2C_ClearIntFlag();		
	
	I2C_MasterWriteBuffer(addr & 0xff);				/*低位ROM 地址*/
	while(!(I2C_MS_SEND_DAT_ACK ==I2C_GetStatus()));	
	I2C_ClearIntFlag();	
	
	I2C_MasterSendRestart();	
	while(!(I2C_MS_RESTART_OK ==I2C_GetStatus()));	
	I2C_ClearIntFlag();	
	
	I2C_MasterWriteAddr(AT24C256_READ);					/*写从机地址+读*/	
	while(!(I2C_MS_SEND_ADDR_R_ACK ==I2C_GetStatus()));	
	I2C_ClearIntFlag();	
	
	I2C_MasterSendACK();	
	for(i=0;i<sizevalue-1;i++)
	{	
		while(!(I2C_MS_READ_DAT_ACK ==I2C_GetStatus()));	/*等待数据接收完成*/				
		*ch++ = I2C_MasterReadBuffer();						/*读数据*/
		I2C_ClearIntFlag();			
	}
	
	I2C_MasterNotSendACK();									/*读取最后一个数据,不发送ACK*/
	while(!(I2C_MS_READ_DAT_NO_ACK ==I2C_GetStatus()));		/*等待数据接收完成*/
	*ch =I2C_MasterReadBuffer();							
	I2C_ClearIntFlag();		
	
	I2C_MasterSendStop();								/*发送停止位*/		
	return 1;
}


/********************************************************************************
 ** \brief	 I2C_Master_Mode
 **			
 ** \param [in] none
 **            	
 ** \return  none
 ** \note  
 ******************************************************************************/
 void I2C_Master_Mode(void)
 {
	/*
	 (1)设置I2C通讯时钟
	 */	 
	 SYS_EnablePeripheralClk(SYS_CLK_I2C_MSK);
	 I2C_ConfigClk(12,1);						/*Fapb =48M,设置时钟SCL = 100K, 采样时钟为2Mhz*/
	/*
	(3)设置IO复用
	*/
	 SYS_SET_IOCFG(IOP06CFG, SYS_IOCFG_P06_SDA0);				/*SDA*/	
	 SYS_SET_IOCFG(IOP07CFG, SYS_IOCFG_P07_SCL0);	 			 /*SCL*/
	 
	 I2C_EnableOutput();
 }

