#include "epwm.h"


INT8U PwmDiv[6] = {
	1,					// EPWM0的时钟分频数 ((1/2/4/8/16/32))
	1,					// EPWM1的时钟分频数 ((1/2/4/8/16/32))
	1,					// EPWM2的时钟分频数 ((1/2/4/8/16/32))
	1,					// EPWM3的时钟分频数 ((1/2/4/8/16/32))
	1,					// EPWM4的时钟分频数 ((1/2/4/8/16/32))
	1					// EPWM5的时钟分频数 ((1/2/4/8/16/32))
};


INT32U PwmFreq[6] = {
	3000,					// EPWM0的频率 (Hz)
	3000,					// EPWM1的频率 (Hz)
	3000,					// EPWM2的频率 (Hz)
	3000,					// EPWM3的频率 (Hz)
	3000,				 	// EPWM4的频率 (Hz)
	3000					// EPWM5的频率 (Hz)
};



INT8U EMS_PwmDiv[6] = {
	32,					// EPWM0的时钟分频数 ((1/2/4/8/16/32))
	32,					// EPWM1的时钟分频数 ((1/2/4/8/16/32))
	1,					// EPWM2的时钟分频数 ((1/2/4/8/16/32))
	1,					// EPWM3的时钟分频数 ((1/2/4/8/16/32))
	1,					// EPWM4的时钟分频数 ((1/2/4/8/16/32))
	1					// EPWM5的时钟分频数 ((1/2/4/8/16/32))
};


INT32U EMS_PwmFreq[6] = {
	1,					// EPWM0的频率 (Hz)
	1,					// EPWM1的频率 (Hz)
	50,					// EPWM2的频率 (Hz)
	50,					// EPWM3的频率 (Hz)
	100,				// EPWM4的频率 (Hz)
	100					// EPWM5的频率 (Hz)
};



INT8U Water_PwmDiv[6] = {
	32,					// EPWM0的时钟分频数 ((1/2/4/8/16/32))
	32,					// EPWM1的时钟分频数 ((1/2/4/8/16/32))
	1,					// EPWM2的时钟分频数 ((1/2/4/8/16/32))
	1,					// EPWM3的时钟分频数 ((1/2/4/8/16/32))
	1,					// EPWM4的时钟分频数 ((1/2/4/8/16/32))
	1					// EPWM5的时钟分频数 ((1/2/4/8/16/32))
};


INT32U Water_PwmFreq[6] = {
	1,					// EPWM0的频率 (Hz)
	1,					// EPWM1的频率 (Hz)
	1645,				// EPWM2的频率 (Hz)
	1645,				// EPWM3的频率 (Hz)
	100,				// EPWM4的频率 (Hz)
	100					// EPWM5的频率 (Hz)
};






