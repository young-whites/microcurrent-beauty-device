#include "cms32f033x.h"
#include "Psfr_system.h"
#include "Psfr_adc.h"

/*****************************************************************************
 ** 功能：	设置ADC运行模式
 ** 参数： [in] ConvertMode:(1)ADC1_CONVERT_SINGLE			
 **							(2)ADC1_CONVERT_CONTINUOUS
 **				ClkDiv :(1)ADC1_CLK_DIV_1
 **						(2)ADC1_CLK_DIV_2
 **						(3)ADC1_CLK_DIV_4
 **						(4)ADC1_CLK_DIV_8
 **						(5)ADC1_CLK_DIV_16
 **						(6)ADC1_CLK_DIV_32
 **						(7)ADC1_CLK_DIV_64
 **						(8)ADC1_CLK_DIV_128
 **				HoldTime:(1)ADC1_HOLD_3P5_CLK
 **						 (2)ADC1_HOLD_4P5_CLK
 **						 (3)ADC1_HOLD_6P5_CLK
 **						 (4)ADC1_HOLD_10P5_CLK
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void ADC1_ConfigRunMode(uint32_t ConvertMode, uint32_t ClkDiv,uint32_t HoldTime)
{
	ADC1->LOCK = ADC1_LOCK_WRITE_KEY;
	ADC1->CON &= ~(ADC1_CON_ADCMS_Msk|ADC1_CON_ADCDIV_Msk|ADC1_CON_ADCSHT_Msk);
	ADC1->CON |= ConvertMode |  ClkDiv | HoldTime;	
	ADC1->LOCK = 0x00;
}

/*****************************************************************************
 ** 功能：	开启ADC1转换通道
 ** 参数： [in] ChannelMask: ADC1_CH_0_MSK ~ ADC1_CH_30_MSK
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void ADC1_EnableScanChannel(uint32_t ChannelMask)
{
	ADC1->LOCK = ADC1_LOCK_WRITE_KEY;	
	ADC1->SCAN |= ChannelMask;
	ADC1->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	关闭ADC1转换通道
 ** 参数： [in] ChannelMask: ADC1_CH_0_MSK ~ ADC1_CH_30_MSK
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void ADC1_DisableScanChannel(uint32_t ChannelMask)
{
	ADC1->LOCK = ADC1_LOCK_WRITE_KEY;	
	ADC1->SCAN &= ~(ChannelMask);
	ADC1->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	设置AN30内部模拟源
 ** 参数： [in] AN30Channel: (1)ADC1_CH_30_BG 
 **							 (2)ADC1_CH_30_OP0O
 **							 (3)ADC1_CH_30_OP1O
 **							 (4)ADC1_CH_30_PGA0O
 **							 (5)ADC1_CH_30_PGA1O
 **							 (6)ADC1_CH_30_VREF_VSS
 **							 (7)ADC1_CH_30_VREF_VDD
 **							 (8)ADC1_CH_30_VSS
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void ADC1_SetAN30Channel(uint32_t AN30Channel)
{
	ADC1->LOCK = ADC1_LOCK_WRITE_KEY;	
	ADC1->CON &= ~(ADC1_CON_ADCICHS_Msk);
	ADC1->CON |= AN30Channel;		
	ADC1->LOCK = 0x00;		
}

/*****************************************************************************
 ** 功能：	获取ADC1转换结果
 ** 参数： [in] Channel: ADC1_CH_0 ~ ADC1_CH_30
 ** 返回：  12 bits Value
 ** 说明：	
*****************************************************************************/
uint32_t ADC1_GetResult(uint32_t Channel)
{
	return(ADC1->DATA[Channel] & 0x0fff);
}

