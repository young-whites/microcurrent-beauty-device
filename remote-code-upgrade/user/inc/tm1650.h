#ifndef __TM1650_H_
#define __TM1650_H_
#include "sys_config.h"
#define DIG_1  0X68
#define DIG_2  0X6A
#define DIG_3  0X6C
#define DIG_4  0X6E

#define SYSTEM_PARAMETER_CMD  0X48
#define KEY_PARAMETER_CMD     0X4F

#if (DOG_versions==1||DOG_versions==2)
#define GPIO_PORT_SCL    GPIO4
#define GPIO_PIN_SCL     GPIO_PIN_4_MSK
#define GPIO_PIN_SCL_NO_MSK  GPIO_PIN_4

#define GPIO_PORT_SDA    GPIO3
#define GPIO_PIN_SDA     GPIO_PIN_0_MSK
#define GPIO_PIN_SDA_NO_MSK  GPIO_PIN_0


#define SDA_OUT_HIGH      GPIO_SET_PIN(GPIO_PORT_SDA,GPIO_PIN_SDA)
#define SDA_OUT_LOW       GPIO_RESET_PIN(GPIO_PORT_SDA,GPIO_PIN_SDA)

#define SCL_OUT_HIGH      GPIO_SET_PIN(GPIO_PORT_SCL,GPIO_PIN_SCL)
#define SCL_OUT_LOW       GPIO_RESET_PIN(GPIO_PORT_SCL,GPIO_PIN_SCL)

#define SDA_OUT        GPIO_ConfigRunMode(GPIO_PORT_SDA, GPIO_PIN_SDA, GPIO_MODE_OUTPUT,SET)
#define SDA_IN         GPIO_ConfigRunMode(GPIO_PORT_SDA, GPIO_PIN_SDA, GPIO_MODE_INPUT_WITH_PULL_DOWN,RESET)
#elif (DOG_versions==3||DOG_versions==4)
#define GPIO_PORT_SCL    GPIO4
#define GPIO_PIN_SCL     GPIO_PIN_4_MSK
#define GPIO_PIN_SCL_NO_MSK  GPIO_PIN_4

#define GPIO_PORT_SDA    GPIO3
#define GPIO_PIN_SDA     GPIO_PIN_0_MSK
#define GPIO_PIN_SDA_NO_MSK  GPIO_PIN_0


#define SDA_OUT_HIGH      GPIO_SET_PIN(GPIO_PORT_SDA,GPIO_PIN_SDA)
#define SDA_OUT_LOW       GPIO_RESET_PIN(GPIO_PORT_SDA,GPIO_PIN_SDA)

#define SCL_OUT_HIGH      GPIO_SET_PIN(GPIO_PORT_SCL,GPIO_PIN_SCL)
#define SCL_OUT_LOW       GPIO_RESET_PIN(GPIO_PORT_SCL,GPIO_PIN_SCL)

#define SDA_OUT        GPIO_ConfigRunMode(GPIO_PORT_SDA, GPIO_PIN_SDA, GPIO_MODE_OUTPUT,SET)
#define SDA_IN         GPIO_ConfigRunMode(GPIO_PORT_SDA, GPIO_PIN_SDA, GPIO_MODE_INPUT_WITH_PULL_DOWN,RESET)
#endif
typedef enum
{
    ICON_IDLE = (u8)0x00,
    ICON_BEEP,
    ICON_MOTOR,
    ICON_ELECTRIC,
    ICON_NUM_1,
    ICON_NUM_2,
    ICON_NUM_3,
    ICON_CLEAR_DISPLAY,
    ICON_ELECTRIC_GRADE,
    ICON_CLEAR,

    ICON_BATTERY_CHARING,
    ICON_BATTERY_CAPACITY_FULL,
    ICON_BATTERY_CAPACITY_MID,
    ICON_BATTERY_CAPACITY_LOW,
    ICON_BATTERY_CAPACITY_ULTRALOW,
    ICON_BATTERY_IDLE,

	ICON_AWAKEN_TM1650,
	ICON_STANDBY_MODE
} eLED_Display;

//DIG2 0X7F  --电池框
//DIG3 0X30  --满电
//DIG3 0XDF  --中电
//DIG3 0XCF  --低电
//DIG4 0X03  --铃声
//DIG4 0X0C  --电机
//DIG4 0x10  --电击
//DIG3 0x40  --电击
//DIG3 0X01&0XFB  --1
//DIG3 0X04&0XFE  --2
//DIG3 &0XF5  0  电击强度十位写0
//DIG3 |0X0A  1  电击强度十位写1
//DIG1 0x3f   0  电击强度个位0
//DIG1 0x06   0  电击强度个位0
//DIG1 0x5b   0  电击强度个位0
//DIG1 0x4f   0  电击强度个位0
//DIG1 0x66   0  电击强度个位0
//DIG1 0x3f   0  电击强度个位0
//DIG1 0x3f   0  电击强度个位0
typedef struct
{
    eLED_Display LedIconDisplay;
    uint8_t LedElectricShockGrade;
    uint16_t LedBatteryLevelLowProcTimer;
    uint8_t BeepClearDisplayTimer;
    uint8_t MotorClearDisplayTimer;
    uint8_t ElectricShockClearDisplayTimer;
	uint8_t DIG_3_Char;
    uint8_t LedElectricShockGrade_1;
    uint8_t LedElectricShockGrade_2;
     
} LED_MSGTypdef_T;

extern LED_MSGTypdef_T LED_Message;
#define LED_MSG_DEFAULTS {ICON_IDLE,0x01,0X00,0X00,0x00,0x00,0x01,0x01}

void TM1650_Display(eLED_Display *IconDisplay, uint8_t DisplayChar);

void TM1650_EnterStandbyMode(void);
void TM1650_Init(void);
void TM1650_IIC_Init(void);
void TM1650_CharingIndicate(eBatteryStatuIndecate CharingStatu, uint16_t IntervalTime);
void TM1650_GPIO_rest(void);
void TM1650_old_test(void);
#define tm1650_on TM1650_DataUpdate(SYSTEM_PARAMETER_CMD,0x01);

#endif
