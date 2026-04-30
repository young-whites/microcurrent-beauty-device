/*****************************************************************************
* 文件名：	Psfr_acmp.c
* 功能：	
* 作者：	陈国华
* 日期：	2021.10.03
* 备注：	
*****************************************************************************/
#include "cms32f033x.h"
#include "Psfr_acmp.h"

/******************************************************************************
 ** 功能：	设置比较器正端输入
 ** 参数： [in] ACMPx: ACMP0 ,ACMP1
 ** 参数： [in] PositiveSource:  (1)ACMP_POSSEL_P0		:C0P0
 **								 (2)ACMP_POSSEL_P1		:C0P1
 **								 (3)ACMP_POSSEL_P2		:C0P2
 **								 (4)ACMP_POSSEL_P3		:C0P3
 ** 返回：  none
 ** 说明：   
*****************************************************************************/
void ACMP_ConfigPositive( uint32_t ACMPx, uint32_t PositiveSource)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		ACMP->C0CON0 &= ~(ACMP_C0CON0_PS_Msk);
		ACMP->C0CON0 |= (PositiveSource<<ACMP_C0CON0_PS_Pos);	
	}
	if( ACMP1 == ACMPx )
	{
		ACMP->C1CON0 &= ~(ACMP_C1CON0_PS_Msk);
		ACMP->C1CON0 |= (PositiveSource<<ACMP_C1CON0_PS_Pos);					
	}
	ACMP->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	设置比较器负端输入
 ** 参数： [in] ACMPx: ACMP0 ,ACMP1
 **				NegativeSource:  (1)ACMP_NEGSEL_N		:CnN  ACMP N输入端口			 
 **								 (2)ACMP_NEGSEL_VREF_VDD_K	:K*VDD电压	
 **								 (3)ACMP_NEGSEL_VREF_BG：	 内部1.2V基准
 **								 (4)ACMP_NEGSEL_VREF_BG_K:	 内部 K*1.2V基准
 **				VBCoefficient: VDD/BG系数K ：0x00~0x0f (V_B_COEFF_2 ... V_B_COEFF_17)
 ** 返回：  none
 ** 说明：   比较器0/1共用基准VDD电压系数
*****************************************************************************/
void ACMP_ConfigNegative( uint32_t ACMPx, uint32_t NegativeSource,uint32_t VBCoefficient)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		ACMP->C0CON0 &= ~(ACMP_C0CON0_NS_Msk);
		if((ACMP_NEGSEL_VREF_VDD_K == NegativeSource) || (ACMP_NEGSEL_VREF_BG  == NegativeSource) ||
			(ACMP_NEGSEL_VREF_BG_K == NegativeSource))
		{
			ACMP->C0CON0 |= 0x01;
		}
	}	
	if( ACMP1 == ACMPx )
	{
		ACMP->C1CON0 &= ~(ACMP_C1CON0_NS_Msk);
		if((ACMP_NEGSEL_VREF_VDD_K == NegativeSource) || (ACMP_NEGSEL_VREF_BG  == NegativeSource) ||
			(ACMP_NEGSEL_VREF_BG_K == NegativeSource))
		{
			ACMP->C1CON0 |= 0x01;
		}
	}	
	
	if((ACMP_NEGSEL_VREF_VDD_K == NegativeSource) || (ACMP_NEGSEL_VREF_BG  == NegativeSource) ||
		(ACMP_NEGSEL_VREF_BG_K == NegativeSource))
	{
		ACMP->CVRCON  &= ~(ACMP_CVRCON_CSVR_Msk | ACMP_CVRCON_CVS_Msk);
		ACMP->CVRCON  |= (NegativeSource<<ACMP_CVRCON_CSVR_Pos) | VBCoefficient;		
	}
	ACMP->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	开启反相输出
 ** 参数： [in] ACMPx: ACMP0 ,ACMP1
 ** 返回：  none
 ** 说明：   
