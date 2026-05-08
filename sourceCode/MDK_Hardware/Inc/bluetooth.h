#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H
#include "sys.h"




extern	INT8U	btPowerOnCnt;	// 蓝牙电源开启计时












void Bluetooth_GPIO_Config(void);
void Bluetooth_Scan(void);
INT8U Bluetooth_GetConnect(void);
INT8U Bluetooth_GetState(void);
void Bluetooth_PowerOn(void);
void Bluetooth_PowerOff(void);









#endif /*__BLUETOOTH_H*/
