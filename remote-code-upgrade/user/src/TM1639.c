
//********************************************************************************************************************************
//TM1639.C
//数码管和按键驱动芯片TM1639函数代码
//********************************************************************************************************************************


//#include "BitOpt.h"
#include "TM1639.h"
#include "gpio.h"
#include "cms32f033.h"
#include "system.h"
#include "clock.h"


// //P13设置为推挽输出模式
// #define DIO_OUT	GPIO_CONFIG_IO_MODE(GPIO1,GPIO_PIN_3,GPIO_MODE_OUTPUT)
// #define DIO_IN	GPIO_CONFIG_IO_MODE(GPIO1,GPIO_PIN_3,GPIO_MODE_INPUT_WITH_PULL_UP);//GPIO_MODE_INPUT_WITH_PULL_UP)

// #define DIO_1    GPIO_SET_PIN(GPIO1,GPIO_PIN_3_MSK)
// #define DIO_0    GPIO_RESET_PIN(GPIO1,GPIO_PIN_3_MSK)
// #define DIO      GPIO_GET_INPUT_PIN(GPIO1,GPIO_PIN_3)

// #define CLK_1    GPIO_SET_PIN(GPIO1,GPIO_PIN_5_MSK)
// #define CLK_0    GPIO_RESET_PIN(GPIO1,GPIO_PIN_5_MSK)

// #define STB_1    GPIO_SET_PIN(GPIO1,GPIO_PIN_4_MSK)
// #define STB_0    GPIO_RESET_PIN(GPIO1,GPIO_PIN_4_MSK)


// //工作模式，可以进行|组合
// #define MD_WRITE        0x40 //采用地址自动加一方式写显存
// #define MD_AUTO         MD_WRITE
// #define MD_NORMAL       MD_WRITE

// #define MD_READKEY      0x42 //读按键命令
// #define MD_FIX          0x44 //采用固定地址方式写显存
// #define MD_TEST         0x48 //测试模式


// //****************************************************************
// // 写入输入1个字节(8bit)到LED_IC程序开始 
// //输入8BIT数据
// //在时钟的上升沿通过MCU向LED驱动IC——TM16xx写数据
// //----------------------------------------------------------------
// void indate(unsigned char input)
// {
// unsigned int i;

// //DIO设为输出
// DIO_OUT;

// STB_0;                //保证“STB”为低电平，程序不依赖于之前端口的状态
//                         //保证程序在实际运行中不会出现“端口迷失”
// for(i=0;i<8;i++) {
//     CLK_0;              //先将“CLK”清零
//     if((input & 0x01)!=0)
//       DIO_1;            //需要传送的数据的低位为“1”，则把“DIO”清零
//     else
//       DIO_0;            //需要传送的数据的低位为“0”，则把“DIO”置高
//     CLK_1;              //送时钟的上升沿
//     input=input>>1;     //准备送下一个BIT
// }                     //送完一个字节后退出循环
// }

// //****************************************************************
// // 写入输入1个字节(8bit)到LED_IC程序开始 
// //从LED_IC读入1个字节(8bit)程序开始 
// //输出8BIT数据
// //在时钟的上升沿通过MCU从LED驱动IC——TM16xx读数据
// //----------------------------------------------------------------
// unsigned char outdate()
// {
// unsigned char i,out=0;    

// //DIO设为上拉输入
// DIO_IN;
// DIO_1;

// STB_0;                  //保证“STB”为低电平，程序不依赖于之前端口的状态
//                           //保证程序在实际运行中不会出现“端口迷失”
// for(i=0;i<8;i++) {
//     CLK_0;                //先将“CLK”清零
//     out=out>>1; 
//     if(DIO == 0) 
//         out=out & 0x7f;   //如果“DIO”为低电平，则把out的最高位清“0”，其他各位保持不变
//     else                  //如果“DIO”为高电平，则把out的最高位置“1”，其他各位保持不变
//         out=out | 0x80; 
//     CLK_1;                //送时钟的上升沿
// }


// return(out);            //返回读到的1字节数据
// }

// //****************************************************************
// //采用固定地址方式传输地址和数据开始
// //采用固定地址方式
// //----------------------------------------------------------------
// /*
// void display2(unsigned char adress,unsigned char data)
// {
// DIO=1;
// CLK=1;
// STB=1;                      //通讯开始前通讯端口全部初始化为“1”

// indate(MD_FIX);             //数据设置命令
// STB=1;
// indate(adress);             //传显示数据对应的地址
// indate(data);               //保持“STB”=“0”，传1BYTE显示数据
// STB=1;                      //传完显示数据后将“STB”置“1”
// }
// */
// /****************************************************************
// 初始IC三个端口
// DIO,CLK,STB全部设置成不带反相器高电平输出
// ****************************************************************/
// void InitTM1639()
// {
// 	unsigned char date[6]={0xaa,0xaa,0xaa,0xaa,0xaa,0xff};
// 	unsigned char date1[6]={0xff,0xff,0xff,0xff,0xff,0xff};
// 	unsigned char date2[6]={0x00,0x00,0x00,0x00,0x00,0x00};


	
//     //P13设置为推挽输出模式
// 	SYS_SET_IOCFG(IOP13CFG,SYS_IOCFG_P13_GPIO);			/*设置P13为GPIO模式*/	
// 	GPIO_CONFIG_IO_MODE(GPIO1,GPIO_PIN_3,GPIO_MODE_OUTPUT);
		
