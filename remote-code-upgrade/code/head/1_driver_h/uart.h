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
/** \file uart.h
**
** History:
** 
*****************************************************************************/
#ifndef __UART_H_
#define __UART_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* Include files */
/*****************************************************************************/
#include "cms32f033.h"
#include "work.h"
/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/

typedef enum
{
    IT_UART_RecLine         = 3,
    IT_UART_RecData         = 2,
    IT_UART_ReceiveFifo     = 6,
    IT_UART_TXEmpt          = 1,
    IT_UART_ModemStachange  = 0,
    IT_UART_No              = 8,
}IT_UART_StatusType;	
	
/*----------------------------------------------------------------------------
 **UART 字长
-----------------------------------------------------------------------------*/
#define		UART_WLS_5			(0x00UL)		/*5位字长*/
#define		UART_WLS_6			(0x01UL)		/*6位字长*/
#define		UART_WLS_7			(0x02UL)		/*7位字长*/		
#define		UART_WLS_8			(0x03UL)		/*8位字长*/
/*----------------------------------------------------------------------------
 **UART 停止位
-----------------------------------------------------------------------------*/
#define		UART_STOP_BIT_1			(0x00UL)		/*1位停止位*/
#define		UART_STOP_BIT_2			(0x01UL<<UART_LCR_SBS_Pos)		/*2位停止位*/
/*----------------------------------------------------------------------------
 **UART 奇偶校验
-----------------------------------------------------------------------------*/
#define		UART_PARITY_NONE		(0x00UL)		/*无奇偶校验*/
#define		UART_PARITY_ODD			(0x00UL|(0x1<<UART_LCR_PEN_Pos))	/*奇偶校验*/
#define		UART_PARITY_EVEN		((0x01UL<<UART_LCR_PSEL_Pos)|(0x1<<UART_LCR_PEN_Pos))	/*偶偶校验*/
#define		UART_PARITY_FORCE_1		((0x02UL<<UART_LCR_PSEL_Pos)|(0x1<<UART_LCR_PEN_Pos))		/*校验位强制为1*/
#define		UART_PARITY_FORCE_0		((0x03UL<<UART_LCR_PSEL_Pos)|(0x1<<UART_LCR_PEN_Pos))		/*校验位强制为0*/


/** @defgroup UART_IER Type
* @{
*/
#define UART_IER_RBRIE              ((uint32_t)(0x00000001))
#define UART_IER_THREIE             ((uint32_t)(0x00000002))
#define UART_IER_RLSIE              ((uint32_t)(0x00000004))
#define UART_IER_MDSIE              ((uint32_t)(0x00000008))
#define UART_IER_XOFIE              ((uint32_t)(0x00000020))
#define UART_IER_RTSIE              ((uint32_t)(0x00000040))
#define UART_IER_CTSIE              ((uint32_t)(0x00000080))


/** @defgroup UART_FIFO Type
  * @{
  */
#define UART_FCR_RXTL1Byte      ((uint32_t)(0x00000000))
#define UART_FCR_RXTL4Byte      ((uint32_t)(0x00000040))
#define UART_FCR_RXTL8Byte      ((uint32_t)(0x00000080))
#define UART_FCR_RXTL14Byte     ((uint32_t)(0x000000C0))

#define UART_FCR_TXTL1Byte      ((uint32_t)(0x00000000))
#define UART_FCR_TXTL4Byte      ((uint32_t)(0x00000010))
#define UART_FCR_TXTL8Byte      ((uint32_t)(0x00000020))
#define UART_FCR_TXTL14Byte     ((uint32_t)(0x00000030))

#define UART_FCR_FIFOEN     ((uint32_t)(0x00000001))
#define UART_FCR_RXFIFORST     ((uint32_t)(0x00000002))
#define UART_FCR_TXFIFORST     ((uint32_t)(0x00000004))


/**
  * @}
  */
#define UART_EFR_AUTOCTS     ((uint32_t)(0x00000080))	
#define UART_EFR_AUTORTS     ((uint32_t)(0x00000040))
#define UART_EFR_TXSWFC_1     ((uint32_t)(0x00000004))	
#define UART_EFR_TXSWFC_2     ((uint32_t)(0x00000008))	
#define UART_EFR_TXSWFC_12     ((uint32_t)(0x0000000c))	

