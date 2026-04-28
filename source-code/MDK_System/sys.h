#ifndef __SYS_H
#define __SYS_H
#include "cms32f033x.h"


#define TEST						1
#define USART1_GPIO_FF_ENABLE		1
#define EMS_One_Wave_Test			1
#define EMS_HEAT_TEST				0
#define K1_TIMER0					1

/* 自定义头文件 */
#include "bluetooth.h"
#include "bsp_beep.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "SystemConfig.h"
#include "Timer.h"
#include "cms32f033_it.h"
#include "Timing.h"
#include "MyTypedef.h"
#include "usart.h"
#include "ccp.h"
#include "Protocol_app.h"
#include "epwm.h"
#include "sn74hc21d.h"
#include "templist.h"
#include "adc.h"
#include "emsPID.h"
#include "oldTest.h"
#include "flash.h"
#include "k1Ctrl.h"
#include "emsCtrl.h"
#include "eptCtrl.h"
#include "sneCtrl.h"
#include "masCtrl.h"


/* 特殊控制寄存器自定义头文件 */
#include "Psfr_system.h"
#include "Psfr_gpio.h"
#include "Psfr_tick.h"
#include "Psfr_timer.h"
#include "Psfr_hwdiv.h"
#include "Psfr_uart.h"
#include "Psfr_ccp.h"
#include "Psfr_epwm.h"
#include "Psfr_adc.h"
#include "Psfr_fmc.h"

/* 头文件 */
#include	"base_type_arm.h"


void SystemWorkStatusCheck(void);
void SystemParaInit(void);

#endif /*__SYS_H*/
