#ifndef _PREG_GPIO_H_
#define _PREG_GPIO_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "cms32f033x.h"

/*----------------------------------------------------------------------------
 **GPIO PORT & PIN
-----------------------------------------------------------------------------*/
#define		GPIO_PIN_NUM_MAX	(0x08UL)
#define 	GPIO_PIN_0_MSK		(0x01UL)		/*GPIO Pin 0 mask*/
#define 	GPIO_PIN_1_MSK		(0x02UL)		/*GPIO Pin 1 mask*/
#define 	GPIO_PIN_2_MSK		(0x04UL)		/*GPIO Pin 2 mask*/
#define 	GPIO_PIN_3_MSK		(0x08UL)		/*GPIO Pin 3 mask*/		
#define 	GPIO_PIN_4_MSK		(0x10UL)		/*GPIO Pin 4 mask*/
#define 	GPIO_PIN_5_MSK		(0x20UL)		/*GPIO Pin 5 mask*/
#define 	GPIO_PIN_6_MSK		(0x40UL)		/*GPIO Pin 6 mask*/
#define 	GPIO_PIN_7_MSK		(0x80UL)		/*GPIO Pin 7 mask*/

#define 	GPIO_PIN_0			(0x00UL)		/*GPIO Pin 0 Num*/
#define 	GPIO_PIN_1			(0x01UL)		/*GPIO Pin 1 Num*/
#define 	GPIO_PIN_2			(0x02UL)		/*GPIO Pin 2 Num*/
#define 	GPIO_PIN_3			(0x03UL)		/*GPIO Pin 3 Num*/
#define 	GPIO_PIN_4			(0x04UL)		/*GPIO Pin 4 Num*/		
#define 	GPIO_PIN_5			(0x05UL)		/*GPIO Pin 5 Num*/
#define 	GPIO_PIN_6			(0x06UL)		/*GPIO Pin 6 Num*/
#define 	GPIO_PIN_7			(0x07UL)		/*GPIO Pin 7 Num*/

/*----------------------------------------------------------------------------
 **GPIO 模式 
-----------------------------------------------------------------------------*/
#define 	GPIO_MODE_INPUT_FLOAT			(0x00UL)		/*浮空输入模式*/
#define 	GPIO_MODE_OUTPUT_PUSH_PULL		(0x01UL)		/*推挽输出模式*/
#define 	GPIO_MODE_OUTPUT_OPEN_DRAIN		(0x02UL)		/*开漏输出模式*/
#define 	GPIO_MODE_INPUT_PULL_UP			(0x03UL)		/*上拉输入模式*/
#define		GPIO_MODE_INPUT_PULL_DOWN		(0x04UL)		/*下拉输入模式*/

/*----------------------------------------------------------------------------
 **GPIO 中断模式
-----------------------------------------------------------------------------*/
#define 	GPIO_INT_LEVEL_LOW			(0x00UL)			/*GPIO电平触发方式中的低电平触发方式*/
#define 	GPIO_INT_LEVEL_HIGH			(0x01UL)			/*GPIO电平触发方式中的高电平触发方式*/
#define 	GPIO_INT_EDGE_FALLING		(0x02UL)			/*GPIO边沿触发方式中的下降沿触发方式*/
#define 	GPIO_INT_EDGE_RISING		(0x03UL)			/*GPIO边沿触发方式中的上升沿触发方式*/
#define 	GPIO_INT_EDGE_BOTH			(0x04UL)			/*GPIO边沿触发方式中的上升和下降沿触发方式*/
/*----------------------------------------------------------------------------
 **GPIO 滤波时钟
-----------------------------------------------------------------------------*/
#define 	GPIO_FILCLK_DIV_1			(0x0UL<<GPIO_DIDB_DBCK_Pos)		/*Filter Clk = HCLK/1*/
#define 	GPIO_FILCLK_DIV_2			(0x1UL<<GPIO_DIDB_DBCK_Pos)		/*Filter Clk = HCLK/2*/
#define 	GPIO_FILCLK_DIV_4			(0x2UL<<GPIO_DIDB_DBCK_Pos)		/*Filter Clk = HCLK/4*/
#define 	GPIO_FILCLK_DIV_6			(0x3UL<<GPIO_DIDB_DBCK_Pos)		/*Filter Clk = HCLK/6*/
#define 	GPIO_FILCLK_DIV_8			(0x4UL<<GPIO_DIDB_DBCK_Pos)		/*Filter Clk = HCLK/8*/
#define 	GPIO_FILCLK_DIV_10			(0x5UL<<GPIO_DIDB_DBCK_Pos)		/*Filter Clk = HCLK/10*/
#define 	GPIO_FILCLK_DIV_12			(0x6UL<<GPIO_DIDB_DBCK_Pos)		/*Filter Clk = HCLK/12*/
#define 	GPIO_FILCLK_DIV_14			(0x7UL<<GPIO_DIDB_DBCK_Pos)		/*Filter Clk = HCLK/14*/



