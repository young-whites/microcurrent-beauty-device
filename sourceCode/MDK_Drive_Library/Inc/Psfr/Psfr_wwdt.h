/*****************************************************************************
* 文件名：	Psfr_wwdt.c
* 功能：	
* 作者：	陈国华
* 日期：	2021.10.03
* 备注：	
*****************************************************************************/
#ifndef _PREG_WWDT_H_
#define _PREG_WWDT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "cms32f033x.h"

/*----------------------------------------------------------------------------
 **WWDT 时钟
-----------------------------------------------------------------------------*/
#define		WWDT_CLK_DIV_2			(0x00UL)		/*Fwwd = FAPB / 2*/
#define		WWDT_CLK_DIV_4			(0x01UL<<WWDT_WWDTCON_PSCSEL_Pos)	/*Fwwd = FAPB / 4*/		
#define		WWDT_CLK_DIV_8			(0x02UL<<WWDT_WWDTCON_PSCSEL_Pos)	/*Fwwd = FAPB / 8*/		
#define		WWDT_CLK_DIV_16			(0x03UL<<WWDT_WWDTCON_PSCSEL_Pos)	/*Fwwd = FAPB / 16*/
#define		WWDT_CLK_DIV_32			(0x04UL<<WWDT_WWDTCON_PSCSEL_Pos)	/*Fwwd = FAPB / 32*/
#define		WWDT_CLK_DIV_64			(0x05UL<<WWDT_WWDTCON_PSCSEL_Pos)	/*Fwwd = FAPB / 64*/
#define		WWDT_CLK_DIV_128		(0x06UL<<WWDT_WWDTCON_PSCSEL_Pos)	/*Fwwd = FAPB / 128*/	
#define		WWDT_CLK_DIV_256		(0x07UL<<WWDT_WWDTCON_PSCSEL_Pos)	/*Fwwd = FAPB / 256*/
#define		WWDT_CLK_DIV_512		(0x08UL<<WWDT_WWDTCON_PSCSEL_Pos)	/*Fwwd = FAPB / 512*/
#define		WWDT_CLK_DIV_1024		(0x09UL<<WWDT_WWDTCON_PSCSEL_Pos)	/*Fwwd = FAPB / 512*/
#define		WWDT_CLK_DIV_2048		(0x0aUL<<WWDT_WWDTCON_PSCSEL_Pos)	/*Fwwd = FAPB / 2048*/
#define		WWDT_CLK_DIV_4096		(0x0bUL<<WWDT_WWDTCON_PSCSEL_Pos)	/*Fwwd = FAPB / 4096*/
#define		WWDT_CLK_DIV_8192		(0x0cUL<<WWDT_WWDTCON_PSCSEL_Pos)	/*Fwwd = FAPB / 8192*/
#define		WWDT_CLK_DIV_16384		(0x0dUL<<WWDT_WWDTCON_PSCSEL_Pos)	/*Fwwd = FAPB / 16384*/
/*----------------------------------------------------------------------------
 **WWDT 重载
-----------------------------------------------------------------------------*/
#define		WWDT_RL_WRITE_KEY		(0x55AAUL)

/*****************************************************************************
 ** 功能：	WWDT在DEBUG模式中使能
 ** 参数： [in] none
 ** 返回：  none
 ** 说明：	 
*****************************************************************************/
#define		WWDT_ENABLE_DEBUG()	do{\
									WWDT->CON |= WWDT_WWDTCON_DEBUG_Msk;\
								}while(0)
/*****************************************************************************
 ** 功能：	WWDT在DEBUG模式中关闭
 ** 参数： [in] none
 ** 返回：  none
 ** 说明：	 
*****************************************************************************/
#define		WWDT_DISABLE_DEBUG() do{\
									WWDT->CON &= ~(WWDT_WWDTCON_DEBUG_Msk);\
								}while(0)

/*****************************************************************************
 ** 功能：	获取WWDT中断源状态
 ** 参数： [in] none
 ** 返回：  1:产生WWDT向下溢出中断 0：无中断
 ** 说明：	 
*****************************************************************************/
#define		WWDT_GET_RIS_FLAG()	(((WWDT->RIS & WWDT_WWDTRIS_WWDTRIS_Msk)? 1:0))
							
/*****************************************************************************
 ** 功能：	开启WWDT模块
 ** 参数： [in] none
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void WWDT_Start(void);
								
/*****************************************************************************
 ** 功能：	关闭WWDT模块
 ** 参数： [in] none
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void WWDT_Stop(void);

/*****************************************************************************
 ** 功能：	配置WWDT时钟
 ** 参数： [in] ClkDiv: (1)WWDT_CLK_DIV_2   
 **						(2)WWDT_CLK_DIV_4  
 **						(3)WWDT_CLK_DIV_8 
 **						(4)WWDT_CLK_DIV_16  
 **						(5)WWDT_CLK_DIV_32
 **						(6)WWDT_CLK_DIV_64  
 **						(7)WWDT_CLK_DIV_128 
 **						(8)WWDT_CLK_DIV_256  
 **						(9)WWDT_CLK_DIV_512 
 **						(10)WWDT_CLK_DIV_1024  
 **						(11)WWDT_CLK_DIV_2048 
 **						(12)WWDT_CLK_DIV_4096  
 **						(13)WWDT_CLK_DIV_8192 
 **						(14)WWDT_CLK_DIV_16384  
 ** 返回：  none
 ** 说明：	WWDT 计数器从0x3F 计数到0 时溢出的时间：(PSCSEL*1024*64)*TAPBCLK
*****************************************************************************/
void WWDT_ConfigClk(uint32_t ClkDiv);

/*****************************************************************************
 ** 功能：	配置WWDT比较值
 ** 参数： [in] Compare: 6BitValue
 ** 返回：  none
 ** 说明：	 
*****************************************************************************/
void WWDT_ConfigCompare(uint32_t Compare);

/*****************************************************************************
 ** 功能：	开启WWDT比较中断
 ** 参数： [in] none
 ** 返回：  none
 ** 说明：	 
*****************************************************************************/
void WWDT_EnableCompareInt(void);

/*****************************************************************************
 ** 功能：	关闭WWDT比较中断
 ** 参数： [in] none
 ** 返回：  none
 ** 说明：	 
*****************************************************************************/
void WWDT_DisableCompareInt(void);

/*****************************************************************************
 ** 功能：	获取WWDT使能并产生中断标志
 ** 参数： [in] none
 ** 返回：  1:使能中断并产生中断 0：无中断
 ** 说明：	 
*****************************************************************************/
uint32_t WWDT_GetCompareIntFlag(void);

/*****************************************************************************
 ** 功能：	清除WWDT中断标志
 ** 参数： [in] none
 ** 返回： none
 ** 说明：	 
*****************************************************************************/
void WWDT_ClearCompareIntFlag(void);

/*****************************************************************************
 ** 功能：	获取WWDT计数值
 ** 参数： [in] none
 ** 返回： 6bit value
 ** 说明：	 
*****************************************************************************/
uint32_t WWDT_GetTimingData(void);

/*****************************************************************************
 ** 功能：	重载WWDT
 ** 参数： [in] none
 ** 返回： none
 ** 说明：	 
*****************************************************************************/
void WWDT_Reload(void);



#ifdef __cplusplus
}
#endif

#endif /* _PREG_WWDT_H_ */

