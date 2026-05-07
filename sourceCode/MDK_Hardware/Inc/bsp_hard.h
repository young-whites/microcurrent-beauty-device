#ifndef __BSP_HARD_H
#define __BSP_HARD_H
#include "sys.h"




/* 散热控制使能引脚初始化 */
void HeatDissipation_GPIO_Config(void);

/* 开启散热（使能高电平） */
void HeatDissipation_On(void);

/* 关闭散热（关闭低电平） */
void HeatDissipation_Off(void);

/* 冷却使能引脚初始化 */
void Cold_Enable_GPIO_Config(void);

#endif /* __BSP_HARD_H */