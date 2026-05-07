#ifndef __BSP_BEEP_H
#define __BSP_BEEP_H
#include "sys.h"



#define		BEEP_GPIO					GPIO0
#define 	BEEP_PIN					GPIO_PIN_5
#define 	BEEP_PIN_MSK			GPIO_PIN_5_MSK



void BEEP_GPIO_Config(void);
void BEEP_Off(void);
void BEEP_On(void);
void BEEP_SetCycleDuty(INT16U Cycle, INT16U Duty);
void BEEP_Blink(INT8U cry, INT8U mute, INT8U repeat);
void BEEP_Init(void);
void BEEP_DrvScan(void);


void BEEP_ShortPress(void);
void BEEP_LongPress(void);
void BEEP_Warn(void);
void BEEP_WarnOff(void);
void BEEP_OutOff(void);
void BEEP_OutOn(void);













#endif /*__BSP_BEEP_H*/
