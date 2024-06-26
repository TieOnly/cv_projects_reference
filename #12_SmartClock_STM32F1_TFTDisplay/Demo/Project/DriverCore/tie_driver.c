#include "tie_driver.h"
//DC : Driver Config

void DC_GPIOC_13();
void DC_GPIOA_0();
//Use for Timer common, Delay global
void DC_TIM3();

//////////////////////////////////////////////////////////////////////////////////////////////////

void DCore_Init()
{
	DC_GPIOC_13();
	DC_GPIOA_0();
	DC_TIM3();
}

void DC_GPIOC_13()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Mode 	= GPIO_Mode_Out_PP;
	gpio_init.GPIO_Pin 		= GPIO_Pin_13;
	gpio_init.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpio_init);

	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}
void DC_GPIOA_0()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Mode 	= GPIO_Mode_Out_PP;
	gpio_init.GPIO_Pin 		= GPIO_Pin_0;
	gpio_init.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_init);

	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
}

void DC_TIM3()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseInitTypeDef tim2_init;
	tim2_init.TIM_ClockDivision = TIM_CKD_DIV1;
	tim2_init.TIM_CounterMode	= TIM_CounterMode_Up;
	tim2_init.TIM_Period		= 0xffff;
	tim2_init.TIM_Prescaler		= 72 - 1;
	TIM_TimeBaseInit( TIM3, &tim2_init );
	// TIM_Cmd( TIM2, ENABLE );
}

//==============Method be used literal==============//
#ifdef __cplusplus
extern "C"
{
#endif

void DCore_Delay( uint16_t ms )
{
	TIM_Cmd( TIM3, ENABLE );
	while( ms-- )
	{
		TIM_SetCounter( TIM3, 0 );
		while( TIM_GetCounter( TIM3 ) < 1000 ){}
	}
	TIM_Cmd( TIM3, DISABLE );
}

void DCore_GPIO_Toggle( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin )
{
	GPIO_WriteBit( 
		GPIOx, GPIO_Pin, 
		GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) ? Bit_RESET : Bit_SET 
	);	
}

#ifdef __cplusplus
}
#endif
