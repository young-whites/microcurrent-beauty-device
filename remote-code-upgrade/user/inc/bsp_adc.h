#ifndef __BSP_ADC1_H_
#define __BSP_ADC1_H_

#include "work.h"
u16 ADC_ConverStable(unsigned char channel);
extern void ADC_InitConfig(void);
extern double KalmanFilter1(const double ResrcData,double ProcessNiose_Q,double MeasureNoise_R);//卡尔曼滤波

#endif