#define UART_EFR_RXSWFC_1     ((uint32_t)(0x00000001))
#define UART_EFR_RXSWFC_2     ((uint32_t)(0x00000002))
#define UART_EFR_RXSWFC_12     ((uint32_t)(0x00000003))

/**
  * @}
  */
#define UART_MCR_IREN     ((uint32_t)(0x00000040))
#define UART_MCR_MLBM     ((uint32_t)(0x00000010))
#define UART_MCR_RTS     ((uint32_t)(0x00000002))


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
						uint32_t Parity, uint32_t StopBit);


/**
  * @brief  UART Init Structure definition
  */

typedef struct
{
  uint32_t UART_BaudRate;            /*!< This member configures the UART communication baud rate.
                                           The baud rate is computed using the following formula:
                                            - IntegerDivider = ((PCLKx) / (16 * (UART_InitStruct->UART_BaudRate)))
                                            - FractionalDivider = ((IntegerDivider - ((uint32_t) IntegerDivider)) * 16) + 0.5 */

  uint32_t UART_WordLength;          /*!< Specifies the number of data bits transmitted or received in a frame.
                                           This parameter can be a value of @ref UART_Word_Length */

  uint32_t UART_StopBits;            /*!< Specifies the number of stop bits transmitted.
                                           This parameter can be a value of @ref UART_Stop_Bits */

  uint32_t UART_Parity;              /*!< Specifies the parity mode.
                                           This parameter can be a value of @ref UART_Parity
                                           @note When parity is enabled, the computed parity is inserted
                                                 at the MSB position of the transmitted data (9th bit when
                                                 the word length is set to 9 data bits; 8th bit when the
                                                 word length is set to 8 data bits). */
  uint32_t UART_ParityType;
} UART_InitTypeDef;

typedef struct
{
    uint32_t UART_FCR_RXTL;
    uint32_t UART_FCR_TXTL;
}UART_InitFifoTypeDef;


/* UART_T constants --------------------------------------------------------*/

/** @defgroup UART_Exported_Constants
  * @{
  */

/** @defgroup UART_Word_Length
  * @{
  */

#define UART_WordLength_8b          ((uint32_t)0x00000003)
#define UART_WordLength_7b          ((uint32_t)0x00000002)
#define UART_WordLength_6b          ((uint32_t)0x00000001)
#define UART_WordLength_5b          ((uint32_t)0x00000000)
#define IS_UART_WORD_LENGTH(LENGTH) (((LENGTH) == UART_WordLength_8b) || \
                                     ((LENGTH) == UART_WordLength_7b) || \
                                     ((LENGTH) == UART_WordLength_6b) || \
                                     ((LENGTH) == UART_WordLength_5b))
/**
  * @}
  */

/** @defgroup UART_Stop_Bits
  * @{
  */

#define UART_StopBits_1                     ((uint32_t)0x00000000)
#define UART_StopBits_2                     ((uint32_t)0x00000004)
#define IS_UART_STOPBITS(STOPBITS)          (((STOPBITS) == UART_StopBits_1) || \
                                            ((STOPBITS) == UART_StopBits_2))
/**
  * @}
  */

/** @defgroup UART_Parity Enable
  * @{
  */
#define UART_Parity_Enable                  ((uint32_t)(0x00000008))
#define UART_Parity_Disable                 ((uint32_t)(0x00000000))
#define IS_UART_PARITY_ENABLE(PARITY)       (((PARITY) == UART_Parity_Enable) || \
                                            ((PARITY) == UART_Parity_Disable))

/**
  * @}
  */

/** @defgroup UART_Parity Type
  * @{
  */
