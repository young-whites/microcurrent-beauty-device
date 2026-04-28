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
/** \file uart.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "uart.h"

USART_ReceiveDataTypedef   USART3_QueueBuf;

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

/*****************************************************************************
 ** \brief	UART_ConfigRunMode
 **			配置异步串口运行模式
 ** \param [in] uart: UART0、UART1
 **				Baudrate:波特率
 **				WordLength:(1)UART_WLS_5
 **							(2)UART_WLS_6
 **							(3)UART_WLS_7
 **							(4)UART_WLS_8
 **				Parity:	(1)UART_PARITY_NONE
 **						(2)UART_PARITY_ODD
 **						(3)UART_PARITY_EVEN
 **						(4)UART_PARITY_FORCE_1
 **					 	(5)UART_PARITY_FORCE_0
 **				StopBit:(1)UART_STOP_BIT_1
 **						(2)UART_STOP_BIT_2
 ** \return  none
 ** \note	
*****************************************************************************/
void UART_ConfigRunMode(UART_T* uart,uint32_t Baudrate,uint32_t WordLength, 
						uint32_t Parity, uint32_t StopBit)
{
	uart->LCR &= ~(UART_LCR_WLS_Msk | UART_LCR_SBS_Msk|UART_LCR_PEN_Msk|UART_LCR_PSEL_Msk);
	uart->LCR |= WordLength | Parity | StopBit;
	uart->DLR = SystemAPBClock /16/Baudrate;
}


///**
//  * @brief  Deinitializes the UARTx peripheral registers to their default reset values.
//  * @param  UARTx: where x can be from 0 or 1 to select the UART peripheral.
//  * @retval None
//  */
//void UART_DeInit(UART_T* UARTx)
//{
//    if(UARTx == UART0)
//        SYS_T->APBCKEN &= ~SYS_APB_UART0CE;
//    else if(UARTx == UART1)
//        SYS_T->APBCKEN &= ~SYS_APB_UART1CE;
//}

/**
  * @brief  Initializes the UARTx peripheral according to the specified
  *         parameters in the USART_InitStruct .
  * @param  USARTx: where x can be from 0 or 1 to select the UART peripheral.
  * @param  UART_InitStruct: pointer to a UART_InitTypeDef structure that contains
  *         the configuration information for the specified USART peripheral.
  * @retval None
  */
void UART_Init(UART_T* UARTx, UART_InitTypeDef* UART_InitStruct)
{
    uint32_t uartReg = 0;
    SystemCoreClockUpdate();
    UARTx->DLR = SystemAPBClock/(16*UART_InitStruct->UART_BaudRate);
    uartReg = UART_InitStruct->UART_Parity | UART_InitStruct->UART_ParityType + \
                UART_InitStruct->UART_StopBits + UART_InitStruct->UART_WordLength;
    UARTx->LCR = uartReg;
}

/**
  * @brief  Fills each UART_InitTypeDef member with its default value.
  * @param  UART_InitStruct: pointer to a UART_InitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void UART_StructInit(UART_InitTypeDef* UART_InitStruct)
{
    UART_InitStruct->UART_BaudRate = 9600;
    UART_InitStruct->UART_Parity     = UART_Parity_Disable;;
    UART_InitStruct->UART_WordLength = UART_WordLength_8b;
    UART_InitStruct->UART_StopBits   = UART_StopBits_1;
}

/**
  * @brief  Transmits single data through the UARTx peripheral.
  * @param  UARTx: where x can be from 0 or 1 to select the UART peripheral.
  * @param  Data: the data to transmit.
  * @retval None
  */
void UART_SendData(UART_T* UARTx, uint16_t Data)
{
    while(!((UARTx->LSR >> 6) & 0x01));
    UARTx->THR = Data & 0xFF;
}

/**
  * @brief  Returns the most recent received data by the UARTx peripheral.
  * @param  UARTx: where x can be from 0 or 1 to select the UART peripheral.
  * @retval The received data.
  */
uint8_t UART_ReceiveData(UART_T* UARTx)
{
    uint8_t data=0;
    if(UARTx->LSR & 0x01)
     data = (UARTx->RBR & 0xFF);
    return data;
}
/************************FIFO Control functions*******************/
/**
  * @brief  UART peripheral FIFO config.
  * @param  UARTx: where x can be from 0 or 1 to select the UART peripheral.
  * @param  UART_FIFOInitStruct: pointer to a UART_InitFifoTypeDef structure that contains
  *         the configuration information for the specified FIFO.
  * @retval none.
  */
void UART_FIFOControl(UART_T* UARTx,UART_InitFifoTypeDef* UART_FIFOInitStruct)
{
	UARTx->FCR |= (UART_FIFOInitStruct->UART_FCR_RXTL | UART_FIFOInitStruct->UART_FCR_TXTL);
}

/**
  * @brief  UART peripheral transmit FIFO reset.
  * @param  UARTx: where x can be from 0 or 1 to select the UART peripheral.
  * @retval none.
  */
void UART_FIFORstTx(UART_T* UARTx)
{
    UARTx->FCR |= 1<<2;
}

