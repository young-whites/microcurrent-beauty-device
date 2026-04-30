    #ifndef   _TM1673_H
    #define   _TM1673_H
    
    // #include "stm32f10x.h"
    // //#include "bsp_SysTick.h”
    
    // //引脚的时钟使能函数的宏定义
    // #define TM1637_RCC_Cmd    RCC_APB2PeriphClockCmd
    // //DIO和CLK相关引脚的宏定义，用户只需要修改相关宏即可
    // #define    TM_DIO_PORT       GPIOA		                 
    // #define    TM_DIO_CLK 	     RCC_APB2Periph_GPIOA		
    // #define    TM_DIO_PIN		 GPIO_Pin_7		        
    
    // #define    TM_CLK_PORT        GPIOA			              
    // #define    TM_CLK_CLK 	      RCC_APB2Periph_GPIOA		
    // #define    TM_CLK_PIN		  GPIO_Pin_6			
    
    // //时钟线和数据线高低电平的宏定义，不需修改
    // #define      CLK_1         GPIO_SetBits(TM_CLK_PORT,TM_CLK_PIN)
    // #define      CLK_0         GPIO_ResetBits(TM_CLK_PORT,TM_CLK_PIN)
    // #define      DIO_1         GPIO_SetBits(TM_DIO_PORT,TM_DIO_PIN)
    // #define      DIO_0         GPIO_ResetBits(TM_DIO_PORT,TM_DIO_PIN)
   
    // //相关函数声明  
    // void Tm1637_GPIO_Config(void);
    // void Tm1637_IO_input(void);
    // void Tm1637_IO_output(void);
    // void I2C_Start(void);
    // void I2C_stop(void);
    // void TM1637_WriteBit(unsigned char mBit);
    // void TM1637_WriteByte(unsigned char Byte);
    // unsigned char TM1637_RadeByte(void);
    // void TM1637_WriteCommand(unsigned char mData);
    // void TM1637_WriteData(unsigned char addr,unsigned char mData);
    // void Time_Display(void);
    // void test(void);
    // void TM1637_display(void);
    // void TM1637_displaychar(unsigned char ch,unsigned char p);
    // void TM1637_displaymaohao(unsigned char onoff);
    // void TM1637_displayled(unsigned char number,unsigned char onoff);
    // void TM1637_displaynumber(unsigned int data);
    // void Displaymaohao(unsigned char en);



    #endif   /* _TM1673_H*/
   /*********************************************END OF FILE**********************/