/*****************************************************************************
 ** 功能：	设置IO口模式
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinNum: GPIO_PIN_0 ~ GPIO_PIN_7
 **				IOMode ：(1)GPIO_MODE_INPUT_FLOAT
 **						 (2)GPIO_MODE_OUTPUT_PUSH_PULL
 **						 (3)GPIO_MODE_OUTPUT_OPEN_DRAIN
 **						 (4)GPIO_MODE_INPUT_PULL_UP
 **						 (5)GPIO_MODE_INPUT_PULL_DOWN
 ** 返回：  none
 ** 说明：	 
*****************************************************************************/
#define   GPIO_CONFIG_IO_MODE(port,PinNum ,IOMode)  do{\
														port->PMS &= ~(GPIO_PMS_PMS0_Msk <<(PinNum*4));\
														port->PMS |= (IOMode <<(PinNum*4));\
													 }while(0)
/*****************************************************************************
 ** 功能：	开启GPIO数据输出写屏蔽
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinMask: GPIO_PIN_0_MSK ~ GPIO_PIN_7_MSK
 ** 返回：  none
 ** 说明：	 
*****************************************************************************/
#define   GPIO_ENABLE_DO_SHIELD(port,PinMask)  do{\
													port->DOM |=  PinMask;\
												}while(0)													 
/*****************************************************************************
 ** 功能：	关闭GPIO数据输出写屏蔽
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinMask: GPIO_PIN_0_MSK ~ GPIO_PIN_7_MSK
 ** 返回：  none
 ** 说明：	 
*****************************************************************************/
#define   GPIO_DISABLE_DO_SHIELD(port,PinMask)  do{\
													port->DOM &= ~(PinMask);\
												}while(0)														 

/*****************************************************************************
 ** 功能：	 关闭GPIO口小电流模式
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinMask: GPIO_PIN_0_MSK ~ GPIO_PIN_7_MSK
 ** 返回：  none
 ** 说明：	 
*****************************************************************************/
#define   GPIO_DISABLE_LOW_CURRENT(port,PinMask)  do{\
													port->DR &= ~(PinMask);\
												}while(0)	
/*****************************************************************************
 ** 功能：	 开启GPIO口小电流模式
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinMask: GPIO_PIN_0_MSK ~ GPIO_PIN_7_MSK
 ** 返回：  none
 ** 说明：	 
*****************************************************************************/
#define   GPIO_ENABLE_LOW_CURRENT(port,PinMask)  do{\
													port->DR |= (PinMask);\
												}while(0)

/*****************************************************************************
 ** 功能：	 关闭GPIO口慢斜率
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinMask: GPIO_PIN_0_MSK ~ GPIO_PIN_7_MSK
 ** 返回：  none
 ** 说明：	 
*****************************************************************************/
#define   GPIO_DISABLE_SLOW_SLOPE(port,PinMask)  do{\
													port->SR &= ~(PinMask);\
												}while(0)	
/*****************************************************************************
 ** 功能：	 开启GPIO口慢斜率
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinMask: GPIO_PIN_0_MSK ~ GPIO_PIN_7_MSK
 ** 返回：  none
 ** 说明：	 
*****************************************************************************/
#define   GPIO_ENABLE_SLOW_SLOPE(port,PinMask)  do{\
													port->SR |= (PinMask);\
												}while(0)	

/*****************************************************************************
 ** 功能：	 管脚置位
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinMask: GPIO_PIN_0_MSK ~ GPIO_PIN_7_MSK
 ** 返回：  none
 ** 说明：	 
*****************************************************************************/
#define   GPIO_SET_PIN(port,PinMask)  do{\
											port->DOSET = (PinMask);\
										}while(0)	
/*****************************************************************************
 ** 功能：	 清零/复位管脚
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinMask: GPIO_PIN_0_MSK ~ GPIO_PIN_7_MSK
 ** 返回：  none
 ** 说明：	 
*****************************************************************************/
#define   GPIO_RESET_PIN(port,PinMask)  do{\
											port->DOCLR = (PinMask);\
										   }while(0)	

/*****************************************************************************
 ** 功能：	 获取IO管脚电平状态
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinMask: GPIO_PIN_0_MSK ~ GPIO_PIN_7_MSK
 ** 返回： 0:低电平， 1：高电平
 ** 说明：	
*****************************************************************************/
#define   GPIO_GET_PIN(port,PinMask)  (((port->DI & (PinMask))? 1:0))

