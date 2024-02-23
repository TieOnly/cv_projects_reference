#pragma once

#include "stdint.h"
#include "string.h"

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_dma.h"

typedef unsigned char   _uc;
typedef uint8_t         _u8;
typedef unsigned int    _ui;
typedef unsigned long   _ul;

#define u8Read(value, bit) (((value) >> (bit)) & 1UL)
#define u8Set(value, bit) ((value) |= (1UL << (bit)))
#define u8Clear(value, bit) ((value) &= ~(1UL << (bit)))
#define u8Toggle(value, bit) ((value) ^= (1UL << (bit)))
#define u8Write(value, bit, bitvalue) ((bitvalue) ? u8Set(value, bit) : u8Clear(value, bit))

namespace TieFunc
{
	uint8_t Arr_BeSame( const char* a, const char* b ); 
	void Arr_Flush( char* a, uint16_t size );
	void GPIO_TOGGLE( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin );
	void Reset_Sys_DMA_USART();
}

class Vec
{
public:
	char x;
	char y;
};
