#include "Timing.h"
#include "bsp_pid.h"
#include "debug_uart.h"
#include "bsp_hard.h"
#include "adc.h"



void Timer0_Timing1_1ms(void)
{


}





/*****************************************************************************
** 功能：	1毫秒定时
*****************************************************************************/
void Timing1_1ms(void)
{
	Flag.WorkStart = 1;
	LED_DrvScan();
	BEEP_DrvScan();
	APP_TimeOverScan();
}


/*****************************************************************************
** 功能：	10毫秒定时
*****************************************************************************/
void Timing1_10ms(void)
{
	KEY_DrvScan();
	/***********************************************************/
	/*                蓝牙工作状态扫描                         */
	/***********************************************************/
	if (Flag.PowerOn == 1)
	{
		Bluetooth_Scan();
	}
		
}



/*****************************************************************************
** 功能：	10毫秒定时
*****************************************************************************/
void Timing1_20ms(void)
{
	KEY_Scan();
}



/*****************************************************************************
** 功能：	50毫秒定时
*****************************************************************************/

void Timing1_50ms(void)
{


}


/*****************************************************************************
** 功能：	100毫秒定时
*****************************************************************************/
void Timing1_100ms(void)
{
    /* Update cooling PID control */
    if (Flag.WorkStart && g_cooling_pid.enabled)
    {
        PID_Update();
        Cooling_SetPower(PID_GetOutput());
    }
}



/*****************************************************************************
** 功能：	250毫秒定时
*****************************************************************************/

void Timing1_250ms(void)
{

}



/*****************************************************************************
** 功能：	500毫秒定时
*****************************************************************************/

void Timing1_500ms(void)
{
    /* Print PID temperature data for debug */
    /* Format: Cur:24.3C Tar:5.0C Out:100 En:1 ADC:2015 */
    int16_t cur = g_cooling_pid.current_temp;
    int16_t tar = g_cooling_pid.target_temp;
    Debug_UART_SendString("Cur:");
    Debug_UART_SendValue((int32_t)(cur / 10));
    Debug_UART_SendByte('.');
    Debug_UART_SendValue((int32_t)(cur % 10));
    Debug_UART_SendString("C Tar:");
    Debug_UART_SendValue((int32_t)(tar / 10));
    Debug_UART_SendByte('.');
    Debug_UART_SendValue((int32_t)(tar % 10));
    Debug_UART_SendString("C Out:");
    Debug_UART_SendValue((int32_t)PID_GetOutput());
    Debug_UART_SendString(" En:");
    Debug_UART_SendValue((int32_t)g_cooling_pid.enabled);
    Debug_UART_SendString(" ADC:");
    Debug_UART_SendValue((int32_t)ADC_Read(9));
    Debug_UART_SendByte(0x0D); Debug_UART_SendByte(0x0A);
}

/*****************************************************************************
** 功能：	1秒定时
*****************************************************************************/

void Timing1_1s(void)
{

}
//----------------------------------------------------------------------------

