#include "cms32f033x.h"
#include "Psfr_ccp.h"

/*****************************************************************************
 ** 功能：	开启CCP模块
 ** 参数： [in] CCPn : CCP0模块、CCP1模块
 ** 返回：  none
 ** 说明：	 
*****************************************************************************/
void CCP_Start(uint32_t CCPn )
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	if(CCP0 == CCPn)
	{
		CCP->CON0 |= (CCP_CCPCON0_CCP0EN_Msk);			
	}
	if(CCP1 == CCPn)
	{
		CCP->CON1 |= (CCP_CCPCON1_CCP1EN_Msk);		
	}	
	CCP->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	关闭CCP模块
 ** 参数： [in] CCPn : CCP0模块、CCP1模块
 ** 返回：  none
 ** 说明：	 
*****************************************************************************/
void CCP_Stop(uint32_t CCPn )
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	if(CCP0 == CCPn)
	{
		CCP->CON0 &= ~(CCP_CCPCON0_CCP0EN_Msk);			
	}
	if(CCP1 == CCPn)
	{
		CCP->CON1 &= ~(CCP_CCPCON1_CCP1EN_Msk);		
	}	
	CCP->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	设置CCPn模块时钟
 ** 参数： [in] CCPn : CCP0模块、CCP1模块
 **				ClkDiv:(1) CCP_CLK_DIV_1
 **					   (2) CCP_CLK_DIV_4
 **					   (3) CCP_CLK_DIV_16
 **					   (4) CCP_CLK_DIV_64
 **				ReloadMode: (1) CCP_RELOAD_0XFFFF    计数器重加载值为0xFFFF (连续模式)
 **							(2) CCP_RELOAD_CCPLOAD   计数器器重加载值为CCPxLOAD
 **				Preiod: 0x00~ 0xffff; （建议加载值不为0）		
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void CCP_ConfigCLK(uint32_t CCPn, uint32_t ClkDiv ,uint32_t ReloadMode, uint32_t Period)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	if(CCP0 == CCPn)
	{
		CCP->CON0 &= ~(CCP_CCPCON0_CCP0PS_Msk);		
		CCP->CON0 |= (ClkDiv<< CCP_CCPCON0_CCP0PS_Pos);
		CCP->LOAD0 = ((uint16_t)Period) | (ReloadMode<< CCP_CCPLOAD0_RELOAD_Pos);		
	}
	if(CCP1 == CCPn)
	{
		CCP->CON1 &= ~(CCP_CCPCON1_CCP1PS_Msk);		
		CCP->CON1 |= (ClkDiv<< CCP_CCPCON1_CCP1PS_Pos);	
		CCP->LOAD1 = ((uint16_t)Period) | (ReloadMode<< CCP_CCPLOAD1_RELOAD_Pos);	
	}	
	CCP->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	读取CCPn模块的重加载值
 ** 参数： [in] CCPn : CCP0模块、CCP1模块
 ** 返回：  重加载值
*****************************************************************************/
uint16_t CCP_ReadLoad(uint32_t CCPn)
{
	uint16_t	Load;
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	if(CCP0 == CCPn)
	{
		Load = CCP->LOAD0;
	}
	if(CCP1 == CCPn)
	{
		Load = CCP->LOAD1;
	}
	CCP->LOCK = 0x00;
	return Load;
}

