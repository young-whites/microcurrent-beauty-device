#ifndef __BSP_UART_H_
#define __BSP_UART_H_

/* Include files */
#include "cms32f033.h"
#include "uart.h"
#include "system.h"
#include "key.h"

/*****************************************************************************
 ** \brief	UART_UART1_Config
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void UART_UART1_Config(void);

void  UART_SendDatas(UART_T* USARTx,uint8_t * buffer, uint8_t size);

void UART_Action(void);
extern u8 REDATE_Flag;
extern u8 upDATE_Flag;



#endif /* __DEMO_UART_H_ */
