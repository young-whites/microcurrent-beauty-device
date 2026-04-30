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
/** \file app_system.h
**
** History:
** 
*****************************************************************************/
#ifndef __APP_SYSTEM_H_
#define __APP_SYSTEM_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include "system.h"
#include "gpio.h"
#include "wdt.h"
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
 ** \brief	SYS_CLK_MT_001
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_CLK_MT_001(void);

/*****************************************************************************
 ** \brief	SYS_CLK_MT_002
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_CLK_MT_002(void);
/*****************************************************************************
 ** \brief	SYS_CLK_MT_003
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_CLK_MT_003(void);
/*****************************************************************************
 ** \brief	SYS_CLK_MT_004
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_CLK_MT_004(void);
/*****************************************************************************
 ** \brief	SYS_PCON_MT_001
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_PCON_MT_001(void);
/*****************************************************************************
 ** \brief	SYS_PCON_MT_002
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_PCON_MT_002(void);

/*****************************************************************************
 ** \brief	SYS_PCON_MT_003
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_PCON_MT_003(void);

/*****************************************************************************
 ** \brief	SYS_LVD_MT_001
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_LVD_MT_001(void);

/*****************************************************************************
 ** \brief	SYS_LVD_MT_002
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_LVD_MT_002(void);

/*****************************************************************************
 ** \brief	SYS_RESET_MT_001
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_RESET_MT_001(void);
/*****************************************************************************
 ** \brief	SYS_RESET_MT_002
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_RESET_MT_002(void);
/*****************************************************************************
 ** \brief	SYS_WAKEUP_MT_001
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_WAKEUP_MT_001(void);
/*****************************************************************************
 ** \brief	SYS_WAKEUP_MT_002
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_WAKEUP_MT_002(void);
/*****************************************************************************
 ** \brief	SYS_WAKEUP_MT_003
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_WAKEUP_MT_003(void);

/*****************************************************************************
 ** \brief	SYS_WAKEUP_MT_004
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void SYS_WAKEUP_MT_004(void);


#ifdef __cplusplus
}
#endif

#endif /* __APP_SYSTEM_H_ */

