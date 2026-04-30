#include "cms32f033_it.h"
 
 
/****************************************************************************
** 功能：	不可屏蔽异常 服务程序
****************************************************************************/
void NMI_Handler(void)
{

}
/****************************************************************************
** 功能：	硬件错误异常 服务程序
****************************************************************************/
void HardFault_Handler(void)
{

}

/****************************************************************************
** 功能：	SVC异常 服务程序
****************************************************************************/
void SVC_Handler(void)
{

}

/****************************************************************************
** 功能：	PendSV异常 服务程序
****************************************************************************/
void PendSV_Handler(void)	
{

}



/****************************************************************************
** 功能：	系统定时器中断 服务程序 （100us定时）
****************************************************************************/

void SysTick_Handler(void)
{
	static	INT16U	tickCnt = 0;
	static  INT8U	cnt = 0;
	static  INT8U 	sneFlag = 0;
	static  INT8U   eptFlag = 0;

	if (TICK_GetOverflowIntFlag())
	{
		if((Record.HandWave == 5)&&(Record.Switch == 1))
		{
			for(;eptFlag < 1;eptFlag = 2){
				tickCnt = 0;
				cnt = 0;
				sneFlag = 0;
			}
			tickCnt++;
			if(tickCnt >= 1){
				tickCnt = 0;
				cnt++;
				if(cnt == 1){
					EPT_ChannelA_On();
				}
				else if(cnt == 2){
					EPT_ChannelAB_Off();
				}
				else if(cnt == 3){
					EPT_ChannelB_On();
				}
				else if(cnt == 4){
					EPT_ChannelAB_Off();
					cnt = 0;
				}
			}
		}
		else if((Record.HandWave == 6)&&(Record.Switch == 1))
		{
			for(;sneFlag < 1;sneFlag = 2){
				eptFlag = 0;
				tickCnt = 0;
				cnt = 0;
			}
			tickCnt++;
			cnt++;
			if(cnt == 1){
				EPT_ChannelA_On();
			}
			else if(cnt == 2){
				EPT_ChannelAB_Off();
			}
			else if(cnt == 3){
				EPT_ChannelB_On();
			}
			else if(cnt == 4){
				EPT_ChannelAB_Off();
				cnt = 0;
			}
		}
	}
	
}



/****************************************************************************
** 功能：	CCP中断 服务程序
****************************************************************************/
void CCP_IRQHandler(void)
{

}


/****************************************************************************
** 功能：	GPIO0管脚触发中断 服务程序
****************************************************************************/
void GPIO0_IRQHandler(void)
{

}

/****************************************************************************
** 功能：	GPIO1管脚触发中断 服务程序
****************************************************************************/
void GPIO1_IRQHandler(void)
{

}
/****************************************************************************
** 功能：	GPIO2管脚触发中断 服务程序
****************************************************************************/
void GPIO2_IRQHandler(void)
{

}
/****************************************************************************
** 功能：	GPIO3管脚触发中断 服务程序
****************************************************************************/
void GPIO3_IRQHandler(void)
{

}
/****************************************************************************
** 功能：	GPIO4管脚触发中断 服务程序
****************************************************************************/
void GPIO4_IRQHandler(void)
{

}



/****************************************************************************
** 功能：	窗口看门狗中断 服务程序
****************************************************************************/
void WWDT_IRQHandler(void)
{

}







/****************************************************************************
** 功能：	TMR0中断 服务程序（100us定时）
****************************************************************************/
void TMR0_IRQHandler(void)
{

	if (TMR_GetOverflowIntFlag(TMR0))
	{
		TMR_ClearOverflowIntFlag(TMR0);
		#if K1_TIMER0
			Timer0_Timing1_1ms();
		#endif 
	}
}


