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
/** \file opa.h
**
** History:
** 
*****************************************************************************/
#ifndef __OPA_H_
#define __OPA_H_

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
/*----------------------------------------------------------------------------
 **OPA	工作模式
-----------------------------------------------------------------------------*/
#define		OPA_FIL_CMP			(0x01UL<<OPA_CON0_FIL_Pos)	/*比较模式*/
#define		OPA_FIL_OPA			(0x00UL)					/*运放模式*/
/*----------------------------------------------------------------------------
 **OPA	正端选择
-----------------------------------------------------------------------------*/
#define		OPA_POSSEL_P		(0x00UL)					/*选择OPA P*/
#define		OPA_POSSEL_1P2		(0x01UL<<OPA_CON0_PS_Pos)	/*选择1.2V Bandgap*/	
/*----------------------------------------------------------------------------
 **OPA	负端选择
-----------------------------------------------------------------------------*/
#define		OPA_NEGSEL_N		(0x00UL)					/*选择OPA N*/
/*----------------------------------------------------------------------------
 **OPA	输出选择
-----------------------------------------------------------------------------*/
#define		OPA_OUTSEL_O		(0x01UL<<OPA_CON0_OS_Pos)	/*选择OPA O*/

/*----------------------------------------------------------------------------
 **opa 调节
------------------------------------------------------------------------------*/
#define OPA_ADJ_SEL_N			(0x00)		/*opa调节模式输入端选择负端*/
#define OPA_ADJ_SEL_P			(0x01<<OPA_CON1_CRS_Pos)		/*opa调节模式输入端选择正端*/
#define OPA_ADJ_USE_CONFIG		(0xFF)		/*opa调节模式失调电压由Config决定*/
#define OPA_ADJ_USE_OPADJ		(0xAA)		/*opa调节模式失调电压由OPnADJ[4:0]决定*/


							
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
 ** \brief	 OPA_ConfigRunMode
 **			 设置运放运行模式
 ** \param [in] opa :OPA0 、OPA1
 **				mode: (1)OPA_FIL_CMP
 **					  (2)OPA_FIL_OPA
 ** \return  none
 ** \note    
 *****************************************************************************/
void OPA_ConfigRunMode(OPA_T *opa, uint32_t mode);

/*****************************************************************************
 ** \brief	 OPA_ConfigPositive
 **			 设置运放正端输入
 ** \param [in] opa :OPA0 、OPA1
 **				PositiveSource:  (1)OPA_POSSEL_P	:OPnP
 **								 (2)OPA_POSSEL_1P2 
 ** \return  none
 ** \note    默认关闭运放模块
 *****************************************************************************/
void OPA_ConfigPositive(OPA_T *opa, uint32_t PositiveSource);

/*****************************************************************************
 ** \brief	 OPA_ConfigNegative
 **			 设置比较器负端输入
 ** \param [in] opa :OPA0 、OPA1
 **				NegativeSource:  (1)OPA_NEGSEL_N		:OPnN			
 ** \return  none
 ** \note   默认关闭运放模块
 *****************************************************************************/
void OPA_ConfigNegative(OPA_T *opa, uint32_t NegativeSource);

/*****************************************************************************
 ** \brief	 OPA_ConfigOutput
 **			 设置比较器输出端
 ** \param [in] opa :OPA0 、OPA1
 **				OutSource:	OPA_OUTSEL_O
 ** \return  none
 ** \note   
 *****************************************************************************/
void OPA_ConfigOutput(OPA_T *opa,uint32_t OutSource);


/********************************************************************************
 ** \brief	 OPA_Start
 **			 开启OP
 ** \param [in] opa :OPA0 、OPA1
 ** \return  none
 ** \note   
 ******************************************************************************/
void OPA_Start(OPA_T *opa);

/********************************************************************************
 ** \brief	 OPA_Stop
 **			 关闭OP
 ** \param [in] opa :OPA0 、OPA1
 ** \return  none
 ** \note   
 ******************************************************************************/
void OPA_Stop(OPA_T *opa);

/********************************************************************************
 ** \brief	 OPA_GetResult
 **			 获取比较器值
 ** \param [in] opa :OPA0 、OPA1
 ** \return  0/1
 ** \note   只适用于调试模式
 ******************************************************************************/
uint32_t OPA_GetResult(OPA_T *opa);

/*****************************************************************************
 ** \brief	OPA_EnableAdjustMode
 **			 开启运放调节模式
 ** \param [in] opa :OPA0 、OPA1
 **				AdjustTarget:    (1)OPA_ADJ_SEL_N		:调节作用于负端输入		
 **								 (2)OPA_ADJ_SEL_P		:调节作用于正端输入	
 ** \return  none
 ** \note   
 *****************************************************************************/
void OPA_EnableAdjustMode(OPA_T *opa, uint32_t AdjustTarget);

/*****************************************************************************
 ** \brief	OPA_DisableAdjustMode
 **			 关闭运放调节模式
 ** \param [in] opa :OPA0 、OPA1
 ** \return  none
 ** \note   
 *****************************************************************************/
void OPA_DisableAdjustMode(OPA_T *opa);

/*****************************************************************************
 ** \brief	OPA_EnableConfigAdjust
 **			 开启运放Config调节
 ** \param [in] opa :OPA0 、OPA1
 ** \return  none
 ** \note    使用Config调节值
 *****************************************************************************/
void OPA_EnableConfigAdjust(OPA_T *opa);

/*****************************************************************************
 ** \brief	OPA_EnableOPADJAdjust
 **			 开启运放OPADJ调节
 ** \param [in] opa :OPA0 、OPA1
 **				AdjustValue: 0x00 ~ 0x1f
 ** \return  none
 ** \note    使用OPnADJ寄存器中的OPnADJ<4:0>
 *****************************************************************************/
void OPA_EnableOPADJAdjust(OPA_T *opa, uint32_t AdjustValue);

/*****************************************************************************
 ** \brief	OPA_GetAutoAdjustResult
 **			 获取运放调节模式结果
 ** \param [in] opa :OPA0 、OPA1
 ** \return  Result
 ** \note    
 *****************************************************************************/
uint32_t  OPA_GetAutoAdjustResult(OPA_T *opa);



#ifdef __cplusplus
}
#endif

#endif /* __OPA_H_ */

