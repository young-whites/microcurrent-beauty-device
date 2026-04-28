/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: (1) "AS IS" WITH NO WARRANTY; AND
 * (2)TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, CMOSTEK SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * Copyright (C) CMOSTEK SZ.
 */

/*!
 * @file    radio.c
 * @brief   Generic radio handlers
 *
 * @version 1.2
 * @date    Jul 17 2017
 * @author  CMOSTEK R@D
 */
 
#include "radio1.h"
#include "cmt2300a_params.h"
#include "cmt2300a_hal.h"
#include <string.h>
#include "sys_config.h"
#include "key.h"
#include "tm1650.h"
#include "io_driver.h"
#if (RF==1)
CMT_RF CMT_RF_t={0,0,0,{0},0,RF_STATE_RX_WAIT,RF_IDLE,0,0,0};//RF_STATE_IDLE
#elif(RF==0)
CMT_RF CMT_RF_t={0,0,0,{0},0,0,0,RF_STATE_IDLE,RF_IDLE,0,0,0};//RF_STATE_IDLE
#endif
TIME_IT TIME_IT_t;
#define RF_RX_TIMEOUT    600         //600ms


#if(rf_test==1)
u8 codedd[60];
void RF_TEST(void)
{
    u8 i=0;
    for(i=0;i<60;i++)
    codedd[i]=CMT2300A_ReadReg(i);
}
#endif
void RF_Init(void)
{
    u8 tmp;
    
	CMT2300A_InitGpio();
	CMT2300A_Init();
    
    /* Config registers */
    CMT2300A_ConfigRegBank(CMT2300A_CMT_BANK_ADDR       , g_cmt2300aCmtBank       , CMT2300A_CMT_BANK_SIZE       );
    CMT2300A_ConfigRegBank(CMT2300A_SYSTEM_BANK_ADDR    , g_cmt2300aSystemBank    , CMT2300A_SYSTEM_BANK_SIZE    );
    CMT2300A_ConfigRegBank(CMT2300A_FREQUENCY_BANK_ADDR , g_cmt2300aFrequencyBank , CMT2300A_FREQUENCY_BANK_SIZE );
    CMT2300A_ConfigRegBank(CMT2300A_DATA_RATE_BANK_ADDR , g_cmt2300aDataRateBank  , CMT2300A_DATA_RATE_BANK_SIZE );
    CMT2300A_ConfigRegBank(CMT2300A_BASEBAND_BANK_ADDR  , g_cmt2300aBasebandBank  , CMT2300A_BASEBAND_BANK_SIZE  );
    CMT2300A_ConfigRegBank(CMT2300A_TX_BANK_ADDR        , g_cmt2300aTxBank        , CMT2300A_TX_BANK_SIZE        );
    
    // xosc_aac_code[2:0] = 2
    tmp = (~0x07) & CMT2300A_ReadReg(CMT2300A_CUS_CMT10);
    CMT2300A_WriteReg(CMT2300A_CUS_CMT10, tmp|0x02);
    
	RF_Config();
}

