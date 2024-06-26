#include "tie_ft.h"

static struct tie_ft_t
{
	uint16_t dtime;
	uint16_t dtime_micro;
} ft_inst;

static void ft_init_driver(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	TIM_TimeBaseInitTypeDef tim1_init;
	tim1_init.TIM_ClockDivision = TIM_CKD_DIV1;
	tim1_init.TIM_CounterMode	= TIM_CounterMode_Up;
	tim1_init.TIM_Period		= 0xffff;
	tim1_init.TIM_Prescaler		= (72 * 10) - 1;	//10us per clock
	TIM_TimeBaseInit( FT_TIMER, &tim1_init );
	TIM_Cmd(FT_TIMER, ENABLE);
}

void ft_init(void)
{
	ft_init_driver();
}
void ft_tick(void)
{
	uint16_t count = TIM_GetCounter( FT_TIMER );
	ft_inst.dtime = 0;
	ft_inst.dtime_micro = count % 100;

	//count == 100 equal to timer FT_TIMER reached 1ms
	if( count >= 100 )
	{
		TIM_SetCounter(FT_TIMER, ft_inst.dtime_micro);
		ft_inst.dtime = count / 100;
	}
}
void ft_delay(uint16_t ms)
{
	while(ms--)
	{
		TIM_SetCounter(FT_TIMER, 0);
		while( TIM_GetCounter(FT_TIMER) < 100 ){}
	}
}
uint16_t ft_dtime(void) { return ft_inst.dtime; }
uint16_t ft_dtime_micro(void) { return ft_inst.dtime_micro; }

static void (*p_rtc_irq_service)(void) = NULL;

void rtc_init(void (*irq_service)(void))
{
	RCC->APB1ENR |= (1 << 28) | (1 << 27);
	PWR->CR |= (1 << 8);
	RCC->BDCR |= (1 << 0);
	while( !(RCC->BDCR & (1 << 1)) ) {}

	RCC->BDCR &= ~(3 << 8);
	RCC->BDCR |= (1 << 8);
	RCC->BDCR |= (1 << 15);
	
	while( !(RTC->CRL & (1 << 5)) ) {}
	RTC->CRL |= (1 << 4);

	RTC->PRLH = 0x0000;
	RTC->PRLL = 0x7fff;

	RTC->CRL &= ~(1 << 4);
	while( !(RTC->CRL & (1 << 5)) ) {}

	RTC->CRH |= (1 << 0);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	p_rtc_irq_service = irq_service;
}
void RTC_IRQHandler(void)
{
	RTC->CRL &= ~ (1 << 0);		//Clear interupt flag
	if (p_rtc_irq_service != NULL) p_rtc_irq_service();
}