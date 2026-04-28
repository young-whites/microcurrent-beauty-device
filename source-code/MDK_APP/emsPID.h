#ifndef __EMSPID_H
#define __EMSPID_H
#include "sys.h"


#define		EMSHot_DTY_Max		(100)		// 占空比输出最大值
#define		EMSHot_DTY_Min		(0)			// 占空比输出最小值

typedef struct {
	float	Kp;									// 比例系数
	float	Ki;									// 积分系数
	float	Kd;									// 微分系数
	INT32S	Ek2;								// 上上次偏差
	INT32S	Ek1;								// 上次偏差
	INT32S	Ek;									// 本次偏差
	INT32S	Uk1;								// 上次输出
	INT32S	Uk;									// 本次输出
	INT32S	P;
	INT32S  I;
	INT32S  D;
}EMS_PID_TypeDef;
extern EMS_PID_TypeDef EMS_PID;

typedef struct {
	INT8U	tSet;								// 温度摄氏度整数部分设定值
	INT8U	tGetTemp;							// 当前温度摄氏度整数部分
	INT16U	tGetAd;								// 温度测量值
}EMS_Temp_TypeDef;
extern EMS_Temp_TypeDef	EMS_Temp;



void EMS_Hot_TempScan(void);
void SetEMS_HotGears(void);
void EMS_Heat_GPIO_Config(void);
void EMS_Heat_Off(void);
void EMS_Heat_On(void);
void EMSTemp_CalcPID(void);
void EMS_HotPID_Init(void);
void Set_HeatStreng(uint32_t CCPn, uint32_t CCPChannel, uint16_t Streng);



#endif /*__EMSPID_H*/
