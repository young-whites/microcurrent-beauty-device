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
 * @file    cmt2300a_hal.h
 * @brief   CMT2300A hardware abstraction layer
 *
 * @version 1.2
 * @date    Jul 17 2017
 * @author  CMOSTEK R@D
 */
 
#ifndef __CMT2300A_HAL_H
#define __CMT2300A_HAL_H
//#include "sys_config.h"
//#include "radio1.h"
//#include "key.h"
//#include "gpio.h"
#include "work.h"

#ifdef __cplusplus 
extern "C" { 
#endif
#if(DOG_versions==1||DOG_versions==2)
#define CMT_GPIO1_Port  GPIO0
#define CMT_GPIO2_Port  GPIO0
#define CMT_GPIO3_Port  GPIO0

#define CMT_GPIO1_PIN  GPIO_PIN_1_MSK
#define CMT_GPIO2_PIN  GPIO_PIN_1_MSK
#define CMT_GPIO3_PIN  GPIO_PIN_1_MSK

#define CMT_GPIO1_PIN_NO_MSK  GPIO_PIN_1
#define CMT_GPIO2_PIN_NO_MSK  GPIO_PIN_1
#define CMT_GPIO3_PIN_NO_MSK  GPIO_PIN_1
/* ************************************************************************
*  The following need to be modified by user   
*  ************************************************************************ */
#define CMT2300A_SetGpio1In()           GPIO_ConfigRunMode(CMT_GPIO1_Port,CMT_GPIO1_PIN,GPIO_MODE_INPUT_WITH_PULL_DOWN,RESET)
#define CMT2300A_SetGpio2In()           GPIO_ConfigRunMode(CMT_GPIO2_Port,CMT_GPIO2_PIN,GPIO_MODE_INPUT_WITH_PULL_DOWN,RESET)
#define CMT2300A_SetGpio3In()           GPIO_ConfigRunMode(CMT_GPIO3_Port,CMT_GPIO3_PIN,GPIO_MODE_INPUT_WITH_PULL_DOWN,RESET)
#define CMT2300A_ReadGpio1()            GPIO_GET_INPUT_PIN(CMT_GPIO1_Port,CMT_GPIO1_PIN_NO_MSK)
#define CMT2300A_ReadGpio2()            GPIO_GET_INPUT_PIN(CMT_GPIO2_Port,CMT_GPIO2_PIN_NO_MSK)
#define CMT2300A_ReadGpio3()            GPIO_GET_INPUT_PIN(CMT_GPIO3_Port,CMT_GPIO3_PIN_NO_MSK)
#define CMT2300A_DelayMs(ms)            Delay_ms11(ms)
#define CMT2300A_DelayUs(us)            Delay_us11(us)
#define CMT2300A_GetTickCount()         CMT_RF_t.g_nSysTickCount   
/* ************************************************************************ */
#elif(DOG_versions==3||DOG_versions==4)

#define CMT_GPIO1_Port  GPIO0
#define CMT_GPIO2_Port  GPIO0
#define CMT_GPIO3_Port  GPIO0

#define CMT_GPIO1_PIN  GPIO_PIN_1_MSK
#define CMT_GPIO2_PIN  GPIO_PIN_1_MSK
#define CMT_GPIO3_PIN  GPIO_PIN_1_MSK

#define CMT_GPIO1_PIN_NO_MSK  GPIO_PIN_1
#define CMT_GPIO2_PIN_NO_MSK  GPIO_PIN_1
#define CMT_GPIO3_PIN_NO_MSK  GPIO_PIN_1
/* ************************************************************************
*  The following need to be modified by user   
*  ************************************************************************ */
#define CMT2300A_SetGpio1In()           GPIO_ConfigRunMode(CMT_GPIO1_Port,CMT_GPIO1_PIN,GPIO_MODE_INPUT_WITH_PULL_DOWN,RESET)
#define CMT2300A_SetGpio2In()           GPIO_ConfigRunMode(CMT_GPIO2_Port,CMT_GPIO2_PIN,GPIO_MODE_INPUT_WITH_PULL_DOWN,RESET)
#define CMT2300A_SetGpio3In()           GPIO_ConfigRunMode(CMT_GPIO3_Port,CMT_GPIO3_PIN,GPIO_MODE_INPUT_WITH_PULL_DOWN,RESET)
#define CMT2300A_ReadGpio1()            GPIO_GET_INPUT_PIN(CMT_GPIO1_Port,CMT_GPIO1_PIN_NO_MSK)
#define CMT2300A_ReadGpio2()            GPIO_GET_INPUT_PIN(CMT_GPIO2_Port,CMT_GPIO2_PIN_NO_MSK)
#define CMT2300A_ReadGpio3()            GPIO_GET_INPUT_PIN(CMT_GPIO3_Port,CMT_GPIO3_PIN_NO_MSK)
#define CMT2300A_DelayMs(ms)            Delay_ms11(ms)
#define CMT2300A_DelayUs(us)            Delay_us11(us)
#define CMT2300A_GetTickCount()         CMT_RF_t.g_nSysTickCount   
/* ************************************************************************ */
#endif



void CMT2300A_InitGpio(void);

u8 CMT2300A_ReadReg(u8 addr);
void CMT2300A_WriteReg(u8 addr, u8 dat);

void CMT2300A_ReadFifo(u8 buf[], u16 len);
void CMT2300A_WriteFifo(const u8 buf[], u16 len);
#ifdef __cplusplus 
} 
#endif

#endif
