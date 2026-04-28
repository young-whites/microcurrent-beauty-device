#include "cms32f033x.h"
#include "Psfr_epwm.h"

/*****************************************************************************
 ** 功能：	设置EPWM的运行模式
 ** 参数： [in] EpwmRunMode: (1)EPWM_COUNT_UP_DOWN
							   (2)EPWM_COUNT_DOWN
							   (3)EPWM_OCU_ASYMMETRIC
							   (4)EPWM_OCU_SYMMETRIC
							   (5)EPWM_WFG_INDEPENDENT
							   (6)EPWM_WFG_COMPLEMENTARY
							   (7)EPWM_WFG_SYNCHRONIZED
							   (8)EPWM_OC_MODE_GROUP
							   (9)EPWM_OC_INDEPENDENT
 ** 返回：  none
 ** 说明：    
*****************************************************************************/
void EPWM_ConfigRunMode(uint32_t EpwmRunMode)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->CON &= ~(EPWM_CON_MODE_Msk | EPWM_CON_GROUNPEN_Msk | EPWM_CON_ASYMEN_Msk | EPWM_CON_CNTTYPE_Msk);
	EPWM->CON |= EpwmRunMode;
	EPWM->LOCK = 0x0;
}

/*****************************************************************************
 ** 功能：	设置EPWM的时钟分频
 ** 参数： [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 
 **				ClkDiv: (1)EPWM_CLK_DIV_1 
 **						(2)EPWM_CLK_DIV_2
 **						(3)EPWM_CLK_DIV_4
 **						(4)EPWM_CLK_DIV_8
 **						(5)EPWM_CLK_DIV_16
 **						(6)EPWM_CLK_DIV_32
 ** 返回：  none	
 ** 说明： 
 **	(1)例因为CLKPSC01共同控制PWM0、PWM1通道 ，所以此函数统一设置CLKPSC01=0x01
 **     若需要其他的分频，请酌情对寄存器赋值.
 **	(2)EPWM 分频分为两级：CLKPSC CLKDIV
 **		第一级分频(CLKPSC)：0x00 预分频停止并且计数器停止 。其他：系统时钟分频系数 CLKPSC+1
 **		第二级分频（CLKDIV): 0x00 FEPWMPSC/2; 0x01 FEPWMPSC/4；0x02:FEPWMPSC/8；0x3 FEPWMPSC/16 0x04 FEPWMPSC ; 其他 Fsys 
 ** 	分频级数：1~4096，例Fsys=8M，EPWM的频率范围：1.95k ~ 8M	   
*****************************************************************************/
void EPWM_ConfigChannelClk(uint32_t EPWMn, uint32_t ClkDiv)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;	
	EPWM->CLKPSC = (0x01<<EPWM_CLKPSC_CLKPSC45_Pos |
					0x01<<EPWM_CLKPSC_CLKPSC23_Pos |
					0x01<<EPWM_CLKPSC_CLKPSC01_Pos);
	EPWM->CLKDIV &= ~(0xfUL <<(EPWMn*4));
	EPWM->CLKDIV |= (ClkDiv <<(EPWMn*4));		
	EPWM->LOCK = 0x0;
}

/*****************************************************************************
 ** 功能：	设置EPWM的时钟分频
 ** 参数： [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 
 **				ClkDiv: (1/2/4/8/16/32)
 ** 返回：  none	
 ** 说明： 
 **	(1)例因为CLKPSC01共同控制PWM0、PWM1通道 ，所以此函数统一设置CLKPSC01=0x01
 **     若需要其他的分频，请酌情对寄存器赋值.
 **	(2)EPWM 分频分为两级：CLKPSC CLKDIV
 **		第一级分频(CLKPSC)：0x00 预分频停止并且计数器停止 。其他：系统时钟分频系数 CLKPSC+1
 **		第二级分频（CLKDIV): 0x00 FEPWMPSC/2; 0x01 FEPWMPSC/4；0x02:FEPWMPSC/8；0x3 FEPWMPSC/16 0x04 FEPWMPSC ; 其他 Fsys 
 ** 	分频级数：1~4096，例Fsys=8M，EPWM的频率范围：1.95k ~ 8M	   
*****************************************************************************/
void EPWM_ConfigChannelDiv(uint32_t EPWMn, uint32_t Div)
{
	uint32_t	ClkDiv;
	switch(Div){
		default:
		case  1:	ClkDiv = EPWM_CLK_DIV_1;	break;
		case  2:	ClkDiv = EPWM_CLK_DIV_2;	break;
		case  4:	ClkDiv = EPWM_CLK_DIV_4;	break;
		case  8:	ClkDiv = EPWM_CLK_DIV_8;	break;
		case 16:	ClkDiv = EPWM_CLK_DIV_16;	break;
		case 32:	ClkDiv = EPWM_CLK_DIV_32;	break;
	}
	EPWM_ConfigChannelClk(EPWMn,ClkDiv);
}

