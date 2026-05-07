#ifndef __MYTYPEDEF_H
#define __MYTYPEDEF_H
#include "sys.h"





/**
  * @brief  用于记录APP下发的数据的类型结构体
  * @param  Flush_IntervalGears [0:4]
  */
typedef struct
{
    uint8_t  PowerKeyCnt;		// 电源按键按下次数
    uint8_t  K1_Switch;         // EMS界面上的按键
    uint8_t  K1IonStrengGears;  // EMS电流强度挡位
	uint8_t  K1ElecMode;
    uint16_t TestPa;
    uint8_t  EMS_HotGears;       // EMS加热的挡位
    uint8_t  EMS_HotCnt;         // EMS加热的计数
    uint16_t OldStartCnt;       // 老化时间程序计数
    uint16_t Duty;          
    uint32_t Period;
    uint8_t  EMSHeatCnt;
    uint8_t  K1_CurrentGears;
    int      K1Compensate;       // K1要补偿的数值
    uint16_t Second;			// K1 波形步调时间(秒)
    uint16_t WaterMs1;
    uint8_t  HandWave;          // 选择波形模式
    uint8_t  EMSIonStrengGears;
}  Record_StructType;
extern   Record_StructType Record;




typedef struct
{
    uint8_t     PowerOn;         // 开始工作标志
    uint8_t     UsartConfig;     // 串口工作状态
    uint8_t     WorkStart;       // 功能运行状态
    uint8_t     update;	         // 升级指令
    uint8_t     OldFlag;         // 老化指令
    uint8_t     cntFlagA;        // 与门A半桥
    uint8_t     cntFlagB;        // 与门B半桥
	uint8_t		BluetoothConnect;// 蓝牙连接状态
    uint8_t     K1_Systick_Start;// 使用滴答定时器的状态标志 
}  Flag_StructType;
extern   Flag_StructType Flag;











#endif /*__MYTYPEDEF_H*/
