#ifndef __ADC_H
#define __ADC_H
#include "sys.h"








void ADC_Config(uint32_t chMask, uint32_t ADC_CHx);
uint32_t ADC_Read(uint32_t ch);
void ADC_Init(uint32_t chMask);
void ADC_DrvScan(void);







#endif /*__ADC_H*/
