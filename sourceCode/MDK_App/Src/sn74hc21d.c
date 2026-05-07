#include "sn74hc21d.h"




void SN74HC21D_OpenA(void)
{
    GPIO_SetPin(GPIO1, GPIO_PIN_2_MSK);      //ION_ENA
    GPIO_ResetPin(GPIO4, GPIO_PIN_0_MSK);    //ION_ENB
    GPIO_SetPin(GPIO1, GPIO_PIN_0_MSK);      //ION_ENAB
    GPIO_SetPin(GPIO3, GPIO_PIN_1_MSK);      //ION_DUTYA
}


void SN74HC21D_OpenB(void)
{
    GPIO_ResetPin(GPIO1, GPIO_PIN_2_MSK);     //ION_ENA
    GPIO_SetPin(GPIO4, GPIO_PIN_0_MSK);       //ION_ENB
    GPIO_SetPin(GPIO1, GPIO_PIN_0_MSK);       //ION_ENAB
    GPIO_SetPin(GPIO3, GPIO_PIN_1_MSK);       //ION_DUTYA	
}



void SN74HC21D_OpenAll(void)
{
    GPIO_SetPin(GPIO1, GPIO_PIN_2_MSK);       //ION_ENA
    GPIO_SetPin(GPIO4, GPIO_PIN_0_MSK);       //ION_ENB
    GPIO_SetPin(GPIO1, GPIO_PIN_0_MSK);       //ION_ENAB
    GPIO_SetPin(GPIO3, GPIO_PIN_1_MSK);       //ION_DUTYA	
}



void SN74HC21D_StopAll(void)
{
    GPIO_ResetPin(GPIO1, GPIO_PIN_2_MSK);       //ION_ENA
    GPIO_ResetPin(GPIO4, GPIO_PIN_0_MSK);       //ION_ENB
    GPIO_ResetPin(GPIO1, GPIO_PIN_0_MSK);       //ION_ENAB
    GPIO_ResetPin(GPIO3, GPIO_PIN_1_MSK);       //ION_DUTYA	
}



void SN74HC21D_CloseAB(void)
{
    GPIO_ResetPin(GPIO1, GPIO_PIN_2_MSK);       //ION_ENA
    GPIO_ResetPin(GPIO4, GPIO_PIN_0_MSK);       //ION_ENB
}










