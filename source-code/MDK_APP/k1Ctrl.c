#include "k1Ctrl.h"





/*水电泳的电压强度控制的占空比控制数组，取500 个数 -- 三角波、正弦波*/
static	const	INT16	WaterGearArr1[250] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
    31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
    51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
    61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
    71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
    81, 82, 83, 84, 85, 86, 87, 88, 89, 90,
    91, 92, 93, 94, 95, 96, 97, 98, 99,
    100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
    110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
    120, 121, 122, 123, 124, 125, 126, 127, 128, 129,
    130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
    140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
	
    150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
    160, 161, 162, 163, 164, 165, 166, 167, 168, 169,
    170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
    180, 181, 182, 183, 184, 185, 186, 187, 188, 189,
    190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
    200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
    210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
    220, 221, 222, 223, 224, 225, 226, 227, 228, 229,
    230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249,
};



/**
  * @brief  更改微电流手柄的GPIO初始化
  * @param  None
  * @retval 通过控制继电器控制
  */
void  CurrentCtrl_Relay_GPIO_Config(void)
{
	// P01管脚配置 (继电器控制引脚)
	SYS_SET_IOCFG(IOP01CFG, SYS_IOCFG_P01_GPIO);						// 设置为GPIO模式
	GPIO_CONFIG_IO_MODE(GPIO0, GPIO_PIN_1, GPIO_MODE_OUTPUT_PUSH_PULL);	// 设置为推挽输出模式
	GPIO_ResetPin(GPIO0, GPIO_PIN_1_MSK);								// 初始化输出为低电平
}




/**
  * @brief  K1手柄微电流引脚配置
  * @param  None
  * @retval None
  */
void K1_Current_GPIO_Config(void)
{
    /*配置 P07 VCC_ION 引脚为CPP模块的PWM输出模式*/
    SYS_SET_IOCFG(IOP07CFG, SYS_IOCFG_P07_CCP1B);
    CCP_Config_PWM_Mode();

    /*配置 P00 ION_FREQA 引脚为 EPWM2 模式*/
    SYS_SET_IOCFG(IOP00CFG, SYS_IOCFG_P00_EPWM2);

    /*配置 P06 ION_FREQB 引脚为 EPWM3 模式*/
    SYS_SET_IOCFG(IOP06CFG, SYS_IOCFG_P06_EPWM3);

    /*配置 P12 ION_ENA   引脚为普通GPIO模式,推挽模式,低电平*/
    SYS_SET_IOCFG(IOP12CFG, SYS_IOCFG_P12_GPIO);
    GPIO_CONFIG_IO_MODE(GPIO1, GPIO_PIN_2, GPIO_MODE_OUTPUT_PUSH_PULL);
    GPIO_SetPin(GPIO1, GPIO_PIN_2_MSK);

    /*配置 P40 ION_ENB   引脚为普通GPIO模式,推挽模式,低电平*/
    SYS_SET_IOCFG(IOP40CFG, SYS_IOCFG_P40_GPIO);
    GPIO_CONFIG_IO_MODE(GPIO4, GPIO_PIN_0, GPIO_MODE_OUTPUT_PUSH_PULL);
    GPIO_SetPin(GPIO4, GPIO_PIN_0_MSK);

    /*配置 P10 ION_ENAB  引脚为普通GPIO模式*/
    SYS_SET_IOCFG(IOP10CFG, SYS_IOCFG_P10_GPIO);
    GPIO_CONFIG_IO_MODE(GPIO1, GPIO_PIN_0, GPIO_MODE_OUTPUT_PUSH_PULL);
    GPIO_SetPin(GPIO1, GPIO_PIN_0_MSK);

    /*配置 P31 ION_DUTYA 引脚为普通GPIO模式*/
    SYS_SET_IOCFG(IOP31CFG, SYS_IOCFG_P31_GPIO);
    GPIO_CONFIG_IO_MODE(GPIO3, GPIO_PIN_1, GPIO_MODE_OUTPUT_PUSH_PULL);
    GPIO_SetPin(GPIO3, GPIO_PIN_1_MSK);

    /******************************** 以上引脚的配置与微电流的控制有关（与门输入控制） ************************************************/
    /* 制冷热的EPWM2配置为普通GPIO，避免开启制冷或者制热，产生冲突 */
    SYS_SET_IOCFG(IOP24CFG, SYS_IOCFG_P24_GPIO);
    GPIO_CONFIG_IO_MODE(GPIO2, GPIO_PIN_4, GPIO_MODE_OUTPUT_PUSH_PULL);
    GPIO_ResetPin(GPIO2, GPIO_PIN_4_MSK);
}





