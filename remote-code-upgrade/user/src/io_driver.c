#include "io_driver.h"

// IO_StatuTypeDef_T GetBatteryIn = READ_BATTERY_DEFAULTS;
// #if (DOG_versions==4)
// bsp_LED bsp_LED_t;
// #endif

// /*IO clock config*/
// /*******************************************************************************
// * Function Name  : IO_Init
// * Description    : initialization IO port and config IO Port clock
// * Input          : None
// * Output         : None
// * Return         : None
// * Author         :baolin.yang
// *******************************************************************************/
// void bsp_IO_Init(void)
// {
//     GPIO_ConfigRunMode(GPIO_PORT_IN,GPIO_PIN_IN,GPIO_MODE_INPUT_WITH_PULL_UP,RESET);//充电检测引脚输入  1
// 	  GPIO_DISABLE_LOW_CURRENT(GPIO_PORT_IN,GPIO_PIN_IN);
	
//     GPIO_ConfigRunMode(GPIO_PORT_OUT, GPIO_PIN_OUT,GPIO_MODE_OUTPUT,RESET);//充电检测输出  0
// 	  GPIO_DISABLE_LOW_CURRENT(GPIO_PORT_OUT,GPIO_PIN_OUT);
	
//     GPIO_ConfigRunMode(GPIO_PORT_DIN, GPIO_PIN_DIN,GPIO_MODE_OUTPUT,SET);//1650数据线   1
// 	  GPIO_DISABLE_LOW_CURRENT(GPIO_PORT_DIN,GPIO_PIN_DIN);
	
   
//     #if(tm1650_mode==2)
//     GPIO_ConfigRunMode(GPIO_Display_PORT,GPIO_Display_PIN,GPIO_MODE_OUTPUT,RESET);//tm1650电源打开
//     LED_Display_ON //先打开屏幕    
//     #endif
//     #if(DOG_versions==1||DOG_versions==2)
    // GPIO_ConfigRunMode(GPIO_PORT_USB_DET,GPIO_PIN_USB_DET,GPIO_MODE_INPUT_WITH_PULL_DOWN,RESET);//电源线插入io  0
// 	GPIO_DISABLE_LOW_CURRENT(GPIO_PORT_USB_DET,GPIO_PIN_USB_DET);
//     GPIO_ConfigRunMode(GPIO_PORT_USB_DET, GPIO_PIN_USB_DET, GPIO_MODE_INPUT_WITH_PULL_DOWN,RESET);    /*3 USB 插入检测检测外部中断模式*/
//     GPIO_EnableInt( GPIO_PORT_USB_DET, GPIO_PIN_USB_DET,GPIO_INT_EDGE_RISING );
//     GPIO_EnableFilter(GPIO_PORT_USB_DET,GPIO_PIN_USB_DET,GPIO_FILCLK_DIV_14);	
//     GPIO_ClearIntFlag(GPIO_PORT_USB_DET,GPIO_PIN_USB_DET_NO_MSK);
// 	NVIC_EnableIRQ(GPIO3_IRQn);
// 	NVIC_SetPriority(GPIO3_IRQn,3);			/* 0~3, 0??, 3??*/	
//     #elif (DOG_versions==3)
//     // GPIO_ConfigRunMode(GPIO_PORT_USB_DET, GPIO_PIN_USB_DET,GPIO_MODE_INPUT_WITH_PULL_DOWN,RESET);//电源线插入io  0
//     GPIO_DISABLE_LOW_CURRENT(GPIO_PORT_USB_DET,GPIO_PIN_USB_DET);
//     GPIO_ConfigRunMode(GPIO_PORT_USB_DET, GPIO_PIN_USB_DET, GPIO_MODE_INPUT_WITH_PULL_DOWN,RESET);    /*3 USB 插入检测检测外部中断模式*/
//     GPIO_EnableInt( GPIO_PORT_USB_DET, GPIO_PIN_USB_DET,GPIO_INT_EDGE_RISING );
//     GPIO_EnableFilter(GPIO_PORT_USB_DET,GPIO_PIN_USB_DET,GPIO_FILCLK_DIV_14);	
//     GPIO_ClearIntFlag(GPIO_PORT_USB_DET,GPIO_PIN_USB_DET_NO_MSK);
// 	NVIC_EnableIRQ(GPIO3_IRQn);
// 	NVIC_SetPriority(GPIO3_IRQn,3);			/* 0~3, 0??, 3??*/	
    
