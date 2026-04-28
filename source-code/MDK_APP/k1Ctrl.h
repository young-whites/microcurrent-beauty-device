#ifndef __K1CTRL_H
#define __K1CTRL_H
#include "sys.h"




#define		EMS_Current_Handle()	GPIO_SetPin(GPIO0,GPIO_PIN_1_MSK)
#define		K1_Current_Handle()		GPIO_ResetPin(GPIO0,GPIO_PIN_1_MSK)


extern uint16_t   cntIndexForWaterArr_Up;
extern uint16_t   cntIndexForWaterArr_Down;


void K1_Current_GPIO_Config(void);
void Set_WaterGears(INT8U* Gears);
void Choose_WaterARR(void);
void AutoSet_WaterStreng(uint32_t CCPn, uint32_t CCPChannel, uint16_t Streng);
void Choose_WaterARR_Trapezoid(void);
void  CurrentCtrl_Relay_GPIO_Config(void);









#endif /*__K1CTRL_H*/