/*****************************************************************************
 ** 函数功能：	设置水电泳的强度函数
    入口参数：  [in] CCPn    : CCP0模块、CCP1模块
               CCPChannel   : CCPxA 、CCPxB
               DutyScale	: 用于计算占空比 (0~250)
    调用说明：
    参数说明：
*****************************************************************************/
void AutoSet_WaterStreng(uint32_t CCPn, uint32_t CCPChannel, uint16_t Streng)
{
    uint16_t Duty, Period;
    uint16_t index;

    if (Streng > 250)
    {
        Streng = 250;
    }

    index = WaterGearArr1[Streng];

    if (index > 500) index = 250;
    Period = CCP_ReadLoad(CCPn);
    Duty = (uint32_t)Period * index / 250;	//设置占空值
    CCP_ConfigCompare(CCPn, CCPChannel, Duty);
}











#if K1_TIMER0
/*****************************************************************************
* 功能:	根据挡位选择水电泳挡位控制的底层数组
* 参数:	这里正弦波和三角波近似等同
*****************************************************************************/
uint16_t   cntIndexForWaterArr_Up = 0;
uint16_t   cntIndexForWaterArr_Down = 0;
void Choose_WaterARR(void)
{

    cntIndexForWaterArr_Up++;

    if(Record.K1_CurrentGears == 0)
	{
		AutoSet_WaterStreng(CCP1, CCPxB, 0);
	}

	else
	{
		if (Flag.cntFlagA == 1)
        {
            SN74HC21D_OpenA();
            EPWM_Start(EPWM_CH_2_MSK);
            EPWM_Stop(EPWM_CH_3_MSK);
            if (cntIndexForWaterArr_Up < (10 + Record.K1_CurrentGears + Record.K1Compensate))
            {
                AutoSet_WaterStreng(CCP1, CCPxB, cntIndexForWaterArr_Up);
            }
            if (cntIndexForWaterArr_Up == (10 + Record.K1_CurrentGears + Record.K1Compensate))
            {
                cntIndexForWaterArr_Down = cntIndexForWaterArr_Up;
            }
            if (cntIndexForWaterArr_Down > 0)
            {
                cntIndexForWaterArr_Down--;
                AutoSet_WaterStreng(CCP1, CCPxB, cntIndexForWaterArr_Down);
                if (cntIndexForWaterArr_Down == 0) {
                    cntIndexForWaterArr_Up = 0;
                    /*计满半个周期，开启另一个半波*/
                    Flag.cntFlagA = 0;
                    Flag.cntFlagB = 1;
                }
            }
        }

        if (Flag.cntFlagB == 1)
        {
            SN74HC21D_OpenB();
            EPWM_Start(EPWM_CH_3_MSK);
            EPWM_Stop(EPWM_CH_2_MSK);
            if (cntIndexForWaterArr_Up < (10 + Record.K1_CurrentGears + Record.K1Compensate))
            {
                AutoSet_WaterStreng(CCP1, CCPxB, cntIndexForWaterArr_Up);
            }
            if (cntIndexForWaterArr_Up == (10 +Record.K1_CurrentGears + Record.K1Compensate))
            {
                cntIndexForWaterArr_Down = cntIndexForWaterArr_Up;
            }
            if (cntIndexForWaterArr_Down > 0)
            {
                cntIndexForWaterArr_Down--;
                AutoSet_WaterStreng(CCP1, CCPxB, cntIndexForWaterArr_Down);
                if (cntIndexForWaterArr_Down == 0) {
                    cntIndexForWaterArr_Up = 0;
                    /*计满半个周期，开启另一个半波*/
                    Flag.cntFlagA = 1;
                    Flag.cntFlagB = 0;
                }
            }
        }
	}


}






