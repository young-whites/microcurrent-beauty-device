#include "cmt_spi3.h"
#include "key.h"
#include "cmt2300a_hal.h"
#include "gpio.h"
#if(DOG_versions==1||DOG_versions==2)
#define GPIO_PORT1  GPIO0
#define GPIO_PORT2  GPIO0
#define GPIO_PORT3  GPIO0
#define GPIO_PORT4  GPIO0


#define CMT_CSB_PIN   GPIO_PIN_6_MSK
#define CMT_FCSB_PIN  GPIO_PIN_7_MSK
#define CMT_SDCK_PIN  GPIO_PIN_4_MSK
#if(RF==0)//TX
#define CMT_SDIO_PIN  GPIO_PIN_5_MSK
#elif(RF==1)//RX
#define CMT_SDIO_PIN  GPIO_PIN_5_MSK
#endif


#define CMT_CSB_PIN_NO_MSK   GPIO_PIN_6
#define CMT_FCSB_PIN_NO_MSK  GPIO_PIN_7
#define CMT_SDCK_PIN_NO_MSK  GPIO_PIN_4
#if(RF==0)//TX
#define CMT_SDIO_PIN_NO_MSK  GPIO_PIN_5
#elif(RF==1)//RX
#define CMT_SDIO_PIN_NO_MSK  GPIO_PIN_5
#endif


#define cmt_spi3_csb_out()      GPIO_ConfigRunMode(GPIO_PORT2,CMT_CSB_PIN,GPIO_MODE_OUTPUT,RESET)
#define cmt_spi3_fcsb_out()     GPIO_ConfigRunMode(GPIO_PORT2,CMT_FCSB_PIN,GPIO_MODE_OUTPUT,RESET)
#define cmt_spi3_sclk_out()     GPIO_ConfigRunMode(GPIO_PORT2,CMT_SDCK_PIN,GPIO_MODE_OUTPUT,RESET)
#define cmt_spi3_sdio_out()     GPIO_ConfigRunMode(GPIO_PORT2,CMT_SDIO_PIN,GPIO_MODE_OUTPUT,RESET)
#define cmt_spi3_sdio_in()      GPIO_ConfigRunMode(GPIO_PORT2,CMT_SDIO_PIN,GPIO_MODE_INPUT_WITH_PULL_DOWN,RESET)

#define cmt_spi3_csb_1()        GPIO_SET_PIN(GPIO_PORT2,CMT_CSB_PIN)
#define cmt_spi3_csb_0()        GPIO_RESET_PIN(GPIO_PORT2,CMT_CSB_PIN)

#define cmt_spi3_fcsb_1()       GPIO_SET_PIN(GPIO_PORT2,CMT_FCSB_PIN)
#define cmt_spi3_fcsb_0()       GPIO_RESET_PIN(GPIO_PORT2,CMT_FCSB_PIN)
    
#define cmt_spi3_sclk_1()       GPIO_SET_PIN(GPIO_PORT2,CMT_SDCK_PIN)
#define cmt_spi3_sclk_0()       GPIO_RESET_PIN(GPIO_PORT2,CMT_SDCK_PIN)

#define cmt_spi3_sdio_1()       GPIO_SET_PIN(GPIO_PORT2,CMT_SDIO_PIN)
#define cmt_spi3_sdio_0()       GPIO_RESET_PIN(GPIO_PORT2,CMT_SDIO_PIN)
#define cmt_spi3_sdio_read()    GPIO_GET_INPUT_PIN(GPIO_PORT2,CMT_SDIO_PIN_NO_MSK)
#elif(DOG_versions==3||DOG_versions==4)
#define GPIO_PORT1  GPIO0
#define GPIO_PORT2  GPIO0
#define GPIO_PORT3  GPIO0
#define GPIO_PORT4  GPIO0

#define CMT_CSB_PIN   GPIO_PIN_6_MSK
#define CMT_FCSB_PIN  GPIO_PIN_7_MSK
#define CMT_SDCK_PIN  GPIO_PIN_4_MSK
#if(RF==0)//TX
#define CMT_SDIO_PIN  GPIO_PIN_5_MSK
#elif(RF==1)//RX
#define CMT_SDIO_PIN  GPIO_PIN_5_MSK
#endif

#define CMT_CSB_PIN_NO_MSK   GPIO_PIN_6
#define CMT_FCSB_PIN_NO_MSK  GPIO_PIN_7
#define CMT_SDCK_PIN_NO_MSK  GPIO_PIN_4
#if(RF==0)//TX
#define CMT_SDIO_PIN_NO_MSK  GPIO_PIN_5
#elif(RF==1)//RX
#define CMT_SDIO_PIN_NO_MSK  GPIO_PIN_5
#endif