//     //P14设置为推挽输出模式
// 	SYS_SET_IOCFG(IOP14CFG,SYS_IOCFG_P14_GPIO);			/*设置P14为GPIO模式*/	
// 	GPIO_CONFIG_IO_MODE(GPIO1,GPIO_PIN_4,GPIO_MODE_OUTPUT);

//     //P15设置为推挽输出模式
// 	SYS_SET_IOCFG(IOP15CFG,SYS_IOCFG_P15_GPIO);			/*设置P15为GPIO模式*/	
// 	GPIO_CONFIG_IO_MODE(GPIO1,GPIO_PIN_5,GPIO_MODE_OUTPUT);
	
// 		GPIO_SET_PIN(GPIO1,GPIO_PIN_3_MSK);
// 		GPIO_SET_PIN(GPIO1,GPIO_PIN_4_MSK);
// 		GPIO_SET_PIN(GPIO1,GPIO_PIN_5_MSK);

// 		SeriesInData(0xc0,date,6,LEVEL_14);
// }



// /****************************************************************
// 连续传递数据，数据最多16byte
// statAddr：起始地址
// data：    连续写入数据的起始数据地址
// len：     连续写入数据的长度，len*2+DIG0 < DIG7
// leve：    亮度级别
// 注：如果len*2+DIG0地址>DIG7那么，多出来的地址返回到DIG0，如此循环。
// 本函数可以防止上电后马上传显示数据，会出现乱码。
// ****************************************************************/
// void SeriesInData(unsigned char starAddr,unsigned char *data,unsigned char len,unsigned char level)
// {
// int i;

// //通讯开始前通讯端口全部初始化为“1”
// DIO_1;
// CLK_1;
// STB_1;                      

// indate(MD_AUTO);                 //传数据设置命令，设置采用地址自动加1方式写显示数据，
// STB_1;                          //数据设置命令传完后需要将“STB”置“1”
// indate(starAddr);               //传起始地址

// //关键部分（数据分两次传递到两个地址里面去）
// for(i = 0;i < len;++i) {
//     indate(*data & 0x0f);         //低地址
//     indate(*data >> 4 & 0x0f);    //高地址
//     ++data;
// }

// STB_1;                      //传完所有的显示数据后（最多14BYTE）后，将“STB置“1”
// indate(level);
// STB_1;                      //显示控制命令传完后将“STB”置“1”
// }

// /****************************************************************
// 连续一位数码管数据
// statAddr：起始地址
// data：    连续写入数据的起始数据地址
// leve：    亮度级别
// 注：本质上这个函数也是使用连续模式写入的，非连续模式我写不进去。
// 这个函数可以防止由于使用SeriesInData函数导致写入长度越界
// ****************************************************************/
// void OneByteInData(unsigned char starAddr,unsigned char *data,unsigned char level)
// {
// DIO_1;
// CLK_1;
// STB_1;                      //通讯开始前通讯端口全部初始化为“1”

// indate(MD_AUTO);            //传数据设置命令，设置采用地址自动加1方式写显示数据，
// STB_1;                      //数据设置命令传完后需要将“STB”置“1”
// indate(starAddr);           //传起始地址

// //关键部分（数据分两次传递到两个地址里面去）
// indate(*data & 0x0f);       //低地址
// indate(*data >> 4 & 0x0f); //高地址

// STB_1;                      //传完所有的显示数据后（最多14BYTE）后，将“STB置“1”
// indate(level);
// STB_1;                      //显示控制命令传完后将“STB”置“1
// }

// /****************************************************************
// 清除所有数据
// 从第一位数码管(地址DIG0)开始连续清除8位数码管数据，并且关闭显示。
// 调用情况1：
// 由于上点过程中数码管显示的数据是随机的，可以先调用此函数对数码管清屏。
// 调用情况2：
// 如果系统运行过程当中显示的位数不一定（有时显示3位有时显示5位），
// 那么当从高位切换到低位的时候需要对多余的几位进行清显操作，
// 可一直接调用此ClearAll函数，也可以调用OneByteInData函数逐位写0清显
// ****************************************************************/
// void ClearAll()
// {
// unsigned int i;             //i——控制本次需要传多少个字节显示数据
    
// //通讯开始前通讯端口全部初始化为“1”
// DIO_1;
// CLK_1;
// STB_1;                      