/*****************************************************************************
 ** 功能：	设置EPWM通道的周期
 ** 参数： [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 
 **				Period: 16位周期值
 ** 返回： none
 ** 说明：    
*****************************************************************************/
void EPWM_ConfigChannelPeriod(uint32_t EPWMn, uint32_t Period)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->PERIOD[EPWMn] = 0xffff & (Period);	//传入的Period等于2000时，计算的重装载值是65535
	EPWM->CON3 |= (0x1UL <<(EPWMn + EPWM_CON3_LOADEN0_Pos));		/*使能加载周期&&占空比*/
	EPWM->LOCK = 0x0;
}

/*****************************************************************************
 ** 功能：	设置对称模式下的EPWM通道的占空比百分率
 ** 参数： [in] EPWMn 		: EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 
 **			    DutyScale		 : 占空比百分率 (0~100)
 ** 返回： none
 ** 说明：	根据占空比自动启动或停止EPWM。
*****************************************************************************/
void EPWM_ConfigChannelSymDutyScale(uint32_t EPWMn,uint8_t DutyScale)
{
	uint16_t	Duty,Period;
	
	if(DutyScale>100)	DutyScale=100;
	
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	
	Period = EPWM->PERIOD[EPWMn];								// 读取周期值(对应STM32的ARR的重装载计数的值)
	Duty = (uint32_t)Period * DutyScale / 100;	// 计算占空值
	if(EPWM->CON & EPWM_COUNT_UP_DOWN){					// 中心对齐方式
		Duty = Period - Duty;
	}
	
	EPWM->CMPADT[EPWMn] &= 0xffff0000;
	EPWM->CMPADT[EPWMn] |= 0xffff & (Duty);
	EPWM->CON3 |= (0x1UL <<(EPWMn +EPWM_CON3_LOADEN0_Pos));		/*使能加载周期&&占空比*/
	EPWM->LOCK = 0x0;
	
//	if(DutyScale)	EPWM_Start(0x01 << EPWMn);
//	else			EPWM_Stop(0x01 << EPWMn);
}

/*****************************************************************************
 ** 功能：	设置对称模式下的EPWM通道的占空比
 ** 参数： [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 
 **			    Duty  : 16位比较值
 ** 返回： none
 ** 说明：   
*****************************************************************************/
void EPWM_ConfigChannelSymDuty(uint32_t EPWMn,uint32_t Duty)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->CMPADT[EPWMn] &= 0xffff0000;
	EPWM->CMPADT[EPWMn] |= 0xffff & (Duty);
	EPWM->CON3 |= (0x1UL <<(EPWMn +EPWM_CON3_LOADEN0_Pos));		/*使能加载周期&&占空比*/
	EPWM->LOCK = 0x0;
}

/*****************************************************************************
 ** 功能：	设置非对称模式下的 EPWM通道的占空比
 ** 参数： [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 
 **			    UpCmp 	  : 16位向上比较值
 **			    DownCmp   : 16位向下比较值
 ** 返回： none
 ** 说明：   
*****************************************************************************/
void EPWM_ConfigChannelAsymDuty(uint32_t EPWMn,uint32_t UpCmp, uint32_t DownCmp)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->CMPADT[EPWMn] = (0xffff & (UpCmp)) | ((0xffff & (DownCmp))<<16);
	EPWM->CON3 |= (0x1UL <<(EPWMn +EPWM_CON3_LOADEN0_Pos));		/*使能加载周期&&占空比*/
	EPWM->LOCK = 0x0;
}

/*****************************************************************************
 ** 功能：	使能单次模式
 ** 参数： [in] EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** 返回： none
 ** 说明：   
*****************************************************************************/
void EPWM_EnableOneShotMode(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->CON &= ~(EPWMnMask); 
	EPWM->LOCK = 0x0;
}

