#ifndef __CH455_H
#define __CH455_H
 #include "cms32f033.h"
#include "gpio.h"
typedef unsigned char   UINT8;
typedef unsigned short  UINT16;	
 
// #define high 	1
// #define low 	0
 

 
 
/* 2线接口的位操作,与单片机有关 */
#define		CH455_SDA_SET		GPIO_SET_PIN(GPIO3,GPIO_PIN_4_MSK)
#define		CH455_SDA_CLR		GPIO_RESET_PIN(GPIO3,GPIO_PIN_4_MSK)

#define		CH455_SCL_SET		GPIO_SET_PIN(GPIO2,GPIO_PIN_1_MSK)
#define		CH455_SCL_CLR		GPIO_RESET_PIN(GPIO2,GPIO_PIN_1_MSK)

#define		CH455_SDA_IN		  GPIO_GET_INPUT_PIN(GPIO3,GPIO_PIN_4)     //  读取SDA输入电平
#define		CH455_SDA_D_OUT	  GPIO_CONFIG_IO_MODE(GPIO3,GPIO_PIN_4,GPIO_MODE_OUTPUT)   // 设置SDA为输出方向,对于双向I/O需切换为输出
#define		CH455_SDA_D_IN		GPIO_CONFIG_IO_MODE(GPIO3,GPIO_PIN_4,GPIO_MODE_INPUT_WITH_PULL_UP) //GPIO_MODE_INPUT_WITH_PULL_UP)   // 设置SDA为输入方向,对于双向I/O需切换为输入
 
/* ********************************************************************************************* */
// 设置系统参数命令
 
#define CH455_BIT_ENABLE	0x01		// 开启/关闭位
#define CH455_BIT_SLEEP		0x04		// 睡眠控制位
#define CH455_BIT_7SEG		0x08		// 7段控制位
#define CH455_BIT_INTENS1	0x10		// 1级亮度
#define CH455_BIT_INTENS2	0x20		// 2级亮度
#define CH455_BIT_INTENS3	0x30		// 3级亮度
#define CH455_BIT_INTENS4	0x40		// 4级亮度
#define CH455_BIT_INTENS5	0x50		// 5级亮度
#define CH455_BIT_INTENS6	0x60		// 6级亮度
#define CH455_BIT_INTENS7	0x70		// 7级亮度
#define CH455_BIT_INTENS8	0x00		// 8级亮度
 
#define CH455_SYSOFF	    0x0400			        // 关闭显示、关闭键盘
#define CH455_SYSON         ( CH455_SYSOFF | CH455_BIT_ENABLE )	// 开启显示、键盘
#define CH455_SLEEPOFF	    CH455_SYSOFF	// 关闭睡眠
#define CH455_SLEEPON	    ( CH455_SYSOFF | CH455_BIT_SLEEP )	// 开启睡眠
#define CH455_7SEG_ON	    ( CH455_SYSON | CH455_BIT_7SEG )	// 开启七段模式
#define CH455_8SEG_ON	    ( CH455_SYSON | 0x00 )	        // 开启八段模式
#define CH455_SYSON_4	    ( CH455_SYSON | CH455_BIT_INTENS4 )	// 开启显示、键盘、4级亮度
#define CH455_SYSON_8	    ( CH455_SYSON | CH455_BIT_INTENS8 )	// 开启显示、键盘、8级亮度
 
 
// 加载字数据命令
#define CH455_DIG0		0x1400			// 数码管位0显示,需另加8位数据
#define CH455_DIG1		0x1500			// 数码管位1显示,需另加8位数据
#define CH455_DIG2		0x1600			// 数码管位2显示,需另加8位数据
#define CH455_DIG3		0x1700			// 数码管位3显示,需另加8位数据
 
 
 
// 读取按键代码命令
#define CH455_GET_KEY	0x0700					// 获取按键,返回按键代码
 
 
// CH455接口定义
#define		CH455_I2C_ADDR		0x40			// CH455的地址
#define		CH455_I2C_MASK		0x3E			// CH455的高字节命令掩码
 
// 对外子程序
extern void CH455_Init(void);           //初始化函数
extern	UINT8 CH455_Read( void );	// 从CH455读取按键代码
extern  void CH455_Write( UINT16 cmd );	// 向CH455发出操作命令
UINT8  CH455_I2c_RdByte( void );
void CH455_I2c_WrByte(unsigned char IIC_Byte);
void CH455_I2c_Stop();
void CH455_I2c_Start();
 
#endif	/* CH455_H */