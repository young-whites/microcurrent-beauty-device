/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CMS32F033_IT_H
#define __CMS32F033_IT_H

/* Includes ------------------------------------------------------------------*/
#include "sys.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);
void HardFault_Handler(void);
void SVC_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void CCP_IRQHandler(void);
void GPIO0_IRQHandler(void);
void GPIO1_IRQHandler(void);
void GPIO2_IRQHandler(void);
void GPIO3_IRQHandler(void);
void GPIO4_IRQHandler(void);
void WWDT_IRQHandler(void);
void TMR0_IRQHandler(void);
void TMR1_IRQHandler(void);


#endif /* __CMS32F033_IT_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
