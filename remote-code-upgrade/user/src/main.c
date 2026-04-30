#include "cms32f033.h"
#include "key.h"
#include "bsp_timer.h"
#include "clock.h"
#include "TM1639.h"
#include "system.h"
#include "gpioctl.h"
#include "gpio.h"
#include "app_epwm.h"
#include "app_adc1.h"
#include "battery.h"
#include "Device_Init.h"
#include "bsp_uart.h"
#include "string.h"
#include "ch455.h"
#include "gpioctl.h"
#include "bsp_adc.h"
#include "uart.h"
#include "fmc.h"
#include "flash_operate.h"
// #include    "Ymode.h"

#define begin_adress  0x7000

 u8 upgr_flag=1;
u8 up_flag;

//void  Update_CommandReceive( USART_ReceiveDataTypedef *Uart_Device_Rx)
void  Update_CommandReceive( u8 data)
{
    static  u8  USART_RX_STA = 0;
    static  u8  cont = 0;

    cont=data;
    switch ( USART_RX_STA) //FWUPDATE
    {
        case 0:
            if(cont==0x55)
            USART_RX_STA = 1;
            break;
        case 1:
        {
            if (cont ==0xAA)    {USART_RX_STA = 2;}
            else                {USART_RX_STA = 0;}
        }break;
        


            

        case 2:
        {
            if ( cont ==0x05 )  USART_RX_STA = 3;
            else                USART_RX_STA = 0;
        }break;
            

            

        case 3:
        {
            if (cont ==0x00)
            {
                USART_RX_STA = 4;
            }
            else
            {
                USART_RX_STA = 0;
            }
        }


            break;

        case 4:
            if ( cont ==0x0B)
            {
                USART_RX_STA = 5;
            }
            else
            {
                USART_RX_STA = 0;
            }

            break;

        case 5:
            if ( cont ==0x31)
            {
                USART_RX_STA = 6;
            }
            else
            {
                USART_RX_STA = 0;
            }

            break;

        case 6:
            if ( cont ==0x02)
            {
                USART_RX_STA = 7;
            }
            else
            {
                USART_RX_STA = 0;
            }

            break;

        case 7:
            if ( cont ==0xAA )
            {
                USART_RX_STA = 8;
            }
            else
            {
                USART_RX_STA = 0;
            }

            break;
        case 8:
            if (cont ==0xBA )
            {
                USART_RX_STA = 9;
            }
            else
            {
                USART_RX_STA = 0;
            }
            break; 
        case 9:
            if ( cont ==0xD2)
            {
                USART_RX_STA = 10;
            }
            else
            {
                USART_RX_STA = 0;
            }
            break;     

        default:
            break;
    }

    if ( USART_RX_STA == 10 )
    {
        USART_RX_STA = 0;
        
       upDATE_Flag=1;
       UART_ReceiveValueInit(&USART3_QueueBuf,UART_DATALENGTH);
       upgr_flag=0;
    }
}

int main(void)
{
	CLOCK_Init();//时钟初�?�化
	Dveice_Init(48000);////初�?�化延迟函数滴答计时�?????
	__disable_irq();//关中�?????
    gpio_init();//IO口初始化
	UART_UART1_Config();
	UART_ReceiveValueInit(&USART3_QueueBuf,UART_DATALENGTH);
	// System_Device_Init();//系统初�?�化
	__enable_irq();	//开�?????�?????

	delay_ms(100);
    up_flag=IAP_ReadOneWord(0x1C000000,0x55);
    delay_ms(100);
    upgr_flag=IAP_ReadOneWord(0x1C000200,0x55);
    if(up_flag!=0x33)//复位，�?�置�??动位�??至APP
    {
        // UART_SendData(UART1,up_flag);
        // UART_SendDatas(UART1,"goapp\r\n",strlen("goapp\r\n"));
        // delay_ms(200);
       IAP_Reset();
    }
      if(upgr_flag!=0x33)//复位，�?�置�??动位�??至APP
    {
        upDATE_Flag=1;
    }
    

	//IAP_WriteOneWord(0x1C000000,0x33,0x55);
	// UART_SendDatas(UART1,"AT+NAMEDr-04\r\n",strlen("AT+NAMEDr-04\r\n"));
	
	while(1)
	{	
        if(upgr_flag!=0x33)//正常流程
        {
            IAP_Erase_512B(0x1C000200,0xAA);
            delay_ms(10);
            IAP_WriteOneWord(0x1C000200,0x33,0x55);//写入进入升级流程标志�??
            delay_ms(100);
            UART_SendData(UART1,0x43);
            delay_ms(200);
            YModem_Receive(); 
            // upgr_flag=1;

        }
        else//打断后流�??
        {
            UART_SendDatas(UART1,"UPGR\r\n",strlen("UPGR\r\n"));
            delay_ms(200);
           
        }
	
        // if(upDATE_Flag==1)
        // {
        // delay_ms(100);
        // UART_SendData(UART1,0x43);
        // delay_ms(200);
        // YModem_Receive();       
        // }
        
        // GPIO_SET_PIN(GPIO3,GPIO_PIN_2_MSK);
        // delay_ms(200);
        // GPIO_RESET_PIN(GPIO3,GPIO_PIN_2_MSK);
        // delay_ms(200);
	}
}













