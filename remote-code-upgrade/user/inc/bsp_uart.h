#ifndef __BSP_UART_H_
#define __BSP_UART_H_

#include "cms32f033.h"
#include "uart.h"
#include "system.h"

void UART_UART1_Config(void);
void UART_SendDatas(UART_T* USARTx,uint8_t * buffer, uint8_t size);

extern u8 REDATE_Flag;
extern u8 upDATE_Flag;

#endif /* __BSP_UART_H_ */