/*****************************************************************************
 ** 功能：	CCPn开启
 ** 参数： [in] CCPn : CCP0模块、CCP1模块
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void CCP_EnableRun(uint32_t CCPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->RUN |= (0x1<<CCPn); 					
	CCP->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	CCPn关闭
 ** 参数： [in] CCPn : CCP0模块、CCP1模块
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void CCP_DisableRun(uint32_t CCPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->RUN &= ~(0x1<<CCPn); 					
	CCP->LOCK = 0x00;
}


/*****************************************************************************
 ** 功能：	开启PWM运行模式
 ** 参数： [in] CCPn : CCP0模块、CCP1模块
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void CCP_EnablePWMMode(uint32_t CCPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP ->CAPCON &= ~(CCP_CAPCON_CAPEN_Msk);

	if(CCP0 == CCPn)
	{
		CCP->CON0 |= CCP_CCPCON0_CCP0MS_Msk;		/*选择为PWM模式*/
	}
	if(CCP1 == CCPn)
	{
		CCP->CON1 |= CCP_CCPCON1_CCP1MS_Msk;		/*选择为PWM模式*/
	}	
	CCP->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	设置CCPn模块通道n 比较值
 ** 参数： [in] CCPn : CCP0模块、CCP1模块
 **				CCPChannel: CCPxA 、CCPxB			
 **				Compare : 0x00~ 0xffff;
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void CCP_ConfigCompare(uint32_t CCPn, uint32_t CCPChannel ,uint32_t Compare)
{
	CCP->LOCK = CCP_P1A_WRITE_KEY;
	if(CCP0 == CCPn)
	{

		if( CCPxA == CCPChannel)
		{
			CCP->D0A &= ~(CCP_CCPD0A_DATA_Msk); 
			CCP->D0A |= Compare;
		}
		if( CCPxB == CCPChannel)
		{
			CCP->D0B &= ~(CCP_CCPD0B_DATA_Msk); 
			CCP->D0B |= Compare;
		}	
	}
	if(CCP1 == CCPn)
	{
		if( CCPxA == CCPChannel)
		{
			CCP->D1A &= ~(CCP_CCPD1A_DATA_Msk); 
			CCP->D1A |= Compare;
		}
		if( CCPxB == CCPChannel)
		{
			CCP->D1B &= ~(CCP_CCPD1B_DATA_Msk); 
			CCP->D1B |= Compare;
		}
	}	
	CCP->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	设置CCPn模块通道n的占空比百分率
 ** 参数： [in] CCPn : CCP0模块、CCP1模块
 **				CCPChannel: CCPxA 、CCPxB			
 **			    DutyScale	: 占空比百分率 (0~100)
 ** 返回： none
*****************************************************************************/
void CCP_ConfigDutyScale(uint32_t CCPn, uint32_t CCPChannel, uint8_t DutyScale)
{
	uint16_t	Duty,Period;
	
	if(DutyScale>100)	DutyScale=100;
	
	Period = CCP_ReadLoad(CCPn);
	Duty = (uint32_t)Period * DutyScale / 100;	// 计算占空值
	CCP_ConfigCompare(CCPn, CCPChannel ,Duty);
}

/*****************************************************************************
 ** 功能：	开启反相输出
 ** 参数： [in] CCPn : CCP0模块、CCP1模块
 **				CCPChannel: CCPxA 、CCPxB
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void CCP_EnableReverseOutput(uint32_t CCPn, uint32_t CCPChannel)
{
	CCP->LOCK = CCP_P1A_WRITE_KEY;
	if(CCP0 == CCPn)
	{
		if( CCPxA == CCPChannel)
		{
			CCP->D0A |= CCP_CCPD0A_PWM0AOP_Msk;
		}
		if( CCPxB == CCPChannel)
		{
			CCP->D0B |= CCP_CCPD0B_PWM0BOP_Msk;
		}	
	}
	if(CCP1 == CCPn)
	{
		if( CCPxA == CCPChannel)
		{
			CCP->D1A |= CCP_CCPD1A_PWM1AOP_Msk;
		}
		if( CCPxB == CCPChannel)
		{
			CCP->D1B |= CCP_CCPD1B_PWM1BOP_Msk;
		}
	}	
	CCP->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	关闭反相输出
 ** 参数： [in] CCPn : CCP0模块、CCP1模块
 **				CCPChannel: CCPxA 、CCPxB
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void CCP_DisableReverseOutput(uint32_t CCPn, uint32_t CCPChannel)
{
	CCP->LOCK = CCP_P1A_WRITE_KEY;
	if(CCP0 == CCPn)
	{
		if( CCPxA == CCPChannel)
		{
			CCP->D0A &= ~(CCP_CCPD0A_PWM0AOP_Msk);
		}
		if( CCPxB == CCPChannel)
		{
			CCP->D0B &= ~(CCP_CCPD0B_PWM0BOP_Msk);
		}	
	}
	if(CCP1 == CCPn)
	{
		if( CCPxA == CCPChannel)
		{
			CCP->D1A &= ~(CCP_CCPD1A_PWM1AOP_Msk);
		}
		if( CCPxB == CCPChannel)
		{
			CCP->D1B &= ~(CCP_CCPD1B_PWM1BOP_Msk);
		}
	}	
	CCP->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	开启溢出中断
 ** 参数： [in] CCPn : CCP0模块、CCP1模块
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void CCP_EnableOverflowInt(uint32_t CCPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->IMSC |= (0x1UL<< (CCPn+4));
	CCP->LOCK = 0x00;		
}

/*****************************************************************************
 ** 功能：	关闭溢出中断
 ** 参数： [in] CCPn : CCP0模块、CCP1模块
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void CCP_DisableOverflowInt(uint32_t CCPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->IMSC &= ~(0x1UL<< (CCPn+4));
	CCP->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	获取溢出中断使能并溢出中断标志位
 ** 参数： [in] CCPn : CCP0模块、CCP1模块
 ** 返回：  1:溢出中断使能并且产生中断  0:无中断
 ** 说明：	
*****************************************************************************/
uint32_t CCP_GetOverflowIntFlag(uint32_t CCPn)
{
	return((CCP->MIS & (0x1UL<<(CCPn + 4)))? 1:0);
}

/*****************************************************************************
 ** 功能：	清除溢出中断标志位
 ** 参数： [in] CCPn : CCP0模块、CCP1模块
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void CCP_ClearOverflowIntFlag(uint32_t CCPn)
{
	CCP->ICLR |= (0x1UL<< (CCPn+4));	
}

/*****************************************************************************
 ** 功能：	开启PWM比较中断
 ** 参数： [in] CCPn : CCP0模块、CCP1模块
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void CCP_EnableCompareInt(uint32_t CCPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->IMSC |= (0x1UL<< CCPn);
	CCP->LOCK = 0x00;		
}

/*****************************************************************************
 ** 功能：	关闭比较中断
 ** 参数： [in] CCPn : CCP0模块、CCP1模块
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void CCP_DisableCompareInt(uint32_t CCPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->IMSC &= ~(0x1UL<< CCPn);
	CCP->LOCK = 0x00;
}

/*****************************************************************************
 ** 功能：	获取比较中断使能并产生中断标志
 ** 参数： [in] CCPn : CCP0模块、CCP1模块
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
uint32_t  CCP_GetCompareIntFlag(uint32_t CCPn)
{
	return((CCP->MIS & (0x1UL<<CCPn))? 1:0);
}

/*****************************************************************************
 ** 功能：	清除比较中断标志
 ** 参数： [in] CCPn : CCP0模块、CCP1模块
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void CCP_ClearCompareIntFlag(uint32_t CCPn)
{
	CCP->ICLR |= (0x1UL<< CCPn);		
}


/*****************************************************************************
 ** 功能：	开启捕获模式0
 ** 参数： [in] CCPn : CCP0模块、CCP1模块
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void  CCP_EnableCAPMode0(uint32_t CCPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP ->CAPCON &= ~(CCP_CAPCON_CAPEN_Msk);

	if(CCP0 == CCPn)
	{
		CCP->CON0 &= ~(CCP_CCPCON0_CCP0MS_Msk);		
	}
	if(CCP1 == CCPn)
	{
		CCP->CON1 &= ~(CCP_CCPCON1_CCP1MS_Msk);		
	}	
	CCP->LOCK = 0x00;		
}

/*****************************************************************************
 ** 功能：	配置捕获模式0 的捕获方式
 ** 参数： [in] CCPn : CCP0模块、CCP1模块	
 **				Mode : (1)CCP_CAP_RISING
 **					   (2)CCP_CAP_FALLING
 **					   (3)CCP_CAP_RISING_TO_FALLING
 **					   (4)CCP_CAP_FALLING_TO_RISING
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void  CCP_ConfigCAPMode0(uint32_t CCPn, uint32_t Mode)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	if(CCP0 == CCPn)
	{
		CCP->CON0 &= ~(CCP_CCPCON0_CCP0CM0ES_Msk); 
		CCP->CON0 |= Mode;
	}
	if(CCP1 == CCPn)
	{
		CCP->CON1 &= ~(CCP_CCPCON1_CCP1CM0ES_Msk); 
		CCP->CON1 |= Mode;
	}	
	CCP->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	选择捕获模式0 的捕获通道
 ** 参数： [in] CCPn : CCP0模块、CCP1模块	
**				Channel : CCPxA 、CCPxB 
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void  CCP_SelCAPMode0Channel(uint32_t CCPn, uint32_t Channel)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	if(CCP0 == CCPn)
	{
		CCP->CON0 &= ~(CCP_CCPCON0_CCP0CM0CS_Msk); 
		CCP->CON0 |= (Channel<< CCP_CCPCON0_CCP0CM0CS_Pos);
	}
	if(CCP1 == CCPn)
	{
		CCP->CON1 &= ~(CCP_CCPCON1_CCP1CM0CS_Msk); 
		CCP->CON1 |= (Channel<< CCP_CCPCON1_CCP1CM0CS_Pos);
	}	
	CCP->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	获取捕获模式0 的捕获通道值
 ** 参数： [in] CCPn : CCP0模块、CCP1模块	
**				Channel : CCPxA 、CCPxB 
 ** 返回：  16bit result
 ** 说明：	
*****************************************************************************/
uint32_t  CCP_GetCAPMode0Result(uint32_t CCPn, uint32_t Channel)
{
	if(CCP0 == CCPn)
	{
		if( CCPxA == Channel)
		{
			return ((uint16_t)(CCP->D0A));
		}
		if( CCPxB == Channel)
		{
			return ((uint16_t)(CCP->D0B));
		}	
	}
	if(CCP1 == CCPn)
	{
		if( CCPxA == Channel)
		{
			return ((uint16_t)(CCP->D1A));
		}
		if( CCPxB == Channel)
		{
			return ((uint16_t)(CCP->D1B));
		}
	}	
	return 0;
}

/*****************************************************************************
 ** 功能：	开启捕获模式捕获中断
 ** 参数： [in] CCPn : CCP0模块、CCP1模块
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void CCP_EnableCAPMode0Int(uint32_t CCPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->IMSC |= (0x1UL<< CCPn);
	CCP->LOCK = 0x00;			
}

/*****************************************************************************
 ** 功能：	关闭捕获模式捕获中断
 ** 参数： [in] CCPn : CCP0模块、CCP1模块
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void CCP_DisableCAPMode0Int(uint32_t CCPn)
{
	CCP->LOCK = CCP_P1A_WRITE_KEY;
	CCP->IMSC &= ~(0x1UL<< CCPn);
	CCP->LOCK = 0x00;
}

/*****************************************************************************
 ** 功能：	获取捕获模式捕获中断使能并且产生中断
 ** 参数： [in] CCPn : CCP0模块、CCP1模块
 ** 返回： 1：产生中断  0:无中断
 ** 说明：	
*****************************************************************************/
uint32_t CCP_GetCAPMode0IntFlag(uint32_t CCPn)
{
	return((CCP->MIS & (0x1UL<<CCPn))? 1:0);
}

/*****************************************************************************
 ** 功能：	清除捕获模式捕获中断标志位
 ** 参数： [in] CCPn : CCP0模块、CCP1模块
 ** 返回： none
 ** 说明：	
*****************************************************************************/
void CCP_ClearCAPMode0IntFlag(uint32_t CCPn)
{
	CCP->ICLR |= (0x1UL<< CCPn);	
}


/*****************************************************************************
 ** 功能：	开启捕获模式1 
 ** 参数： [in] none
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void  CCP_EnableCAPMode1(void)
{	
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->CAPCON |= CCP_CAPCON_CAPEN_Msk;
	CCP->LOCK = 0x00;		
}

/*****************************************************************************
 ** 功能：	关闭捕获模式1 
 ** 参数： [in] none
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void  CCP_DisableCAPMode1(void)
{	
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->CAPCON &= ~(CCP_CAPCON_CAPEN_Msk);
	CCP->LOCK = 0x00;		
}

/*****************************************************************************
 ** 功能：	开启捕获模式1下CAPn捕获触发CCP0加载
 ** 参数： [in] CAPn: CAP0、CAP1、CAP2、CAP3 
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void  CCP_EnableCAPnReload(uint32_t CAPn)
{	
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->CAPCON |= (0x1<<(CCP_CAPCON_CAP0RLEN_Pos + CAPn));
	CCP->LOCK = 0x00;		
}

/*****************************************************************************
 ** 功能：	关闭捕获模式1下CAPn捕获触发CCP0加载
 ** 参数： [in] CAPn: CAP0、CAP1、CAP2、CAP3 
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void  CCP_DisableCAPnReload(uint32_t CAPn)
{	
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->CAPCON &= ~((0x1<<(CCP_CAPCON_CAP0RLEN_Pos + CAPn)));
	CCP->LOCK = 0x00;		
}

/*****************************************************************************
 ** 功能：	配置捕获模式1 通道捕获模式
 ** 参数： [in] CAPn: CAP0、CAP1、CAP2、CAP3 
 **				Mode:(1)CCP_CAP_MODE1_RISING
 **					 (2)CCP_CAP_MODE1_FALLING
 **					 (3)CCP_CAP_MODE1_BOTH_EDGE
 **					 (4)CCP_CAP_MODE1_DISABLE
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void  CCP_ConfigCAPMode1(uint32_t CAPn, uint32_t Mode)
{	
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->CAPCON &= ~(0x3UL<<(CAPn *2));
	CCP->CAPCON |= (Mode<<(CAPn *2)) ;
	CCP->LOCK = 0x00;		
}

/*****************************************************************************
 ** 功能：	选择捕获模式1 的捕获通道
 ** 参数： [in] CAPn: CAP0、CAP1、CAP2、CAP3 
 **				Channel :(1) ECAP00 ~ ECAP03
 **						 (2) ECAP10 ~ ECAP13
 **						 (3) CAP0_CCP0A 、CAP1_CCP0B、CAP2_CCP1A、CAP3_CCP1B
 **						 (4) CAP3_ACMP0、CAP3_ACMP1 ：  CAP3通道特有
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void  CCP_SelCAPMode1Channel(uint32_t CAPn, uint32_t Channel)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	switch(Channel)
	{
		case ECAP00:
		case ECAP01:
		case ECAP02:	
		case ECAP03:	
			CCP->CAPCHS &= ~(CCP_CAPCHS_ECAPS_Msk);
			CCP->CAPCHS &= ~(0x0FUL<<(CAPn * 4));
			CCP->CAPCHS |= Channel<<(CAPn * 4);
			break;
		case ECAP10:
		case ECAP11:
		case ECAP12:
		case ECAP13:
			CCP->CAPCHS |= CCP_CAPCHS_ECAPS_Msk;
			CCP->CAPCHS &= ~(0x0FUL<<(CAPn * 4));
			CCP->CAPCHS |= (Channel & 0xF)<<(CAPn * 4);
			break;
		case CAP0_CCP0A:
		case CAP1_CCP0B:
		case CAP2_CCP1A:
		case CAP3_CCP1B:
			CCP->CAPCHS &= ~(0x0FUL<<(CAPn * 4));
			CCP->CAPCHS |= (Channel & 0xF)<<(CAPn * 4);		
			 break;
		case CAP3_ACMP0:
		case CAP3_ACMP1:	
			CCP->CAPCHS &= ~(CCP_CAPCHS_CAP3CHS_Msk);
			CCP->CAPCHS |= (Channel & 0xF)<< CCP_CAPCHS_CAP3CHS_Pos;
			break;
		default:
			break;
	}
	CCP->LOCK = 0x00;	
}

/*****************************************************************************
 ** 功能：	写寄存器触发捕获模式1捕获功能
 ** 参数： [in] CAPn ：CAP0 、CAP1、CAP2 、CAP3
 ** 返回：  none
 ** 说明：	 
*****************************************************************************/
void CCP_TriggerCaputer(uint32_t CAPn)
{
	uint32_t * Point;
	Point = (uint32_t *)& CCP->CAP0DAT;
	CCP->LOCK = CCP_P1A_WRITE_KEY;
	*(Point + CAPn) = 0x55AA0000;
	CCP->LOCK = 0x00;
}

/*****************************************************************************
 ** 功能：	获取捕获模式1 的捕获通道值
 ** 参数： [in] CAPn ：CAP0 、CAP1、CAP2 、CAP3	
 ** 返回：  16bit result
 ** 说明：	
*****************************************************************************/
uint32_t  CCP_GetCAPMode1Result(uint32_t CAPn)
{
	uint32_t * Point;
	Point = (uint32_t *)&CCP->CAP0DAT;
	return(0xFFFF & Point[CAPn]);
}

/*****************************************************************************
 ** 功能：	开启捕获模式1中断
 ** 参数： [in]CAPn ：CAP0 、CAP1、CAP2 、CAP3	
 ** 返回：  none
 ** 说明：	 
*****************************************************************************/
void CCP_EnableCAPMode1Int(uint32_t CAPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->IMSC |= (0x1UL<< (CAPn+8));
	CCP->LOCK = 0x00;
}

/*****************************************************************************
 ** 功能：	关闭捕获模式1中断
 ** 参数： [in]CAPn ：CAP0 、CAP1、CAP2 、CAP3	
 ** 返回：  none
 ** 说明：	 
*****************************************************************************/
void CCP_DisableCAPMode1Int(uint32_t CAPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->IMSC &= ~(0x1UL<< (CAPn+8));
	CCP->LOCK = 0x00;
}

/*****************************************************************************
 ** 功能：	获取捕获模式1中断使能并且产生中断标志位
 ** 参数： [in]CAPn ：CAP0 、CAP1、CAP2 、CAP3	
 ** 返回：  1:中断使能并且产生中断   0：无中断
 ** 说明：	 
*****************************************************************************/
uint32_t  CCP_GetCAPMode1IntFlag(uint32_t CAPn)
{
	return((CCP->MIS & (0x1UL<<(CAPn+8)))? 1:0);
}

/*****************************************************************************
 ** 功能：	清除捕获模式1中断标志位
 ** 参数： [in]CAPn ：CAP0 、CAP1、CAP2 、CAP3	
 ** 返回：  none
 ** 说明：	 
*****************************************************************************/
void  CCP_ClearCAPMode1IntFlag(uint32_t CAPn)
{
	CCP->ICLR |= (0x1UL<< (CAPn+8));
}