/*****************************************************************************
 ** 功能：	使能连续加载模式
 ** 参数： [in] EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** 返回： none
 ** 说明：   
*****************************************************************************/
void EPWM_EnableAutoLoadMode(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->CON |= EPWMnMask; 
	EPWM->LOCK = 0x0;
}

/*****************************************************************************
 ** 功能：	配置加载方式以及中断方式
 ** 参数： [in]  EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 
 **				 Mode: (1)EPWM_EACH_PERIOD_ZERO
 **					   (2)EPWM_EACH_ZERO
 **					   (3)EPWM_FIRST_ZERO_NEXT_PERIOD
 **					   (4)EPWM_EVERY_TWO_ZERO
 ** 返回： none
 ** 说明：   
*****************************************************************************/
void EPWM_ConfigLoadAndIntMode(uint32_t EPWMn, uint32_t Mode)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->CON3 &= ~(0x3UL<< ((EPWMn*2) + EPWM_CON3_LOADTYP0_Pos));
	EPWM->CON3 |= Mode << ((EPWMn*2) + EPWM_CON3_LOADTYP0_Pos);	
	EPWM->LOCK = 0x0;			
}

/*****************************************************************************
 ** 功能：	开启反相输出
 ** 参数： [in] EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** 返回： none
 ** 说明：   
*****************************************************************************/
void EPWM_EnableReverseOutput(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->CON |= (EPWMnMask<< EPWM_CON_PINV0_Pos); 
	EPWM->LOCK = 0x0;	
}

/*****************************************************************************
 ** 功能：	关闭反相输出
 ** 参数： [in] EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** 返回： none
 ** 说明：   
*****************************************************************************/
void EPWM_DisableReverseOutput(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->CON &= ~(EPWMnMask<< EPWM_CON_PINV0_Pos); 
	EPWM->LOCK = 0x0;	
}

/*****************************************************************************
 ** 功能：	开启死区
 ** 参数： [in] EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 **				DeadZone : 0x01 ~ 0x400;
 ** 返回： none
 ** 说明：   
*****************************************************************************/
void EPWM_EnableDeadZone(uint32_t EPWMnMask, uint32_t DeadZone)
{
	
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	if(( EPWMnMask & EPWM_CH_0_MSK) || (EPWMnMask & EPWM_CH_1_MSK))
	{
		EPWM->CON |= EPWM_CON_ENDT01_Msk;
		EPWM->DTCTL &= ~(EPWM_DTCTL_DTI01_Msk);
		EPWM->DTCTL |= 0x3ff & (DeadZone-1);
	}
	if(( EPWMnMask & EPWM_CH_2_MSK) || (EPWMnMask & EPWM_CH_3_MSK))
	{
		EPWM->CON |= EPWM_CON_ENDT23_Msk;
		EPWM->DTCTL &= ~(EPWM_DTCTL_DTI23_Msk);
		EPWM->DTCTL |=  ((0x3ff & (DeadZone-1))<<EPWM_DTCTL_DTI23_Pos);
	}	
	if(( EPWMnMask & EPWM_CH_4_MSK) || (EPWMnMask & EPWM_CH_5_MSK))
	{
		EPWM->CON |= EPWM_CON_ENDT45_Msk;
		EPWM->DTCTL &= ~(EPWM_DTCTL_DTI45_Msk);
		EPWM->DTCTL |= (0x3ff & (DeadZone-1)) << EPWM_DTCTL_DTI45_Pos;
	}	
	EPWM->LOCK = 0x0;	
}

/*****************************************************************************
 ** 功能：	关闭死区
 ** 参数： [in] EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** 返回： none
 ** 说明：   
*****************************************************************************/
void EPWM_DisableDeadZone(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	if(( EPWMnMask & EPWM_CH_0_MSK) || (EPWMnMask & EPWM_CH_1_MSK))
	{
		EPWM->CON &= ~(EPWM_CON_ENDT01_Msk);
	}
	if(( EPWMnMask & EPWM_CH_2_MSK) || (EPWMnMask & EPWM_CH_3_MSK))
	{
		EPWM->CON &= ~(EPWM_CON_ENDT23_Msk);
	}	
	if(( EPWMnMask & EPWM_CH_4_MSK) || (EPWMnMask & EPWM_CH_5_MSK))
	{
		EPWM->CON &= ~(EPWM_CON_ENDT45_Msk);
	}	
	EPWM->LOCK = 0x0;	
}


