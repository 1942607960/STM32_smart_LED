#ifndef __BSP_TIMEBASE_H
#define __BSP_TIMEBASE_H

#include "stm32f10x.h"


void GENERAL4_TIM_Init(void);
void NVIC_Configuration(void);
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_IRQHandler(void);
#endif