void RF_Config(void)
{
	 u8 nInt2Sel,i=10;
#ifdef ENABLE_ANTENNA_SWITCH
    /* If you enable antenna switch, GPIO1/GPIO2 will output RX_ACTIVE/TX_ACTIVE,
       and it can't output INT1/INT2 via GPIO1/GPIO2  如果启用天线开关，GPIO1 / GPIO2将输出RX_ACTIVE / TX_ACTIVE，
        并且无法通过GPIO1 / GPIO2输出INT1 / INT2*/
    CMT2300A_EnableAntennaSwitch(0);
#endif
#if (RF==0)
    
//    for(i=0;i<RF_PACKET_SIZE;i++)
//    {
//      CMT_RF_t.g_txBuffer[i]=1+i;
//    }
	   /* Config GPIOs */
    CMT2300A_ConfigGpio(CMT2300A_GPIO3_SEL_INT2);   /* INT2 > GPIO3 */    
    /* Config interrupt */
    nInt2Sel=	CMT2300A_INT_SEL_TX_DONE; /* Config INT2 */
    nInt2Sel &= CMT2300A_MASK_INT2_SEL;
    nInt2Sel |= (~CMT2300A_MASK_INT2_SEL) & CMT2300A_ReadReg(CMT2300A_CUS_INT2_CTL);
    CMT2300A_WriteReg(CMT2300A_CUS_INT2_CTL, nInt2Sel); 
	
    /* Enable interrupt */
      CMT2300A_EnableInterrupt(
	                              CMT2300A_MASK_TX_DONE_EN 
//															CMT2300A_MASK_PKT_DONE_EN | 
//															CMT2300A_MASK_PREAM_OK_EN |
//															CMT2300A_MASK_SYNC_OK_EN  |
//															CMT2300A_MASK_NODE_OK_EN  |
//															CMT2300A_MASK_CRC_OK_EN   
	                             );

#elif (RF==1)
	  CMT2300A_ConfigGpio( CMT2300A_GPIO1_SEL_INT1|   /* INT1 > GPIO1 */
	                       CMT2300A_GPIO3_SEL_INT2|   /* INT2 > GPIO2 */
                         CMT2300A_GPIO2_SEL_DOUT 	                      
	                      );
   
	  CMT2300A_ConfigInterrupt( CMT2300A_INT_SEL_SYNC_OK,  /* GPIO1 > SYNC_OK */
	                            CMT2300A_INT_SEL_PKT_DONE
                              //|CMT2300A_INT_SEL_PREAM_OK /* GPIO2 > PKT_DONE*/
	                          );
    /* Enable interrupt */
      CMT2300A_EnableInterrupt(
       CMT2300A_MASK_PKT_DONE_EN  //|
        // CMT2300A_MASK_PREAM_OK_EN |
        //  CMT2300A_MASK_SYNC_OK_EN  //|
          // CMT2300A_MASK_SL_TMO_EN  // |
          // CMT2300A_INT_SEL_RSSI_VLD
        //  CMT2300A_MASK_RX_TMO_EN
//        CMT2300A_MASK_NODE_OK_EN  |
//        CMT2300A_MASK_CRC_OK_EN   |
//        CMT2300A_MASK_TX_DONE_EN 
        );
#endif
    /* Disable low frequency OSC calibration 禁用低频OSC校准*/
    //CMT2300A_EnableLfosc(TRUE);//TRUE FALSE
    
    /* Use a single 64-byte FIFO for either Tx or Rx 将单个64字节FIFO用于Tx或Rx*/
    //CMT2300A_EnableFifoMerge(TRUE);
    
    //CMT2300A_SetFifoThreshold(16); // FIFO_TH
    
    /* This is optional, only needed when using Rx fast frequency hopping 这是可选的，仅在使用Rx快速跳频时才需要* /
    /* See AN142 and AN197 for details */
    //CMT2300A_SetAfcOvfTh(0x27);
    
    /* Go to sleep for configuration to take effect  进入睡眠以使配置生效*/
    CMT2300A_GoSleep();

    while(i>0)
    {
       if(CMT2300A_IsExist()) 
        {   
           break;
        }
        else {
           i--;
        }
    }
 if(i==0)
 {
     g_tSystemMsg.CMT_err = SET;
 }

}

 /**
  * @brief  大概微妙延时
  * @param  cnt 延时us长度 最长 65535 us
  * @retval None
  * 0.75mips  750000条指令每秒 一个指令周期为1.333 clock
  * 1/16000000 = 1/16 us
  * 一条时钟为0.0625us 一条指令周期为0.083333us
  * 大概需要12个nop();
  */
 void Delay_us11(u16 cnt) 
{ 
    u16 i=0,j=0;
    for(i=0;i<cnt;i++)
    {
      for(j=0;j<3;j++)
      {
         __NOP();
      }
    }
} 
 /**
  * @brief  大概毫妙延时
  * @param  cnt 延时ms长度 最长 65535 us
  * @retval None
  * 1us大概需要6个nop();
  */

 void Delay_ms11(u16 cnt) 
{ 
  u16 i=0,j=0,k=0;
   for(i=0;i<cnt;i++)
   {
     for(j=0;j<1000;j++)
     {
         for(k=0;k<3;k++)
         {
          __NOP();
         }
     }
   }
}
void TIME_Delay_ms(u16 cnt)
{
   if(TIME_IT_t.TIME_Flag==0)
   {
     TIME_IT_t.TIME_Flag = 0x01;
     TIME_IT_t.TIME_Cnt = cnt;
   }
   while(TIME_IT_t.TIME_Cnt>0);

   TIME_IT_t.TIME_Flag = 0;
   TIME_IT_t.TIME_Cnt = 0;
}
// void RF_TEST(void)
// {
//   u8 i=0;
//   for(i=0;i<CMT2300A_SYSTEM_BANK_SIZE;i++)
//   {
//     CMT_RF_t.g_txBuffer[i]=CMT2300A_ReadReg(CMT2300A_SYSTEM_BANK_ADDR+i);
//   }
// }
// //进入TX状态
// void RF_StartTx(void)
// { 
//     if(CMT_RF_t.g_nRecvCount<2)
//     {
//       CMT_RF_t.g_nNextRFState = RF_STATE_TX_START;
//     }
// }
#if (RF==0)
//射频发送函数-
EnumRFResult RF_transmission(void)
{
    EnumRFResult nRes = RF_BUSY;
  
    switch(CMT_RF_t.g_nNextRFState) 
    {
    case RF_STATE_IDLE:
    {
        nRes = RF_IDLE;
        break;
    }
    case RF_STATE_TX_INIT:
    {
      CMT_RF_t.g_nNextRFState  = RF_STATE_TX_START;
#if(DOG_versions==1||DOG_versions==2||DOG_versions==4||Target_situation==0)
      if(g_tSystemMsg.ReceiverNumber == 0)
      {
          RF_SendPacket(g_tSystemMsg.KeyCode, CMT_RF_t.g_txBuffer, LED_Message.LedElectricShockGrade);
      }
      else 
      {
          RF_SendPacket(g_tSystemMsg.KeyCode,  CMT_RF_t.g_txBuffer, LED_Message.LedElectricShockGrade_1);
      }
#elif(DOG_versions==3&&Target_situation==1)
      if(g_tSystemMsg.ReceiverNumber == 0)
      {
           RF_SendPacket(g_tSystemMsg.KeyCode, CMT_RF_t.g_txBuffer, LED_Message.LedElectricShockGrade);
      }
      else if(g_tSystemMsg.ReceiverNumber == 1)
      {
          RF_SendPacket(g_tSystemMsg.KeyCode,  CMT_RF_t.g_txBuffer, LED_Message.LedElectricShockGrade_1);
      }
      else if(g_tSystemMsg.ReceiverNumber == 2)
      {
          RF_SendPacket(g_tSystemMsg.KeyCode,  CMT_RF_t.g_txBuffer, LED_Message.LedElectricShockGrade_2);
      }
#endif
      break;
    }
    case RF_STATE_TX_START:
    {
        CMT2300A_GoStby();
        CMT2300A_ClearInterruptFlags();
        
        /* Must clear FIFO after enable SPI to read or write the FIFO */
        CMT2300A_EnableWriteFifo();
        CMT2300A_ClearTxFifo();
        /* The length need be smaller than 32 */
        CMT2300A_WriteFifo(CMT_RF_t.g_txBuffer, RF_PACKET_SIZE);
        
        if( 0==(CMT2300A_MASK_TX_FIFO_NMTY_FLG & CMT2300A_ReadReg(CMT2300A_CUS_FIFO_FLAG)) )
             CMT_RF_t.g_nNextRFState = RF_STATE_ERROR;
        if(FALSE==CMT2300A_GoTx())
        {
            CMT_RF_t.g_nNextRFState = RF_STATE_ERROR;
        }
        else {
            CMT_RF_t.g_nNextRFState = RF_STATE_TX_WAIT;      
            CMT_RF_t.g_nRfTxtimeoutCount = 0;
            CMT_RF_t.g_nRfTxtimeoutCount_flag = 1;
        }
        break;
    }      
    case RF_STATE_TX_WAIT:
    {
#ifdef ENABLE_ANTENNA_SWITCH
        if(CMT2300A_MASK_TX_DONE_FLG & CMT2300A_ReadReg(CMT2300A_CUS_INT_CLR1))  /* Read TX_DONE flag */
#else
        if(CMT2300A_ReadGpio3())  /* Read INT1, TX_DONE */
#endif
        {
            CMT_RF_t.g_nRfTxtimeoutCount_flag =0;
            CMT_RF_t.g_nRfTxtimeoutCount =0;   
            CMT_RF_t.g_nRecvCount++;
            if(g_tSystemMsg.rest_flag == 0x01)
            {
                  if(CMT_RF_t.g_nRecvCount<2)
                {
                    CMT_RF_t.g_nNextRFState = RF_STATE_IDLE;
                    CMT_RF_t.tx_timer_flag  = 0x01;
                    CMT_RF_t.tx_timer_cnt   = 0;
                    CMT2300A_ClearInterruptFlags();
                    CMT2300A_GoSleep();
                }
                else {
                    CMT_RF_t.g_nNextRFState = RF_STATE_TX_DONE;
                    CMT_RF_t.g_nRecvCount = 0;
                    CMT_RF_t.tx_timer_cnt = 0;
                    CMT_RF_t.tx_timer_flag  = 0x00;
                    g_tSystemMsg.rest_flag = 0x00;
                }
            }
          else {
                    CMT_RF_t.g_nNextRFState = RF_STATE_TX_DONE;
                    CMT_RF_t.g_nRecvCount = 0;
                    CMT_RF_t.tx_timer_cnt = 0;
            }
        }

          if(CMT_RF_t.g_nRfTxtimeoutCount>RF_RX_TIMEOUT) //根据实际应用可以调整Time Out
		{ 
            CMT_RF_t.g_nNextRFState = RF_STATE_TX_TIMEOUT;
        }
            
        break;
    }
            
    case RF_STATE_TX_DONE:
    {
        CMT2300A_ClearInterruptFlags();
        CMT2300A_GoSleep();
        CMT_RF_t.g_nRecvCount = 0x00;
        CMT_RF_t.g_nNextRFState = RF_STATE_IDLE;//RF_STATE_IDLE
        nRes = RF_TX_DONE;//RF_TX_DONE
        break;
    }
    
    case RF_STATE_TX_TIMEOUT:
    {
        CMT2300A_GoSleep();
        CMT_RF_t.g_nRecvCount = 0x00;
        CMT_RF_t.g_nRfTxtimeoutCount_flag = 0;
        CMT_RF_t.g_nRfTxtimeoutCount = 0;
        CMT_RF_t.g_nNextRFState = RF_STATE_ERROR;
        nRes = RF_TXO_TIMEOUT;
        break;
    }
    case RF_STATE_ERROR:
    {
        CMT_RF_t.g_nRecvCount = 0x00;
        CMT2300A_SoftReset();
        CMT2300A_DelayMs(20);
        RF_Init(); 
        CMT2300A_GoStby();
        CMT2300A_ClearInterruptFlags();        
        /* Must clear FIFO after enable SPI to read or write the FIFO */
        CMT2300A_EnableWriteFifo();
        CMT2300A_ClearTxFifo();
        CMT2300A_GoSleep();   
        CMT2300A_DelayMs(20);     
        CMT_RF_t.g_nNextRFState = RF_STATE_IDLE;
        nRes = RF_ERROR;
        break;
    }
    default:
        break;
    }
    
    return nRes;
}
//数据打包发送
/*
函数名：RF_SendPacket
参数：KeyCode 按键键值-对应实际功能
参数：DataPacket 发送包
参数：GradeValue  对应键值额外帧值
参数：
*/
void RF_SendPacket(uint8_t KeyCode, uint8_t *DataPacket, uint8_t GradeValue)
{
    #if (Shock_limit==1)//0 不合成  1  合成
     u8 linit = 0;
    #endif
    switch (KeyCode)
    {
        case KEY_DOWN_K1:
        case KEY_DOWN_K2:
            DataPacket[13] = '4'; /*ENERGY ADJUST-*/
            DataPacket[14] = GradeValue;
            break;
        case KEY_DOWN_K4://1
            DataPacket[13] = '1'; /*BEEP*/
            DataPacket[14] = 0;

            break;
        case KEY_DOWN_K5://2
            DataPacket[13] = '2'; /*MOTOR*/
            DataPacket[14] = 0;
            break;
        case KEY_DOWN_K6://3
            DataPacket[13] = '3'; /*ElectricShock*/
            #if (Shock_limit==1)//0 不合成  1  合成
            linit = GradeValue;
            if(linit>16)
            linit = 16;
            DataPacket[14] = linit;
            #elif (Shock_limit==0)
            DataPacket[14] = GradeValue;
            #endif
            break;
        case KEY_UP_K6://4
            DataPacket[13] = '5'; /*ElectricShock*/
            DataPacket[14] = 0;
            break;
        case KEY_PAIR_DOWN:
            DataPacket[13] = 'P'; /*PAIR*/
            DataPacket[14] = 'A';
            break;
        case KEY_FACTORY_DOWN:
            DataPacket[13] = 'F'; /*FACTORY*/
            DataPacket[14] = 'Y';
            break;

    }

}
#elif (RF==1)
void RF_MODE_INIT(void)
{
        CMT2300A_GoStby();
        CMT2300A_ClearInterruptFlags();  //清中断
        /* Must clear FIFO after enable SPI to read or write the FIFO */
        CMT2300A_EnableReadFifo();  //使能FIFO
        CMT2300A_ClearRxFifo();     //清楚接收FIFO
        CMT2300A_GoSleep();
}
void bRadio_Process1(void)
{
  //SLP_mode_0
  #if(SLP_MODE==1)
  #elif(SLP_MODE==12)
//   #ifdef ENABLE_ANTENNA_SWITCH
//         if(CMT2300A_MASK_TX_DONE_FLG & CMT2300A_ReadReg(CMT2300A_CUS_INT_CLR1))  /* Read TX_DONE flag */
// #else
//         if(CMT2300A_ReadGpio3()&&CMT_RF_t.g_nNextRFState==RF_STATE_IDLE)  /* Read INT1, TX_DONE -根据中断进入接收*/
// #endif
//      {
//          //CMT2300A_GoStby();
//         // CMT2300A_ClearInterruptFlags();  //清中断
//         // /* Must clear FIFO after enable SPI to read or write the FIFO */
//         // CMT2300A_EnableReadFifo();  //使能FIFO
//         // CMT2300A_ClearRxFifo();     //清楚接收FIFO
        
//         if(FALSE==CMT2300A_GoRx()){  //进入
//             CMT_RF_t.g_nNextRFState = RF_STATE_ERROR;
//         }
//         else {
//            CMT_RF_t.g_nNextRFState = RF_STATE_RX_WAIT;
//             CMT_RF_t.g_nRfRxtimeoutCount_flag = 1;
//             CMT_RF_t.g_nRfRxtimeoutCount = 0 ;
//         }        
//      }
     switch (CMT_RF_t.g_nNextRFState)
     {
     case RF_STATE_RX_WAIT: //接收状态
       /* code */
#ifdef ENABLE_ANTENNA_SWITCH
        if(CMT2300A_MASK_PKT_OK_FLG & CMT2300A_ReadReg(CMT2300A_CUS_INT_FLAG))  /* Read PKT_OK flag */
#else
         if(CMT2300A_ReadGpio3())  /* Read INT2, PKT_OK */
#endif
        {
            CMT_RF_t.g_nSysTickCount = 0;
            //CMT2300A_GoStby();
            CMT_RF_t.g_nNextRFState = RF_STATE_RX_DONE;
            CMT_RF_t.g_nRfRxtimeoutCount=0;  //清除 Time Out计数
            CMT_RF_t.g_nRfRxtimeoutCount_flag = 1;
        }
        if(CMT_RF_t.g_nRfRxtimeoutCount>=20000) //根据实际应用可以调整Time Out
			 {
            CMT_RF_t.g_nRfRxtimeoutCount_flag = 0;
            CMT_RF_t.g_nNextRFState = RF_STATE_RX_TIMEOUT;
       }       
       break;
      case RF_STATE_RX_DONE:
         /* The length need be smaller than 32 */
        CMT2300A_GoStby();
        CMT2300A_ReadFifo(CMT_RF_t.g_rxBuffer, RF_PACKET_SIZE);
        // memcpy(&CMT_RF_t.CMT_Rx_buf[0],CMT_RF_t.g_rxBuffer,RF_PACKET_SIZE);
        // memset(CMT_RF_t.g_rxBuffer,0x00,RF_PACKET_SIZE);
        CMT2300A_ClearInterruptFlags();  //清中断
        CMT2300A_ClearRxFifo();     //清楚接收FIFO    
        CMT2300A_GoSleep();
        CMT_RF_t.g_nRfRxtimeoutCount=0;  //清除 Time Out计数
        CMT_RF_t.g_nRfRxtimeoutCount_flag = 0;
     if(CMT_RF_t.g_rxBuffer[14] == 'P' && CMT_RF_t.g_rxBuffer[15] == 'A')
        {
          if(g_tSystemMsg.PairMode == SET)
            {
                disableInterrupts();
                PAIR_StoreChipIDandReceiverNumber(&CMT_RF_t.g_rxBuffer[1]);//保存新的ID
                PAIR_ReadChipIDandReceiverNumber(CHIP_ID);
                enableInterrupts();
                PAIR_Finished();
              }
        }

            if(CMT_RF_t.g_rxBuffer[14] == 'F' && CMT_RF_t.g_rxBuffer[15] == 'Y')
            {
                if(g_tSystemMsg.PairMode == SET)
                {
                    System_FactoryResetIndicate();
                    WWDG_SWReset();
                }
            }

            if(gSampleCode_StringCompare1(&CMT_RF_t.g_rxBuffer[1], CHIP_ID, sizeof(CHIP_ID)) == TRUE)
            {
                if( g_tSystemMsg.PairMode == RESET)
                {
                    bRadio1_ReceiveHandler();
                }
                g_tSystemMsg.RC_TriggerFlag = RESET;
            }
            else
            {
                if(g_tSystemMsg.RC_TriggerFlag == SET)
                {
                    g_tSystemMsg.AutoStandbyTimerCount = _12_SECOND;
                }
            }
        memset(CMT_RF_t.g_rxBuffer,0x00,RF_PACKET_SIZE); 
        CMT_RF_t.g_nRfRxtimeoutCount=0;
        CMT_RF_t.g_nNextRFState = RF_STATE_RX_WAIT;   //RF_STATE_IDLE    
       break;
      case RF_STATE_RX_TIMEOUT:
        CMT2300A_SoftReset();
        CMT2300A_GoSleep();
        CMT_RF_t.g_nRfRxtimeoutCount = 0;
        CMT_RF_t.g_nNextRFState = RF_STATE_RX_WAIT; //RF_STATE_IDLE
      break;
     default:
       break;
     }
     #elif (SLP_MODE==13)
     #endif

}
/*!
 * This function is used to compare the content of the received packet to a string.
 *
 * @return  None.
 */