*****************************************************************************/
void  ACMP_EnableReverseOutput( uint32_t ACMPx)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		ACMP->C0CON2 |= (ACMP_C0CON2_POS_Msk);
	}
	if( ACMP1 == ACMPx )
	{
		ACMP->C1CON2 |= (ACMP_C1CON2_POS_Msk);				
	}		
	ACMP->LOCK = 0x00;		
}

/*****************************************************************************
 ** 功能：	关闭反向输出
 ** 参数： [in] ACMPx: ACMP0 ,ACMP1
 ** 返回：  none
 ** 说明：   
******************************************************************************/
void  ACMP_DisableReverseOutput( uint32_t ACMPx)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		ACMP->C0CON2 &= ~(ACMP_C0CON2_POS_Msk);
	}
	if( ACMP1 == ACMPx )
	{
		ACMP->C1CON2 &= ~(ACMP_C1CON2_POS_Msk);				
	}		
	ACMP->LOCK = 0x00;	
}

/******************************************************************************
 ** 功能：	开启滤波
 ** 参数： [in] ACMPx: ACMP0 ,ACMP1
 **				FilterTime: (1)ACMP_NFCLK_1_TSYS .... ACMP_NFCLK_513_TSYS
 ** 返回：  none
 ** 说明：   
******************************************************************************/
void  ACMP_EnableFilter( uint32_t ACMPx, uint32_t FilterTime)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		ACMP->C0CON2 &= ~(ACMP_C0CON2_FS_Msk);
		ACMP->C0CON2 |= ACMP_C0CON2_FE_Msk | (FilterTime & 0xf);
	}
	if( ACMP1 == ACMPx )
	{
		ACMP->C1CON2 &= ~(ACMP_C1CON2_FS_Msk);	
		ACMP->C1CON2 |= ACMP_C1CON2_FE_Msk | (FilterTime & 0xf);		
	}		
	ACMP->LOCK = 0x00;	
}

/******************************************************************************
 ** 功能：	关闭滤波
 ** 参数： [in] ACMPx: ACMP0 ,ACMP1
 ** 返回：  none
 ** 说明：   
*****************************************************************************/
void  ACMP_DisableFilter( uint32_t ACMPx)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		ACMP->C0CON2 &= ~(ACMP_C0CON2_FE_Msk);
	}
	if( ACMP1 == ACMPx )
	{
		ACMP->C1CON2 &= ~(ACMP_C1CON2_FE_Msk);		
	}		
	ACMP->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	开启比较器事件输出
 ** 参数： [in] ACMPx: ACMP0 ,ACMP1
 ** 返回：  none
 ** 说明：   
 *****************************************************************************/
void ACMP_EnableEventOut( uint32_t ACMPx)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		ACMP->CEVCON |=  ACMP_CEVCON_EVE0_Msk;		
	}
	if( ACMP1 == ACMPx )
	{
		ACMP->CEVCON |=  ACMP_CEVCON_EVE1_Msk ;	
	}		
	ACMP->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	设置事件与中断产生模式
 ** 参数： [in] ACMPx: ACMP0 ,ACMP1
 **				EventAndIntMode: 	(1)ACMP_EVENT_INT_RISING 		: ACMPx的输出上升沿触发
 **				 			 		(2)ACMP_EVENT_INT_FALLING 		: ACMPx的输出下降沿触发
 **						     		(3)ACMP_EVENT_INT_BOTH_EDGE 	: ACMPx的输出从0->1的跳变或从1->0的跳变触发
 ** 返回：  none
 ** 说明：   
 *****************************************************************************/
void ACMP_ConfigEventAndIntMode( uint32_t ACMPx, uint32_t EventAndIntMode)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		ACMP->CEVCON &= ~(ACMP_CEVCON_EVS0_Msk);
		ACMP->CEVCON |=  EventAndIntMode;		
	}
	if( ACMP1 == ACMPx )
	{
		ACMP->CEVCON &= ~(ACMP_CEVCON_EVS1_Msk);
		ACMP->CEVCON |= (EventAndIntMode<< ACMP_CEVCON_EVS1_Pos);		
	}		
	ACMP->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	关闭比较器事件输出
 ** 参数： [in] ACMPx: ACMP0 ,ACMP1
 ** 返回：  none
 ** 说明：   
 *****************************************************************************/