//     GPIO_ConfigRunMode(GPIO_PORT_Flashlight,GPIO_PIN_Flashlight,GPIO_MODE_OUTPUT,RESET);//手电筒输出低
//     GPIO_ConfigRunMode(GPIO_Display_PORT,GPIO_Display_PIN,GPIO_MODE_OUTPUT,SET);//tm1650电源打开
//     #elif (DOG_versions==4)
//     // GPIO_ConfigRunMode(GPIO_PORT_USB_DET, GPIO_PIN_USB_DET,GPIO_MODE_INPUT_WITH_PULL_DOWN,RESET);//电源线插入io  0
// 	GPIO_DISABLE_LOW_CURRENT(GPIO_PORT_USB_DET,GPIO_PIN_USB_DET);
//     GPIO_ConfigRunMode(GPIO_PORT_USB_DET, GPIO_PIN_USB_DET, GPIO_MODE_INPUT_WITH_PULL_DOWN,RESET);    /*3 USB 插入检测检测外部中断模式*/
//     GPIO_EnableInt( GPIO_PORT_USB_DET, GPIO_PIN_USB_DET,GPIO_INT_EDGE_RISING );
//     GPIO_EnableFilter(GPIO_PORT_USB_DET,GPIO_PIN_USB_DET,GPIO_FILCLK_DIV_14);	
//     GPIO_ClearIntFlag(GPIO_PORT_USB_DET,GPIO_PIN_USB_DET_NO_MSK);
// 				NVIC_EnableIRQ(GPIO3_IRQn);
// 				NVIC_SetPriority(GPIO3_IRQn,3);			/* 0~3, 0??, 3??*/	

// 	GPIO_ConfigRunMode(GPIO_PORT_Flashlight,GPIO_PIN_Flashlight,GPIO_MODE_OUTPUT,RESET);//手电筒输出低
// 	GPIO_DISABLE_LOW_CURRENT(GPIO_PORT_Flashlight,GPIO_PIN_Flashlight);

//       #if (Led_synthesis==0)
//     GPIO_ConfigRunMode(GPIO_PORT_RLED,GPIO_PIN_RLED,GPIO_MODE_OUTPUT,SET);
// 	GPIO_DISABLE_LOW_CURRENT(GPIO_PORT_RLED,GPIO_PIN_RLED);

//     GPIO_ConfigRunMode(GPIO_PORT_GLED,GPIO_PIN_GLED,GPIO_MODE_OUTPUT,SET);
// 	GPIO_DISABLE_LOW_CURRENT(GPIO_PORT_GLED,GPIO_PIN_GLED);

//     GPIO_ConfigRunMode(GPIO_PORT_BLED,GPIO_PIN_BLED,GPIO_MODE_OUTPUT,SET);
// 	GPIO_DISABLE_LOW_CURRENT(GPIO_PORT_BLED,GPIO_PIN_BLED);

//     RLED_OFF
//     GLED_OFF
//     BLED_OFF
//       #elif (Led_synthesis==1)
//     GPIO_ConfigRunMode(GPIO_PORT_RLED,GPIO_PIN_RLED,GPIO_MODE_OUTPUT,SET);
// 	GPIO_DISABLE_LOW_CURRENT(GPIO_PORT_RLED,GPIO_PIN_RLED);

//     GPIO_ConfigRunMode(GPIO_PORT_GLED,GPIO_PIN_GLED,GPIO_MODE_OUTPUT,SET);
// 	GPIO_DISABLE_LOW_CURRENT(GPIO_PORT_GLED,GPIO_PIN_GLED);

//     RLED_OFF
//     GLED_OFF
//       #endif
//     #endif
// }

