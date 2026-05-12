#ifndef __EPWM_H
#define __EPWM_H
#include "sys.h"


extern INT32U Water_PwmFreq[6];
extern INT8U Water_PwmDiv[6];


void EPWM_Config_Independent_Mode(uint32_t chMask, INT32U Freq[6], INT8U Div[6]);

/**
  * @brief  Dynamically adjust duty cycle for a single EPWM channel (center-aligned).
  * @param  ch_idx: channel index (0~5)
  * @param  duty: duty cycle percentage (0~100)
  * @retval None
  */
void EPWM_SetDutyPercent(uint8_t ch_idx, uint8_t duty);

/**
  * @brief  Read current period value for a given EPWM channel.
  * @param  ch_idx: channel index (0~5)
  * @retval period value (half-period for center-aligned mode)
  */
uint32_t EPWM_ReadPeriod(uint8_t ch_idx);

#endif /*__EPWM_H*/