void ACMP_DisableEventOut( uint32_t ACMPx)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		ACMP->CEVCON &= ~(ACMP_CEVCON_EVE0_Msk);		
	}
	if( ACMP1 == ACMPx )
	{
		ACMP->CEVCON &= ~(ACMP_CEVCON_EVE1_Msk);	
	}			
	ACMP->LOCK = 0x00;	
}

/******************************************************************************
 ** 功能：	开启ACMP中断
 ** 参数： [in] ACMPx: ACMP0 ,ACMP1
 ** 返回：  none
 ** 说明：   
 *****************************************************************************/
void ACMP_EnableInt(uint32_t ACMPx)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	ACMP->IMSC |=(0x1<<ACMPx);				
	ACMP->LOCK = 0x00;		
}

/*****************************************************************************
 ** 功能：	关闭ACMP中断
 ** 参数： [in] ACMPx: ACMP0 ,ACMP1
 ** 返回：  none
 ** 说明：   
*****************************************************************************/
void ACMP_DisableInt( uint32_t ACMPx)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	ACMP->IMSC &= ~(0x1<<ACMPx);				
	ACMP->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	获取ACMP已使能中断标志
 ** 参数： [in] ACMPx: ACMP0 ,ACMP1
 ** 返回：  0: 无中断产生 ， 1：中断使能并产生中断
 ** 说明：   
******************************************************************************/
uint32_t ACMP_GetIntFlag( uint32_t ACMPx)
{
	return((ACMP->MIS &(0x1<<ACMPx))? 1:0);
}

/*****************************************************************************
 ** 功能：	清ACMP中断
 ** 参数： [in] ACMPx: ACMP0 ,ACMP1
 ** 返回：  none
 ** 说明：   
******************************************************************************/
void ACMP_ClearIntFlag( uint32_t ACMPx)
{
	ACMP->ICLR |=(0x1<<ACMPx);
}

/*****************************************************************************
 ** 功能：	开启ACMP
 ** 参数： [in] ACMPx: ACMP0 ,ACMP1
 ** 返回：  none
 ** 说明：   
******************************************************************************/
void ACMP_Start( uint32_t ACMPx)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		ACMP->C0CON0 |= ACMP_C0CON0_EN_Msk;		
	}
	if( ACMP1 == ACMPx )
	{
		ACMP->C1CON0 |= ACMP_C1CON0_EN_Msk;	
	}			
	ACMP->LOCK = 0x00;		
}

/*****************************************************************************
 ** 功能：	关闭ACMP
 ** 参数： [in] ACMPx: ACMP0 ,ACMP1
 ** 返回：  none
 ** 说明：   
**********************************************************************?******/
void ACMP_Stop( uint32_t ACMPx)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		ACMP->C0CON0 &= ~(ACMP_C0CON0_EN_Msk);		
	}
	if( ACMP1 == ACMPx )
	{
		ACMP->C1CON0 &= ~(ACMP_C1CON0_EN_Msk);	
	}			
	ACMP->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	获取比较器值
 ** 参数： [in] ACMPx: ACMP0 ,ACMP1
 ** 返回：  0/1
 ** 说明：   
 *****************************************************************************/
uint32_t ACMP_GetResult( uint32_t ACMPx)
{
	if( ACMP0 == ACMPx )
	{
		return ((ACMP->C0CON1 & ACMP_C0CON1_OUT_Msk)? 1:0);		
	}
	if( ACMP1 == ACMPx )
	{
		return ((ACMP->C1CON1 & ACMP_C1CON1_OUT_Msk)? 1:0);		
	}
	return 0;
}

/*****************************************************************************
 ** 功能：	使能比较器Config调节
 ** 参数： [in] ACMPx: ACMP0 ,ACMP1
 ** 返回：  none
 ** 说明：   使用内部Config调节值
 *****************************************************************************/
