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
/** \file .h
**
** History:
** 
*****************************************************************************/
#ifndef ____
#define ____

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include "cms32f033.h"

#define	FMC_WRITE_KEY	(0x55AA6699)

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/
/*----------------------------------------------------------------------------
 **   ±÷”
-----------------------------------------------------------------------------*/


/*****************************************************************************
 ** \brief	
 **			
 ** \param [in] none
 ** \return  none
 ** \note	 
*****************************************************************************/

							
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
 ** \brief	
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/

     
#define FMC_Lock_Key                        ((uint32_t)(0x55AA6699))     
     
#define FMC_FMCCON_FMCBNormal               ((uint32_t)(0x00000000))
#define FMC_FMCCON_FMCBBusy                 ((uint32_t)(0x00000020))
#define FMC_FMCCON_FMCBISPS                 ((uint32_t)(0x00000010))

#define FMC_FMCCON_FMCE_MapEn               ((uint32_t)(0x00000000))
#define FMC_FMCCON_FMCE_MapDis              ((uint32_t)(0x00000010))
     
#define FMC_FMCCMD_ReadData                 ((uint32_t)(0x00000001))
#define FMC_FMCCMD_WriteData                ((uint32_t)(0x00000002))
#define FMC_FMCCMD_ErasPage                 ((uint32_t)(0x00000003))
#define FMC_FMCCMD_ErasAll                  ((uint32_t)(0x00000006))
#define FMC_FMCCMD_CRC                  		((uint32_t)(0x0000000D))     
     
     
void FMC_UnLock(void);
void FMC_Lock(void);
uint32_t FMC_ReadData(uint32_t address);
void FMC_WriteData(uint32_t address,uint32_t data);
void FMC_ErasPage(uint32_t pageaddress);
void FMC_ErasAll(void);
void FMC_SetKey(uint32_t key);

#ifdef __cplusplus
}
#endif

#endif /* ____ */

