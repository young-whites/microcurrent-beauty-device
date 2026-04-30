#ifndef __VK1056B_H
#define __VK1056B_H

#include "cms32f033.h"
#include "work.h"


//基本命令
#define OSC_OFF    	0x00         				// 关闭震荡器
#define OSC_ON    	0x01         				// 开启震荡器
#define DISP_OFF 	0x02								// 关LCD Bias
#define DISP_ON   	0x03         				// 开LCD Bias
#define COM_1_3__4  0x29  							// 1/3bias 4com
#define COM_1_3__3  0x25  							// 1/3bias 3com
#define COM_1_3__2  0x21  							// 1/3bias 2com
#define COM_1_2__4  0x28  							// 1/2bias 4com
#define COM_1_2__3  0x24  							// 1/2bias 3com
#define COM_1_2__2  0x20  							// 1/2bias 2com
//扩展命令，该功能未引出关闭可降低功耗
#define TIMER_DIS   0x04         				// 
#define WDT_DIS   	0x05         				// 
#define BUZZ_OFF  	0x08         				// 关闭蜂鸣器 
#define RC256K  	0X18         				// 
#define IRQ_DIS  	0X80        				// 

enum mode
{
    ModeNull=0,//空白
    PowerHit=1,//电击
    WarmUp=2,//加热
    Shake=4,//震动
    Atom=8,//雾化
};

#define	WR1	GPIO1->DO_f.P3 = 0;
#define	WR0	GPIO1->DO_f.P3 = 0;

#define	DATA1   GPIO1->DO_f.P3 = 0;	 
#define	DATA0   GPIO1->DO_f.P3 = 0;	 

#define	CS1   GPIO1->DO_f.P3 = 0;	 
#define	CS0   GPIO1->DO_f.P3 = 0;	 


/* Exported functions ------------------------------------------------------- */
void Vk1056B_Init(void);  
void Vk1056B_Display_Init(void);

void Write1DataVk1056B(unsigned char Addr,unsigned char Dat);
void WritenDataVk1056B(unsigned char Addr,unsigned char *Databuf,unsigned char Cnt);

void WriteCommandVk1056B(unsigned char FunctonCode);


void Vk1056B_Display_Mode(u8 mode);

void Vk1056B_Display_Position(u8 state);
void Vk1056B_Display_BLE(u8 state);
void Vk1056B_Display_Power(u8 state);
void Vk1056B_Display_0123(u8 i,u8 num);


//单个显示
void Vk1056B_ON(u8 seg,u8 com);
//单个关闭
void Vk1056B_OFF(u8 seg,u8 com);


#endif  

