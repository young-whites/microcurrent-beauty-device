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

/*****************************************************************************/
/** \file app_i2c.h
**
** History:
** 
*****************************************************************************/
#ifndef __APP_I2C_H_
#define __APP_I2C_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include "i2c.h"
#include "system.h"
/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/

							
/*****************************************************************************/
/* Global type definitions ('typedef') */
/*****************************************************************************/


/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source) */
/*****************************************************************************/


/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source) */
/*****************************************************************************/

/*****************************************************************************
 ** \brief	 I2C_Master_Mode
 **			
 ** \param [in] none
 **            	
 ** \return  none
 ** \note  
 *****************************************************************************/
 void I2C_Master_Mode(void);
 /*****************************************************************************
 ** \brief	 At24c256_read_str
 **			 连续读取At24c256数据
 ** \param [in] addr ：起始地址
 **            	ch	 : 数据保存的容器(数组)  
**				sizevalue: 读取的数据的个数
 ** \return  0/1
 ** \note  	 例：  unsigned char  array[10];			//定义一个10字节的容器
 **	  At24c256_read_str(0x0010, Buffer, 5); //从地址0x0010开始读取5个字节的数据放到数组array中
 *****************************************************************************/
int16_t At24c256_read_str(uint32_t addr,  uint32_t *ch, uint32_t sizevalue);
 
/*****************************************************************************
 ** \brief	 At24c256_read_byte
 **			 从At24C256中读数据
 ** \param [in]  addr : 地址
 **            	
 ** \return  返回8位数据
 ** \note   
 ****************************************************************************/
uint8_t  At24c256_read_byte(uint32_t addr);
/*****************************************************************************
 ** \brief	 At24c256_write_byte
 **			 写数据到At24C256
 ** \param [in] addr ：地址
**				ch   : 数据
 **            	
 ** \return  -1：超出地址范围 0：写完成
 ** \note  
 *****************************************************************************/ 
int16_t  At24c256_write_byte(uint32_t addr , uint32_t ch);





#ifdef __cplusplus
}
#endif

#endif /* __APP_I2C_H_ */

