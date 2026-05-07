#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"






void TMR0_Config(void);
void TMR1_Config(void);


void Dveice_Init(uint32_t SYSCLK);
void delay_ms(uint16_t nms);








#endif /*__TIMER_H*/
