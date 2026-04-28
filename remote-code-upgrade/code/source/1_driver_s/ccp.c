/*******************************************************************************
* Copyright (C) 2019 China Micro Semiconductor Limited Company. All Rights Reserved.
*
* This software is owned and published by:
* CMS LLC, No 2609-10, Taurus Plaza, TaoyuanRoad, NanshanDistrict, Shenzhen, China.
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with CMS
* components. This software is licensed by CMS to be adapted only
* for use in systems utilizing CMS components. CMS shall not be
* responsible for misuse or illegal use of this software for devices not
* supported herein. CMS is providing this software "AS IS" and will
* not be responsible for issues arising from incorrect user implementation
* of the software.
*
* This software may be replicated in part or whole for the licensed use,
* with the restriction that this Disclaimer and Copyright notice must be
* included with each copy of this software, whether used in part or whole,
* at all times.
*/

/****************************************************************************/
/** \file ccp.c
**
**	History:
**	
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include "ccp.h"
/****************************************************************************/
/*	Local pre-processor symbols/macros('#define')
*****************************************************************************/

/****************************************************************************/
/*	Global variable definitions(declared in header file with 'extern')
*****************************************************************************/

/****************************************************************************/
/*	Local type definitions('typedef')
*****************************************************************************/

/****************************************************************************/
/*	Local variable  definitions('static')
*****************************************************************************/


/****************************************************************************/
/*	Local function prototypes('static')
*****************************************************************************/

