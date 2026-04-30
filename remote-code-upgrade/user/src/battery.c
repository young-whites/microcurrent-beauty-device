#include "battery.h"
#include "bsp_adc.h"
#include "work.h"
#include "TM1639.h"
#include "key.h"

BAT_StatuTypeDef_T BatteryStatu;
// static u8 firsttime=0;

// static u8 low_flag=0,off_flag=0;
 u16 VBatVolt ;
 u8 Battery_Ah_flg;
// GO_Status K_Status ;
void battery(void)
{
    static u8 MENU=0;
    static u8 ad_delay=0;
    u16 t=1,i,j=0;
    u8 NCHGR_Status = 0, NSTDBY_Status = 0;
    static u8 last_NCHGR_Status = NO_CHARGE;

    NCHGR_Status= IO_NCHGR_Status();
    NSTDBY_Status= IO_NSTDBY_Status();

    if(( NCHGR_Status == 0 ) && ( NSTDBY_Status == 1 )) //正在充电
    {
        BatteryStatu.ChargeStatu = CHARING;
    }
    else if(( NCHGR_Status == 1 ) && ( NSTDBY_Status == 0 )) //充满
    {
        BatteryStatu.ChargeStatu = STDBY;
    }
    else if(( NCHGR_Status == 1 ) && ( NSTDBY_Status == 1 )) //未充电
    {
        BatteryStatu.ChargeStatu = NO_CHARGE;
    }


    if ( last_NCHGR_Status != BatteryStatu.ChargeStatu ) //充满电，电源指示灯常亮
    {
        last_NCHGR_Status = BatteryStatu.ChargeStatu;

        if(BatteryStatu.ChargeStatu == STDBY)
        {
			LEDPWR_ON;
        }      
    }

    if(BatteryStatu.ChargeStatu==CHARING)//如果在充电，呼吸灯
    {
          if(MENU==0)
          {
              for (t= 1;t<601 ;t++ )
              {
                  LEDPWR_ON;
                  delay_us(t);
                  LEDPWR_OFF;
                  delay_us(601-t);
              }
              if(t==601)
              {
                 MENU=1;
              }
          }
        if(MENU==1)
          {
              for (t=600;t>0 ;t--)
              {
                  LEDPWR_ON;
                  delay_us(t);
                  LEDPWR_OFF;
                  delay_us(601-t);
              }
              if(t==0)
              {
                 MENU=0;
              }
          }
    }
    else if(BatteryStatu.ChargeStatu==NO_CHARGE)//没有充电，保持
    {
       
    }	
//电量检测
 if ((K_Status == ON )&& ( NCHGR_Status == 1 ) && ( NSTDBY_Status == 1 ))
  {
    VBatVolt= KalmanFilter1(ADC_ConverStable(1),1,100);
    if (j==0)
    {
       ad_delay++;
    }
    if (VBatVolt<=2800&&ad_delay>=100) //低电量关机提醒
    {

        Battery_Ah_flg=bat_maxlow;
        //    j=1;
        //    LEDPWR_ON;
        //    SetLongDelay2(50);
        //    LEDPWR_OFF;
        //    SetLongDelay2(50); //快闪提醒
        //    CTRLPWR_OFF;   //关机   
    }
    else if(VBatVolt<=3000&&VBatVolt>2800&&ad_delay>=100) //低电量提醒
    {
        Battery_Ah_flg=bat_low;
        // j=1;
        // LEDPWR_ON;
        // SetLongDelay2(200);
        // LEDPWR_OFF;
        // SetLongDelay2(200);
    }
    else if(VBatVolt>3205 && ad_delay>=100) //正常电量
    {
        j=1;
        LEDPWR_ON;
        Battery_Ah_flg=FULL;
    }
  }
    
}
        // if(VBatVolt<LOFF_LOW_POWER_WARM)
        // {
        //     j++;
        //     if(VBatVolt[i]<LOFF_LOW_POWER_OFF)
        //     j++;
        // }
        
    //     if(j==10)off_flag=1;
    //     if((j==5)&&(low_flag==0))
    //     {
    //         low_flag=1;
    //         DEBUG( "LOFF_LOW_POWER_WARM\n" );
    //         IO_status_Set( FLICKER, 10000, 20000, &IO_BATTERY_Ctrl,0 );
    //         BATTERY_PARAT.power_state1=0;
    //         //   DEBUG( "VBatVolt=%d\n",BATTERY_PARAT.VBatVolt );
    //         temp_buffer[1]=BATTARY_LOW_CMD;
    //         SendDataPacketToPeripheral(1,READDATA_STATUS,RECEIVE_DATA_SUCCESS,temp_buffer+1);

    //     }

    //      if(off_flag==1)
    //         {
    //             switch ( SoftWarePara.power_state1 )
    //             {
    //                 case 0:
    //                 {
    //                     DEBUG( "LOFF_LOW_POWER_OFF\n" );
    //                     IO_status_Set( FLICKER, 2500, 5000, &IO_BATTERY_Ctrl,0 );
    //                     SetLongDelay5( DELAY_10S );
    //                     SoftWarePara.power_state1 = 1;
    //                     break;
    //                 }
    //                 case 1:
    //                 if ( TRUE == TimerOutLongDelay5() )
    //                 {
    //                     DEBUG( "LOFF_LOW_POWER_OFFOK\n" );
    //                     SoftWarePara.power_state1 = 0;
    //                     EEPROM_WriteData(LOW_POWER_FLAG_ADDR,0xAB);
    //                     LowPower_Enable();
    //                     IO_status_Set( NORMALLYOFF, 5000, 10000, &IO_BATTERY_Ctrl,0 );
    //                     break;
    //                 }
    //                 default:
    //                     break;
    //             }
    //         }
    // }     
    // System_StatusParameter.batteryvalue=battery_get();
//}
// u8 battery_get( void )
// {
//     static u16 BATTERY_val=0,r_BATTERY_val=0;

//     SoftWarePara.VBatVolt = (VBatVolt[0]+VBatVolt[1]+VBatVolt[2]+VBatVolt[3]+VBatVolt[4])/5;
//     if(firsttime==0)
//     {
//         firsttime=1;
//         r_BATTERY_val=SoftWarePara.VBatVolt;
//     }
//     if(r_BATTERY_val>SoftWarePara.VBatVolt)
//     {
//         if(BATTERY_val<=2)//&&off_flag)
//         {
//             if(off_flag)
//             {
//                 if(r_BATTERY_val-SoftWarePara.VBatVolt>=2)
//                 {
//                 r_BATTERY_val-=2;
//                 }
//                 else 
//                 {
//                 r_BATTERY_val=SoftWarePara.VBatVolt;
//                 }
//             }
//         }
//         else
//         {
//            if(r_BATTERY_val-SoftWarePara.VBatVolt>=2)
//             {
//             r_BATTERY_val-=2;
//             }
//             else 
//             {
//             r_BATTERY_val=BATTERY_PARAT.VBatVolt;
//             }
//         }
//         BATTERY_val=( u8 )( (( r_BATTERY_val - LOFF_LOW_POWER_OFF ) * 100) / ( LOFF_FULL_POWER - LOFF_LOW_POWER_OFF ) );
//     }
//     // else if(r_BATTERY_val<=BATTERY_PARAT.VBatVolt)
//     // {
//     //     BATTERY_val=( u8 )( (( r_BATTERY_val - LOFF_LOW_POWER_OFF ) * 100 )/ ( LOFF_FULL_POWER - LOFF_LOW_POWER_OFF ) );
//     // }
//     if(BATTERY_val>100)BATTERY_val=100;
//     return BATTERY_val;
// }



