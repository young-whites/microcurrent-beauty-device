#include "cms32f033x.h"
#include "Psfr_uart.h"

/*****************************************************************************
 ** 功能：	配置异步串口运行模式
 ** 参数： [in] uart: UART0、UART1
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
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void UART_ConfigRunMode(UART_T* uart,uint32_t Baudrate,uint32_t WordLength, uint32_t Parity, uint32_t StopBit)
{
	uart->LCR &= ~(UART_LCR_WLS_Msk | UART_LCR_SBS_Msk|UART_LCR_PEN_Msk|UART_LCR_PSEL_Msk);
	uart->LCR |= WordLength | Parity | StopBit;
	uart->DLR = SystemAPBClock /16/Baudrate;
}

/*****************************************************************************
 ** 功能：	获取发送寄存器空标志位
 ** 参数： [in] uart: UART0、UART1
 ** 返回：  1:空 0：非空
 ** 说明：	
*****************************************************************************/
uint32_t UART_TxdIsEmpty(UART_T* uart)
{
	return((uart->LSR & UART_LSR_THRE_Msk)? 1:0);	
}

/*****************************************************************************
 ** 功能：	获取接收数据有效标志位
 ** 参数： [in] uart: UART0、UART1
 ** 返回：  1:有未读数据 0：无未读数据
 ** 说明：	
*****************************************************************************/
uint32_t UART_RxdIsFull(UART_T* uart)
{
	return((uart->LSR & UART_LSR_RDR_Msk)? 1:0);	
}

/*****************************************************************************
 ** 功能：	开启串口发送中断
 ** 参数： [in] uart: UART0、UART1
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void UART_EnableTxdInt(UART_T* uart)
{
	uart->IER |= UART_IER_THREIE_Msk;
}

/*****************************************************************************
 ** 功能：	关闭串口发送中断
 ** 参数： [in] uart: UART0、UART1
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void UART_DisableTxdInt(UART_T* uart)
{
	uart->IER &= ~(UART_IER_THREIE_Msk);
}

/*****************************************************************************
 ** 功能：	开启串口接收中断
 ** 参数： [in] uart: UART0、UART1
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void UART_EnableRxdInt(UART_T* uart)
{
	uart->IER |= UART_IER_RBRIE_Msk;
}

/*****************************************************************************
 ** 功能：	关闭串口接收中断
 ** 参数： [in] uart: UART0、UART1
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void UART_DisableRxdInt(UART_T* uart)
{
	uart->IER &= ~(UART_IER_RBRIE_Msk);
}

/*****************************************************************************
 ** 功能：	获取串口中断状态标志
 ** 参数： [in] uart: UART0、UART1
** 返回：  0:Modem状态发生改变  1:发送保持寄存器为空  2:接收数据有效  3:接收到线状态  6:接收定时器溢出
 ** 说明：	
*****************************************************************************/
uint32_t UART_GetIntFlag(UART_T* uart)
{
	return((uart->IIR & UART_IIR_INTID_Msk) >> UART_IIR_INTID_Pos);
}

/*****************************************************************************
 ** 功能：	发送一个数据到串口
 ** 参数： [in] uart: UART0、UART1
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void UART_Send(UART_T* uart, uint32_t Data)
{
	uart->THR = Data;
}

/*****************************************************************************
 ** 功能：	接收一个数据从串口
 ** 参数： [in] uart: UART0、UART1
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
uint32_t UART_Rece(UART_T* uart)
{
	return uart->RBR;
}

