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
/** \file app_spi.h
**
** History:
** 
*****************************************************************************/
#ifndef __APP_SPI_H_
#define __APP_SPI_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include "ssp.h"
#include "system.h"
/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/
/*----------------------------------------------------------------------------
 **Flash  W25Q128FV 
 **	W25Q128 
 **
 ---------------------------------------------------------------------------*/
#define 	W25Q128_W_EN				(0x06)
#define 	W25Q128_W_EN_FOR_STATUS_REG		(0x50)
#define 	W25Q128_W_DIS				(0x04)

#define		W25Q128_R_STATUS_REG1		(0x05)
#define		W25Q128_R_STATUS_REG2		(0x35)
#define		W25Q128_R_STATUS_REG3		(0x15)
#define		W25Q128_W_STATUS_REG1		(0x01)
#define		W25Q128_W_STATUS_REG2		(0x31)
#define		W25Q128_W_STATUS_REG3		(0x11)

#define 	W25Q128_R_DAT				(0x03)
#define		W25Q128_CHIP_ERASE			(0x60)

#define 	W25Q128_R_JEDEC_ID			(0x9F)

#define 	W25Q128_RESET_EN			(0x66)
#define 	W25Q128_RESET				(0x99)

#define 	W25Q128_PAGE_PROGRAM		(0x02)

#define		W25Q128_BLOCK_POS			(16)		
#define		W25Q128_SECTOR_POS			(12)



#define		W25Q128_IS_BUSY				(0x01)
							
/*****************************************************************************/
/* Global type definitions ('typedef') */
/*****************************************************************************/


/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source) */
/*****************************************************************************/

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source) */
/*****************************************************************************/

/***************************************************************************
 ** \brief	 SPI_Master_Mode
 **
 ** \param [in]  none   
 ** \return none
 ** \note
***************************************************************************/
void SPI_Master_Mode(void);


/***************************************************************************
 ** \brief	 SPI_W25Q128_Write
 **			  
 ** \param [in]  addr: ¦Ì??¡¤
 **				 buf¡êo¨ºy?Y
 ** \return 
 ** \note
***************************************************************************/
void SPI_W25Q128_Write(uint32_t addr, uint32_t buf);
/***************************************************************************
 ** \brief	 SPI_W25Q128_Read_Data
 **			
** \param [in]  addr : ¦Ì??¡¤  
 ** \return 8bit Data
 ** \note
***************************************************************************/
uint32_t SPI_W25Q128_Read_Data(uint32_t addr);
/***************************************************************************
 ** \brief	 SPI_W25Q128_Reset
 **			
** \param [in]  none  
 ** \return none
 ** \note
***************************************************************************/
void SPI_W25Q128_Reset(void);
/***************************************************************************
 ** \brief	 SPI_W25Q128_Read_SFR
 **			
 ** \param [in]  cmd  W25Q128_R_STATUS_REG1,,W25Q128_R_STATUS_REG,W25Q128_R_STATUS_REG3
 **						W25Q128_R_JEDEC_ID.....	
 ** \return 8bit Data
 ** \note
***************************************************************************/
uint32_t  SPI_W25Q128_Read_SFR(uint32_t cmd);










#ifdef __cplusplus
}
#endif

#endif /* __APP_SPI_H_ */

