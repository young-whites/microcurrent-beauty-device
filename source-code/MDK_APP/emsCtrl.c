#include "emsCtrl.h"





/**
  * @brief  EMS的能量强度挡位对应占空比的数组
  * @param  None
  * @retval  0~10强度对应的占空比值
  */
static	const	INT8U	EMSTabDuty[11] =
{
	//	0	1	2	3	4	5	6	7	8	9	10
		0,  4,  6,  8,  10, 12, 14, 16, 18, 20, 100
};





/**
  * @brief  EMS微电流控制的GPIO引脚初始化
  * @param  None
  * @retval None
  */
void EMS_GPIO_Config(void)
{
	if (Record.HandWave == 1)
	{
		/*配置 P10 ION_ENAB  引脚为普通GPIO模式*/
		SYS_SET_IOCFG(IOP10CFG, SYS_IOCFG_P10_GPIO);
		GPIO_CONFIG_IO_MODE(GPIO1, GPIO_PIN_0, GPIO_MODE_OUTPUT_PUSH_PULL);
		GPIO_SetPin(GPIO1, GPIO_PIN_0_MSK);

		// P00管脚配置 (ION_FREQA)
		SYS_SET_IOCFG(IOP00CFG, SYS_IOCFG_P00_EPWM2);				// P00管脚配置为EPWM2

		// P06管脚配置 (ION_FREQB)
		SYS_SET_IOCFG(IOP06CFG, SYS_IOCFG_P06_EPWM3);				// P06管脚配置为EPWM3

		// P31管脚配置 (ION_DUTYA)
		SYS_SET_IOCFG(IOP31CFG, SYS_IOCFG_P31_EPWM4);				// P31管脚配置为EPWM4

		// P07管脚配置 (EN_VCC_ION)
		SYS_SET_IOCFG(IOP07CFG, SYS_IOCFG_P07_CCP1B);				// P07管脚配置为CCP1B
		CCP_Config_PWM_Mode();

		/******************************** 以上引脚的配置与微电流的控制有关（与门输入控制） ************************************************/
	}
	else if ((Record.HandWave == 2) || (Record.HandWave == 3))
	{


		SYS_SET_IOCFG(IOP10CFG, SYS_IOCFG_P10_EPWM1);				// (P10)ION_ENAB  配置为EPWM1
		// P00管脚配置 (ION_FREQA)
		SYS_SET_IOCFG(IOP00CFG, SYS_IOCFG_P00_EPWM2);				// P00管脚配置为EPWM2

		// P06管脚配置 (ION_FREQB)
		SYS_SET_IOCFG(IOP06CFG, SYS_IOCFG_P06_EPWM3);				// P06管脚配置为EPWM3

		// P31管脚配置 (ION_DUTYA)
		SYS_SET_IOCFG(IOP31CFG, SYS_IOCFG_P31_EPWM4);				// P31管脚配置为EPWM4

		// P07管脚配置 (EN_VCC_ION)
		SYS_SET_IOCFG(IOP07CFG, SYS_IOCFG_P07_CCP1B);				// P07管脚配置为CCP1B
		CCP_Config_PWM_Mode();
	}

}


/**
  * @brief  刷新EMS电流强度
  * @param  None
  * @retval None
  */
void Refr_EMSIonStreng(INT8U Streng)
{
	INT8U	Duty;

	if (Streng > 100)	Streng = 100;
	Duty = EMSTabDuty[Streng];
	CCP_ConfigDutyScale(CCP1, CCPxB, Duty);
}




void Set_EMSIonStreng(uint8_t* Streng)
{
	if (*Streng > 100)		*Streng = 100;				// 超范围
	Record.EMSIonStrengGears = *Streng;
	Refr_EMSIonStreng(Record.EMSIonStrengGears);	// 刷新输出
}








