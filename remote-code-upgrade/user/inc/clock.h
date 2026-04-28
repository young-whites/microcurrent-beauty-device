#ifndef __CLOCK_H_
#define __CLOCK_H_
#include "cms32f033.h"


void Delay_Us (uint32_t delay);
void Delay_Ms(uint32_t delay );
void HAL_Delay_nMS( uint32_t Delay );
void CLOCK_Init(void);
void Systick_Config(void);
extern volatile  uint32_t TickCounter;
#define HAL_GetTick()  TickCounter

#endif
