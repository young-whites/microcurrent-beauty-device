#include "work.h"
#include "app_adc1.h"
#include "app_epwm.h"
#include "pid.h"
#include "bsp_adc.h"
#include "key.h"
#include "gpioctl.h"
#include "Device_Init.h"

// E_Status P_Status = Null;
static u8 postion_Statu=0;
static u32 time=0;

void  motor_ctrl( void )
{
 static int i=0;
 static int j=0;
    if( P_Status == Low&&postion_Statu==0)
        {
					CTRLmotor1_ON;
					delay_ms(10);
          CTRLmotor_ON;
          time=350;
          postion_Statu=1;
        }//Ò»µµ
    else if( P_Status ==Mid&&postion_Statu==1)
        {
           time=700;             
					postion_Statu=2;
        }//¶þµµ
    else if( P_Status==High&&postion_Statu==2)
        {
          time=1000;
           postion_Statu=3;
        }//Èýµµ
     else if( P_Status == Null&&postion_Statu==3)
      {
        EPWM_ConfigChannelSymDuty(EPWM1, 0);
        time=0;
        delay_ms(1000);
        CTRLmotor_OFF;
        CTRLmotor1_OFF;
        postion_Statu=0;
      }

      if(P_Status!=Null)
      {
        if(i<time&&j==0)
        {
          EPWM_ConfigChannelSymDuty(EPWM1, 2400);
          j=1;
        }
        else if(i==time)
        {
          EPWM_ConfigChannelSymDuty(EPWM1, 0);
        }
        if(j==1)
        {
          i++;
        }
        if(i==5000)
        {
          j=0;
          i=0;
        }
				

      }
    
}