/*****************************************************************************
* 功能:	面部训练仪的模式1的波形
* 参数:	Mode：1
*****************************************************************************/
void EMS_Wave_1(void)
{

	/*配置引脚初始化的函数在选用手柄时已经定义，接下来直接对引脚进行操作*/
	SYS_SET_IOCFG(IOP12CFG, SYS_IOCFG_P12_GPIO);						// (P12)ION_ENA 配置为GPIO
	GPIO_CONFIG_IO_MODE(GPIO1, GPIO_PIN_2, GPIO_MODE_OUTPUT_PUSH_PULL);	// (P12)ION_ENA 配置为推挽输出模式
	GPIO_SetPin(GPIO1, GPIO_PIN_2_MSK);									// (P12)ION_ENA 输出为高电平

	SYS_SET_IOCFG(IOP40CFG, SYS_IOCFG_P40_GPIO);						// (P40)ION_ENB 配置为GPIO
	GPIO_CONFIG_IO_MODE(GPIO4, GPIO_PIN_0, GPIO_MODE_OUTPUT_PUSH_PULL);	// (P40)ION_ENB 配置为推挽输出模式
	GPIO_SetPin(GPIO4, GPIO_PIN_0_MSK);									// (P40)ION_ENB 输出为高电平

	EPWM_ConfigChannelPeriod(EPWM2, 40000 / 2);							// (P00)ION_FREQA FreqA = 50Hz, Period = Pclk / FreqA / PwmDiv[2] = 2MHz / 50Hz / 1 = 40000.
	EPWM_ConfigChannelPeriod(EPWM3, 40000 / 2);							// (P06)ION_FREQB FreqB = 50Hz, Period = Pclk / FreqA / PwmDiv[3] = 2MHz / 50Hz / 1 = 40000.
	EPWM_ConfigChannelPeriod(EPWM4, 20000 / 2);							// (P31)ION_DUTYA FreqA = 100Hz, Period = Pclk / FreqA / PwmDiv[2] = 2MHz / 100Hz / 1 = 20000.

	EPWM_ConfigChannelSymDutyScale(EPWM2, 48);							// (P00)ION_FREQA Duty = 48%.
	EPWM_ConfigChannelSymDutyScale(EPWM3, 52);							// (P06)ION_FREQB Duty = 52%.
	EPWM_ConfigChannelSymDutyScale(EPWM4, 20);							// (P31)ION_DUTYA Duty = 20%.


	/*启用波形输出*/
	EPWM_ClearDownCmpIntFlag(EPWM2);
	EPWM_EnableDownCmpInt(EPWM_CH_2_MSK);
	EPWM_Start(EPWM_CH_2_MSK);

	EPWM_ClearDownCmpIntFlag(EPWM3);
	EPWM_EnableDownCmpInt(EPWM_CH_3_MSK);
	EPWM_Start(EPWM_CH_3_MSK);

	CCP_Start(CCP1);
}






void EMS_Wave_3(void)
{
	SYS_SET_IOCFG(IOP12CFG, SYS_IOCFG_P12_GPIO);						// (P12)ION_ENA 配置为GPIO
	GPIO_CONFIG_IO_MODE(GPIO1, GPIO_PIN_2, GPIO_MODE_OUTPUT_PUSH_PULL);	// (P12)ION_ENA 配置为推挽输出模式
	GPIO_SetPin(GPIO1, GPIO_PIN_2_MSK);									// (P12)ION_ENA 输出为高电平

	SYS_SET_IOCFG(IOP40CFG, SYS_IOCFG_P40_GPIO);						// (P40)ION_ENB 配置为GPIO
	GPIO_CONFIG_IO_MODE(GPIO4, GPIO_PIN_0, GPIO_MODE_OUTPUT_PUSH_PULL);	// (P40)ION_ENB 配置为推挽输出模式
	GPIO_SetPin(GPIO4, GPIO_PIN_0_MSK);									// (P40)ION_ENB 输出为高电平


	EPWM_ConfigChannelPeriod(EPWM1, 36250 / 2);							// (P00)ION_ENAB  高:200ms, 低:380ms. 周期Tp = 580000us, PWMclk = 16us, Period = Tp / PWMclk = 580000us / 16us = 36250.
	EPWM_ConfigChannelSymDuty(EPWM1,36250 / 2 - 12500 / 2);			// (P10)ION_ENAB  高:200ms, 低:380ms. 占空Tc = 200000us, PWMclk = 16us, Cmpdat = Tc / PWMclk = 200000us / 16us = 12500.
	EPWM_ConfigChannelPeriod(EPWM2, 20000 / 2);							// (P00)ION_FREQA FreqA = 100Hz, Period = Pclk / FreqA / PwmDiv[2] = 2MHz / 100Hz / 1 = 20000.
	EPWM_ConfigChannelPeriod(EPWM3, 20000 / 2);							// (P06)ION_FREQB FreqB = 100Hz, Period = Pclk / FreqA / PwmDiv[3] = 2MHz / 100Hz / 1 = 20000.
	EPWM_ConfigChannelPeriod(EPWM4, 10000 / 2);							// (P31)ION_DUTYA FreqA = 200Hz, Period = Pclk / FreqA / PwmDiv[2] = 2MHz / 200Hz / 1 = 10000.
	EPWM_ConfigChannelPeriod(EPWM5, 10000 / 2);							// (P21)ION_DUTYB FreqB = 200Hz, Period = Pclk / FreqA / PwmDiv[3] = 2MHz / 200Hz / 1 = 10000.
	EPWM_ConfigChannelSymDutyScale(EPWM2, 48);							// (P00)ION_FREQA Duty = 48%.
	EPWM_ConfigChannelSymDutyScale(EPWM3, 52);							// (P06)ION_FREQB Duty = 52%.
	EPWM_ConfigChannelSymDutyScale(EPWM4, 40);							// (P31)ION_DUTYA Duty = 40%.
	EPWM_ConfigChannelSymDutyScale(EPWM5, 40);							// (P21)ION_DUTYB Duty = 40%.

	/*启用波形输出*/
	EPWM_ClearDownCmpIntFlag(EPWM1);
	EPWM_EnableDownCmpInt(EPWM_CH_1_MSK);
	EPWM_Start(EPWM_CH_1_MSK);

	CCP_Start(CCP1);

}




