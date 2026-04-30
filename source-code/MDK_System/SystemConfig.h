#ifndef __SYSTEMCONFIG_H
#define __SYSTEMCONFIG_H
#include "sys.h"









void SYS_ProtectEnable(void);
void SYS_ProtectDisable(void);
void SYS_Config(void);
void SysTick_Config(void);
void All_GPIO_NVIC_Config(void);
void HWDIV_Open(void);
void HWDIV_Close(void);












#endif /*__SYSTEMCONFIG_H*/
