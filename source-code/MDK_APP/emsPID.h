#ifndef __EMSPID_H
#define __EMSPID_H
#include "sys.h"

#define		EMSCool_DTY_Max		(100)
#define		EMSCool_DTY_Min		(0)

typedef struct {
	float	Kp;
	float	Ki;
	float	Kd;
	INT32S	Ek2, Ek1, Ek;
	INT32S	Uk1, Uk;
	INT32S	P, I, D;
}EMS_PID_TypeDef;
extern EMS_PID_TypeDef EMS_PID;

typedef struct {
	INT8U	tSet;			// 目标温度（制冷时为想要达到的低温）
	INT8U	tGetTemp;		// 当前实测温度
	INT16U	tGetAd;
}EMS_Temp_TypeDef;
extern EMS_Temp_TypeDef	EMS_Temp;

// ====================== 制冷专用函数 ======================
void EMS_Cool_TempScan(void);        // 替换原来的 EMS_Hot_TempScan
void SetEMS_CoolGears(void);         // 替换原来的 SetEMS_HotGears
void EMS_CoolPID_Init(void);
void EMSTemp_CalcPID(void);          // PID核心，已改为制冷逻辑

// 硬件相关（根据你的硬件可保留或改名）
void EMS_Heat_GPIO_Config(void);     // 改成制冷GPIO也行
void EMS_Heat_On(void);
void EMS_Heat_Off(void);

#endif