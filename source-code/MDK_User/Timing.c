#include "Timing.h"




#if K1_TIMER0
uint32_t Timer0cny1;
uint32_t Timer0cny2;
uint32_t Timer0cny3;
uint32_t Timer0cny4;
uint32_t Timer0cny4_2;
uint32_t Timer0cny4_3;
uint32_t Timer0cny4_4;
uint32_t Timer0cny4_5;
uint32_t Timer0cny4_6;
uint32_t Timer0cny4_7;
void Timer0_Timing1_1ms(void)
{
	static uint16_t step_time;


	step_time = Record.Second % 63;
	switch (step_time)
	{
		case 8:
		{
			AutoSet_WaterStreng(CCP1, CCPxB, 0);
			Water_PwmFreq[2] = 1645;
			Water_PwmFreq[3] = 1645;
			EPWM_Config_Independent_Mode(EPWM_CH_2_MSK | EPWM_CH_3_MSK, Water_PwmFreq, Water_PwmDiv);
		}break;


		case 17:
		{
			AutoSet_WaterStreng(CCP1, CCPxB, 0);
			Water_PwmFreq[2] = 1563;
			Water_PwmFreq[3] = 1563;
			EPWM_Config_Independent_Mode(EPWM_CH_2_MSK | EPWM_CH_3_MSK, Water_PwmFreq, Water_PwmDiv);
		}break;


		case 26:
		{
			AutoSet_WaterStreng(CCP1, CCPxB, 0);
			Water_PwmFreq[2] = 1250;
			Water_PwmFreq[3] = 1250;
			EPWM_Config_Independent_Mode(EPWM_CH_2_MSK | EPWM_CH_3_MSK, Water_PwmFreq, Water_PwmDiv);
		}break;

		case 35:
		{
			AutoSet_WaterStreng(CCP1, CCPxB, 0);
			Water_PwmFreq[2] = 1042;
			Water_PwmFreq[3] = 1042;
			EPWM_Config_Independent_Mode(EPWM_CH_2_MSK | EPWM_CH_3_MSK, Water_PwmFreq, Water_PwmDiv);
		}break;

		case 44:
		{
			AutoSet_WaterStreng(CCP1, CCPxB, 0);
			Water_PwmFreq[2] = 919;
			Water_PwmFreq[3] = 919;
			EPWM_Config_Independent_Mode(EPWM_CH_2_MSK | EPWM_CH_3_MSK, Water_PwmFreq, Water_PwmDiv);
		}break;


		case 53:
		{
			AutoSet_WaterStreng(CCP1, CCPxB, 0);
			Water_PwmFreq[2] = 665;
			Water_PwmFreq[3] = 665;
			EPWM_Config_Independent_Mode(EPWM_CH_2_MSK | EPWM_CH_3_MSK, Water_PwmFreq, Water_PwmDiv);
		}break;

		case 62:
		{
			AutoSet_WaterStreng(CCP1, CCPxB, 0);
			Water_PwmFreq[2] = 496;
			Water_PwmFreq[3] = 496;
			EPWM_Config_Independent_Mode(EPWM_CH_2_MSK | EPWM_CH_3_MSK, Water_PwmFreq, Water_PwmDiv);
		}break;

		default:break;
	}

	/**********************************************************************************************************************************************************************************************/
	/**********************************************************************************************************************************************************************************************/


	/* 进入变频模式 -- 参考K1波形，关断与开启频率是开8s，关1s */
	if (step_time < 8)
	{
		Timer0cny4++;
		if (Timer0cny4 == 4)
		{
			Timer0cny4 = 0;
			Choose_WaterARR();
		}
	}
	else if ((step_time > 8) && (step_time < 17))
	{
		Timer0cny4_2++;
		if (Timer0cny4_2 == 2)
		{
			Timer0cny4_2 = 0;
			Choose_WaterARR();
		}
	}
	else if ((step_time > 17) && (step_time < 26))
	{
		Timer0cny4_3++;
		if (Timer0cny4_3 == 1)
		{
			Timer0cny4_3 = 0;
			/* 梯形波 */
			Choose_WaterARR_Trapezoid();
		}
	}
	else if ((step_time > 26) && (step_time < 35))
	{
		Timer0cny4_4++;
		if (Timer0cny4_4 == 1)
		{
			Timer0cny4_4 = 0;
			Choose_WaterARR();
		}
	}
	else if ((step_time > 35) && (step_time < 44))
	{
		Timer0cny4_5++;
		if (Timer0cny4_5 == 1)
		{
			Timer0cny4_5 = 0;
			Choose_WaterARR();
		}
	}
	else if ((step_time > 44) && (step_time < 53))
	{
		Timer0cny4_6++;
		if (Timer0cny4_6 == 1)
		{
			Timer0cny4_6 = 0;
			/* 梯形波 */
			Choose_WaterARR_Trapezoid();
		}
	}
	else if ((step_time > 53) && (step_time < 62))
	{
		Timer0cny4_7++;
		if (Timer0cny4_7 == 1)
		{
			Timer0cny4_7 = 0;
			/* 正弦波 */
			Choose_WaterARR();
		}
	}
}

