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
