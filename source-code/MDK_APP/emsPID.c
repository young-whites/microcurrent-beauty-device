#include "emsPID.h"


EMS_Temp_TypeDef	EMS_Temp;
EMS_PID_TypeDef		EMS_PID;

/* 测试模式标志: 1=制冷片直接拉高, 屏蔽PID */
INT8U EMS_CoolTestMode = 0;



/* ====================== GPIO & 开关 ====================== */
void EMS_Heat_GPIO_Config(void)
{
	SYS_SET_IOCFG(IOP30CFG, SYS_IOCFG_P30_CCP0A);   // 你的制冷PWM引脚
}


void EMS_Heat_On(void)  { CCP_Start(CCP0); }
void EMS_Heat_Off(void) { CCP_Stop(CCP0); }

/* ====================== 挡位设置 ====================== */
void SetEMS_CoolGears(void)
{
	if (Record.EMS_HotGears > 50) Record.EMS_HotGears = 50;  // 复用挡位变量也行
	EMS_Temp.tSet = EMSHotTemp[Record.EMS_HotGears];   // 复用温度表（低温段）
}


/* ====================== 温度扫描 + PID ====================== */
void EMS_Cool_TempScan(void)
{
	/* 测试模式: 跳过PID, 制冷片已由 GPIO 拉高 */
	if (EMS_CoolTestMode == 1) return;

	EMS_Temp.tGetTemp = temp_ConverStable();
	EMSTemp_CalcPID();

	Record.Period = CCP_ReadLoad(CCP0);
	Record.Duty = Record.Period * EMS_PID.Uk / 100;   // 直接正向映射

	CCP_ConfigCompare(CCP0, CCPxA, Record.Duty);
}

/* ====================== PID 初始化 ====================== */
void EMS_CoolPID_Init(void)
{
	EMS_PID.Kp = 1.2f;     // 制冷响应更快，Kp 可稍大
	EMS_PID.Ki = 0.5f;     // 积分减小防过冲
	EMS_PID.Kd = 0.9f;
	EMS_PID.Ek2 = EMS_PID.Ek1 = EMS_PID.Ek = 0;
	EMS_PID.Uk1 = EMS_PID.Uk = 0;
}

/* ====================== PID 计算（制冷版）====================== */
void EMSTemp_CalcPID(void)
{
	/* 制冷误差方向反转：温度越高越需要制冷 */
	EMS_PID.Ek = EMS_Temp.tGetTemp - EMS_Temp.tSet;

	EMS_PID.P = EMS_PID.Kp * (EMS_PID.Ek - EMS_PID.Ek1);
	EMS_PID.I = EMS_PID.Ki * EMS_PID.Ek;
	EMS_PID.D = EMS_PID.Kd * (EMS_PID.Ek - 2*EMS_PID.Ek1 + EMS_PID.Ek2);

	/* 积分抗饱和（制冷版） */
	if (EMS_PID.Ek < -5) EMS_PID.I = 0;   // 已经太冷了，停止积分

	EMS_PID.Uk = EMS_PID.Uk1 + EMS_PID.P + EMS_PID.I + EMS_PID.D;

	EMS_PID.Ek2 = EMS_PID.Ek1;
	EMS_PID.Ek1 = EMS_PID.Ek;
	EMS_PID.Uk1 = EMS_PID.Uk;

	/* 强制输出逻辑（制冷版） */
	if ((EMS_PID.Ek >= 0) && (EMS_PID.Ek < 4))      // 温度刚好达到或略高 → 维持小制冷
	{
		EMS_PID.Uk = 15;
	}
	else if (EMS_PID.Ek <= -4)                      // 已经过冷 → 关闭制冷
	{
		EMS_PID.Uk = 0;
	}

	if (EMS_PID.Uk < EMSCool_DTY_Min) EMS_PID.Uk = EMSCool_DTY_Min;
	if (EMS_PID.Uk > EMSCool_DTY_Max) EMS_PID.Uk = EMSCool_DTY_Max;
}


/* ====================== 硬件测试模式 ====================== */
/*
 * EMS_CoolTest_Enable: 进入制冷片硬件测试模式
 * 将 P30 从 CCP0A (PWM) 重新配置为普通 GPIO 并拉高,
 * 同时屏蔽 PID 控制, 制冷片全功率运行.
 */
void EMS_CoolTest_Enable(void)
{
	/* 停止 CCP0 PWM 输出 */
	CCP_Stop(CCP0);

	/* 将 P30 重新配置为普通 GPIO 推挽输出 */
	SYS_SET_IOCFG(IOP30CFG, SYS_IOCFG_P30_GPIO);
	GPIO_CONFIG_IO_MODE(GPIO3, GPIO_PIN_0, GPIO_MODE_OUTPUT_PUSH_PULL);

	/* 拉高引脚 -> 制冷片全功率 */
	GPIO_SetPin(GPIO3, GPIO_PIN_0_MSK);

	/* 设置测试模式标志, EMS_Cool_TempScan 将跳过 PID */
	EMS_CoolTestMode = 1;
}


/*
 * EMS_CoolTest_Disable: 退出制冷片硬件测试模式
 * 恢复 P30 为 CCP0A (PWM) 模式, PID 控制恢复.
 */
void EMS_CoolTest_Disable(void)
{
	/* 先拉低引脚 */
	GPIO_ResetPin(GPIO3, GPIO_PIN_0_MSK);

	/* 恢复 P30 为 CCP0A PWM 模式 */
	SYS_SET_IOCFG(IOP30CFG, SYS_IOCFG_P30_CCP0A);

	/* 清除测试模式标志 */
	EMS_CoolTestMode = 0;
}