void ACMP_EnableConfigAdjust( uint32_t ACMPx)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		ACMP->C0ADJE = ACMP_ADJ_USE_CONFIG;		
	}
	if( ACMP1 == ACMPx )
	{
		ACMP->C1ADJE = ACMP_ADJ_USE_CONFIG;	
	}			
	ACMP->LOCK = 0x00;		
}

/*****************************************************************************
 ** 功能：	开启比较器CnADJ调节
 ** 参数： [in] ACMPx: ACMP0 ,ACMP1
 **				AdjustValue: 0x00 ~ 0x1f
 ** 返回：  none
 ** 说明：    使用CnCON1 寄存器中的CnADJ<4:0>调节
 *****************************************************************************/
void ACMP_EnableCADJAdjust( uint32_t ACMPx, uint32_t AdjustValue)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		ACMP->C0CON1 &= ~(ACMP_C0CON1_ADJ_Msk);
		ACMP->C0CON1 |= (0x1f & AdjustValue);
		ACMP->C0ADJE = ACMP_ADJ_USE_CNADJE;					
	}
	if( ACMP1 == ACMPx )
	{
		ACMP->C1CON1 &= ~(ACMP_C1CON1_ADJ_Msk);
		ACMP->C1CON1 |= (0x1f & AdjustValue);				
		ACMP->C1ADJE = ACMP_ADJ_USE_CNADJE;	
	}			
	ACMP->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	开启比较器调节模式
 ** 参数： [in] ACMPx: ACMP0 ,ACMP1
 **				AdjustTarget:    (1)ACMP_ADJ_SEL_N		:调节作用于负端输入		
 **								 (2)ACMP_ADJ_SEL_P		:调节作用于正端输入	
 ** 返回：  none
 ** 说明：   
*****************************************************************************/
void ACMP_EnableAdjustMode( uint32_t ACMPx, uint32_t AdjustTarget)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		ACMP->C0CON1 &= ~(ACMP_C0CON1_CRS_Msk);
		ACMP->C0CON1 |= (AdjustTarget<<ACMP_C0CON1_CRS_Pos);
		ACMP->C0CON0 |= (ACMP_C0CON0_COFM_Msk)| ACMP_C0CON0_N2GND_Msk;	/*默认负端接地*/		
	}
	if( ACMP1 == ACMPx )
	{
		ACMP->C1CON1 &= ~(ACMP_C1CON1_CRS_Msk);
		ACMP->C1CON1 |= (AdjustTarget<<ACMP_C1CON1_CRS_Pos);		
		ACMP->C1CON0 |= (ACMP_C1CON0_COFM_Msk)| ACMP_C1CON0_N2GND_Msk;	/*默认负端接地*/		
	}			
	ACMP->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	关闭比较器调节模式
 ** 参数： [in] ACMPx: ACMP0 ,ACMP1
 ** 返回：  none
 ** 说明：   
*****************************************************************************/
void ACMP_DisableAdjustMode( uint32_t ACMPx)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		ACMP->C0CON0 &= ~(ACMP_C0CON0_COFM_Msk);		
	}
	if( ACMP1 == ACMPx )
	{		
		ACMP->C1CON0 &= ~(ACMP_C1CON0_COFM_Msk);		
	}			
	ACMP->LOCK = 0x00;
}