BOOL gSampleCode_StringCompare1(uint8_t* pbiPacketContent, uint8_t* pbiString, uint8_t lenght)
{
    while ((*pbiPacketContent++ == *pbiString++) && (lenght > 0))
    {
        if( (--lenght) == 0 )
        {
            return TRUE;
        }
    }

    return FALSE;
}
void bRadio1_ReceiveHandler(void)
{
    if(g_tSystemMsg.FilterRadioCmdTimer)
    {
        memset(&CMT_RF_t.g_rxBuffer[0],0x00,RF_PACKET_SIZE); 
        return;
    }

    switch (CMT_RF_t.g_rxBuffer[14])
    {
        case '1':

            if(++g_tSystemMsg.CmdInterTimeFlag == 1)
            {
                g_tSystemMsg.FilterRadioCmdTimer = CMD_INTERVAL_TIME1;
            }
            else
            {
                g_tSystemMsg.FilterRadioCmdTimer = 51;
            }

            g_tSystemMsg.AutoStandbyTimerCount = 0;
            g_tSystemMsg.BeepTime = 50;
            LED_Message.LED_Ctrl = LED_BEEP_INDICATE;

            break;
        case '2':

            if(++g_tSystemMsg.CmdInterTimeFlag == 1)
            {
                g_tSystemMsg.FilterRadioCmdTimer = CMD_INTERVAL_TIME1;
            }
            else
            {
                g_tSystemMsg.FilterRadioCmdTimer = 91;
            }
            g_tSystemMsg.AutoStandbyTimerCount = 0;
            g_tSystemMsg.MotorRunTime = 90;
            LED_Message.LED_Ctrl = LED_MOTOR_INDICATE;
            break;
        case '3':
			
            //g_tSystemMsg.EnergyGrade = customRadioPacket[14];

            if(++g_tSystemMsg.CmdInterTimeFlag == 1)
            {
                g_tSystemMsg.FilterRadioCmdTimer = CMD_INTERVAL_TIME1;
            }
            else
            {
                g_tSystemMsg.FilterRadioCmdTimer = 36;
            }
            g_tSystemMsg.ElectricShockTime = 35;
            g_tSystemMsg.AutoStandbyTimerCount = 0;
           // System_EnergyAdjust(g_tSystemMsg.EnergyGrade);
           
			      System_EnergyAdjust(CMT_RF_t.g_rxBuffer[15]);
            System_EnergySwitch(ENABLE);
            LED_Message.LED_Ctrl = LED_ELECTRIC_SHOCK_INDICATE;
			
            break;
        case '4':
           // g_tSystemMsg.EnergyGrade = customRadioPacket[14];
            break;

        case '5':
            System_TurnOffAllDevice();
            break;
       default:
       break;
    }


}
#endif
void Chip_error_handling_mechanism(void)
{     
     uint8_t GET_flag = 0;
     CMT_RF_t.GET_flag_sta = CMT2300A_GetChipStatus();
     if(CMT_RF_t.GET_flag_sta>=0x00&&CMT_RF_t.GET_flag_sta<=0x04)
     {
         CMT_RF_t.RF_Get_status_tag = 0x00;
         CMT_RF_t.RF_Get_tag_cnt = 0x00; 
     }
     else if(CMT_RF_t.GET_flag_sta>0x04&&CMT_RF_t.GET_flag_sta<=0x09)
     {
         if(CMT_RF_t.RF_Get_status_tag==0x00)
         {
             CMT_RF_t.RF_Get_status_tag = 0x01;
         }
     }
     
     if(CMT_RF_t.RF_Get_status_tag == 0x01)
     {
         if(CMT_RF_t.RF_Get_tag_cnt>=3000)
         {
            g_tSystemMsg.CMT_err = SET;
            CMT_RF_t.RF_Get_status_tag = 0x00;
            CMT_RF_t.RF_Get_tag_cnt = 0x00; 
         }
     }
}
void CMT_EnterPairModeIndicate(void)
{

    static uint8_t TimeCount = 0;
    static uint16_t TimeCount1 = 0;
    if(g_tSystemMsg.CMT_err == SET)
    {
        TimeCount++;
        TimeCount1++;
        switch (TimeCount)
        {
        case 10:
             LED_Message.LedIconDisplay = ICON_NUM_1;
             TM1650_Display(&LED_Message.LedIconDisplay, 0);            
        break;
        case 20:
             LED_Message.LedIconDisplay = ICON_NUM_2;
             TM1650_Display(&LED_Message.LedIconDisplay, 0);  
             TimeCount = 0 ;          
        break;
        default:
            break;
        }
        if(TimeCount1>=200)
        {
        CMT2300A_SoftReset();
        CMT2300A_DelayMs(20);
        RF_Init(); 
        CMT2300A_GoStby();
        CMT2300A_ClearInterruptFlags();        
        /* Must clear FIFO after enable SPI to read or write the FIFO */
        CMT2300A_EnableWriteFifo();
        CMT2300A_ClearTxFifo();
        CMT2300A_GoSleep();   
        CMT2300A_DelayMs(20);
        TimeCount1 = 0;
        }      
    }
}
