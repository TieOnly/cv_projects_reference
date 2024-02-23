#pragma once

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_exti.h"
#include "misc.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern void DCore_Init();

//Use TIM3
void DCore_Delay( uint16_t ms );
//Use GPIOx, Pinx
void DCore_GPIO_Toggle( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin );

#ifdef __cplusplus
}
#endif