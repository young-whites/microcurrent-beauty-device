#ifndef __BSP_TIMER_H_
#define __BSP_TIMER_H_
#include "timer.h"
#include "work.h"

extern volatile u16 SysMsCntForDela1;
extern volatile u32 SysMsCntForDela2;

void SetLongDelay2( u32 TimeMs );
u8 TimerOutLongDelay2( void );



#define timer0_mode 0 //0 16位模式  1 32位模式
#define timer1_mode 0 //0 16位模式  1 32位模式
void Timer0_init(void);
#endif