/*****************************************************************************
 ** 功能：	开启EPWM通道输出
 ** 参数： [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** 返回： none
 ** 说明：  
*****************************************************************************/
void EPWM_EnableOutput(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->POEN |= EPWMnMask;
	EPWM->LOCK = 0x0;
}

/*****************************************************************************
 ** 功能：	关闭EPWM通道输出
 ** 参数： [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** 返回： none
 ** 说明：   
*****************************************************************************/
void EPWM_DisableOutput(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->POEN &= ~(EPWMnMask);
	EPWM->LOCK = 0x0;	
}

/*****************************************************************************
 ** 功能：	开启EPWM
 ** 参数： [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** 返回： none
 ** 说明：   
*****************************************************************************/
void EPWM_Start(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->CON2 |= EPWMnMask;	
	EPWM->LOCK = 0x0;
}

/*****************************************************************************
 ** 功能：	关闭EPWM
 ** 参数： [in] EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** 返回： none
 ** 说明：   
*****************************************************************************/
void EPWM_Stop(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->CON2 &= ~(EPWMnMask);			
	EPWM->CON3 |= EPWMnMask;			/*计数清0*/
	EPWM->LOCK = 0x0;
}

/*****************************************************************************
 ** 功能：	强制关闭EPWM
 ** 参数： [in]  none
 ** 返回： none
 ** 说明：   使用掩码控制EPWM
*****************************************************************************/
void EPWM_ForceStop(void)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	
	EPWM->LOCK = 0x0;	
}

/*****************************************************************************
 ** 功能：	开启向上比较中断
 ** 参数： [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 	
 ** 返回： none
 ** 说明：  
*****************************************************************************/
void  EPWM_EnableUpCmpInt(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->IMSC |= (EPWMnMask<<EPWM_IMSC_ENUIFn_Pos);
	EPWM->LOCK = 0x0;		
}

/*****************************************************************************
 ** 功能：	关闭向上比较中断
 ** 参数： [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 	
 ** 返回： none
 ** 说明：  
*****************************************************************************/
void  EPWM_DisableUpCmpInt(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->IMSC &= ~(EPWMnMask<<EPWM_IMSC_ENUIFn_Pos);
	EPWM->LOCK = 0x0;	
}

/*****************************************************************************
 ** 功能：	获取向上比较中断使能并产生中断标志位
 ** 参数： [in]EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 	
 ** 返回： 1：中断使能并且产生中断  0：无中断
 ** 说明：  
*****************************************************************************/
uint32_t  EPWM_GetUpCmpIntFlag(uint32_t EPWMn)
{
	return ((EPWM->MIS&(0x01UL<<(EPWMn+EPWM_MIS_UIFn_Pos)))? 1:0);
}

/*****************************************************************************
 ** 功能：	清除向上比较中断标志位
 ** 参数： [in]EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 	
 ** 返回： none
 ** 说明：  
*****************************************************************************/
void  EPWM_ClearUpCmpIntFlag(uint32_t EPWMn)
{
	EPWM->ICLR |= (0x1UL<<(EPWMn+EPWM_ICLR_UIFn_Pos));
}

/*****************************************************************************
 ** 功能：	开启向下比较中断
 ** 参数： [in] EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** 返回： none
 ** 说明：  
*****************************************************************************/
void  EPWM_EnableDownCmpInt(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->IMSC |= (EPWMnMask<<EPWM_IMSC_ENDIFn_Pos);
	EPWM->LOCK = 0x0;		
}

/*****************************************************************************
 ** 功能：	关闭向下比较中断
 ** 参数： [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** 返回： none
 ** 说明：  
*****************************************************************************/
void  EPWM_DisableDownCmpInt(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->IMSC &= ~(EPWMnMask<<EPWM_IMSC_ENDIFn_Pos);
	EPWM->LOCK = 0x0;
}