/*****************************************************************************
* 功能:	根据挡位选择水电泳挡位控制的底层数组
* 参数: 梯形波
*****************************************************************************/
uint16_t   cntIndexForTrapezoid_Up = 0;
uint16_t   cntIndexForTrapezoid_Down = 0;
void Choose_WaterARR_Trapezoid(void)
{
    cntIndexForTrapezoid_Up++;

      
    if(Record.K1_CurrentGears == 0){
			AutoSet_WaterStreng(CCP1, CCPxB, 0);
		}
		else
		{
			if (Flag.cntFlagA == 1)
			{
					SN74HC21D_OpenA();
					EPWM_Start(EPWM_CH_2_MSK);
					EPWM_Stop(EPWM_CH_3_MSK);
					if (cntIndexForTrapezoid_Up < (10 + Record.K1_CurrentGears + Record.K1Compensate))
					{
							AutoSet_WaterStreng(CCP1, CCPxB, cntIndexForTrapezoid_Up);
					}
					if ((cntIndexForTrapezoid_Up >= (10 + Record.K1_CurrentGears + Record.K1Compensate)) && (cntIndexForTrapezoid_Up < 500))
					{
							cntIndexForTrapezoid_Down = (10 + Record.K1_CurrentGears + Record.K1Compensate);
							AutoSet_WaterStreng(CCP1, CCPxB, cntIndexForTrapezoid_Down);
					}
					if ((cntIndexForTrapezoid_Down > 0) && (cntIndexForTrapezoid_Up >= 500))
					{
							cntIndexForTrapezoid_Down--;
							AutoSet_WaterStreng(CCP1, CCPxB, cntIndexForTrapezoid_Down);
							if (cntIndexForTrapezoid_Down == 0) {
									cntIndexForTrapezoid_Up = 0;
									/*计满半个周期，开启另一个半波*/
									Flag.cntFlagA = 0;
									Flag.cntFlagB = 1;
							}
					}
			}
			if (Flag.cntFlagB == 1)
			{
					SN74HC21D_OpenB();
					EPWM_Start(EPWM_CH_3_MSK);
					EPWM_Stop(EPWM_CH_2_MSK);
					if (cntIndexForTrapezoid_Up < (10 + Record.K1_CurrentGears + Record.K1Compensate))
					{
							AutoSet_WaterStreng(CCP1, CCPxB, cntIndexForTrapezoid_Up);
					}
					if ((cntIndexForTrapezoid_Up >= (10 + Record.K1_CurrentGears + Record.K1Compensate)) && (cntIndexForTrapezoid_Up < 500))
					{
							cntIndexForTrapezoid_Down = (10 + Record.K1_CurrentGears + Record.K1Compensate);
							AutoSet_WaterStreng(CCP1, CCPxB, cntIndexForTrapezoid_Down);
					}
					if ((cntIndexForTrapezoid_Down > 0) && (cntIndexForTrapezoid_Up >= 500))
					{
							cntIndexForTrapezoid_Down--;
							AutoSet_WaterStreng(CCP1, CCPxB, cntIndexForTrapezoid_Down);
							if (cntIndexForTrapezoid_Down == 0) {
									cntIndexForTrapezoid_Up = 0;
									/*计满半个周期，开启另一个半波*/
									Flag.cntFlagA = 1;
									Flag.cntFlagB = 0;
							}
					}
			}
		}

}

#endif 
















