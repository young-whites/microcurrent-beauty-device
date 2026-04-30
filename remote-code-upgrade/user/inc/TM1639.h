//数码管和按键驱动芯片TM1639驱动代码2010年06月24日 星期四 00:28//********************************************************************************************************************************
//TM1639.h
//数码管和按键驱动芯片TM1639函数接口和芯片操作相关宏定义
//********************************************************************************************************************************

#ifndef _TM1639_H
#define _TM1639_H

// #include "cms32f033.h"
// #include "work.h"
// #include "gpio.h"
// //位使能
// #define DIG0 0xc0
// #define DIG1 0xc2
// #define DIG2 0xc4
// #define DIG3 0xc6
// #define DIG4 0xc8
// #define DIG5 0xca
// #define DIG6 0xcc
// #define DIG7 0xce

// //显示亮度级别
// #define LEVEL_OFF   0x80
// #define LEVEL_1     0x88
// #define LEVEL_2     0x89
// #define LEVEL_4     0x8a
// #define LEVEL_10    0x8b
// #define LEVEL_11    0x8c
// #define LEVEL_12    0x8d
// #define LEVEL_13    0x8e
// #define LEVEL_14    0x8f


// //初始化IC端口
// void InitTM1639();
// //连续传送多个数据
// void SeriesInData (unsigned char starAddr,unsigned char *data,unsigned char loop,unsigned char level);
// //传送一个数据（本质是连续模式）
// void OneByteInData(unsigned char starAddr,unsigned char *data,unsigned char level);
// //清楚所有显示数据
// void ClearAll();
// //读取按键键值
// unsigned int KeyRead( void);
// //测试模式，内部使用，检验硬件完整性
// void ICTest();
// void TM1639_Display(unsigned char num,unsigned char date);
// void Display_power(unsigned char num);
// void Display_Position(unsigned char num);
// void Display_Mode(unsigned char num);

#endif