#endif








/*****************************************************************************
** 功能：	1毫秒定时
*****************************************************************************/
INT16U	sneStreng_up = 0;
INT16U	sneStreng_down = 0;	
void Timing1_1ms(void)
{

	Flag.WorkStart = 1;
	LED_DrvScan();
	BEEP_DrvScan();
	APP_TimeOverScan();

	if ((Record.K1_CurrentGears != 0) && (Record.Switch == 1))
	{
		Record.WaterMs1++;
		if (Record.WaterMs1 == 1000)
		{
			Record.WaterMs1 = 0;
			Record.Second++;
		}
	}

}


void Timing1_2ms(void)
{
	//----------------------------------------------------------------------------------------------------------
	/* 能量变动 */
	if((Record.SNEIonStrengGears != 0)&&(Record.HandWave == 6)&&(Record.Switch == 1))
	{
		if(sneStreng_up < Record.SNEIonStrengGears){
			sneStreng_up++;
			CCP_ConfigDutyScale(CCP1, CCPxB, sneStreng_up);
		}
		if(sneStreng_up == Record.SNEIonStrengGears){
			sneStreng_down = sneStreng_up;
			sneStreng_up++;
			CCP_ConfigDutyScale(CCP1, CCPxB, sneStreng_down);
		}
		if(sneStreng_down > 1){
			sneStreng_down--;
			CCP_ConfigDutyScale(CCP1, CCPxB, sneStreng_down);
		}
		if(sneStreng_down == 1){
			CCP_ConfigDutyScale(CCP1, CCPxB, 0);
			sneStreng_up = 0;
			sneStreng_down = 0;
		}
	}
	else if(((Record.SNEIonStrengGears == 0)||Record.Switch == 0)&&(Record.HandWave == 6)){
			CCP_ConfigDutyScale(CCP1, CCPxB, 0);
	}
}




/*****************************************************************************
** 功能：	10毫秒定时
*****************************************************************************/
void Timing1_10ms(void)
{
	KEY_DrvScan();
	/***********************************************************/
	/*                蓝牙工作状态扫描                         */
	/***********************************************************/
	if (Flag.PowerOn == 1)
	{
		Bluetooth_Scan();
	}
	

		
}



/*****************************************************************************
** 功能：	10毫秒定时
*****************************************************************************/
void Timing1_20ms(void)
{
	KEY_Scan();
}



/*****************************************************************************
** 功能：	50毫秒定时
*****************************************************************************/

void Timing1_50ms(void)
{


}


/*****************************************************************************
** 功能：	100毫秒定时
*****************************************************************************/
void Timing1_100ms(void)
{

}



/*****************************************************************************
** 功能：	250毫秒定时
*****************************************************************************/

void Timing1_250ms(void)
{

}



/*****************************************************************************
** 功能：	500毫秒定时
*****************************************************************************/

void Timing1_500ms(void)
{


	if ( (Record.Switch == 1) && (Record.EMS_HotGears != 0))
	{
		SetEMS_CoolGears();      
		EMS_Cool_TempScan();  
	}
	

}

/*****************************************************************************
** 功能：	1秒定时
*****************************************************************************/

void Timing1_1s(void)
{
	Old_Test();


}
//----------------------------------------------------------------------------




