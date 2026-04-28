/*****************************************************************************
* 文件名：	Psfr_opa.c
* 功能：	
* 作者：	陈国华
* 日期：	2021.10.03
* 备注：	
*****************************************************************************/
#include "cms32f033x.h"
#include "Psfr_opa.h"

/*****************************************************************************
 ** 功能：	设置运放运行模式
 ** 参数： [in] opa :OPA0 、OPA1
 **				mode: (1)OPA_FIL_CMP
 **					  (2)OPA_FIL_OPA
 ** 返回：  none
 ** 说明：    
 *****************************************************************************/
void OPA_ConfigRunMode(OPA_T *opa, uint32_t mode)
{
	opa->CON0 &= ~(OPA_CON0_FIL_Msk);
	opa->CON0 |= mode;		
}

/*****************************************************************************
 ** 功能：	设置运放正端输入
 ** 参数： [in] opa :OPA0 、OPA1
 **				PositiveSource:  (1)OPA_POSSEL_P	:OPnP
 **								 (2)OPA_POSSEL_1P2 
 ** 返回：  none
 ** 说明：   默认关闭运放模块
 *****************************************************************************/
void OPA_ConfigPositive(OPA_T *opa, uint32_t PositiveSource)
{
	opa->CON0 &= ~(OPA_CON0_EN_Msk|OPA_CON0_PS_Msk);
	opa->CON0 |= PositiveSource;
}

/*****************************************************************************
 ** 功能：	设置比较器负端输入
 ** 参数： [in] opa :OPA0 、OPA1
 **				NegativeSource:  (1)OPA_NEGSEL_N		:OPnN			
 ** 返回：  none
 ** 说明：  默认关闭运放模块
 *****************************************************************************/
void OPA_ConfigNegative(OPA_T *opa, uint32_t NegativeSource)
{
	opa->CON0 &= ~(OPA_CON0_EN_Msk|OPA_CON0_NS_Msk);
	opa->CON0 |= NegativeSource;
}

/*****************************************************************************
 ** 功能：	设置比较器输出端
 ** 参数： [in] opa :OPA0 、OPA1
 **				OutSource:	OPA_OUTSEL_O
 ** 返回：  none
 ** 说明：  
 *****************************************************************************/
void OPA_ConfigOutput(OPA_T *opa,uint32_t OutSource)
{
	opa->CON0 &= ~(OPA_CON0_EN_Msk|OPA_CON0_OS_Msk);
	opa->CON0 |= OutSource;
}


/********************************************************************************
 ** 功能：	开启OP
 ** 参数： [in] opa :OPA0 、OPA1
 ** 返回：  none
 ** 说明：   
 ******************************************************************************/
void OPA_Start(OPA_T *opa)
{
	opa->CON0 |= OPA_CON0_EN_Msk;
}

/********************************************************************************
 ** 功能：	关闭OP
 ** 参数： [in] opa :OPA0 、OPA1
 ** 返回：  none
 ** 说明：  
 ******************************************************************************/
void OPA_Stop(OPA_T *opa)
{
	opa->CON0 &= ~(OPA_CON0_EN_Msk);
}

/********************************************************************************
 ** 功能：	获取调节模式的调节结果/比较模式的比较结果
 ** 参数： [in] opa :OPA0 、OPA1
 ** 返回：  0/1
 ** 说明：  
 ******************************************************************************/
uint32_t OPA_GetResult(OPA_T *opa)
{
	return((opa->CON1 & OPA_CON1_OUT_Msk)? 1:0);
}

/*****************************************************************************
 ** 功能：	开启运放调节模式
 ** 参数： [in] opa :OPA0 、OPA1
 **				AdjustTarget:    (1)OPA_ADJ_SEL_N		:调节作用于负端输入		
 **								 (2)OPA_ADJ_SEL_P		:调节作用于正端输入	
 ** 返回：  none
 ** 说明：  
 *****************************************************************************/
void OPA_EnableAdjustMode(OPA_T *opa, uint32_t AdjustTarget)
{
	opa->CON1 &= ~(OPA_CON1_CRS_Msk);	
	opa->CON1 |= (AdjustTarget);
	opa->CON0 |= OPA_CON0_COFM_Msk;
}

/*****************************************************************************
 ** 功能：	关闭运放调节模式
 ** 参数： [in] opa :OPA0 、OPA1
 ** 返回：  none
 ** 说明：   
 *****************************************************************************/
void OPA_DisableAdjustMode(OPA_T *opa)
{
	opa->CON0 &= ~(OPA_CON0_COFM_Msk);
}

/*****************************************************************************
 ** 功能：	开启运放Config调节
 ** 参数： [in] opa :OPA0 、OPA1
 ** 返回：  none
 ** 说明：  使用Config调节值
 *****************************************************************************/
void OPA_EnableConfigAdjust(OPA_T *opa)
{
	opa->ADJE = OPA_ADJ_USE_CONFIG;
}

/*****************************************************************************
 ** 功能：	开启运放OPADJ调节
 ** 参数： [in] opa :OPA0 、OPA1
 **				AdjustValue: 0x00 ~ 0x1f
 ** 返回：  none
 ** 说明：  使用OPnADJ寄存器中的OPnADJ<4:0>
 *****************************************************************************/
void OPA_EnableOPADJAdjust(OPA_T *opa, uint32_t AdjustValue)
{
	opa->CON1 &= ~(OPA_CON1_ADJ_Msk);
	opa->CON1 |= AdjustValue & 0x1f;
	opa->ADJE = OPA_ADJ_USE_OPADJ;
}

/*****************************************************************************
 ** 功能：	获取运放调节模式结果
 ** 参数： [in] opa :OPA0 、OPA1
 ** 返回：  Result
 ** 说明：    
 *****************************************************************************/
uint32_t  OPA_GetAutoAdjustResult(OPA_T *opa)
{
	uint32_t AdjVlue, WaitTime, AdjResult;
	OPA_EnableAdjustMode(opa,OPA_ADJ_SEL_P);		/*开启运放, 使用正端输入调节*/
	opa->ADJE = OPA_ADJ_USE_OPADJ;		
	opa->CON1 &= ~(OPA_CON1_ADJ_Msk);
	OPA_Start(opa);									/*开启OPA*/
	for(WaitTime=100; WaitTime>0;WaitTime--);	
	
	AdjResult = OPA_GetResult(opa);
	for(AdjVlue=0; AdjVlue<32;AdjVlue++)
	{
//		opa->CON1 &= ~(OPA_CON1_ADJ_Msk);
		opa->CON1 |= AdjVlue;
		for(WaitTime=100; WaitTime>0;WaitTime--);	
		if(AdjResult != OPA_GetResult(opa))
		{
			OPA_DisableAdjustMode(opa);
			OPA_Stop(opa);		
			return AdjVlue;
		}
	}
	return 0x10;
}


