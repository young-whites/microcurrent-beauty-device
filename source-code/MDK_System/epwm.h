#ifndef __EPWM_H
#define __EPWM_H
#include "sys.h"





extern INT32U Water_PwmFreq[6];
extern INT8U Water_PwmDiv[6];






void EPWM_Config_Independent_Mode(uint32_t chMask, INT32U Freq[6], INT8U Div[6]);
void EPWM_Config(void);










#endif /*__EPWM_H*/
