#ifndef __BSP_HARD_H
#define __BSP_HARD_H
#include "sys.h"

/* Heat dissipation fan control (P25) */
void HeatDissipation_GPIO_Config(void);
void HeatDissipation_On(void);
void HeatDissipation_Off(void);

/* Cooling pad GPIO init (P30 as CCP0A) */
void Cold_Enable_GPIO_Config(void);

/* Cooling pad power control */
void Cooling_Init(void);
void Cooling_SetPower(uint8_t duty);
void Cooling_Off(void);

#endif /* __BSP_HARD_H */