/* ************************************************************************
*  The following need to be modified by user
*  ************************************************************************ */
#define cmt_spi3_csb_out()      GPIO_ConfigRunMode(GPIO_PORT2,CMT_CSB_PIN,GPIO_MODE_OUTPUT,RESET)
#define cmt_spi3_fcsb_out()     GPIO_ConfigRunMode(GPIO_PORT2,CMT_FCSB_PIN,GPIO_MODE_OUTPUT,RESET)
#define cmt_spi3_sclk_out()     GPIO_ConfigRunMode(GPIO_PORT2,CMT_SDCK_PIN,GPIO_MODE_OUTPUT,RESET)
#define cmt_spi3_sdio_out()     GPIO_ConfigRunMode(GPIO_PORT2,CMT_SDIO_PIN,GPIO_MODE_OUTPUT,RESET)
#define cmt_spi3_sdio_in()      GPIO_ConfigRunMode(GPIO_PORT2,CMT_SDIO_PIN,GPIO_MODE_INPUT_WITH_PULL_DOWN,RESET)

#define cmt_spi3_csb_1()        GPIO_SET_PIN(GPIO_PORT2,CMT_CSB_PIN)
#define cmt_spi3_csb_0()        GPIO_RESET_PIN(GPIO_PORT2,CMT_CSB_PIN)

#define cmt_spi3_fcsb_1()       GPIO_SET_PIN(GPIO_PORT2,CMT_FCSB_PIN)
#define cmt_spi3_fcsb_0()       GPIO_RESET_PIN(GPIO_PORT2,CMT_FCSB_PIN)
    
#define cmt_spi3_sclk_1()       GPIO_SET_PIN(GPIO_PORT2,CMT_SDCK_PIN)
#define cmt_spi3_sclk_0()       GPIO_RESET_PIN(GPIO_PORT2,CMT_SDCK_PIN)

#define cmt_spi3_sdio_1()       GPIO_SET_PIN(GPIO_PORT2,CMT_SDIO_PIN)
#define cmt_spi3_sdio_0()       GPIO_RESET_PIN(GPIO_PORT2,CMT_SDIO_PIN)
#define cmt_spi3_sdio_read()    GPIO_GET_INPUT_PIN(GPIO_PORT2,CMT_SDIO_PIN_NO_MSK)
/* ************************************************************************ */
#endif

void cmt_spi_halt(void)
{
    cmt_spi3_csb_1();
    cmt_spi3_fcsb_1();
    cmt_spi3_sclk_0();
    cmt_spi3_sdio_out();
    cmt_spi3_sdio_1();
}
void cmt_spi3_delay(void)
{
    u32 n = 21;
    while(n--);
}

void cmt_spi3_delay_us(void)
{
    u16 n = 24;
    while(n--);
}

void cmt_spi3_init(void)
{
    cmt_spi3_csb_1();
    cmt_spi3_csb_out();
	  GPIO_DISABLE_LOW_CURRENT(GPIO_PORT2,CMT_CSB_PIN);

    cmt_spi3_csb_1();   /* CSB has an internal pull-up resistor */
    
    cmt_spi3_sclk_0();
    cmt_spi3_sclk_out();
	  GPIO_DISABLE_LOW_CURRENT(GPIO_PORT2,CMT_SDCK_PIN);

    cmt_spi3_sclk_0();   /* SCLK has an internal pull-down resistor */
    
    cmt_spi3_sdio_1();
    cmt_spi3_sdio_out();
	  GPIO_DISABLE_LOW_CURRENT(GPIO_PORT2,CMT_SDIO_PIN);

    cmt_spi3_sdio_1();
    
    cmt_spi3_fcsb_1();
    cmt_spi3_fcsb_out();
	  GPIO_DISABLE_LOW_CURRENT(GPIO_PORT2,CMT_FCSB_PIN);
    cmt_spi3_fcsb_1();  /* FCSB has an internal pull-up resistor */

    cmt_spi3_delay();
}

void cmt_spi3_send(u8 data8)
{
    u8 i;

    for(i=0; i<8; i++)
    {
        cmt_spi3_sclk_0();

        /* Send byte on the rising edge of SCLK */
        if(data8 & 0x80)
            cmt_spi3_sdio_1();
        else            
            cmt_spi3_sdio_0();

        cmt_spi3_delay();

        data8 <<= 1;
        cmt_spi3_sclk_1();
        cmt_spi3_delay();
    }
}
//u8  aaa=0;
u8 cmt_spi3_recv(void)
{
    u8 i;
    u8 data8 = 0xFF;
//    aaa =  GPIO_GET_INPUT_PIN(GPIO_PORT2,CMT_SDIO_PIN_NO_MSK);
    for(i=0; i<8; i++)
    {
        cmt_spi3_sclk_0();
        cmt_spi3_delay();
        data8 <<= 1;

        cmt_spi3_sclk_1();
        
        /* Read byte on the rising edge of SCLK */
        if(cmt_spi3_sdio_read())
            data8 |= 0x01;
        else
            data8 &= ~0x01;

        cmt_spi3_delay();
    }

    return data8;
}