/*****************************************************************************
 ** 功能：	开启硬件触发ADC1转换功能
 ** 参数： [in] TriggerSource: (1)ADC1_TG_EXT_FALLING
 **							(2)ADC1_TG_EXT_RISING
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void ADC1_EnableHardwareTrigger(uint32_t TriggerSource)
{
	ADC1->LOCK = ADC1_LOCK_WRITE_KEY;
	switch(TriggerSource)
	{
		case ADC1_TG_EXT_FALLING:
		case ADC1_TG_EXT_RISING:
			ADC1->HWTG |= TriggerSource;
			break;
		default:
			break;
	}	
	ADC1->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	关闭硬件触发ADC1转换功能
 ** 参数： [in] TriggerSource: (1)ADC1_TG_EXT_FALLING
 **								(2)ADC1_TG_EXT_RISING
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void ADC1_DisableHardwareTrigger(uint32_t TriggerSource)
{
	ADC1->LOCK = ADC1_LOCK_WRITE_KEY;
	switch(TriggerSource)
	{
		case ADC1_TG_EXT_FALLING:
		case ADC1_TG_EXT_RISING:
			ADC1->HWTG &= ~(ADC1_HWTG_ADCEXTEN_Msk);
			break;
		default:
			break;
	}	
	ADC1->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	开启ADC1比较功能
 ** 参数： [in]none
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void ADC1_EnableCompare(void)
{
	ADC1->LOCK = ADC1_LOCK_WRITE_KEY;
	ADC1->CMP0	|= ADC1_CMP0_ADCCMP0EN_Msk;
	ADC1->LOCK = 0x00;		
}

/*****************************************************************************
 ** 功能：	关闭ADC1比较功能
 ** 参数： [in] Compare : ADC1_CMP0 、ADC1_CMP1
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void ADC1_DisableCompare(void)
{
	ADC1->LOCK = ADC1_LOCK_WRITE_KEY;
	ADC1->CMP0	&= ~(ADC1_CMP0_ADCCMP0EN_Msk);
	ADC1->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	配置ADC1比较功能
 ** 参数： [in]  CmpMode:(1)ADC1_CMP_LESS_THAN_RESULT
 **						(2)ADC1_CMP_NOT_LESS_THAN_RESULT
 **				 CmpCount: 0x00~0xf  预设比较次数
 **				 CmpChannel: ADC1_CH_0 ~ ADC1_CH_30
 **				 CmpData : 12 bit 比较值
 ** 返回：  none
 ** 说明：	当内部计数器等于ADCCMPxMCNT+1的值时产生ADC比较事件。
*****************************************************************************/
void ADC1_ConfigCompare( uint32_t CmpMode, uint32_t CmpCount, uint32_t CmpChannel, uint32_t CmpData)
{
	ADC1->LOCK = ADC1_LOCK_WRITE_KEY;
	ADC1->CMP0	&= ~(ADC1_CMP0_ADCCMP0COND_Msk | ADC1_CMP0_ADCCMP0MCNT_Msk|
							 ADC1_CMP0_ADCCMP0CHS_Msk | ADC1_CMP0_ADCCMP0DATA_Msk);
	ADC1->CMP0	|= CmpMode | ((0xf & CmpCount)<<ADC1_CMP0_ADCCMP0MCNT_Pos) | ((0x1f& CmpChannel)<<ADC1_CMP0_ADCCMP0CHS_Pos)|
							(0xfff & CmpData) ;
	ADC1->LOCK = 0x00;		
}

/*****************************************************************************
 ** 功能：	获取ADC1比较器结果
 ** 参数： [in] Compare : ADC1_CMP0 、ADC1_CMP1
 ** 返回：  1 /0
 ** 说明：	
*****************************************************************************/
uint32_t ADC1_GetCompareResult(uint32_t Compare)
{
	return((ADC1->CMP0 & ADC1_CMP0_ADCCMP0O_Msk)? 1:0);
}


