#include "cms32f033x.h"
#include "Psfr_gpio.h"

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
void GPIO_ConfigRunMode(GPIO_T *port, uint32_t PinMask, uint32_t Mode)
{
	uint32_t Num;
	uint32_t Size =4;
	for(Num =0; Num <GPIO_PIN_NUM_MAX; Num++)
	{
		if(PinMask & (0x1 <<Num))
		{
			port->PMS &= ~(0x7 <<(Num*Size));
			port->PMS |= (Mode <<(Num*Size));
		}
	}
}

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
void GPIO_EnableInt(GPIO_T *port, uint32_t PinMask, uint32_t IntMode)
{
	switch(IntMode)
	{
		case GPIO_INT_LEVEL_LOW:
			port->ITYPE |= PinMask;
			port->IVAL  &= ~(PinMask);
			break;
		case GPIO_INT_LEVEL_HIGH:
			port->ITYPE |= PinMask;
			port->IVAL  |= PinMask;	
			break;
		case GPIO_INT_EDGE_FALLING:
			port->ITYPE &= ~(PinMask);
			port->IVAL  &= ~(PinMask);
			port->IANY  &= ~(PinMask);
			break;
		case GPIO_INT_EDGE_RISING:
			port->ITYPE &= ~(PinMask);
			port->IVAL  |= PinMask;
			port->IANY  &= ~(PinMask);
			break;
		case GPIO_INT_EDGE_BOTH:
			port->ITYPE &= ~(PinMask);
			port->IANY  |= PinMask;
			break;
		default:
			break;
	}
	port->IMSC  |= PinMask;
}

/*****************************************************************************
 ** 功能：	 关闭IO口中断
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinMask: GPIO_PIN_0_MSK ~ GPIO_PIN_7_MSK
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void GPIO_DisableInt(GPIO_T *port, uint32_t PinMask)
{
	port->IMSC  &= ~(PinMask);
}

/*****************************************************************************
 ** 功能：	 获取IO口已使能中断标志位
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinNum: GPIO_PIN_0 ~ GPIO_PIN_7
 ** 返回：  0：未产生中断 1：中断已使能并产生中断
 ** 说明：	
*****************************************************************************/
uint32_t GPIO_GetIntFlag(GPIO_T *port, uint32_t PinNum)
{
	return((port->MIS &(0x1<< PinNum))? 1:0);
}

/*****************************************************************************
 ** 功能：	 清除IO口中断标志位
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinNum: GPIO_PIN_0 ~ GPIO_PIN_7
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void GPIO_ClearIntFlag(GPIO_T *port, uint32_t PinNum)
{
	port->ICLR |= (0x1<< PinNum);
}

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
void GPIO_EnableFilter(GPIO_T *port, uint32_t PinMask, uint32_t Fclk)
{
	port->DIDB &= ~(GPIO_DIDB_DBCK_Msk);
	port->DIDB |=(PinMask | Fclk);	
}

/*****************************************************************************
 ** 功能：	 关闭IO口输入滤波
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinMask: GPIO_PIN_0_MSK ~ GPIO_PIN_7_MSK
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void GPIO_DisableFilter(GPIO_T *port, uint32_t PinMask)
{
	port->DIDB &= ~(PinMask);
}

/*****************************************************************************
 ** 功能：	 获取IO管脚电平状态
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinMask: GPIO_PIN_0_MSK ~ GPIO_PIN_7_MSK
 ** 返回：  0：低电平 1：高电平
 ** 说明：	
*****************************************************************************/
uint32_t GPIO_GetPin(GPIO_T *port, uint32_t PinMask)
{
	return((port->DI &PinMask)? 1:0);
}

/*****************************************************************************
 ** 功能：	 置位管脚
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinMask: GPIO_PIN_0_MSK ~ GPIO_PIN_7_MSK
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void GPIO_SetPin(GPIO_T *port, uint32_t PinMask)
{
	port->DOSET = PinMask;
}

/*****************************************************************************
 ** 功能：	 清零/复位管脚
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinMask: GPIO_PIN_0_MSK ~ GPIO_PIN_7_MSK
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void GPIO_ResetPin(GPIO_T *port, uint32_t PinMask)
{
	port->DOCLR = PinMask;
}

/*****************************************************************************
 ** 功能：	 翻转管脚
 ** 参数： [in] port : GPIO0、GPIO1 、GPIO2、GPIO3、GPIO4
 **				PinMask: GPIO_PIN_0_MSK ~ GPIO_PIN_7_MSK
 ** 返回：  none
 ** 说明：	
*****************************************************************************/
void GPIO_TogglePin(GPIO_T *port, uint32_t PinMask)
{
	if(GPIO_GetPin(port,PinMask))	GPIO_ResetPin(port,PinMask);
	else							GPIO_SetPin(port,PinMask);
}