/*****************************************************************************
 ** 功能：	 获取IO中断源标志
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinNum: GPIO_PIN_0 ~ GPIO_PIN_7
 ** 返回： 0:产生中断， 1：未产生中断
 ** 说明：	 
*****************************************************************************/
#define   GPIO_GET_RIS_FLAG(port,PinNum)  (((port->RIS & (0x1UL<<PinNum))? 1:0))


/*****************************************************************************
 ** 功能：	 配置IO口运行模式
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinMask: GPIO_PIN_0_MSK ~ GPIO_PIN_7_MSK
 **				Mode:	 (1)GPIO_MODE_INPUT_FLOAT
 **						 (2)GPIO_MODE_OUTPUT_PUSH_PULL
 **						 (3)GPIO_MODE_OUTPUT_OPEN_DRAIN
 **						 (4)GPIO_MODE_INPUT_PULL_UP
 **						 (5)GPIO_MODE_INPUT_PULL_DOWN
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void GPIO_ConfigRunMode(GPIO_T *port, uint32_t PinMask, uint32_t Mode);

/*****************************************************************************
 ** 功能：	 开启IO口中断
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinMask: GPIO_PIN_0_MSK ~ GPIO_PIN_7_MSK
 **				Mode: (1)GPIO_INT_LEVEL_LOW
 **					  (2)GPIO_INT_LEVEL_HIGH
 **					  (3)GPIO_INT_EDGE_FALLING 
 **					  (4)GPIO_INT_EDGE_RISING
 **					  (5)GPIO_INT_EDGE_BOTH
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void GPIO_EnableInt(GPIO_T *port, uint32_t PinMask, uint32_t IntMode);

/*****************************************************************************
 ** 功能：	 关闭IO口中断
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinMask: GPIO_PIN_0_MSK ~ GPIO_PIN_7_MSK
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void GPIO_DisableInt(GPIO_T *port, uint32_t PinMask);

/*****************************************************************************
 ** 功能：	 获取IO口已使能中断标志位
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinNum: GPIO_PIN_0 ~ GPIO_PIN_7
 ** 返回：  0：未产生中断 1：中断已使能并产生中断
 ** 说明：	
*****************************************************************************/
uint32_t GPIO_GetIntFlag(GPIO_T *port, uint32_t PinNum);

/*****************************************************************************
 ** 功能：	 清除IO口中断标志位
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinNum: GPIO_PIN_0 ~ GPIO_PIN_7
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void GPIO_ClearIntFlag(GPIO_T *port, uint32_t PinNum);

/*****************************************************************************
 ** 功能：	 开启IO口输入滤波
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinMask: GPIO_PIN_0_MSK ~ GPIO_PIN_7_MSK
 **				Fclk:	 (1)GPIO_FILCLK_DIV_1
 **						 (2)GPIO_FILCLK_DIV_2
 **						 (3)GPIO_FILCLK_DIV_4
 **						 (4)GPIO_FILCLK_DIV_6
 **						 (5)GPIO_FILCLK_DIV_8
 **						 (6)GPIO_FILCLK_DIV_10
 **						 (7)GPIO_FILCLK_DIV_12
 **						 (8)GPIO_FILCLK_DIV_14
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void GPIO_EnableFilter(GPIO_T *port, uint32_t PinMask, uint32_t Fclk);

/*****************************************************************************
 ** 功能：	 关闭IO口输入滤波
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinMask: GPIO_PIN_0_MSK ~ GPIO_PIN_7_MSK
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void GPIO_DisableFilter(GPIO_T *port, uint32_t PinMask);

/*****************************************************************************
 ** 功能：	 获取IO管脚电平状态
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinMask: GPIO_PIN_0_MSK ~ GPIO_PIN_7_MSK
 ** 返回：  0：低电平 1：高电平
 ** 说明：	
*****************************************************************************/
uint32_t GPIO_GetPin(GPIO_T *port, uint32_t PinMask);

/*****************************************************************************
 ** 功能：	 置位管脚
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinMask: GPIO_PIN_0_MSK ~ GPIO_PIN_7_MSK
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void GPIO_SetPin(GPIO_T *port, uint32_t PinMask);

/*****************************************************************************
 ** 功能：	 清零/复位管脚
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinMask: GPIO_PIN_0_MSK ~ GPIO_PIN_7_MSK
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void GPIO_ResetPin(GPIO_T *port, uint32_t PinMask);

/*****************************************************************************
 ** 功能：	 翻转管脚
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinMask: GPIO_PIN_0_MSK ~ GPIO_PIN_7_MSK
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void GPIO_TogglePin(GPIO_T *port, uint32_t PinMask);





#ifdef __cplusplus
}
#endif
#endif /* _PREG_GPIO_H_ */