/*****************************************************************************
 ** 功能：	获取向下比较中断使能并产生中断标志位
 ** 参数： [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 	
 ** 返回： 1：中断使能并且产生中断  0：无中断
 ** 说明：  
*****************************************************************************/
uint32_t  EPWM_GetDownCmpIntFlag(uint32_t EPWMn)
{
	return ((EPWM->MIS&(0x01UL<<(EPWMn+EPWM_MIS_DIFn_Pos)))? 1:0);
}

/*****************************************************************************
 ** 功能：	清除向下比较中断标志位
 ** 参数： [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 	
 ** 返回： none
 ** 说明：  
*****************************************************************************/
void  EPWM_ClearDownCmpIntFlag(uint32_t EPWMn)
{
	EPWM->ICLR |= (0x1UL<<(EPWMn+EPWM_ICLR_DIFn_Pos));	
}

/*****************************************************************************
 ** 功能：	开启周期中断
 ** 参数： [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 	
 ** 返回： none
 ** 说明：  
*****************************************************************************/
void  EPWM_EnablePeriodInt(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->IMSC |= (EPWMnMask<<EPWM_IMSC_ENPIFn_Pos);
	EPWM->LOCK = 0x0;		
}

/*****************************************************************************
 ** 功能：	关闭周期中断
 ** 参数： [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 	
 ** 返回： none
 ** 说明：  
*****************************************************************************/
void  EPWM_DisablePeriodInt(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->IMSC &= ~(EPWMnMask<<EPWM_IMSC_ENPIFn_Pos);
	EPWM->LOCK = 0x0;		
}

/*****************************************************************************
 ** 功能：	获取周期中断使能并产生中断标志位
 ** 参数： [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 	
 ** 返回： 1：中断使能并且产生中断  0：无中断
 ** 说明：  
*****************************************************************************/
uint32_t  EPWM_GetPeriodIntFlag(uint32_t EPWMn)
{
	return ((EPWM->MIS&(0x01UL<<(EPWMn+EPWM_MIS_PIFn_Pos)))? 1:0);
}

/*****************************************************************************
 ** 功能：	清除周期中断标志位
 ** 参数： [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 	
 ** 返回： none
 ** 说明：  
*****************************************************************************/
void  EPWM_ClearPeriodIntFlag(uint32_t EPWMn)
{
	EPWM->ICLR |= (0x1UL<<(EPWMn+EPWM_ICLR_PIFn_Pos));
}

/*****************************************************************************
 ** 功能：	开启零点中断
 ** 参数： [in]  EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 
 ** 返回： none
 ** 说明：  
*****************************************************************************/
void  EPWM_EnableZeroInt(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->IMSC |= (EPWMnMask<<EPWM_IMSC_ENZIFn_Pos);
	EPWM->LOCK = 0x0;	
}

/*****************************************************************************
 ** 功能：	关闭零点中断
 ** 参数： [in] EPWMnMask : EPWM_CH_0_MSK 、EPWM_CH_1_MSK 、EPWM_CH_2_MSK 、
 **							 EPWM_CH_3_MSK 、EPWM_CH_4_MSK 、EPWM_CH_5_MSK 	
 ** 返回： none
 ** 说明：  
*****************************************************************************/
void  EPWM_DisableZeroInt(uint32_t EPWMnMask)
{
	EPWM->LOCK = EPWM_LOCK_P1B_WRITE_KEY;
	EPWM->IMSC &= ~(EPWMnMask<<EPWM_IMSC_ENZIFn_Pos);
	EPWM->LOCK = 0x0;	
}

/*****************************************************************************
 ** 功能：	获取零点中断使能并产生中断标志位
 ** 参数： [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 	
 ** 返回： 1：中断使能并且产生中断  0：无中断
 ** 说明：  
*****************************************************************************/
uint32_t  EPWM_GetZeroIntFlag(uint32_t EPWMn)
{
	return ((EPWM->MIS&(0x01UL<<(EPWMn+EPWM_MIS_ZIFn_Pos)))? 1:0);
}

/*****************************************************************************
 ** 功能：	清除零点中断标志位
 ** 参数： [in] EPWMn : EPWM0 、EPWM1 、EPWM2 、EPWM3 、EPWM4 、EPWM5 	
 ** 返回： none
 ** 说明：  
*****************************************************************************/
void  EPWM_ClearZeroIntFlag(uint32_t EPWMn)
{
	EPWM->ICLR |= (0x1UL<<(EPWMn+EPWM_ICLR_ZIFn_Pos));
}