/*****************************************************************************
* 功能:		epwm配置为独立模式
* 参数:		chMask:	通道掩码 可多选
* 			Freq	频率指针 (Hz)
* 			Div		分频数指针
*****************************************************************************/
void EPWM_Config_Independent_Mode(uint32_t chMask, INT32U Freq[6], INT8U Div[6])
{
	INT32U	Period;
	INT8U	i, mask;

	/*开启EPWM时钟*/
	SYS_EnablePeripheralClk(SYS_CLK_EPWM_MSK);						// 开启EPWM时钟

	/*设置运行模式*/
	EPWM_ConfigRunMode( EPWM_COUNT_UP_DOWN | 						// 上下计数模式(中心对齐)
						EPWM_OCU_SYMMETRIC |						// 对称模式
						EPWM_WFG_INDEPENDENT |						// 独立模式（非互补）
						EPWM_OC_INDEPENDENT);						// 独立模式（不成组）

	/*设置总中断开关与优先级*/
	NVIC_EnableIRQ(EPWM_IRQn);											// 允许中断
	NVIC_SetPriority(EPWM_IRQn, 3);										// 优先级3 （0最高、3最低）

	for (mask = 0x01, i = 0; i < 6; i++)
	{																	// 逐个通道检查
		if (chMask & mask)
		{																// 当前通道选通
			/*设置通道中断*/
			if (i <= 5)
			{
				EPWM_EnableUpCmpInt(mask);								// 开启向上比较点中断
				EPWM_EnableDownCmpInt(mask);							// 开启向下比较点中断
			}

			/*设置EPWM加载方式*/
			EPWM_EnableAutoLoadMode(mask);								// 设置EPWMi为自动连续加载模式
			EPWM_ConfigLoadAndIntMode(i, EPWM_EACH_PERIOD_ZERO);		// 加载点设置为每个周期点和零点

			/*设置IO口输出*/
			EPWM_EnableOutput(mask);									// 开启EPWMi输出到管脚

			/*设置EPWM反向输出*/
			EPWM_DisableReverseOutput(mask);

			/*设置EPWM时钟周期与死区*/
			if (i == 1)
			{
				EPWM_ConfigChannelDiv(i, Div[i]);							// n分频, EPWMiclk = Pclk/n = 1.5MHz/n
				HWDIV_EnableUnsingedMode();
				Period = HWDIV_Div(SystemAPBClock, Freq[i]);				// 计算分频周期  1.5Mhz/30Hz = 50000(个数)
				Period = HWDIV_Div(Period, Div[i]);							// 计算分频周期  25000
				EPWM_ConfigChannelPeriod(i, Period >> 1);					// 中心对齐
				// EPWM_ConfigChannelSymDuty(i, 2500);						// 中心对齐占空比
				EPWM_DisableDeadZone(mask);									// 关闭死区			
			}
			else
			{
				EPWM_ConfigChannelDiv(i, Div[i]);							// n分频, EPWMiclk = Pclk/n = 3MHz/n
				HWDIV_EnableUnsingedMode();
				Period = HWDIV_Div(SystemAPBClock, Freq[i]);				// 计算分频周期  3Mhz/3000Hz = 1000(个数)
				Period = HWDIV_Div(Period, Div[i]);							// 计算分频周期  
				EPWM_ConfigChannelPeriod(i, Period >> 1);					// 中心对齐
				EPWM_ConfigChannelSymDuty(i, 100);							// 中心对齐占空比
				EPWM_DisableDeadZone(mask);									// 关闭死区
			}

			if (Record.K1ElecMode != 0)
			{
				EPWM_Start(mask);
			}
			else
			{
				/*停止EPWM*/
				EPWM_Stop(mask);											// 停止EPWMi
			}

		}
		mask <<= 1;
	}
}



/*****************************************************************************
* 功能:		epwm配置为独立模式
* 参数:		chMask:	通道掩码 可多选
* 			Freq	频率指针 (Hz)
* 			Div		分频数指针
*****************************************************************************/
void EMS_EPWM_Config_Independent_Mode(uint32_t chMask, INT32U Freq[6], INT8U Div[6])
{
	INT32U	Period;
	INT8U	i, mask;

	SYS_EnablePeripheralClk(SYS_CLK_EPWM_MSK);						// 开启EPWM时钟
	EPWM_ConfigRunMode(	EPWM_COUNT_UP_DOWN | 						// 上下计数模式(中心对齐)
						EPWM_OCU_SYMMETRIC |						// 对称模式
						EPWM_WFG_INDEPENDENT |						// 独立模式（非互补）
						EPWM_OC_INDEPENDENT);						// 独立模式（不成组）

	NVIC_EnableIRQ(EPWM_IRQn);										// 允许中断
	NVIC_SetPriority(EPWM_IRQn, 3);									// 优先级3 （0最高、3最低）

	for (mask = 0x01, i = 0; i < 6; i++)
	{
		if (chMask & mask)
		{
			if (i <= 3)
			{
				EPWM_EnableUpCmpInt(mask);							// 开启向上比较点中断
				EPWM_EnableDownCmpInt(mask);						// 开启向下比较点中断
			}
			EPWM_EnableAutoLoadMode(mask);							// 设置EPWMi为自动连续加载模式
			EPWM_ConfigLoadAndIntMode(i, EPWM_EACH_PERIOD_ZERO);	// 加载点设置为每个周期点和零点
			EPWM_EnableOutput(mask);								// 开启EPWMi输出到管脚
			if (i == 3)	EPWM_EnableReverseOutput(mask);				// 开启EPWM3反相输出
			else		EPWM_DisableReverseOutput(mask);			// 关闭EPWMi反相输出
			EPWM_ConfigChannelDiv(i, Div[i]);						// n分频, EPWMiclk = Pclk/n = 3MHz/n
			HWDIV_EnableUnsingedMode();
			Period = HWDIV_Div(SystemAPBClock, Freq[i]);			// 计算分频周期
			Period = HWDIV_Div(Period, Div[i]);						// 计算分频周期
			EPWM_ConfigChannelPeriod(i, Period >> 1);				// 中心对齐
			EPWM_ConfigChannelSymDuty(i, Period >> 2);				// 对称中心对齐占空比 50%.
			EPWM_DisableDeadZone(mask);								// 关闭死区
			EPWM_Stop(mask);										// 停止EPWMi
		}
		mask <<= 1;
	}
}




