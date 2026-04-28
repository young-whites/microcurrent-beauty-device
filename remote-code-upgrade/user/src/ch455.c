/*
 * File:   CH455.c
 * Author: JiangXiaowei
 */
 
#include "ch455.h"		// 修改该文件以适应硬件环境/单片机型号等
#include "cmt2300a.h"

#include "gpio.h"
#include "cms32f033.h"
#include "system.h"
#include "clock.h"
 
//#define DelayUs(us)  CMT2300A_DelayUs(us)         

//定义
#define I2C_DELAY 1
 
// void CH455_Init(void)
// {

//     //P16设置为推挽输出模式sda
// 	SYS_SET_IOCFG(IOP34CFG,SYS_IOCFG_P34_GPIO);			/*设置P16为GPIO模式*/	
// 	GPIO_CONFIG_IO_MODE(GPIO3,GPIO_PIN_4,GPIO_MODE_OUTPUT);
		
//     //P17设置为推挽输出模式scl
// 	SYS_SET_IOCFG(IOP21CFG,SYS_IOCFG_P21_GPIO);			/*设置P17为GPIO模式*/	
// 	GPIO_CONFIG_IO_MODE(GPIO2,GPIO_PIN_1,GPIO_MODE_OUTPUT);

	
//     GPIO_SET_PIN(GPIO3,GPIO_PIN_4_MSK);
//     GPIO_SET_PIN(GPIO2,GPIO_PIN_1_MSK);
	 
// 		//CH455_SDA_D_OUT;            //数据线设置回输出模式
//     //CH455_SDA_CLR;          //SDA=1;
//     //Delay_Us(I2C_DELAY);
//     //CH455_SCL_SET;          //SCL=1;
//     //Delay_Us(I2C_DELAY);
//     //CH455_SCL_CLR;
// 	CH455_Write( CH455_SYSON_8 );		//八段式
// }
 
// /*
//  * 函数功能：IIC产生起始信号函数
//  * 输入参数：无
//  * 输出参数：无
//  */
// void CH455_I2c_Start(void)
// {
//     CH455_SDA_D_OUT;
//     CH455_SCL_SET;   //SCL = high;
//     Delay_Us(I2C_DELAY);
//     CH455_SDA_SET;   //SDA = high;
//     Delay_Us(I2C_DELAY);
//     CH455_SDA_CLR;    //SDA = low;
//     Delay_Us(I2C_DELAY);
//     CH455_SCL_CLR;    //SCL = low;
//     Delay_Us(I2C_DELAY);
// }
 
// /*
//  * 函数功能：IIC产生停止信号
//  * 输入参数：无
//  * 输出参数：无
//  */
// void CH455_I2c_Stop()
// {
//     CH455_SDA_D_OUT;
//     CH455_SCL_CLR;     //SCL = low;
//     Delay_Us(I2C_DELAY);
//     CH455_SDA_CLR;     //SDA = low;
//     Delay_Us(I2C_DELAY);
//     CH455_SCL_SET;    //SCL = high;
//     Delay_Us(I2C_DELAY);
//     CH455_SDA_SET;    //SDA = high;
//     Delay_Us(I2C_DELAY);
// }
 
// /*
//  * 函数功能：IIC发送一个字节
//  * 输入参数：一个字节
//  * 输出参数：无
//  */
// void CH455_I2c_WrByte(unsigned char IIC_Byte)
// {
//     unsigned char i;
//     CH455_SDA_D_OUT;
//     for(i = 0; i < 8; i++)
//     {
//         if(IIC_Byte & 0x80)
//             CH455_SDA_SET;   //SDA=high;
//         else
//             CH455_SDA_CLR;   //SDA=low;
//         Delay_Us(I2C_DELAY);
//         CH455_SCL_SET;      //SCL=high;
//         Delay_Us(I2C_DELAY);
//         CH455_SCL_CLR;       //SCL=low;
//         Delay_Us(I2C_DELAY);
//         IIC_Byte<<=1;
//     }
//     Delay_Us(I2C_DELAY);
//     CH455_SDA_SET;          //SDA=1;
//     Delay_Us(I2C_DELAY);
//     CH455_SCL_SET;          //SCL=1;
//     Delay_Us(I2C_DELAY);
//     CH455_SCL_CLR;           //SCL=0;
//     Delay_Us(I2C_DELAY);
// }
 
// unsigned char  CH455_I2c_RdByte( void )		//读一个字节数据
// {    
//     unsigned char i, bytedata;
//     CH455_SDA_D_IN;     //将数据设置为输入模式
//     Delay_Us(I2C_DELAY);
//     CH455_SDA_SET;      //数据线拉高
//     Delay_Us(I2C_DELAY);
//     CH455_SCL_CLR;
//     Delay_Us(I2C_DELAY);
//     for(i = 0; i < 8; i++)      //读8位数据
//     { 
//         CH455_SCL_SET;
//         Delay_Us(I2C_DELAY);
//         bytedata <<= 1;
//         Delay_Us(I2C_DELAY);
//         bytedata |= (CH455_SDA_IN);
//         Delay_Us(I2C_DELAY);
//         CH455_SCL_CLR;
//         Delay_Us(I2C_DELAY);
//     }
//     CH455_SDA_D_OUT;            //数据线设置回输出模式
//     CH455_SDA_SET;          //SDA=1;
//     Delay_Us(I2C_DELAY);
//     CH455_SCL_SET;          //SCL=1;
//     Delay_Us(I2C_DELAY);
//     CH455_SCL_CLR;           //SCL=0;
//     Delay_Us(I2C_DELAY);
//     return(bytedata);//返回数据
// }
 
// void CH455_Write( UINT16 cmd )	//写命令
// {
// 	CH455_I2c_Start();               //启动总线
//    	CH455_I2c_WrByte(((UINT8)(cmd>>7)&CH455_I2C_MASK)|CH455_I2C_ADDR);
//    	CH455_I2c_WrByte((UINT8)cmd);     //发送数据
//   	CH455_I2c_Stop();                 //结束总线 
// }
 
// UINT8 CH455_Read( void )		//读取按键
// {
// 	UINT8 keycode;
//    	CH455_I2c_Start();                //启动总线
//    	CH455_I2c_WrByte((UINT8)(CH455_GET_KEY>>7)&CH455_I2C_MASK|0x01|CH455_I2C_ADDR);
//    	keycode=CH455_I2c_RdByte();      //读取数据
// 	CH455_I2c_Stop();                //结束总线
// 	return keycode;
// }

