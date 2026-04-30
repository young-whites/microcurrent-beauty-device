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
/** \file hwdiv.h
**
** History:
** 
*****************************************************************************/
#ifndef __HWDIV_H_
#define __HWDIV_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include "cms32f033.h"
/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/
/*****************************************************************************
 ** \brief	HWDIV_DIV_IS_0
 **			判断除数是否为0
 ** \param [in] none
 ** \return  1：除数为0  0：除数不为0
 ** \note	 
*****************************************************************************/
#define	 HWDIV_DIV_IS_0()	(((HWDIV->CON & HDIV_CON_DIVBY0_Msk)?1:0))
/*****************************************************************************
 ** \brief	HWDIV_IS_IDLE
 **			判断除法器是空闲
 ** \param [in] none
 ** \return  1：空闲  0：正在运算
 ** \note	 
*****************************************************************************/
#define	 HWDIV_IS_IDLE()	(((HWDIV->CON & HDIV_CON_READY_Msk)?1:0))

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
 ** \brief	HWDIV_Div
 **			实现 "/" (取商)
 ** \param [in] x:被除数
 **				y:除数
 ** \return  none
 ** \note	 
*****************************************************************************/
int32_t HWDIV_Div(int32_t x, int32_t y);
	  
/*****************************************************************************
 ** \brief	HWDIV_Mod
 **			实现 "%" (取余)
 ** \param [in] x:被除数
 **				y:除数
 ** \return  none
 ** \note	 
*****************************************************************************/
int32_t HWDIV_Mod(int32_t x, int32_t y);

/*****************************************************************************
 ** \brief	HWDIV_EnableSingedMode
 **			使能有符号除法器功能
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void HWDIV_EnableSingedMode(void);
/*****************************************************************************
 ** \brief	HWDIV_EnableUnsingedMode
 **			使能无符号除法器功能
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void HWDIV_EnableUnsingedMode(void);





#ifdef __cplusplus
}
#endif

#endif /* __HWDIV_H_*/

