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
    uint16_t OldStartCnt;       // 老化时间程序计数
    uint8_t  CoolingLevel;      // Cooling gear 1~5
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
	uint8_t		EnergyOutput;   // 微电流能量输出使能标志
	uint8_t		WaterLevelOK;      // 1=water level OK (low), 0=water level low (high)
	uint8_t		WaterLevelReport;  // Water level low report enable flag
}  Flag_StructType;
extern   Flag_StructType Flag;











#endif /*__MYTYPEDEF_H*/
