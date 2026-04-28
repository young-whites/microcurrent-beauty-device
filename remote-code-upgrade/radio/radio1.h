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
 * @file    radio.h
 * @brief   Generic radio handlers
 *
 * @version 1.2
 * @date    Jul 17 2017
 * @author  CMOSTEK R@D
 */
 
#ifndef __RADIO1_H
#define __RADIO1_H

#include "cmt2300a.h"
#define rf_test 0
#ifdef __cplusplus 
extern "C" { 
#endif


/* RF state machine */
typedef enum {
    RF_STATE_IDLE = 0,
    RF_STATE_RX_START,
    RF_STATE_RX_WAIT,
    RF_STATE_RX_DONE,
    RF_STATE_RX_TIMEOUT,
    RF_STATE_TX_START,
    RF_STATE_TX_WAIT,
    RF_STATE_TX_DONE,
    RF_STATE_TX_TIMEOUT,
    RF_STATE_ERROR,
    RF_STATE_TX_INIT,
} EnumRFStatus;

/* RF process function results */
typedef enum {
    RF_IDLE = 0,
    RF_BUSY,
    RF_RX_DONE,
    RF_RXO_TIMEOUT,
    RF_TX_DONE,
    RF_TXO_TIMEOUT,
    RF_ERROR,
} EnumRFResult;
#define RF_PACKET_SIZE 20 
typedef struct 
{
volatile u32 g_nSysTickCount;
#if (RF==0)
volatile u32 g_nRfTxtimeoutCount;
volatile u8  g_nRfTxtimeoutCount_flag;
 u8 g_txBuffer[RF_PACKET_SIZE];   /* RF Tx buffer */
 u8 tx_timer_cnt;
 u8 tx_timer_flag;
#elif (RF==1)

volatile u32 g_nRfRxtimeoutCount;
volatile u8  g_nRfRxtimeoutCount_flag;
 u8 g_rxBuffer[RF_PACKET_SIZE];   /* RF Rx buffer */

#endif
 volatile u32 g_nRecvCount;
 EnumRFStatus g_nNextRFState;
 EnumRFResult Real_status;
 volatile u8  RF_Get_status_tag;
 volatile u16 RF_Get_tag_cnt;
          u8 GET_flag_sta;
}CMT_RF;
typedef struct 
{
    u8  TIME_Flag;
    u16 TIME_Cnt;
}TIME_IT;

//#define ENABLE_ANTENNA_SWITCH //�������ӿ���

void RF_Init(void);
void RF_Config(void);
void RF_TEST(void);
// void RF_TEST(void);
// void RF_StartTx(void);

EnumRFResult RF_transmission(void);
void Delay_us11(u16 cnt);
void Delay_ms11(u16 cnt);



void RF_Receive_time_process(void);

void TIME_Delay_ms(u16 cnt);
//数据打包发送
void RF_SendPacket(uint8_t KeyCode, uint8_t *DataPacket, uint8_t GradeValue);
void HAL_Delay_nMS( uint32_t Delay );
void CMT_EnterPairModeIndicate(void);
void Chip_error_handling_mechanism(void);
#ifdef __cplusplus 
} 
#endif
extern CMT_RF CMT_RF_t;
extern TIME_IT TIME_IT_t;
#endif