/*****************************************************************************
 ** 功能：	配置比较器迟滞模式
 ** 参数： [in] ACMPx: ACMP0 ,ACMP1
 **				HYSSelect	:(1)ACMP_HYS_0_TO_1	: 输出0->1时迟滞
 **							 (2)ACMP_HYS_1_TO_0	: 输出1->0时迟滞
 **				HYSValue 	:(1)ACMP_HYS_S_0	：0mV 迟滞
 **							 (2)ACMP_HYS_S_10	: 10mV迟滞
 **							 (3)ACMP_HYS_S_20	：20mV迟滞
 **							 (4)ACMP_HYS_S_60	：60mV迟滞 
 ** 返回：  none
 ** 说明：   
*****************************************************************************/
void ACMP_ConfigHYS( uint32_t ACMPx ,uint32_t HYSSelect, uint32_t HYSValue)
{
	ACMP->LOCK = ACMP_WRITE_KEY;
	if( ACMP0 == ACMPx )
	{
		ACMP->C0CON2 &= ~(ACMP_C0CON2_HYSLS_Msk | ACMP_C0CON2_HYSVS_Msk);	
		ACMP->C0CON2 |= (HYSSelect<< ACMP_C0CON2_HYSLS_Pos) | (HYSValue<<ACMP_C0CON2_HYSVS_Pos);
	}
	if( ACMP1 == ACMPx )
	{		
		ACMP->C1CON2 &= ~(ACMP_C1CON2_HYSLS_Msk | ACMP_C1CON2_HYSVS_Msk);
		ACMP->C1CON2 |= (HYSSelect<< ACMP_C1CON2_HYSLS_Pos) | (HYSValue<<ACMP_C1CON2_HYSVS_Pos);			
	}			
	ACMP->LOCK = 0x00;
}

/*****************************************************************************
 ** 功能：	获取比较器自动调节结果
 ** 参数： [in] ACMPx: ACMP0 ,ACMP1
 ** 返回：  8bits
 ** 说明：   此函数自动获取合适的电压调节值
 *****************************************************************************/
uint32_t ACMP_EnableAutoAdjust( uint32_t ACMPx)
{	
	uint32_t  AdjValue;
	uint32_t  AdjResltFlag;
	volatile uint32_t  AdjDelay;

	if( ACMP0 == ACMPx )
	{
		ACMP_EnableAdjustMode(ACMP0,ACMP_ADJ_SEL_P);
		ACMP->LOCK = ACMP_WRITE_KEY;
		ACMP->C0CON1 &= ~(ACMP_C0CON1_ADJ_Msk);
		ACMP->C0ADJE = ACMP_ADJ_USE_CNADJE;	
		ACMP->LOCK = 0x00;
		ACMP_Start(ACMP0);
		for(AdjDelay=100; AdjDelay>0; AdjDelay--);
		
		AdjResltFlag =ACMP_GetResult(ACMP0);
		for(AdjValue =0; AdjValue<32;AdjValue++)
		{
			ACMP->LOCK = ACMP_WRITE_KEY;
//			ACMP->C0CON1 &= ~(ACMP_C0CON1_ADJ_Msk);
			ACMP->C0CON1 |= AdjValue;
			ACMP->LOCK = 0x00;
			for(AdjDelay=100; AdjDelay>0; AdjDelay--);
			
			if(AdjResltFlag != ACMP_GetResult(ACMP0))
			{
				ACMP_DisableAdjustMode(ACMP0);
				ACMP_Stop(ACMP0);
				return  AdjValue;
			}		
		}	
	}
	if( ACMP1 == ACMPx )
	{		
		ACMP_EnableAdjustMode(ACMP1,ACMP_ADJ_SEL_P);
		ACMP->LOCK = ACMP_WRITE_KEY;
		ACMP->C1CON1 &= ~(ACMP_C1CON1_ADJ_Msk);
		ACMP->C1ADJE = ACMP_ADJ_USE_CNADJE;		
		ACMP->LOCK = 0x00;
		ACMP_Start(ACMP1);
		for(AdjDelay=100; AdjDelay>0; AdjDelay--);
		
		AdjResltFlag =ACMP_GetResult(ACMP1);
		for(AdjValue =0; AdjValue<32;AdjValue++)
		{
			ACMP->LOCK = ACMP_WRITE_KEY;
//			ACMP->C1CON1 &= ~(ACMP_C1CON1_ADJ_Msk);
			ACMP->C1CON1 |= AdjValue;
			ACMP->LOCK = 0x00;
			for(AdjDelay=100; AdjDelay>0; AdjDelay--);
			
			if(AdjResltFlag != ACMP_GetResult(ACMP1))
			{
				ACMP_DisableAdjustMode(ACMP1);
				ACMP_Stop(ACMP1);
				return  AdjValue;
			}			
		}				
	}			

	return 0;
}




