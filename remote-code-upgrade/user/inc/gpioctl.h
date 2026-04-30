#ifndef    _GPIOCTL_H
#define    _GPIOCTL_H
#include "gpio.h" 

#define		LED1_ON		   GPIO_SET_PIN(GPIO0,GPIO_PIN_0_MSK)
#define		LED1_OFF	   GPIO_RESET_PIN(GPIO0,GPIO_PIN_0_MSK)

#define		LED2_ON		   GPIO_SET_PIN(GPIO0,GPIO_PIN_1_MSK)
#define		LED2_OFF	   GPIO_RESET_PIN(GPIO0,GPIO_PIN_1_MSK)

#define		LED3_ON		   GPIO_SET_PIN(GPIO0,GPIO_PIN_4_MSK)
#define		LED3_OFF	   GPIO_RESET_PIN(GPIO0,GPIO_PIN_4_MSK)

#define		LEDPWR_ON	   GPIO_SET_PIN(GPIO1,GPIO_PIN_4_MSK)
#define		LEDPWR_OFF	   GPIO_RESET_PIN(GPIO1,GPIO_PIN_4_MSK)

#define		CTRLPWR_ON	   GPIO_SET_PIN(GPIO3,GPIO_PIN_0_MSK)
#define		CTRLPWR_OFF	   GPIO_RESET_PIN(GPIO3,GPIO_PIN_0_MSK)

#define		CTRLmotor_ON	   GPIO_SET_PIN(GPIO2,GPIO_PIN_2_MSK)
#define		CTRLmotor_OFF	   GPIO_RESET_PIN(GPIO2,GPIO_PIN_2_MSK)

#define		CTRLmotor1_ON	   GPIO_SET_PIN(GPIO1,GPIO_PIN_2_MSK)
#define		CTRLmotor1_OFF	   GPIO_RESET_PIN(GPIO1,GPIO_PIN_2_MSK)

void gpio_init(void);
void GPIO_Interrupt(unsigned char status);

void led_display(unsigned char num);
void led_displayEnable(unsigned char en);
void motorEnable(unsigned char en);

#endif