/****************************************************************************
** 功能：	TMR1中断 服务程序（1ms定时）
****************************************************************************/
void TMR1_IRQHandler(void)
{
	static	INT16U	msCnt = 0;

	if (TMR_GetOverflowIntFlag(TMR1))
	{
		TMR_ClearOverflowIntFlag(TMR1);

		if (++msCnt >= 60000) {						// 1m    定时
			msCnt = 0;
		}
		if ((msCnt % 1000) == 0)	Timing1_1s();		// 1s    定时
		if ((msCnt % 500) == 0)		Timing1_500ms();	// 500ms 定时
		if ((msCnt % 250) == 0)		Timing1_250ms();	// 250ms 定时
		if ((msCnt % 100) == 0)		Timing1_100ms();	// 100ms 定时
		if ((msCnt % 50) == 0)		Timing1_50ms();		// 100ms 定时
		if ((msCnt % 20) == 0)		Timing1_20ms();		// 20ms  定时
		if ((msCnt % 10) == 0)		Timing1_10ms();		// 10ms  定时
		if ((msCnt % 2) == 0)		Timing1_2ms();		// 10ms  定时
									Timing1_1ms();		// 1ms   定时
	}
}




/****************************************************************************
** 功能：	UART1中断 服务程序
****************************************************************************/
void UART1_IRQHandler(void)
{
	switch (UART_GetIntFlag(UART1)) {
	case UART_INTFLAG_MODEM:				// MODEM状态发生改变
		break;
	case UART_INTFLAG_TXD_EMPTY:			// 发送保持寄存器为空
		break;
	case UART_INTFLAG_RXD_FULL:				// 接收数据有效
		UART1_RecvByte(UART_Rece(UART1));	// 将接收到的数据存入到接收缓冲区
		// UART1_SendByte(UART_Rece(UART1));	// 仅调试用
		break;
	case UART_INTFLAG_LINE:					// 接收到线状态
		break;
	case UART_INTFLAG_OVER:					// 接收定时器溢出
		break;
	}
}






/****************************************************************************
** 功能：	EPWM中断 服务程序
****************************************************************************/
uint8_t EPWM2_zeroCnt = 0;
void EPWM_IRQHandler(void)
{


	if ((Record.HandWave == 1) || (Record.HandWave == 2) || (Record.HandWave == 3))
	{
		if (EPWM_GetUpCmpIntFlag(EPWM1))
		{
			EPWM_ClearUpCmpIntFlag(EPWM1);

			EPWM_Start(EPWM_CH_2_MSK | EPWM_CH_3_MSK);
		}
		if (EPWM_GetDownCmpIntFlag(EPWM1))
		{
			EPWM_ClearDownCmpIntFlag(EPWM1);

			EPWM_Stop(EPWM_CH_2_MSK | EPWM_CH_3_MSK);
		}

		if (EPWM_GetUpCmpIntFlag(EPWM2))
		{
			EPWM_ClearUpCmpIntFlag(EPWM2);

			EPWM_Start(EPWM_CH_4_MSK);
		}
		if (EPWM_GetDownCmpIntFlag(EPWM2))
		{
			EPWM_ClearDownCmpIntFlag(EPWM2);

			EPWM_Stop(EPWM_CH_4_MSK);
		}

		if (EPWM_GetUpCmpIntFlag(EPWM3))
		{
			EPWM_ClearUpCmpIntFlag(EPWM3);

			EPWM_Stop(EPWM_CH_4_MSK);
		}
		if (EPWM_GetDownCmpIntFlag(EPWM3))
		{
			EPWM_ClearDownCmpIntFlag(EPWM3);

			EPWM_Start(EPWM_CH_4_MSK);
		}
	}



	if (Record.HandWave == 4)
	{
		if (EPWM_GetUpCmpIntFlag(EPWM2))
		{
			EPWM_ClearUpCmpIntFlag(EPWM2);
		}

		if (EPWM_GetDownCmpIntFlag(EPWM2))
		{
			EPWM_ClearDownCmpIntFlag(EPWM2);
		}

		if (EPWM_GetUpCmpIntFlag(EPWM3))
		{
			EPWM_ClearUpCmpIntFlag(EPWM3);
		}

		if (EPWM_GetDownCmpIntFlag(EPWM3))
		{
			EPWM_ClearDownCmpIntFlag(EPWM3);
		}
	}

}