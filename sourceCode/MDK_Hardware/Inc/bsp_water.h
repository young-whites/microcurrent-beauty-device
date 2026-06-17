#ifndef __BSP_WATER_H
#define __BSP_WATER_H
#include "sys.h"

#define WATER_LEVEL_PIN         GPIO_PIN_3
#define WATER_LEVEL_PORT        GPIO4
#define WATER_LEVEL_GPIO        GPIO4
#define WATER_LEVEL_OK          0   // Low = sufficient water
#define WATER_LEVEL_LOW         1   // High = insufficient water

#define WATER_DEBOUNCE_COUNT    3   // 3 consecutive samples to confirm state change

void WaterLevel_Init(void);
uint8_t WaterLevel_Read(void);
void WaterLevel_Scan(void);
uint8_t WaterLevel_IsOK(void);

#endif /* __BSP_WATER_H */
