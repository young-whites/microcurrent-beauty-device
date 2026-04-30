#ifndef __SNECTRL_H
#define __SNECTRL_H


#include "sys.h"








void SNE_Current_GPIO_Config(void);
void SNE_ChannelA_On(void);
void SNE_ChannelB_On(void);
void SNE_ChannelAB_Off(void);
void AutoSet_SNEStreng(uint32_t CCPn, uint32_t CCPChannel, uint16_t Streng);












#endif /*__SNECTRL_H*/