/****************************************************************************/
/*	Function implementation - global ('extern') and local('static')
*****************************************************************************/
/*****************************************************************************
 ** \brief	CCP_Start
 **			开启CCP模块
 ** \param [in] CCPn : CCP0模块、CCP1模块
 ** \return  none
 ** \note	 
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
 ** \brief	CCP_Stop
 **			关闭CCP模块
 ** \param [in] CCPn : CCP0模块、CCP1模块
 ** \return  none
 ** \note	 
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
 ** \brief	CCP_ConfigCLK
 **			设置CCPn模块时钟
 ** \param [in] CCPn : CCP0模块、CCP1模块
 **				ClkDiv:(1) CCP_CLK_DIV_1
 **					   (2) CCP_CLK_DIV_4
 **					   (3) CCP_CLK_DIV_16
 **					   (4) CCP_CLK_DIV_64
 **				ReloadMode: (1) CCP_RELOAD_0XFFFF    计数器重加载值为0xFFFF (连续模式)
 **							(2) CCP_RELOAD_CCPLOAD   计数器器重加载值为CCPxLOAD
 **				Preiod: 0x00~ 0xffff; （建议加载值不为0）		
 ** \return  none
 ** \note	
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
 ** \brief	CCP_EnableRun
 **			CCPn开启
 ** \param [in] CCPn : CCP0模块、CCP1模块
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_EnableRun(uint32_t CCPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->RUN |= (0x1<<CCPn); 					
	CCP->LOCK = 0x00;	
}
/*****************************************************************************
 ** \brief	CCP_DisableRun
 **			CCPn关闭
 ** \param [in] CCPn : CCP0模块、CCP1模块
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_DisableRun(uint32_t CCPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->RUN &= ~(0x1<<CCPn); 					
	CCP->LOCK = 0x00;
}


/*****************************************************************************
 ** \brief	CCP_EnablePWMMode
 **			开启PWM运行模式
 ** \param [in] CCPn : CCP0模块、CCP1模块
 ** \return  none
 ** \note	
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
 ** \brief	CCP_ConfigCompare
 **			设置CCPn模块通道n 比较值
 ** \param [in] CCPn : CCP0模块、CCP1模块
 **				CCPChannel: CCPxA 、CCPxB			
 **				Compare : 0x00~ 0xffff;
 ** \return  none
 ** \note	
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
 ** \brief	CCP_EnableReverseOutput
 **			开启反相输出
 ** \param [in] CCPn : CCP0模块、CCP1模块
 **				CCPChannel: CCPxA 、CCPxB
 ** \return  none
 ** \note	
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
 ** \brief	CCP_DisableReverseOutput
 **			关闭反相输出
 ** \param [in] CCPn : CCP0模块、CCP1模块
 **				CCPChannel: CCPxA 、CCPxB
 ** \return  none
 ** \note	
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
 ** \brief	CCP_EnableOverflowInt
 **			开启溢出中断
 ** \param [in] CCPn : CCP0模块、CCP1模块
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_EnableOverflowInt(uint32_t CCPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->IMSC |= (0x1UL<< (CCPn+4));
	CCP->LOCK = 0x00;		
}
/*****************************************************************************
 ** \brief	CCP_DisableOverflowInt
 **			关闭溢出中断
 ** \param [in] CCPn : CCP0模块、CCP1模块
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_DisableOverflowInt(uint32_t CCPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->IMSC &= ~(0x1UL<< (CCPn+4));
	CCP->LOCK = 0x00;	
}
/*****************************************************************************
 ** \brief	CCP_GetPWMOverflowIntFlag
 **			获取溢出中断使能并溢出中断标志位
 ** \param [in] CCPn : CCP0模块、CCP1模块
 ** \return  1:溢出中断使能并且产生中断  0:无中断
 ** \note	
*****************************************************************************/
uint32_t CCP_GetOverflowIntFlag(uint32_t CCPn)
{
	return((CCP->MIS & (0x1UL<<(CCPn + 4)))? 1:0);
}
/*****************************************************************************
 ** \brief	CCP_ClearPWMOverflowIntFlag
 **			清除溢出中断标志位
 ** \param [in] CCPn : CCP0模块、CCP1模块
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_ClearOverflowIntFlag(uint32_t CCPn)
{
	CCP->ICLR |= (0x1UL<< (CCPn+4));	
}

/*****************************************************************************
 ** \brief	CCP_EnableCompareInt
 **			开启PWM比较中断
 ** \param [in] CCPn : CCP0模块、CCP1模块
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_EnableCompareInt(uint32_t CCPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->IMSC |= (0x1UL<< CCPn);
	CCP->LOCK = 0x00;		
}
/*****************************************************************************
 ** \brief	CCP_DisableCompareInt
 **			关闭比较中断
 ** \param [in] CCPn : CCP0模块、CCP1模块
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_DisableCompareInt(uint32_t CCPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->IMSC &= ~(0x1UL<< CCPn);
	CCP->LOCK = 0x00;
}
/*****************************************************************************
 ** \brief	CCP_GetCompareIntFlag
 **			获取比较中断使能并产生中断标志
 ** \param [in] CCPn : CCP0模块、CCP1模块
 ** \return  none
 ** \note	
*****************************************************************************/
uint32_t  CCP_GetCompareIntFlag(uint32_t CCPn)
{
	return((CCP->MIS & (0x1UL<<CCPn))? 1:0);
}
/*****************************************************************************
 ** \brief	CCP_ClearCompareIntFlag
 **			清除比较中断标志
 ** \param [in] CCPn : CCP0模块、CCP1模块
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_ClearCompareIntFlag(uint32_t CCPn)
{
	CCP->ICLR |= (0x1UL<< CCPn);		
}



/*****************************************************************************
 ** \brief	CCP_EnableCAPMode0
 **			开启捕获模式0
 ** \param [in] CCPn : CCP0模块、CCP1模块
 ** \return  none
 ** \note	
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
 ** \brief	CCP_ConfigCAPMode0
 **			配置捕获模式0 的捕获方式
 ** \param [in] CCPn : CCP0模块、CCP1模块	
 **				Mode : (1)CCP_CAP_RISING
 **					   (2)CCP_CAP_FALLING
 **					   (3)CCP_CAP_RISING_TO_FALLING
 **					   (4)CCP_CAP_FALLING_TO_RISING
 ** \return  none
 ** \note	
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
 ** \brief	CCP_SelCAPMode0Channel
 **			选择捕获模式0 的捕获通道
 ** \param [in] CCPn : CCP0模块、CCP1模块	
**				Channel : CCPxA 、CCPxB 
 ** \return  none
 ** \note	
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
 ** \brief	CCP_GetCAPMode0Result
 **			获取捕获模式0 的捕获通道值
 ** \param [in] CCPn : CCP0模块、CCP1模块	
**				Channel : CCPxA 、CCPxB 
 ** \return  16bit result
 ** \note	
*****************************************************************************/
uint32_t  CCP_GetCAPMode0Result(uint32_t CCPn, uint32_t Channel)
{
	if(CCP0 == CCPn)
	{
		if( CCPxA == Channel)
		{
			return ((uint16_t)CCP->D0A);
		}
		if( CCPxB == Channel)
		{
			return ((uint16_t)CCP->D0B);
		}	
	}
	if(CCP1 == CCPn)
	{
		if( CCPxA == Channel)
		{
			return ((uint16_t)CCP->D1A);
		}
		if( CCPxB == Channel)
		{
			return ((uint16_t)CCP->D1B);
		}
	}	
	return 0;
}

