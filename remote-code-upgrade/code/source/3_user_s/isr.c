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
/** \file isr.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "cms32f033.h"
#include "gpio.h"
#include "epwm.h"
#include "system.h"
#include "adc1.h"
#include "ccp.h"
#include "wdt.h"
#include "wwdt.h"
#include "timer.h"
#include "i2c.h"
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
     
/****************************************************************************
 ** \brief	NMI_Handler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void NMI_Handler(void)
{
	
}
/****************************************************************************
 ** \brief	HardFault_Handler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void HardFault_Handler(void)
{
	
}

/****************************************************************************
 ** \brief	SVC_Handler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void SVC_Handler(void)
{
	
}

/****************************************************************************
 ** \brief	PendSV_Handler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void PendSV_Handler(void)
{
	
}
/****************************************************************************
 ** \brief	SysTick_Handler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void SysTick_Handler(void)
{
	
}
           
/****************************************************************************
 ** \brief	GPIO0_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void GPIO0_IRQHandler(void)
{
	if(GPIO_GetIntFlag(GPIO0,GPIO_PIN_1))
	{
		//GPIO1->DO_f.P2 = ~GPIO1->DO_f.P2;		
		GPIO_ClearIntFlag(GPIO0,GPIO_PIN_1);
	}	
}

/****************************************************************************
 ** \brief	GPIO1_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void GPIO1_IRQHandler(void)
{
	if(GPIO_GetIntFlag(GPIO1,GPIO_PIN_5))
	{
		GPIO1->DO_f.P2 = ~GPIO1->DO_f.P2;		
		GPIO_ClearIntFlag(GPIO1,GPIO_PIN_5);
	}		
}
/****************************************************************************
 ** \brief	GPIO2_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void GPIO2_IRQHandler(void)
{
	
}
/****************************************************************************
 ** \brief	GPIO3_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void GPIO3_IRQHandler(void)
{
	
}
/****************************************************************************
 ** \brief	GPIO4_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void GPIO4_IRQHandler(void)
{
	
}
/****************************************************************************
 ** \brief	CCP_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void CCP_IRQHandler(void)
{
	if(CCP_GetOverflowIntFlag(CCP1))
	{
		GPIO1->DO_f.P2 = ~GPIO1->DO_f.P2;
		CCP_ClearOverflowIntFlag(CCP1);
	}
	
	if(CCP_GetCAPMode1IntFlag(CAP0))
	{
		GPIO1->DO_f.P3 = ~GPIO1->DO_f.P3;
		CCP_ClearCAPMode1IntFlag(CAP0);	
	}
	if(CCP_GetCAPMode1IntFlag(CAP1))
	{
		CCP_ClearCAPMode1IntFlag(CAP1);		
	}
	if(CCP_GetCAPMode1IntFlag(CAP2))
	{
		CCP_ClearCAPMode1IntFlag(CAP2);			
	}	
}
/****************************************************************************
 ** \brief	ADC0_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void ADC0_IRQHandler(void)
{
	
}
/****************************************************************************
 ** \brief	WWDT_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void WWDT_IRQHandler(void)
{
	if(WWDT_GetCompareIntFlag())
	{
		GPIO1->DO_f.P2 = ~GPIO1->DO_f.P2;		
		WWDT_ClearCompareIntFlag();
	}		
}
/****************************************************************************
 ** \brief	EPWM_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void EPWM_IRQHandler(void)
{
	
}
/****************************************************************************
 ** \brief	ADC1_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void ADC1_IRQHandler(void)
{
	;
}
/****************************************************************************
 ** \brief	ACMP_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void ACMP_IRQHandler(void)
{
	
}
/****************************************************************************
 ** \brief	UART0_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void UART0_IRQHandler(void)
{
	
}
/****************************************************************************
 ** \brief	UART1_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void UART1_IRQHandler(void)
{
	
}
/****************************************************************************
 ** \brief	TMR0_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void TMR0_IRQHandler(void)
{
	if(TMR_GetOverflowIntFlag(TMR0))
	{
		GPIO1->DO_f.P2 = ~GPIO1->DO_f.P2;	
		TMR_ClearOverflowIntFlag(TMR0);
	}	
}
/****************************************************************************
 ** \brief	TMR1_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void TMR1_IRQHandler(void)
{
                       
}
/****************************************************************************
 ** \brief	WDT_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void WDT_IRQHandler(void)
{
	if(WDT_GetOverflowIntFlag())
	{
		GPIO1->DO_f.P2 = ~GPIO1->DO_f.P2;		
		WDT_ClearOverflowIntFlag();
	}	
}
/****************************************************************************
 ** \brief	I2C_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void I2C_IRQHandler(void)
{
	
}
/****************************************************************************
 ** \brief	SPI_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void SPI_IRQHandler(void)
{
	
}
/****************************************************************************
 ** \brief	GPIO0_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void LVD_IRQHandler(void)
{	
	if(SYS_GetLVDIntFlag())
	{
		GPIO1->DO_f.P2 = ~GPIO1->DO_f.P2;		
		SYS_ClearLVDIntFlag();
	}	
}