/**
  * @brief  UART peripheral receive FIFO reset.
  * @param  UARTx: where x can be from 0 or 1 to select the UART peripheral.
  * @retval none.
  */
void UART_FIFORstRx(UART_T* UARTx)
{
    UARTx->FCR |= 1<<1;
}

/**
  * @brief  UART peripheral FIFO reset enable/disable.
  * @param  UARTx: where x can be from 0 or 1 to select the UART peripheral.
  * @param  NewState: new state of the UARTx peripheral. 
  *          This parameter can be: ENABLE or DISABLE. 
  * @retval none.
  */
void UART_FIFOCmd(UART_T* UARTx,FunctionalState NewStatus)
{
    if(NewStatus != DISABLE)
    {
        UARTx->FCR |= 1<<0;
    }
    else
    {
        UARTx->FCR &= ~(1<<0);
    }
}

/* Interrupts and flags management functions **********************************/

/**
  * @brief  Enables or disables the specified UART interrupts.
  * @param  USARTx: where x can be from 0 or 1 to select the USART peripheral.
  * @param  UART_IT: specifies the USART interrupt sources to be enabled or disabled.
  *          This parameter can be one of the following values:
*            @arg UART_IER_RBRIE  :RX Buffer Register Interrupt
*            @arg UART_IER_THREIE :TX Holding Register Empty Interrupt
*            @arg UART_IER_RLSIE  :RX Line Status Interrupt
*            @arg UART_IER_MDSIE  :Modem Status Interrupt 
*            @arg UART_IER_XOFIE  :XOFF Interrupt
*            @arg UART_IER_RTSIE  :RTS Interrupt
*            @arg UART_IER_CTSIE  :CTS Interrupt
  * @param  NewState: new state of the specified UARTx interrupts.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_ITConfig(UART_T* UARTx, uint32_t UART_IT, FunctionalState NewState)
{
    if(NewState == ENABLE)
    {
        UARTx->IER |= UART_IT;
    }
    else
    {
        UARTx->IER &= ~UART_IT;
    }
}

/**
  * @brief  Checks whether the specified USART interrupt has occurred or not.
  * @param  UARTx: where x can be from 0 or 1 to select the UART peripheral.
  * @retval IT_UART_StatusType :detail please check for IT_UART_StatusType enum structure.
  */
IT_UART_StatusType UART_GetITStatus(UART_T* UARTx)
{
    IT_UART_StatusType itStatus = IT_UART_No;
    if(!(UARTx->IIR & 0x01))
    {
        switch((UARTx->IIR >> 1) & 7)
        {
            case 0x00 :
            {
                itStatus = IT_UART_ModemStachange;
            }break;
            
            case 0x01 :
            {
                itStatus = IT_UART_TXEmpt;
            }break;
            
            case 0x02 :
            {
                itStatus = IT_UART_RecData;
            }break;
            
            case 0x03 :
            {
                itStatus = IT_UART_RecLine;
            }break;
            
            case 0x06 :
            {
                itStatus = IT_UART_ReceiveFifo;
            }break;
        }
    }
    return itStatus;
}


void UART_ModemControl(UART_T* UARTx,uint32_t modemctl)
{
	UARTx->MCR |= modemctl;
}

u8 UART_GetByte (USART_ReceiveDataTypedef *Uart_Device_Rx)
{
    u8 data = 0;

    data = *(Uart_Device_Rx->get_last);
    Uart_Device_Rx->get_last++;
    Uart_Device_Rx->receive_full_flag = 0;

    if (Uart_Device_Rx->get_last >= Uart_Device_Rx->Receive_Buffer + Uart_Device_Rx->receive_length) 
        Uart_Device_Rx->get_last = Uart_Device_Rx->Receive_Buffer;

    if (Uart_Device_Rx->get_last == Uart_Device_Rx->receive_last) //获取完时清除信号
       Uart_Device_Rx->receive_signal_flag = 0;

    return  data;
}

void    UART_ReceiveValueInit(USART_ReceiveDataTypedef *Uart_Device_Rx,u16 Length)
{
    Uart_Device_Rx->receive_last = Uart_Device_Rx->Receive_Buffer;
    Uart_Device_Rx->get_last = Uart_Device_Rx->Receive_Buffer;//初始化
    Uart_Device_Rx->receive_length = Length;
    Uart_Device_Rx->receive_signal_flag=0;
    Uart_Device_Rx->receive_signal_flag=0;
}


void    UART_Receive (USART_ReceiveDataTypedef *Uart_Device_Rx,u8 Data)
{
    if(!Uart_Device_Rx->receive_full_flag)
    {
        *(Uart_Device_Rx->receive_last) = Data;  
        Uart_Device_Rx->receive_last++;
        Uart_Device_Rx->receive_signal_flag = 1;

        if (Uart_Device_Rx->receive_last >= Uart_Device_Rx->Receive_Buffer + Uart_Device_Rx->receive_length)
        {
            Uart_Device_Rx->receive_last = Uart_Device_Rx->Receive_Buffer;
        }
        if (Uart_Device_Rx->receive_last == Uart_Device_Rx->get_last)
        {
            Uart_Device_Rx->receive_full_flag = 1;
        }
    }
}