void EMS_Wave_6(void)
{
	SYS_SET_IOCFG(IOP12CFG, SYS_IOCFG_P12_GPIO);						// (P12)ION_ENA 配置为GPIO
	GPIO_CONFIG_IO_MODE(GPIO1, GPIO_PIN_2, GPIO_MODE_OUTPUT_PUSH_PULL);	// (P12)ION_ENA 配置为推挽输出模式
	GPIO_SetPin(GPIO1, GPIO_PIN_2_MSK);									// (P12)ION_ENA 输出为高电平

	SYS_SET_IOCFG(IOP40CFG, SYS_IOCFG_P40_GPIO);						// (P40)ION_ENB 配置为GPIO
	GPIO_CONFIG_IO_MODE(GPIO4, GPIO_PIN_0, GPIO_MODE_OUTPUT_PUSH_PULL);	// (P40)ION_ENB 配置为推挽输出模式
	GPIO_SetPin(GPIO4, GPIO_PIN_0_MSK);									// (P40)ION_ENB 输出为高电平

	EPWM_ConfigChannelPeriod(EPWM1, 15625 / 2);							// (P00)ION_ENAB  高:40ms, 低:210ms. 周期Tp = 250000us, PWMclk = 16us, Period = Tp / PWMclk = 250000us / 16us = 15625.
	EPWM_ConfigChannelSymDuty(EPWM1, 15625 / 2 - 2500 / 2);				// (P10)ION_ENAB  高:40ms, 低:210ms. 占空Tc =  40000us, PWMclk = 16us, Cmpdat = Tc / PWMclk =  40000us / 16us = 2500.
	EPWM_ConfigChannelPeriod(EPWM2, 10000 / 2);							// (P00)ION_FREQA FreqA = 200Hz, Period = Pclk / FreqA / PwmDiv[2] = 2MHz / 200Hz / 1 = 10000.
	EPWM_ConfigChannelPeriod(EPWM3, 10000 / 2);							// (P06)ION_FREQB FreqB = 200Hz, Period = Pclk / FreqA / PwmDiv[3] = 2MHz / 200Hz / 1 = 10000.
	EPWM_ConfigChannelPeriod(EPWM4, 5000 / 2);							// (P31)ION_DUTYA FreqA = 400Hz, Period = Pclk / FreqA / PwmDiv[2] = 2MHz / 400Hz / 1 = 5000.
	EPWM_ConfigChannelPeriod(EPWM5, 5000 / 2);							// (P21)ION_DUTYB FreqB = 400Hz, Period = Pclk / FreqA / PwmDiv[3] = 2MHz / 400Hz / 1 = 5000.
	EPWM_ConfigChannelSymDutyScale(EPWM2, 48);							// (P00)ION_FREQA Duty = 48%.
	EPWM_ConfigChannelSymDutyScale(EPWM3, 52);							// (P06)ION_FREQB Duty = 52%.
	EPWM_ConfigChannelSymDutyScale(EPWM4, 80);							// (P31)ION_DUTYA Duty = 80%.
	EPWM_ConfigChannelSymDutyScale(EPWM5, 80);							// (P21)ION_DUTYB Duty = 80%.

	/*启用波形输出*/
	EPWM_ClearDownCmpIntFlag(EPWM1);
	EPWM_EnableDownCmpInt(EPWM_CH_1_MSK);
	EPWM_Start(EPWM_CH_1_MSK);

	CCP_Start(CCP1);

}



void EMS_Wave_1_Start(void)
{
	EPWM_ClearDownCmpIntFlag(EPWM2);
	EPWM_EnableDownCmpInt(EPWM_CH_2_MSK);
	EPWM_Start(EPWM_CH_2_MSK);

	EPWM_ClearDownCmpIntFlag(EPWM3);
	EPWM_EnableDownCmpInt(EPWM_CH_3_MSK);
	EPWM_Start(EPWM_CH_3_MSK);
}



void EMS_Wave_1_Stop(void)
{
	EPWM_Stop(EPWM_CH_1_MSK | EPWM_CH_2_MSK | EPWM_CH_3_MSK | EPWM_CH_4_MSK);
}



void EMS_Wave_3_6_Start(void)
{
	EPWM_ClearDownCmpIntFlag(EPWM2);
	EPWM_EnableDownCmpInt(EPWM_CH_2_MSK);
	EPWM_Start(EPWM_CH_2_MSK);

	EPWM_ClearDownCmpIntFlag(EPWM3);
	EPWM_EnableDownCmpInt(EPWM_CH_3_MSK);
	EPWM_Start(EPWM_CH_3_MSK);
}



void EMS_Wave_3_6_Stop(void)
{
	EPWM_Stop(EPWM_CH_1_MSK | EPWM_CH_2_MSK | EPWM_CH_3_MSK | EPWM_CH_4_MSK | EPWM_CH_5_MSK);
}