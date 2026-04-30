#ifndef __OLDTEST_H
#define __OLDTEST_H
#include "sys.h"










void Old_Test(void);


/* 散热控制使能引脚初始化 */
void HeatDissipation_GPIO_Config(void);

/* 开启散热（使能高电平） */
void HeatDissipation_On(void);

/* 关闭散热（关闭低电平） */
void HeatDissipation_Off(void);










#endif /*__OLDTEST_H*/