void cmt_spi3_write(u8 addr, u8 dat)
{
    cmt_spi3_sdio_1();
    cmt_spi3_sdio_out();

    cmt_spi3_sclk_0();
    cmt_spi3_sclk_out();
    cmt_spi3_sclk_0(); 

    cmt_spi3_fcsb_1();
    cmt_spi3_fcsb_out();
    cmt_spi3_fcsb_1();

    cmt_spi3_csb_0();

    /* > 0.5 SCLK cycle */
    cmt_spi3_delay();
    cmt_spi3_delay();

    /* r/w = 0 */
    cmt_spi3_send(addr&0x7F);

    cmt_spi3_send(dat);

    cmt_spi3_sclk_0();

    /* > 0.5 SCLK cycle */
    cmt_spi3_delay();
    cmt_spi3_delay();

    cmt_spi3_csb_1();
    
    cmt_spi3_sdio_1();
    cmt_spi3_sdio_in();
    
    cmt_spi3_fcsb_1();    
}

void cmt_spi3_read(u8 addr, u8* p_dat)
{
    cmt_spi3_sdio_1();
    cmt_spi3_sdio_out();

    cmt_spi3_sclk_0();
    cmt_spi3_sclk_out();
    cmt_spi3_sclk_0(); 

    cmt_spi3_fcsb_1();
    cmt_spi3_fcsb_out();
    cmt_spi3_fcsb_1();

    cmt_spi3_csb_0();

    /* > 0.5 SCLK cycle */
    cmt_spi3_delay();
    cmt_spi3_delay();

    /* r/w = 1 */
    cmt_spi3_send(addr|0x80);

    /* Must set SDIO to input before the falling edge of SCLK */
    cmt_spi3_sdio_in();
    
    *p_dat = cmt_spi3_recv();

    cmt_spi3_sclk_0();

    /* > 0.5 SCLK cycle */
    cmt_spi3_delay();
    cmt_spi3_delay();

    cmt_spi3_csb_1();
    
    cmt_spi3_sdio_1();
    cmt_spi3_sdio_in();
    
    cmt_spi3_fcsb_1();
}

void cmt_spi3_write_fifo(const uint8_t* p_buf, uint16_t len)
{
    u16 i;

    cmt_spi3_fcsb_1();
    cmt_spi3_fcsb_out();
    cmt_spi3_fcsb_1();

    cmt_spi3_csb_1();
    cmt_spi3_csb_out();
    cmt_spi3_csb_1();

    cmt_spi3_sclk_0();
    cmt_spi3_sclk_out();
    cmt_spi3_sclk_0();

    cmt_spi3_sdio_out();

    for(i=0; i<len; i++)
    {
        cmt_spi3_fcsb_0();

        /* > 1 SCLK cycle */
        cmt_spi3_delay();
        cmt_spi3_delay();

        cmt_spi3_send(p_buf[i]);

        cmt_spi3_sclk_0();

        /* > 2 us */
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();

        cmt_spi3_fcsb_1();

        /* > 4 us */
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
    }

    cmt_spi3_sdio_in();
    
    cmt_spi3_fcsb_1();
}

void cmt_spi3_read_fifo(uint8_t* p_buf, uint16_t len)
{
    u16 i;

    cmt_spi3_fcsb_1();
    cmt_spi3_fcsb_out();
    cmt_spi3_fcsb_1();

    cmt_spi3_csb_1();
    cmt_spi3_csb_out();
    cmt_spi3_csb_1();

    cmt_spi3_sclk_0();
    cmt_spi3_sclk_out();
    cmt_spi3_sclk_0();

    cmt_spi3_sdio_in();

    for(i=0; i<len; i++)
    {
        cmt_spi3_fcsb_0();

        /* > 1 SCLK cycle */
        cmt_spi3_delay();
        cmt_spi3_delay();

        p_buf[i] = cmt_spi3_recv();

        cmt_spi3_sclk_0();

        /* > 2 us */
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();

        cmt_spi3_fcsb_1();

        /* > 4 us */
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
        cmt_spi3_delay_us();
    }

    cmt_spi3_sdio_in();
    
    cmt_spi3_fcsb_1();
}
