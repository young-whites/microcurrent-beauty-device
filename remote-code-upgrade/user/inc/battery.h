#ifndef __BSP_BATT_H_
#define __BSP_BATT_H_

#include "work.h"

typedef enum
{
    CHARING,//正在充电
    STDBY,//已充满
    NO_CHARGE,
    
} eBatteryStatuIndecate;

typedef enum
{
    FULL,//正常电量
    bat_low,//低电量
    bat_maxlow,//极低电量
    
} eBattery_Ah;

typedef struct
{
    eBatteryStatuIndecate ChargeStatu;
    //eBatteryStatuIndecate Capacity;
	//eBatteryStatuIndecate CapacityPrevStatu;
	
	//eBatteryStatuIndecate USB_InsertStatu;
	//uint16_t SmapleTimes;
	//eBatteryStatuIndecate PrevCapacityStatu;
	//FlagStatus USB_InsetNoUpdateLED;/*USB充电在低功耗唤醒时，禁止更新LED，防止充电指示灯外其他的灯一下，再灭掉*/
} BAT_StatuTypeDef_T;

extern BAT_StatuTypeDef_T BatteryStatu;

// extern BAT_StatuTypeDef_T BatteryStatu;

void battery(void);

extern u8 Battery_Ah_flg;

// //充电ing检测
#define IO_NCHGR_Status()          GPIO_GET_INPUT_PIN(GPIO3,GPIO_PIN_2)
// //充满检测
#define IO_NSTDBY_Status()         GPIO_GET_INPUT_PIN(GPIO3,GPIO_PIN_4)

#endif
