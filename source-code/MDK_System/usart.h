#ifndef __USART_H
#define __USART_H
#include "sys.h"





#define		UART1_SEND_ENB			(1)			// 发送允许
#define		UART1_RECV_ENB			(1)			// 接收允许



typedef struct {
	INT8U* Addr;			// FIFO缓冲区的首地址
	INT8U	Size;			// FIFO缓冲区的字节大小
	INT8U	Read;			// FIFO缓冲区的读取位置偏移
	INT8U	Writ;			// FIFO缓冲区的写入位置偏移
}FIFO_TypeDef;


void UART1_SetFifoTxD(INT8U* adr, INT8U siz);
void UART1_SendScan(void);
INT8U UART1_SendByte(INT8U c);
INT8U UART1_SendBytes(INT16U n, INT8U* s);
INT8U UART1_SendString(INT8* s);
INT8U UART1_SendValueU(INT32U v, INT8U n, INT8U c);
INT8U UART1_SendValueS(INT32S v, INT8U n, INT8U c);
void UART1_SetFifoRxD(INT8U* adr, INT8U siz);
INT8U UART1_RecvByte(INT8U c);
void UART1_RecvScan(void);
void UART1_Init(INT32U baud);


INT8U FIFO_GetCnt(FIFO_TypeDef* fifo);
INT8U FIFO_IsEmpty(FIFO_TypeDef* fifo);
INT8U FIFO_IsFull(FIFO_TypeDef* fifo);
INT8U FIFO_In(FIFO_TypeDef* fifo, INT8U dat);
INT8U FIFO_Out(FIFO_TypeDef* fifo, INT8U* dat);









#endif /*__USART_H*/
