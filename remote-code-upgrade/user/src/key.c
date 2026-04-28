#include "key.h"
#include "Device_Init.h"
#include "vk1056b.h"
#include "work.h"
#include "gpio.h"
#include "system.h"
#include "TM1639.h"
#include "string.h"
#include "bsp_uart.h"
#include "gpioctl.h"
#include "app_epwm.h"


/*--------------------------------------------------------------------------
* 函数名  : KEY_StatusGet
* 描述    : 按键检测
* 输入    : 无
* 输出    : 无
* 返回    : 无
* 日期    : 2019.07.17
* 作者    : dinghx
---------------------------------------------------------------------------*/
E_Status P_Status = Null;
 GO_Status K_Status = other;
// u8  key_status;
static u8 postion_Statu;
u8  KEY_StatusGet( void )
{

    static  u16  key_count = 0;
    static  u8  key_id = 0; 

    u8  key_status;
    u8  key_value = 0;

key_status=GPIO_GET_INPUT_PIN(GPIO1,GPIO_PIN_5);


    if ( 0 == key_status ) //电源按下
    {
       delay_ms(10);//防抖
       if(key_status==0)
       {
          delay_ms(100);
          key_status=GPIO_GET_INPUT_PIN(GPIO1,GPIO_PIN_5);
          if(key_status)
          {
            key_value=2; //短按
          }
          else
          {
             delay_ms(1000);
             delay_ms(500);
             key_status=GPIO_GET_INPUT_PIN(GPIO1,GPIO_PIN_5);
             if(key_status==0)//长按
             {
              key_value=1;
             }

          }
       }
    }
    return  key_value;
}


/*---------------------------------------------------------------------------
* 函数名  : KEY_Handle
* 描述    : 按键功能处理
* 输入    : 无
* 输出    : 无
* 返回    : 无
* 日期    : 2019.07.17
* 作者    : dinghx
---------------------------------------------------------------------------*/
void  KEY_Handle( void )
{
     u8  key_cz;
   //   static u8  i=0;
     static  u8  pwr_sta = 0;
     key_cz=KEY_StatusGet();
     if (key_cz==1)
     {
         if(pwr_sta==0)
         {
             pwr_sta =1;
             LEDPWR_ON;
            CTRLPWR_ON;//长按开机
            delay_ms(500);
            K_Status = ON;
         }
         else
         {
            pwr_sta =0;
            CTRLPWR_OFF;//长按关机
            LEDPWR_OFF;
            LED1_OFF;	
            LED2_OFF;
            LED3_OFF;
             K_Status = OFF;
         }
     }
     else if(key_cz==2)//短按
     {
         if (postion_Statu==0&&pwr_sta==1)
         {
            LED1_ON;	
            LED2_OFF;
            LED3_OFF;
            postion_Statu=1; 
            P_Status = Low;
         }
         else if(postion_Statu==1&&pwr_sta==1)
         {
           LED1_OFF;   
	        LED2_ON;
           LED3_OFF; 
           postion_Statu=2; 
           P_Status = Mid;
         }
		    else if(postion_Statu==2&&pwr_sta==1)
         {
            LED2_OFF;  
	         LED3_ON;		   
		      LED1_OFF; 
            postion_Statu=3;
            P_Status = High;
         } 
         else if(postion_Statu==3&&pwr_sta==1)
         {
            LED2_OFF;  
	         LED3_OFF;		   
		      LED1_OFF; 
            postion_Statu=0;
            P_Status = Null; 
         }             
     }


}



/* ************ ****** ************ THE FILE END  ************ ****** ************ */

