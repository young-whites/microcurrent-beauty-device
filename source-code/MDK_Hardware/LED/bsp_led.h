#ifndef __BSP_LED_H
#define __BSP_LED_H
#include "sys.h"







#define			LED_ENABLE				1
#define			LED_DISABLE				0



//------------------------------------------------------------------------
#define		LED_NUM			(10)				// LED数量

/*LED名称类型*/
typedef	enum
{
	LED_Name_FunctionRun = (0x01),		// 系统连接成功指示		LED名称值
	LED_Name_MessageSuccessful,			// 通讯成功指示灯		LED名称值
	LED_Name_Retain,					// 预留的LED指示灯		LED名称值
	LED_Name_EMSLED,					// 热电手柄的LED灯		LED名称值
	LED_Name_HOTLED,					// 制冷热手柄的LED灯	LED名称值
	LED_Name_PowerKeyOn,				// 电源按键的LED灯开	LED名称值
	LED_Name_PowerKeyOff,				// 电源按键的LED灯关	LED名称值
}LED_Name_TypeDef;
//------------------------------------------------------------------------





void LED_GPIO_Config(void);
void LED_Off(INT8U ledName);
void LED_On(INT8U ledName);
void LED_Toggle(INT8U ledName);
void LED_Grad(INT8U ledName);
void LED_BlinkSetCycleDuty(INT8U ledName, INT8U Cycle, INT8U Duty);
void LED_Blink(INT8U ledName, INT8U cry, INT8U mute, INT8U repeat);
void LED_Fancy(INT8U mode);
void LED_DrvScan(void);
void LED_Init(void);
void LED_Out(INT8U ledName, INT8U ledState);
INT8U LED_GetNumber(void);













#endif /*__BSP_LED_H*/
