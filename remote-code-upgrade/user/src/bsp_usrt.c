#include "bsp_uart.h"

u8 REDATE_Flag;
u8 upDATE_Flag;

void UART_UART1_Config(void)
{
	uint32_t  BuadRate =9600;
	UART_ConfigRunMode(UART1, BuadRate, UART_WLS_8, UART_PARITY_NONE,UART_STOP_BIT_1);
	SYS_EnablePeripheralClk(SYS_CLK_UART1_MSK);
	SYS_SET_IOCFG(IOP34CFG, SYS_IOCFG_P34_TXD1);
	SYS_SET_IOCFG(IOP35CFG, SYS_IOCFG_P35_RXD1);
	UART_ITConfig(UART1,UART_IER_RBRIE,ENABLE);
	NVIC_ClearPendingIRQ(UART1_IRQn);
	NVIC_EnableIRQ(UART1_IRQn);
}

void UART_SendDatas(UART_T* USARTx,uint8_t * buffer, uint8_t size)
{
	unsigned char i = 0;
	for(i=0; i<size; i++)
	{
		UART_SendData(USARTx, (uint16_t)buffer[i]);
	}
}
