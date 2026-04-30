#ifndef _KEY_H_
#define _KEY_H_

#include "cms32f033.h"
#include "work.h"
#include "gpio.h"



typedef enum
{ 
   Null=0,
   Low,
   Mid,
   High,
}E_Status;

typedef enum
{ 
   other=0,
   ON,
   OFF,
}GO_Status;

extern GO_Status K_Status;
extern E_Status P_Status;
extern void  KEY_Handle( void );

extern u8 postion_Statu;

#endif
