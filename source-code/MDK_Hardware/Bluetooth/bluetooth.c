#include "bluetooth.h"



//----------------------------------------------------------------------------
INT8U	btPowerOnCnt = 0xFF;						// 蓝牙电源开启计时
static	INT8U	bBtCnct = 0;						// 蓝牙连接状态
//----------------------------------------------------------------------------



/*****************************************************************************
* 功能:		蓝牙模块 端口初始化
*****************************************************************************/
void Bluetooth_GPIO_Config(void)
{
	// P36管脚配置 (BLE_INT/MCU_DET_BT_STATUS)
	SYS_SET_IOCFG(IOP36CFG, SYS_IOCFG_P36_GPIO);						
	GPIO_CONFIG_IO_MODE(GPIO3, GPIO_PIN_6, GPIO_MODE_INPUT_PULL_DOWN);	
	GPIO_DisableInt(GPIO3, GPIO_PIN_6_MSK);								

	// P26管脚配置 (BLE_VCC_BLE)
	SYS_SET_IOCFG(IOP26CFG, SYS_IOCFG_P26_GPIO);							
	GPIO_CONFIG_IO_MODE(GPIO2, GPIO_PIN_6, GPIO_MODE_OUTPUT_PUSH_PULL);	
	GPIO_DisableInt(GPIO2, GPIO_PIN_6_MSK);								

	Bluetooth_PowerOff();
}




/*****************************************************************************
* 功能:		蓝牙模块 扫描 （10ms扫描一次）
*****************************************************************************/
void Bluetooth_Scan(void)
{
	static	int8_t	send = 1;
	static	int8_t	cnt = 0;
	int8_t	curr;

	if (btPowerOnCnt < 0xFF) {								// 首次上电开机后0.5秒，设置一次名称
		btPowerOnCnt++;
		if (send && (btPowerOnCnt == 50)) {
			send = 0;
			UART1_SendString("AT+NAMEDr-05\r\n");
		}
	}

	curr = Bluetooth_GetState();							// 获取蓝牙模块的连接状态
	if (curr != bBtCnct)
	{
		/* 连续10次状态改变 才认为真的改变 */
		if (++cnt >= 10)
		{
			cnt = 0;
			bBtCnct = curr;									// 保存新的蓝牙连接状态
			if (bBtCnct)
			{
				BEEP_Blink(2,0,0);							// 蓝牙刚联通时，蜂鸣器鸣叫2声
			}
		}
	}
	else 
	{
		cnt = 0;
	}
}

/*****************************************************************************
* 功能:		获取蓝牙模块的连接状态
* 返回:		0：未连接；1：已连接
*****************************************************************************/
INT8U Bluetooth_GetConnect(void)
{
	return bBtCnct;
}



/*****************************************************************************
* 功能:		获取蓝牙模块的连接状态
* 返回:		0：未连接；1：已连接
*****************************************************************************/
INT8U Bluetooth_GetState(void)
{
	return GPIO_GetPin(GPIO3, GPIO_PIN_6_MSK) ? 1 : 0;
}




/*****************************************************************************
* 功能:		蓝牙模块电源开启
*****************************************************************************/
void Bluetooth_PowerOn(void)
{
	GPIO_SetPin(GPIO2, GPIO_PIN_6_MSK);					
}



/*****************************************************************************
* 功能:		蓝牙模块电源切断
*****************************************************************************/
void Bluetooth_PowerOff(void)
{
	GPIO_ResetPin(GPIO2, GPIO_PIN_6_MSK);		
}

//----------------------------------------------------------------------------