// indate(MD_AUTO);            //传数据设置命令，设置采用地址自动加1方式写显示数据，
// STB_1;                      //数据设置命令传完后需要将“STB”置“1”
// indate(DIG0);               //传起始地址
//                               //地址命令传完后，“STB”保持为“0”继续传需要显示的数据
// for(i=0;i<16;i++)           //datacount=2*grid=2*5=10???
//       indate(0);              //在连续传显示的数据过程中，“STB”一直保持为”0“
      
// STB_1;                      //传完所有的显示数据后（最多14BYTE）后，将“STB置“1”
// indate(0x80);               //传显示控制命令
// STB_1;                      //显示控制命令传完后将“STB”置“1
// }

// /****************************************************************
// 按键扫描程序
// key:存储按键按下的信息int形数据，最多一次可以得到两个按键按下的结果
// key1或key2：4、8、64、128，每个变量可以指示4个按键按下，key1或key2里面允许组合两个按键，
// ****************************************************************/

// unsigned int KeyRead( void)
// {
//  unsigned int key1,key2,key;
//  STB_1;
	
// indate(MD_READKEY);         //传数据设置命令，设置为读按键    
//                               //“STB”保持为“0”，读键值
// 		key1 = outdate();
// 		key2 = outdate();
// 		key = key1 | (key2 << 8);
	
//    CLK_0;
//    DIO_0;
// 	 STB_1;                      //读键结束将“STB”置“1”
// 	return key;
// }


// /****************************************************************
// 测试模式，内部使用，检验硬件完整性
// ****************************************************************/
// void ICTest()
// {
// STB_1;
// indate(MD_TEST);
// STB_1;
// indate(0x8c); 
// STB_1;
// }
 
// void TM1639_Display(unsigned char num,unsigned char date)
// {   
// 	static unsigned char tab[6]={0xaa,0xaa,0xaa,0xaa,0xaa,0xff};
// 	if(date>0x40)tab[num]&=date;
// 	else tab[num]|=date;
// 	SeriesInData(0xc0|num*2,&tab[num],1,LEVEL_14);	
// }

// void Display_power(unsigned char num)
// {
// 	unsigned char i;
// 	 for(i=0;i<5;i++)
// 	 {
// 	 	TM1639_Display(i,0xfe);
// 	 }
// 	 for(i=0;i<num;i++)
// 	 {
// 	 	TM1639_Display(4-i,0x01);
// 	 }
// }

// void Display_Position(unsigned char num)
// {
// 	unsigned char i;
// 	 for(i=0;i<5;i++)
// 	 {
// 	 	TM1639_Display(i,0xfb);
// 	 }
// 	 for(i=0;i<num;i++)
// 	 {
// 	 	TM1639_Display(4-i,0x04);
// 	 }
// }
// void Display_Mode(unsigned char num)
// {
// 	unsigned char i;
//     for(i=0;i<5;i++)
//     {
//         TM1639_Display(i,0xef);
//         TM1639_Display(i,0xbf);
//     }
//     switch( num )
//     {
//         case 1:	 	
//         TM1639_Display(0,0x10);
// 	 	TM1639_Display(0,0x40);
//         TM1639_Display(1,0x10);
// 	 	TM1639_Display(1,0x40);
//         TM1639_Display(2,0x10);
// 	 	TM1639_Display(2,0x40);
//          break;
//         case 2:	       
//         TM1639_Display(0,0x10);
// 	 	TM1639_Display(0,0x40);
//         TM1639_Display(1,0x10);
// 	 	TM1639_Display(1,0x40);
//         break;
//         case 3:
//         TM1639_Display(0,0x10);
// 	 	TM1639_Display(0,0x40);
//         TM1639_Display(2,0x10);
// 	 	TM1639_Display(2,0x40);
//         break;
//         case 4:
//         TM1639_Display(2,0x10);
// 	 	TM1639_Display(2,0x40);
//         TM1639_Display(3,0x10);
// 	 	TM1639_Display(3,0x40);
//         break;
//         case 5:	        
//         TM1639_Display(1,0x10);
// 	 	TM1639_Display(1,0x40);
//         break;
//         case 6:	        
//         TM1639_Display(2,0x10);
// 	 	TM1639_Display(2,0x40);
//         break;
//         case 7:	        
//         TM1639_Display(0,0x10);
// 	 	TM1639_Display(0,0x40);
//         break;
//         case 8:	        
//         TM1639_Display(4,0x10);
// 	 	TM1639_Display(4,0x40);
//         break;
//         default:break;

//         // case 1:	Display_Mode(1);Display_Mode(2);Display_Mode(3);break;
//         // case 2:	Display_Mode(1);Display_Mode(2);break;
//         // case 3:	Display_Mode(1);Display_Mode(3);break;
//         // case 4:	Display_Mode(4);Display_Mode(3);break;
//         // case 5:	Display_Mode(2);break;
//         // case 6:	Display_Mode(3);break;
//         // case 7:	Display_Mode(1);break;
//         // case 8:	Display_Mode(5);break;
//     }

// }