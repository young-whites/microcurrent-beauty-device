#include "cms32f033.h"
#include "work.h"

#ifndef __Device_Init_H
#define __Device_Init_H 			   


/*用于系统初始化 包括系统时钟 各外设的结构体赋值 还包括delay函数*/
//void delay_init(u8 SYSCLK); 已在device_init()中设置为72M;
//对72M条件下,nms<=1864; 
void Dveice_Init(u32 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);

//void MCU_TIM_Init(void);
//void TIM2_Configuration(void);
//void TIM1_PWMShiftInit(void);
//void TIM3_PWMShiftInit(void);
//void TIM4_PWMShiftInit(void);
//void RCC_Configuration(void);
//void GPIO_Configuration(void);
//void NVIC_Configuration(void);
//void SoftWarePara_Init(void);

//void Mode_Shake_Ctrl(u8 enable,u8 Position);
//void Mode_WarmUp_Ctrl(u8 enable,u8 Position);
//void Mode_PowerHit_Ctrl(u8 enable,u8 Position);
//void Mode_Atom_Ctrl(u8 enable,u8 Position);
void System_Device_Lowpower(void);
void System_Device_Init(void);
void SoftWarePara_Reset(void);
void init_mode(void);
void Power_Off(void);
void EPWM_OutPut_IndependentMode(void);

#endif





























