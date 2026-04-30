
#include "bsp_uart.h"


/*****************************************************************************
 ** \brief	UART_UART1_Config
 **			
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
static u8 postion_Statu;
u8 REDATE_Flag;
u8 upDATE_Flag;

void UART_UART1_Config(void)
{
	uint32_t  BuadRate =9600;
	/*
	(1)设置UARTx模式
	*/
	UART_ConfigRunMode(UART1, BuadRate, UART_WLS_8, UART_PARITY_NONE,UART_STOP_BIT_1);
	/*
	(2)开启UARTx时钟
	*/	
	SYS_EnablePeripheralClk(SYS_CLK_UART1_MSK);
	/*
	(3)开启UARTx输出
	*/		
	SYS_SET_IOCFG(IOP34CFG, SYS_IOCFG_P34_TXD1);
	/*
	(4)开启UARTx输入
	*/		
    SYS_SET_IOCFG(IOP35CFG, SYS_IOCFG_P35_RXD1);
	/*
	(5)开启UARTx�?�?
	*/
    UART_ITConfig(UART1,UART_IER_RBRIE,ENABLE);

	NVIC_ClearPendingIRQ(UART1_IRQn);
    NVIC_EnableIRQ(UART1_IRQn);
	
}

void  UART_SendDatas(UART_T* USARTx,uint8_t * buffer, uint8_t size)
{
    unsigned char i = 0;
    for(i=0; i<size; i++)
    {
        UART_SendData(USARTx, (uint16_t)buffer[i]);
       // while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
    }
}

void UART_Action(void)
{
	u8 j;
	if(USART3_QueueBuf.receive_full_flag==1)
	{
      j=USART3_QueueBuf.Receive_Buffer[2];
	  switch (j)
	  {
	  case 0: 
	          LED2_OFF;
	          LED3_OFF;		   
		      LED1_OFF;
			  postion_Statu=3;
			  P_Status = Null;
			  break;
	 case 1:  
	          LED2_OFF;
	          LED3_OFF;		   
		      LED1_ON;
			  postion_Statu=0;
			  P_Status = Low;
			  break;
    case 2:  
	          LED2_ON;
	          LED3_OFF;		   
		      LED1_OFF;
			  postion_Statu=1;
			  P_Status = Mid;
			  break;
	case 3:  
	          LED2_OFF;
	          LED3_ON;		   
		      LED1_OFF;
			  postion_Statu=2;
			  P_Status = High;
			  break;
			  default:break;
	  }

    USART3_QueueBuf.receive_full_flag=0;
	}
}