/*****************************************************************************
 ** \brief	CCP_EnableCAPMode0Int
 **			开启捕获模式捕获中断
 ** \param [in] CCPn : CCP0模块、CCP1模块
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_EnableCAPMode0Int(uint32_t CCPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->IMSC |= (0x1UL<< CCPn);
	CCP->LOCK = 0x00;			
}

/*****************************************************************************
 ** \brief	CCP_DisableCAPMode0Int
 **			关闭捕获模式捕获中断
 ** \param [in] CCPn : CCP0模块、CCP1模块
 ** \return  none
 ** \note	
*****************************************************************************/
void CCP_DisableCAPMode0Int(uint32_t CCPn)
{
	CCP->LOCK = CCP_P1A_WRITE_KEY;
	CCP->IMSC &= ~(0x1UL<< CCPn);
	CCP->LOCK = 0x00;
}

/*****************************************************************************
 ** \brief	CCP_GetCAPMode0IntFlag
 **			获取捕获模式捕获中断使能并且产生中断
 ** \param [in] CCPn : CCP0模块、CCP1模块
 ** \return 1：产生中断  0:无中断
 ** \note	
*****************************************************************************/
uint32_t CCP_GetCAPMode0IntFlag(uint32_t CCPn)
{
	return((CCP->MIS & (0x1UL<<CCPn))? 1:0);
}

/*****************************************************************************
 ** \brief	CCP_ClearCAPMode0IntFlag
 **			清除捕获模式捕获中断标志位
 ** \param [in] CCPn : CCP0模块、CCP1模块
 ** \return none
 ** \note	
*****************************************************************************/
void CCP_ClearCAPMode0IntFlag(uint32_t CCPn)
{
	CCP->ICLR |= (0x1UL<< CCPn);	
}




/*****************************************************************************
 ** \brief	CCP_EnableCAPMode1
 **			开启捕获模式1 
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_EnableCAPMode1(void)
{	
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->CAPCON |= CCP_CAPCON_CAPEN_Msk;
	CCP->LOCK = 0x00;		
}
/*****************************************************************************
 ** \brief	CCP_DisableCAPMode1
 **			关闭捕获模式1 
 ** \param [in] none
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_DisableCAPMode1(void)
{	
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->CAPCON &= ~(CCP_CAPCON_CAPEN_Msk);
	CCP->LOCK = 0x00;		
}
/*****************************************************************************
 ** \brief	CCP_EnableCAPnReload
 **			开启捕获模式1下CAPn捕获触发CCP0加载
 ** \param [in] CAPn: CAP0、CAP1、CAP2、CAP3 
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_EnableCAPnReload(uint32_t CAPn)
{	
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->CAPCON |= (0x1<<(CCP_CAPCON_CAP0RLEN_Pos + CAPn));
	CCP->LOCK = 0x00;		
}
/*****************************************************************************
 ** \brief	CCP_DisableCAPnReload
 **			关闭捕获模式1下CAPn捕获触发CCP0加载
 ** \param [in] CAPn: CAP0、CAP1、CAP2、CAP3 
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_DisableCAPnReload(uint32_t CAPn)
{	
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->CAPCON &= ~((0x1<<(CCP_CAPCON_CAP0RLEN_Pos + CAPn)));
	CCP->LOCK = 0x00;		
}

/*****************************************************************************
 ** \brief	CCP_ConfigCAPMode1
 **			配置捕获模式1 通道捕获模式
 ** \param [in] CAPn: CAP0、CAP1、CAP2、CAP3 
 **				Mode:(1)CCP_CAP_MODE1_RISING
 **					 (2)CCP_CAP_MODE1_FALLING
 **					 (3)CCP_CAP_MODE1_BOTH_EDGE
 **					 (4)CCP_CAP_MODE1_DISABLE
 ** \return  none
 ** \note	
*****************************************************************************/
void  CCP_ConfigCAPMode1(uint32_t CAPn, uint32_t Mode)
{	
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->CAPCON &= ~(0x3UL<<(CAPn *2));
	CCP->CAPCON |= (Mode<<(CAPn *2)) ;
	CCP->LOCK = 0x00;		
}