/*****************************************************************************
* 功能:		epwm配置为独立模式
* 参数:		chMask:	通道掩码 可多选
* 			Freq	频率指针 (Hz)
* 			Div		分频数指针
*****************************************************************************/
void EPT_EPWM_Config_Independent_Mode(uint32_t chMask, INT32U Freq[6], INT8U Div[6])
{
	INT32U	Period;
	INT8U	i, mask;

	SYS_EnablePeripheralClk(SYS_CLK_EPWM_MSK);						// 开启EPWM时钟
	EPWM_ConfigRunMode(	EPWM_COUNT_UP_DOWN | 						// 上下计数模式(中心对齐)
						EPWM_OCU_SYMMETRIC |						// 对称模式
						EPWM_WFG_INDEPENDENT |						// 独立模式（非互补）
						EPWM_OC_INDEPENDENT);						// 独立模式（不成组）

	NVIC_EnableIRQ(EPWM_IRQn);										// 允许中断
	NVIC_SetPriority(EPWM_IRQn, 3);									// 优先级3 （0最高、3最低）

	for (mask = 0x01, i = 0; i < 6; i++)
	{
		if (chMask & mask)
		{
			if (i <= 3)
			{
				EPWM_EnableUpCmpInt(mask);							// 开启向上比较点中断
				EPWM_EnableDownCmpInt(mask);						// 开启向下比较点中断
			}
			EPWM_EnableAutoLoadMode(mask);							// 设置EPWMi为自动连续加载模式
			EPWM_ConfigLoadAndIntMode(i, EPWM_EACH_PERIOD_ZERO);	// 加载点设置为每个周期点和零点
			EPWM_EnableOutput(mask);								// 开启EPWMi输出到管脚
			if (i == 3)	EPWM_EnableReverseOutput(mask);				// 开启EPWM3反相输出
			else		EPWM_DisableReverseOutput(mask);			// 关闭EPWMi反相输出
			EPWM_ConfigChannelDiv(i, Div[i]);						// n分频, EPWMiclk = Pclk/n = 3MHz/n
			HWDIV_EnableUnsingedMode();
			Period = HWDIV_Div(SystemAPBClock, Freq[i]);			// 计算分频周期
			Period = HWDIV_Div(Period, Div[i]);						// 计算分频周期
			EPWM_ConfigChannelPeriod(i, Period >> 1);				// 中心对齐
			EPWM_ConfigChannelSymDuty(i, Period >> 2);				// 对称中心对齐占空比 50%.
			EPWM_DisableDeadZone(mask);								// 关闭死区
			EPWM_Stop(mask);										// 停止EPWMi
		}
		mask <<= 1;
	}
}





/**
  * @brief  K1波形的EPWM工作模式配置函数
  * @param  None
  */
void EPWM_Config(void)
{

	if (Record.HandWave == 1) {
		EMS_EPWM_Config_Independent_Mode(EPWM_CH_1_MSK | EPWM_CH_2_MSK | EPWM_CH_3_MSK | EPWM_CH_4_MSK, EMS_PwmFreq, EMS_PwmDiv);
	}
	else if ((Record.HandWave == 2) || (Record.HandWave == 3))
	{
		EMS_EPWM_Config_Independent_Mode(EPWM_CH_1_MSK | EPWM_CH_2_MSK | EPWM_CH_3_MSK | EPWM_CH_4_MSK | EPWM_CH_5_MSK, EMS_PwmFreq, EMS_PwmDiv);
	}
	else if (Record.HandWave == 4) {
		EPWM_Config_Independent_Mode(EPWM_CH_2_MSK | EPWM_CH_3_MSK, Water_PwmFreq, Water_PwmDiv);
	} 
	else if(Record.HandWave == 5){
		EPT_Current_GPIO_Config();
	}
}