#define UART_Parity_Odd                      ((uint32_t)(0x00000000))
#define UART_Parity_Even                     ((uint32_t)(0x00000010))
#define UART_Parity_Forced1                  ((uint32_t)(0x00000020))
#define UART_Parity_Forced0                  ((uint32_t)(0x00000030))
#define IS_UART_PARITY(PARITY)              (((PARITY) == UART_Parity_Odd) || \
                                             ((PARITY) == UART_Parity_Even) || \
                                             ((PARITY) == UART_Parity_Forced0 || \
                                             ((PARITY) == )
/**
  * @}
  */

/** @defgroup UART_IER Type
* @{
*/
#define UART_IER_RBRIE              ((uint32_t)(0x00000001))
#define UART_IER_THREIE             ((uint32_t)(0x00000002))
#define UART_IER_RLSIE              ((uint32_t)(0x00000004))
#define UART_IER_MDSIE              ((uint32_t)(0x00000008))
#define UART_IER_XOFIE              ((uint32_t)(0x00000020))//((uint32_t)(0x00000002))
#define UART_IER_RTSIE              ((uint32_t)(0x00000040))//((uint32_t)(0x00000004))
#define UART_IER_CTSIE              ((uint32_t)(0x00000080))//((uint32_t)(0x00000008))
/**
  * @}
  */

/** @defgroup UART_FIFO Type
  * @{
  */
#define UART_FCR_RXTL1Byte      ((uint32_t)(0x00000000))
#define UART_FCR_RXTL4Byte      ((uint32_t)(0x00000040))
#define UART_FCR_RXTL8Byte      ((uint32_t)(0x00000080))
#define UART_FCR_RXTL14Byte     ((uint32_t)(0x000000C0))

#define UART_FCR_TXTL1Byte      ((uint32_t)(0x00000000))
#define UART_FCR_TXTL4Byte      ((uint32_t)(0x00000010))
#define UART_FCR_TXTL8Byte      ((uint32_t)(0x00000020))
#define UART_FCR_TXTL14Byte     ((uint32_t)(0x00000030))

/**
  * @}
  */
#define UART_EFR_AUTOCTS     ((uint32_t)(0x00000080))	
#define UART_EFR_AUTORTS     ((uint32_t)(0x00000040))
#define UART_EFR_TXSWFC_1     ((uint32_t)(0x00000004))	
#define UART_EFR_TXSWFC_2     ((uint32_t)(0x00000008))	
#define UART_EFR_TXSWFC_12     ((uint32_t)(0x0000000c))	

#define UART_EFR_RXSWFC_1     ((uint32_t)(0x00000001))
#define UART_EFR_RXSWFC_2     ((uint32_t)(0x00000002))
#define UART_EFR_RXSWFC_12     ((uint32_t)(0x00000003))

/**
  * @}
  */
#define UART_MCR_IREN     ((uint32_t)(0x00000040))
#define UART_MCR_MLBM     ((uint32_t)(0x00000010))
#define UART_MCR_RTS     ((uint32_t)(0x00000002))


/* UART_T macro ------------------------------------------------------------*/
/* UART_T functions ------------------------------------------------------- */

/* Initialization and Configuration functions *********************************/
void UART_DeInit(UART_T* UARTx);
void UART_Init(UART_T* UARTx, UART_InitTypeDef* UART_InitStruct);
void UART_StructInit(UART_InitTypeDef* UART_InitStruct);

/* Data transfers functions ***************************************************/
void UART_SendData(UART_T* UARTx, uint16_t Data);
uint8_t UART_ReceiveData(UART_T* UARTx);

/* FIFO Control functions *****************************************************/
void UART_FIFOControl(UART_T* UARTx,UART_InitFifoTypeDef* UART_FIFOInitStruct);
void UART_FIFORstTx(UART_T* UARTx);
void UART_FIFORstRx(UART_T* UARTx);
void UART_FIFOCmd(UART_T* UARTx,FunctionalState NewStatus);

/* Interrupts flags management functions **********************************/
void UART_ITConfig(UART_T* UARTx, uint32_t UART_IT, FunctionalState NewState);
IT_UART_StatusType UART_GetITStatus(UART_T* UARTx);

void UART_ModemControl(UART_T* UARTx,uint32_t modemctl);

#define  UART_DATALENGTH 200

typedef struct
{
    u8 Receive_Buffer[UART_DATALENGTH];//数据缓冲区
    u8 *receive_last;//接收末尾
    u8 *get_last;//获取末尾
    u8 receive_signal_flag;//有收到数据标志位
    u8 receive_full_flag;//接收满标志位
    u16 receive_length;//接收长度
} USART_ReceiveDataTypedef; //定义队列数据类型

extern USART_ReceiveDataTypedef    USART3_QueueBuf;

void    UART_Receive (USART_ReceiveDataTypedef *Uart_Device_Rx,u8 Data);

void    UART_ReceiveValueInit(USART_ReceiveDataTypedef *Uart_Device_Rx,u16 Length);

u8 UART_GetByte (USART_ReceiveDataTypedef *Uart_Device_Rx);


#ifdef __cplusplus
}
#endif

#endif /* __UART_H_ */

