#ifndef __BSP_KEY_H
#define __BSP_KEY_H
#include "sys.h"


/****************************** KEY_Flush 引脚配置参数定义***************************************/
#define             macKEY_Power_GPIO_PORT                       GPIO2
#define             macKEY_Power_GPIO_PIN                        GPIO_PIN_2
#define             macKEY_Power_Pin_MSK                         GPIO_PIN_2_MSK







//------------------------------------------------------------------------
typedef	enum {					// 按键事件类型
    KEY_Evt_Press = (0x80),		// 按下
    KEY_Evt_Release = (0x40),	// 松开
    KEY_Evt_Long2S = (0x20),	// 长按2s
    KEY_Evt_Long4S = (0x10)		// 长按4s
}KEY_Evt_TypeDef;
//------------------------------------------------------------------------
#define		KEY_NUM			(4)		// 按键数量
typedef	enum {					    // 键值类型
    KEY_Val_Power = (0x01),		    // 电源 按键键值

}KEY_Val_TypeDef;
//------------------------------------------------------------------------

void    KEY_GPIO_Config(void);
void    KEY_Write(uint8_t value);
uint8_t KEY_Read(void);
void    KEY_DrvScan(void);
void    KEY_Scan(void);

//------------------------------------------------------------------------







#endif /*__BSP_KEY_H*/