/*****************************************************************************
 ** \brief	CCP_SelCAPMode1Channel
 **			选择捕获模式1 的捕获通道
 ** \param [in] CAPn: CAP0、CAP1、CAP2、CAP3 
 **				Channel :(1) ECAP00 ~ ECAP05
 **						 (2) ECAP10 ~ ECAP15
 **						 (3) CAP0_CCP0A 、CAP1_CCP0B、CAP2_CCP1A、CAP3_CCP1B
 **						 (4) CAP3_ACMP0、CAP3_ACMP1 ：  CAP3通道特有
 ** \return  none
 ** \note	
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
		case ECAP04:
		case ECAP05:
			CCP->CAPCHS &= ~(CCP_CAPCHS_ECAPS_Msk);
			CCP->CAPCHS &= ~(0x0FUL<<(CAPn * 4));
			CCP->CAPCHS |= Channel<<(CAPn * 4);
			break;
		case ECAP10:
		case ECAP11:
		case ECAP12:
		case ECAP13:
		case ECAP14:
		case ECAP15:
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
 ** \brief	CCP_TiggerCAMMode1
 **			写寄存器触发捕获模式1捕获功能
 ** \param [in] CAPn ：CAP0 、CAP1、CAP2 、CAP3
 ** \return  none
 ** \note	 
*****************************************************************************/
void CCP_TiggerCaputer(uint32_t CAPn)
{
	uint32_t * Point;
	Point = (uint32_t *)& CCP->CAP0DAT;
	CCP->LOCK = CCP_P1A_WRITE_KEY;
	*(Point + CAPn) = 0x55AA0000;
	CCP->LOCK = 0x00;
}
/*****************************************************************************
 ** \brief	CCP_GetCAPMode1Result
 **			获取捕获模式1 的捕获通道值
 ** \param [in] CAPn ：CAP0 、CAP1、CAP2 、CAP3	
 ** \return  16bit result
 ** \note	
*****************************************************************************/
uint32_t  CCP_GetCAPMode1Result(uint32_t CAPn)
{
	uint32_t * Point;
	Point = (uint32_t *)&CCP->CAP0DAT;
	return(0xFFFF & Point[CAPn]);
}

/*****************************************************************************
 ** \brief	CCP_EnableCAPMode1Int
 **			开启比较模式1中断
 ** \param [in]CAPn ：CAP0 、CAP1、CAP2 、CAP3	
 ** \return  none
 ** \note	 
*****************************************************************************/
void CCP_EnableCAPMode1Int(uint32_t CAPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->IMSC |= (0x1UL<< (CAPn+8));
	CCP->LOCK = 0x00;
}
/*****************************************************************************
 ** \brief	CCP_DisableCAPMode1Int
 **			关闭比较模式1中断
 ** \param [in]CAPn ：CAP0 、CAP1、CAP2 、CAP3	
 ** \return  none
 ** \note	 
*****************************************************************************/
void CCP_DisableCAPMode1Int(uint32_t CAPn)
{
	CCP->LOCK = CCP_P1AB_WRITE_KEY;
	CCP->IMSC &= ~(0x1UL<< (CAPn+8));
	CCP->LOCK = 0x00;
}
/*****************************************************************************
 ** \brief	CCP_GetCAPMode1IntFlag
 **			获取比较模式1中断使能并且产生中断标志位
 ** \param [in]CAPn ：CAP0 、CAP1、CAP2 、CAP3	
 ** \return  1:中断使能并且产生中断   0：无中断
 ** \note	 
*****************************************************************************/
uint32_t  CCP_GetCAPMode1IntFlag(uint32_t CAPn)
{
	return((CCP->MIS & (0x1UL<<(CAPn+8)))? 1:0);
}
/*****************************************************************************
 ** \brief	CCP_ClearCAPMode1IntFlag
 **			清除比较模式1中断标志位
 ** \param [in]CAPn ：CAP0 、CAP1、CAP2 、CAP3	
 ** \return  none
 ** \note	 
*****************************************************************************/
void  CCP_ClearCAPMode1IntFlag(uint32_t CAPn)
{
	CCP->ICLR |= (0x1UL<< (CAPn+8));
}