/*****************************************************************************
 ** 功能：	开启ADC1
 ** 参数： [in] none
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void ADC1_Start(void)
{
	ADC1->LOCK = ADC1_LOCK_WRITE_KEY;	
	ADC1->CON |= ADC1_CON_ADCEN_Msk;
	ADC1->LOCK = 0x00;		
}

/*****************************************************************************
 ** 功能：	关闭ADC1
 ** 参数： [in] none
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void ADC1_Stop(void)
{
	ADC1->LOCK = ADC1_LOCK_WRITE_KEY;	
	ADC1->CON &= ~(ADC1_CON_ADCEN_Msk);
	ADC1->LOCK = 0x00;		
}

/*****************************************************************************
 ** 功能：	开启ADC1校准
 ** 参数： [in] none
** 返回：  none
 ** 说明：	
*****************************************************************************/
void ADC1_StartAdjust(void)
{
	ADC1->LOCK = ADC1_LOCK_WRITE_KEY;	
	ADC1->CON2 |= (ADC1_CON2_ADCCALEN_Msk);
	ADC1->LOCK = 0x00;	
	while(ADC1->CON2 & ADC1_CON2_ADCCALEN_Msk);
}

/*****************************************************************************
 ** 功能：	开启ADC1带有校准数据的ADC转换
 ** 参数： [in] none
 ** 返回：   none
 ** 说明：	
*****************************************************************************/
void ADC1_EnableAdjust(void)
{
	ADC1->LOCK = ADC1_LOCK_WRITE_KEY;	
	ADC1->CON2 &= ~(ADC1_CON2_ADCCALCONV_Msk);
	ADC1->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	关闭ADC1带有校准数据的ADC转换
 ** 参数： [in] none
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void ADC1_DisableAdjust(void)
{
	ADC1->LOCK = ADC1_LOCK_WRITE_KEY;	
	ADC1->CON2 |= (ADC1_CON2_ADCCALCONV_Msk);
	ADC1->LOCK = 0x00;
}

/*****************************************************************************
 ** 功能：	复位ADC1
 ** 参数： [in] none
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void ADC1_EnableReset(void)
{
	ADC1->LOCK = ADC1_LOCK_WRITE_KEY;	
	ADC1->CON |= ADC1_CON_ADCRST_Msk;
	ADC1->LOCK = 0x00;		
}

/*****************************************************************************
 ** 功能：	获取ADC1 转换错误标志位
 ** 参数： [in] none
 ** 返回：  1 ：转换出现错误	0：无错误
 ** 说明：	
*****************************************************************************/
uint32_t ADC1_GetConvertErrorFlag(void)
{
	return((ADC1->CON & ADC1_CON_ADCCONVER_Msk)? 1:0);	
}

/*****************************************************************************
 ** 功能：	获取ADC1 校准错误标志位
 ** 参数： [in] none
 ** 返回：  1 ：校准出现错误	0：无错误
 ** 说明：	
*****************************************************************************/
uint32_t ADC1_GetAdjustErrorFlag(void)
{
	return((ADC1->CON & ADC1_CON_ADCCALERR_Msk)? 1:0);	
}

/*****************************************************************************
 ** 功能：	清除ADC1 转换错误标志位
 ** 参数： [in] none
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void ADC1_ClearConvertErrorFlag(void)
{
	ADC1->LOCK = ADC1_LOCK_WRITE_KEY;	
	ADC1->CON |= ADC1_CON_ADCCONVERRCLR_Msk;
	ADC1->LOCK = 0x00;		
}

/*****************************************************************************
 ** 功能：	清除ADC1 校准错误标志位
 ** 参数： [in] none
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void ADC1_ClearAdjustErrorFlag(void)
{
	ADC1->LOCK = ADC1_LOCK_WRITE_KEY;	
	ADC1->CON |= ADC1_CON_ADCCALERRCLR_Msk;
	ADC1->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	开启ADC1比较中断
 ** 参数： [in] none
 ** 返回：  none
 ** 说明：	 
*****************************************************************************/
void ADC1_EnableCompareInt(void)
{
	ADC1->LOCK = ADC1_LOCK_WRITE_KEY;	
	ADC1->IMSC |= ADC1_IMSC_IMSC31_Msk;
	ADC1->LOCK = 0x00;		
}

/*****************************************************************************
 ** 功能：	关闭ADC1比较中断
 ** 参数： [in] none
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void ADC1_DisableCompareInt(void)
{
	ADC1->LOCK = ADC1_LOCK_WRITE_KEY;	
	ADC1->IMSC &= ~(ADC1_IMSC_IMSC31_Msk);
	ADC1->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	获取ADC1比较中断使能并且产生中断标志位
 ** 参数： [in] none
 ** 返回：  1:中断使能并且产生中断 0：无中断
 ** 说明：	
*****************************************************************************/
uint32_t ADC1_GetCompareIntFlag(void)
{
	return((ADC1->MIS & ADC1_MIS_MIS31_Msk)? 1:0);
}

/*****************************************************************************
 ** 功能：	清除ADC1比较中断标志位
 ** 参数： [in] none
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void ADC1_ClearCompareIntFlag(void)
{
	ADC1->LOCK = ADC1_LOCK_WRITE_KEY;	
	ADC1->ICLR |= (ADC1_ICLR_ICLR31_Msk);
	ADC1->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	开启ADC1通道n中断
 ** 参数： [in]ChannelMask: ADC1_CH_0_MSK ~ ADC1_CH_30_MSK
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void ADC1_EnableChannelInt(uint32_t ChannelMask)
{
	ADC1->LOCK = ADC1_LOCK_WRITE_KEY;	
	ADC1->IMSC |= ChannelMask ;
	ADC1->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	关闭ADC1通道n中断
 ** 参数： [in] ChannelMask: ADC1_CH_0_MSK ~ ADC1_CH_30_MSK
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void ADC1_DisableChannelInt(uint32_t ChannelMask)
{
	ADC1->LOCK = ADC1_LOCK_WRITE_KEY;	
	ADC1->IMSC &= ~(ChannelMask);
	ADC1->LOCK = 0x00;
}

/*****************************************************************************
 ** 功能：	获取ADC1通道n中断使能并且产生中断标志位
 ** 参数： [in] Channel:ADC1_CH_0 ~ ADC1_CH_30
 ** 返回：  1:中断使能并且产生中断 0：无中断
 ** 说明：	
*****************************************************************************/
uint32_t ADC1_GetChannelIntFlag(uint32_t Channel)
{
	return((ADC1->MIS & (0x1UL<<Channel))? 1:0);	
}

/*****************************************************************************
 ** 功能：	清除ADC1通道n中断标志位
 ** 参数： [in] Channel:ADC1_CH_0 ~ ADC1_CH_30
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void ADC1_ClearChannelIntFlag(uint32_t Channel)
{
	ADC1->LOCK = ADC1_LOCK_WRITE_KEY;	
	ADC1->ICLR |= (0x01UL<< Channel);
	ADC1->LOCK = 0x00;			
}

/*****************************************************************************
 ** 功能：	获取ADC1通道n允许扫描标志位
 ** 参数： [in] Channel:ADC1_CH_0 ~ ADC1_CH_30
 ** 返回：  1:允许扫描 0：禁止扫描
 ** 说明：	
*****************************************************************************/
uint32_t ADC1_GetChannelScanFlag(uint32_t Channel)
{
	return((ADC1->SCAN & (0x1UL<<Channel))? 1:0);	
}

/*****************************************************************************
 ** 功能：	指定AN通道对应管脚配置为模拟方式
 ** 参数： [in] Channel:ADC1_CH_0 ~ ADC1_CH_29
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void ADC1_ConfigPinAN(uint32_t Channel)
{
	switch(Channel)
	{
		case 0:		SYS_SET_IOCFG(IOP00CFG,SYS_IOCFG_P00_AN0);	break;	// P00/AN0  配置为模拟量方式
		case 1:		SYS_SET_IOCFG(IOP01CFG,SYS_IOCFG_P01_AN1);	break;	// P01/AN1  配置为模拟量方式
		case 2:		SYS_SET_IOCFG(IOP04CFG,SYS_IOCFG_P04_AN2);	break;	// P04/AN2  配置为模拟量方式
		case 3:		SYS_SET_IOCFG(IOP05CFG,SYS_IOCFG_P05_AN3);	break;	// P05/AN3  配置为模拟量方式
		case 4:		SYS_SET_IOCFG(IOP06CFG,SYS_IOCFG_P06_AN4);	break;	// P06/AN4  配置为模拟量方式
		case 5:		SYS_SET_IOCFG(IOP07CFG,SYS_IOCFG_P07_AN5);	break;	// P07/AN5  配置为模拟量方式
		case 6:		SYS_SET_IOCFG(IOP10CFG,SYS_IOCFG_P10_AN6);	break;	// P10/AN6  配置为模拟量方式
		case 7:		SYS_SET_IOCFG(IOP12CFG,SYS_IOCFG_P12_AN7);	break;	// P12/AN7  配置为模拟量方式
		case 8:		SYS_SET_IOCFG(IOP13CFG,SYS_IOCFG_P13_AN8);	break;	// P13/AN8  配置为模拟量方式
		case 9:		SYS_SET_IOCFG(IOP14CFG,SYS_IOCFG_P14_AN9);	break;	// P14/AN9  配置为模拟量方式
		case 10:	SYS_SET_IOCFG(IOP15CFG,SYS_IOCFG_P15_AN10);	break;	// P15/AN10 配置为模拟量方式
		case 11:	SYS_SET_IOCFG(IOP16CFG,SYS_IOCFG_P16_AN11);	break;	// P16/AN11 配置为模拟量方式
		case 12:	SYS_SET_IOCFG(IOP17CFG,SYS_IOCFG_P17_AN12);	break;	// P17/AN12 配置为模拟量方式
		case 13:	SYS_SET_IOCFG(IOP21CFG,SYS_IOCFG_P21_AN13);	break;	// P21/AN13 配置为模拟量方式
		case 14:	SYS_SET_IOCFG(IOP22CFG,SYS_IOCFG_P22_AN14);	break;	// P22/AN14 配置为模拟量方式
		case 15:	SYS_SET_IOCFG(IOP23CFG,SYS_IOCFG_P23_AN15);	break;	// P23/AN15 配置为模拟量方式
		case 16:	SYS_SET_IOCFG(IOP24CFG,SYS_IOCFG_P24_AN16);	break;	// P24/AN16 配置为模拟量方式
		case 17:	SYS_SET_IOCFG(IOP25CFG,SYS_IOCFG_P25_AN17);	break;	// P25/AN17 配置为模拟量方式
		case 18:	SYS_SET_IOCFG(IOP26CFG,SYS_IOCFG_P26_AN18);	break;	// P26/AN18 配置为模拟量方式
		case 19:	SYS_SET_IOCFG(IOP30CFG,SYS_IOCFG_P30_AN19);	break;	// P30/AN19 配置为模拟量方式
		case 20:	SYS_SET_IOCFG(IOP31CFG,SYS_IOCFG_P31_AN20);	break;	// P31/AN20 配置为模拟量方式
		case 21:	SYS_SET_IOCFG(IOP32CFG,SYS_IOCFG_P32_AN21);	break;	// P32/AN21 配置为模拟量方式
		case 22:	SYS_SET_IOCFG(IOP34CFG,SYS_IOCFG_P34_AN22);	break;	// P34/AN22 配置为模拟量方式
		case 23:	SYS_SET_IOCFG(IOP35CFG,SYS_IOCFG_P35_AN23);	break;	// P35/AN23 配置为模拟量方式
		case 24:	SYS_SET_IOCFG(IOP36CFG,SYS_IOCFG_P36_AN24);	break;	// P36/AN24 配置为模拟量方式
		case 25:	SYS_SET_IOCFG(IOP40CFG,SYS_IOCFG_P40_AN25);	break;	// P40/AN25 配置为模拟量方式
		case 26:	SYS_SET_IOCFG(IOP43CFG,SYS_IOCFG_P43_AN26);	break;	// P43/AN26 配置为模拟量方式
		case 27:	SYS_SET_IOCFG(IOP44CFG,SYS_IOCFG_P44_AN27);	break;	// P44/AN27 配置为模拟量方式
		case 28:	SYS_SET_IOCFG(IOP46CFG,SYS_IOCFG_P46_AN28);	break;	// P46/AN28 配置为模拟量方式
		case 29:	SYS_SET_IOCFG(IOP47CFG,SYS_IOCFG_P47_AN29);	break;	// P47/AN29 配置为模拟量方式
		default:												break;	// 未定义的通道
	}
}
