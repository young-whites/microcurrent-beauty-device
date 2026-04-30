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
//#include "sys_config.h"
//#include "bsp_adc.h"
//#include "bsp_task.h"
#include "key.h"
#include "bsp_timer.h"
#include "work.h"
#include "app_epwm.h"
#include "Device_Init.h"
#include "battery.h"
#include "uart.h"
#include "bsp_uart.h"


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
	//TickCounter++;
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
	;
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

}
/****************************************************************************
 ** \brief	GPIO2_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
// uint8_t sss=0;
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
#if (DOG_versions==1||DOG_versions==2)
   if(GPIO_GetIntFlag(GPIO_PORT_USB_DET,GPIO_PIN_USB_DET_NO_MSK))
  {
    GPIO_ClearIntFlag(GPIO_PORT_USB_DET,GPIO_PIN_USB_DET_NO_MSK);
        if( g_tSystemMsg.StandbyFlag == SET&&g_tSystemMsg.sleep_mode_flag==SET)
    {
        if(GPIO_GET_INPUT_PIN(GPIO_PORT_USB_DET, GPIO_PIN_USB_DET_NO_MSK)!= 0)
        {
            BatteryStatu.USB_InsetNoUpdateLED = SET;
        } 
        g_tSystemMsg.AwakenFlag = SET;
        g_tSystemMsg.StandbyFlag = RESET;
        g_tSystemMsg.sleep_mode_flag =RESET;
       if(g_tSystemMsg.PreventAgainPowerOff1 == 0x02)//唤醒开�??,�??按电源按�??后长按电源按�??�??以关�??
        {
           g_tSystemMsg.PreventAgainPowerOff = RESET;
           g_tSystemMsg.PreventAgainPowerOff1 = 0x00;               
        }	
        //  Clear_interrupt_settings(6);
    }
  }
#elif (DOG_versions==3||DOG_versions==4)
   if(GPIO_GetIntFlag(GPIO_PORT_USB_DET,GPIO_PIN_USB_DET_NO_MSK))
  {
    GPIO_ClearIntFlag(GPIO_PORT_USB_DET,GPIO_PIN_USB_DET_NO_MSK);
        if( g_tSystemMsg.StandbyFlag == SET&&g_tSystemMsg.sleep_mode_flag==SET)
    {
        if(GPIO_GET_INPUT_PIN(GPIO_PORT_USB_DET, GPIO_PIN_USB_DET_NO_MSK)!= 0)
        {
            BatteryStatu.USB_InsetNoUpdateLED = SET;
        } 
        g_tSystemMsg.AwakenFlag = SET;
        g_tSystemMsg.StandbyFlag = RESET;
        g_tSystemMsg.sleep_mode_flag =RESET;
       if(g_tSystemMsg.PreventAgainPowerOff1 == 0x02)//唤醒开�??,�??按电源按�??后长按电源按�??�??以关�??
        {
           g_tSystemMsg.PreventAgainPowerOff = RESET;
           g_tSystemMsg.PreventAgainPowerOff1 = 0x00;               
        }	
        //  Clear_interrupt_settings(6);     
    }

  }
#endif	
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
	#if (adc_IRQ_MODE==1)
	if(ADC_SmapleFinish==RESET)
	{
	  
	  ADC1_ClearChannelIntFlag(ADC1_CH_10);
	  Buffer[adc_count] = ADC1_GetResult(ADC1_CH_10);
	  adc_count++;
      ADC1_Go();
	  if(adc_count>=ADCLEN)//直到adc数组存满18组数�??
	  {
		ADC_SmapleFinish = SET;
		adc_count = 0;  
	  }
	}
	#elif (adc_IRQ_MODE==0)
	;
    #endif
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
  u8 date;
  static u8 cnt=0;
  if(UART_GetITStatus(UART1)==IT_UART_RecData)
   {
     date=UART_ReceiveData(UART1);
    //  UART_SendData(UART1,date);
     if(upDATE_Flag==0)
     {
      Update_CommandReceive(date);
     }
     else
     {
       UART_Receive( &USART3_QueueBuf, date);
     }
     
   }
     
   NVIC_ClearPendingIRQ(UART1_IRQn);
	
}
/****************************************************************************
 ** \brief	TMR0_IRQHandler
 **
 ** \param [in]  none   
 ** \return none
 ** \note
****************************************************************************/
void TMR0_IRQHandler(void)//1ms
{
   
  //  static int i=0;
  // if (Battery_Ah_flg==bat_maxlow)
  // {
  //   GPIO1->DO_f.P4 = ~GPIO1->DO_f.P4; 
  //   i++;
  // }
  // if (i>=200)
  // {
  //   CTRLPWR_OFF;   //关机 
  //   CTRLPWR_OFF;   //关机 
  //   CTRLPWR_OFF;   //关机 
  //   i=0;
  // }
  //  TMR_ClearOverflowIntFlag(TMR0);   
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
  // if (Battery_Ah_flg==bat_low)
  // {
  //   GPIO1->DO_f.P4 = ~GPIO1->DO_f.P4; 
  // }
  //  TMR_ClearOverflowIntFlag(TMR1);                    
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

}



