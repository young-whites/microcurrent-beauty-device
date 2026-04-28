#ifndef __EMSCTRL_H
#define __EMSCTRL_H
#include "sys.h"







void EMS_GPIO_Config(void);
void Set_EMSIonStreng(uint8_t* Streng);
void EMS_Wave_1(void);
void EMS_Wave_1_Start(void);
void EMS_Wave_1_Stop(void);
void EMS_Wave_6(void);
void EMS_Wave_3(void);
void EMS_Wave_3_6_Start(void);
void EMS_Wave_3_6_Stop(void);
void Refr_EMSIonStreng(INT8U Streng);






#endif /* __EMSCTRL_H */