/*******************************************************************************
* Function Name  : IO_SetOrReset
* Description    : IO set or reset
* Input          : eIOPort :eFan1---eMotorDIR_R, eIOState:SET or RESET
* Output         : None
* Return         : None
* Author         :baolin.yang
*******************************************************************************/
// void IO_SetOrReset(ePeripheralDriver eIOPort, uint8_t eIOState)
// {
//     switch(eIOPort)
//     {
//         case CTRL_BatteryOut:
//             if(eIOState == SET)
//             {
//                 GPIO_SET_PIN(GPIO_PORT_OUT, GPIO_PIN_OUT);
//             }
//             else
//             {
//                 GPIO_RESET_PIN(GPIO_PORT_OUT, GPIO_PIN_OUT);
//             }
//             break;
//         case CTRL_LED_DIN:
//             if(eIOState == SET)
//             {
//                 GPIO_SET_PIN(GPIO_PORT_DIN, GPIO_PIN_DIN);
//             }
//             else
//             {
//                 GPIO_RESET_PIN(GPIO_PORT_DIN, GPIO_PIN_DIN);
//             }
//             break;
//         #if (DOG_versions==4)
//         case CTRL_RLED_DIN: //红灯控制
//             if(eIOState == SET)
//             {
//                 RLED_ON
//             }
//             else
//             {
//                 RLED_OFF
//                 bsp_LED_t.LED_sta = 0;
//                 bsp_LED_t.LED_mode = 0;
//             }        
//         break;
//         case CTRL_GLED_DIN: //绿灯控制
//             if(eIOState == SET)
//             {
//                 GLED_ON
//             }
//             else
//             {
//                 GLED_OFF
//                 bsp_LED_t.LED_sta = 0;
//                 bsp_LED_t.LED_mode = 0;
//             }        
//         break;
//         case CTRL_BLED_DIN: //蓝灯控制
//             if(eIOState == SET)
//             {
//                 BLED_ON
//             }
//             else
//             {
//                 BLED_OFF
//                 bsp_LED_t.LED_sta = 0;
//                 bsp_LED_t.LED_mode = 0;
//             }        
//         break;
//         case CTRL_RGLED_DIN: //两色灯红灯-绿灯控制
//             if(eIOState == SET)
//             {
//                 RLED_ON
//                 GLED_ON
//             }
//             else
//             {
//                 RLED_OFF
//                 GLED_OFF
//                 bsp_LED_t.LED_sta = 0;
//                 bsp_LED_t.LED_mode = 0;
//             }        
//         break;
//         #endif
//         default:
//             break;
//     }
// }

// /*******************************************************************************
// * Function Name  : IO_ReadStatu
// * Description    : read GPIO input statu
// * Input          : eIOPort:GPIO number
// * Output         : temp_statu: HIGH OR LOW
// * Return         : None
// * Author         :baolin.yang
// *******************************************************************************/
// uint8_t IO_ReadStatu(eInputDetectPort eIOPort)
// {
//     uint8_t temp_statu = 0;

//     switch (eIOPort)
//     {
//         case READ_BATTERY_IN:
           
//             if(GPIO_GET_INPUT_PIN(GPIO_PORT_IN,GPIO_PIN_IN_NO_MSK)==0)
//             {
//                temp_statu =  0 ;
//             }
//             else {
//                temp_statu =  1;
//             }
//             break;
      
//         default:
//             break;
//     }

//     return temp_statu;
// }
// /*******************************************************************************
// * Function Name  : IO_GetIOStatu
// * Description    : filter
// * Input          : *pIO : read GPIO relevant variable
// * Output         : SET OR RESET
// * Return         : None
// * Author         :baolin.yang
// *******************************************************************************/
// uint8_t IO_GetIOStatu(IO_StatuTypeDef_T *pIO)
// {
//     pIO->IO_GetStatu = pIO->ReadIOStatu(pIO->IO_ID);
//     if(pIO->IO_Statu != pIO->IO_GetStatu)
//     {
//         if(pIO->IO_StatuFlag == SET)
//         {
//             pIO->IO_Statu = pIO->IO_GetStatu;
//             pIO->IO_StatuFlag = RESET;
//         }
//         else
//         {
//             pIO->IO_StatuFlag = SET;
//         }
//     }
//     else
//     {
//         pIO->IO_StatuFlag = RESET;
//     }
//     return pIO->IO_Statu;
// }
// #if (DOG_versions==4)
// void LED_srtuct_Giv(u8 sta,u8 cnt,u8 mode)
// {
//     bsp_LED_t.LED_sta = sta;
//     bsp_LED_t.LED_time = cnt;
//     bsp_LED_t.LED_mode = mode;
// }
// void LED_display_handler(void)
// {
//     switch (bsp_LED_t.LED_sta)
//     {
//     case 0x01://RLED
//     IO_SetOrReset(CTRL_RLED_DIN,SET);
//     bsp_LED_t.LED_sta = 2;
//         /* code */
//     break;
//     case 0x03://GLED
//     IO_SetOrReset(CTRL_GLED_DIN,SET);
//     bsp_LED_t.LED_sta = 4;
//     break;
//     case 0x05://YLED
//     IO_SetOrReset(CTRL_BLED_DIN,SET);
//     bsp_LED_t.LED_sta = 6;
//     break;
//     case 0x07://RGLED
//     IO_SetOrReset(CTRL_RGLED_DIN,SET);
//     bsp_LED_t.LED_sta = 8;
//     break;
//     default:
//     break;
//     }
//   if (bsp_LED_t.LED_sta>0&&bsp_LED_t.LED_mode==0)
//   {
//       if(bsp_LED_t.LED_time>0)
//       {
//           bsp_LED_t.LED_time --;
//             if(bsp_LED_t.LED_time==0)
//             {
//                 RLED_OFF  GLED_OFF   BLED_OFF
//                 bsp_LED_t.LED_sta = 0;
//             }
//       }

//   }
// }
// #endif
