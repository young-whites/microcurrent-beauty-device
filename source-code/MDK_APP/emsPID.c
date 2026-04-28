#include "emsPID.h"


EMS_Temp_TypeDef	EMS_Temp;
EMS_PID_TypeDef		EMS_PID;



/**************************************************************************************************
* 功能:	发热丝 端口初始化
**************************************************************************************************/
void EMS_Heat_GPIO_Config(void)
{
	SYS_SET_IOCFG(IOP30CFG, SYS_IOCFG_P30_CCP0A);
}



/**************************************************************************************************
* 功能:	关闭发热丝
**************************************************************************************************/
void EMS_Heat_Off(void)
{
	CCP_Stop(CCP0);
}


/**************************************************************************************************
* 功能:	开启发热丝
**************************************************************************************************/
void EMS_Heat_On(void)
{
	CCP_Start(CCP0);
}



/************************
 * 功能 ： 制冷热挡位设置并刷新
 *
 * 说明 ：*GEARS   -->  挡位参数
 ***/
void SetEMS_HotGears(void)
{
	if (Record.EMS_HotGears > 100)
	{
		Record.EMS_HotGears = 100;
	}


	EMS_Temp.tSet = EMSHotTemp[Record.EMS_HotGears];
}




void EMS_Hot_TempScan(void)
{

	EMS_Temp.tGetTemp = temp_ConverStable();
	EMSTemp_CalcPID();

	Record.Period = CCP_ReadLoad(CCP0);
	if ((EMS_Temp.tGetTemp < EMS_Temp.tSet) && (EMS_PID.Uk < 100))
	{
		Record.Duty = Record.Period * (100 - EMS_PID.Uk) / 100;
	}
	else
	{
		Record.Duty = Record.Period * (EMS_PID.Uk) / 100;
	}

	CCP_ConfigCompare(CCP0, CCPxA, Record.Duty);

}




void EMS_HotPID_Init(void)
{
	EMS_PID.Kp = 1;
	EMS_PID.Ki = 0.7;
	EMS_PID.Kd = 0.8;
	EMS_PID.Ek2 = 0;
	EMS_PID.Ek1 = 0;
	EMS_PID.Ek = 0;
	EMS_PID.Uk1 = 0;
	EMS_PID.Uk = 0;
}





/**************************************************************************************************
* 功能:		PID计算
**************************************************************************************************/
void EMSTemp_CalcPID(void)
{
	/*计算本次偏差*/
	EMS_PID.Ek = EMS_Temp.tSet - EMS_Temp.tGetTemp ;

	//	pid.Uk = pid.Uk1 + pid.Kp*(pid.Ek-pid.Ek1) + pid.Ki*pid.Ek + pid.Kd*(pid.Ek-2*pid.Ek1+pid.Ek2);
	EMS_PID.P = EMS_PID.Kp * (EMS_PID.Ek - EMS_PID.Ek1);
	EMS_PID.I = EMS_PID.Ki * (EMS_PID.Ek);
	EMS_PID.D = EMS_PID.Kd * (EMS_PID.Ek - 2 * EMS_PID.Ek1 + EMS_PID.Ek2);

	if (EMS_PID.Ek > 5)
	{
		EMS_PID.I = 0;
	}
	EMS_PID.Uk = EMS_PID.Uk1 + EMS_PID.P + EMS_PID.I + EMS_PID.D;
	EMS_PID.Ek2 = EMS_PID.Ek1;
	EMS_PID.Ek1 = EMS_PID.Ek;
	EMS_PID.Uk1 = EMS_PID.Uk;

	/* 温度达到强制关闭输出 */
	if ((EMS_PID.Ek <= 0) && (EMS_PID.Ek > -4))
	{
		EMS_PID.Uk = 15;
		//Record.EMSHeatCnt = 1;
	}
	else if (EMS_PID.Ek <= -4)
	{
		EMS_PID.Uk = 0;
	}


	if (EMS_PID.Uk < EMSHot_DTY_Min)	EMS_PID.Uk = EMSHot_DTY_Min;
	if (EMS_PID.Uk > EMSHot_DTY_Max)	EMS_PID.Uk = EMSHot_DTY_Max;
}

