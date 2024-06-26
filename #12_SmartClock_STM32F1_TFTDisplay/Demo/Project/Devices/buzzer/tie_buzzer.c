#include "tie_buzzer.h"
#include "tie_driver.h"

static tie_buzzer bz_inst;

static void bz_drive_init()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	// Enable the GPIO clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	// Configure the buzzer pin as alternate function
	GPIO_InitStruct.GPIO_Pin = BUZZER_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BUZZER_GPIO_PORT, &GPIO_InitStruct);

	// Enable the TIM2 clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	// Configure the TIM2 PWM mode, TIM4 count 1ms interrupt mode
	TIM_TimeBaseInitTypeDef tim2_init;
	tim2_init.TIM_ClockDivision = TIM_CKD_DIV1;
	tim2_init.TIM_CounterMode	= TIM_CounterMode_Up;
	tim2_init.TIM_Period		= bz_inst.volumn_max*2;
	tim2_init.TIM_Prescaler		= (72000/BUZZ_PERIOD) - 1;//Ftimer2 = 1Mhz
	TIM_TimeBaseInit( BUZZ_TIMER, &tim2_init );

	// Configure the TIM2 PWM channel
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStruct.TIM_Pulse = bz_inst.volumn_cur;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(BUZZ_TIMER, &TIM_OCInitStruct);
	TIM_Cmd(BUZZ_TIMER, ENABLE);

	// Enable the TIM4 clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_TimeBaseInitTypeDef tim4_init;
	tim4_init.TIM_ClockDivision = TIM_CKD_DIV1;
	tim4_init.TIM_CounterMode	= TIM_CounterMode_Up;
	tim4_init.TIM_Period		= BUZZ_PERIOD;
	tim4_init.TIM_Prescaler		= (72000/BUZZ_PERIOD) - 1;//Ftimer4 = 1Mhz
	TIM_TimeBaseInit( TIM4, &tim4_init );

	// Setup interrupt timer4 every 1ms for measure duration tone
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);
	TIM_Cmd(TIM4, ENABLE);
}

static inline void bz_set_sound( tie_buzzer* bz, 
	const _ui _node_lenght, const _ui* _freq, const _ui* _dura, const _ui* _start)
{
	bz->count_for_next_tone = 0;
	bz->count_for_cur_duration = 0;
	bz->idx = 0;
	bz->node_lenght = _node_lenght-1;
	bz->freq = _freq;
	bz->dura = _dura;
	bz->start = _start;
}

void TIM4_IRQHandler(void)
{
	// if (TIM_GetITStatus(BUZZ_TIMER_DURA, TIM_IT_Update) != RESET)

	TIM_ClearITPendingBit(BUZZ_TIMER_DURA, TIM_IT_Update);
	
	if( bz_inst.cur_theme == BZT_None || bz_inst.enable == 0 ) return;
	
	if( bz_inst.count_for_cur_duration > 0 ) 
	{
		if ( --bz_inst.count_for_cur_duration == 0 ) 
		{
			TIM_SetAutoreload( BUZZ_TIMER, 0 );
			// TIM_Cmd(BUZZ_TIMER, DISABLE);
		}	
	}

	++bz_inst.count_for_next_tone;

	if ( bz_inst.count_for_next_tone >= bz_inst.start[bz_inst.idx] )
	{
		if( bz_inst.idx < bz_inst.node_lenght  ) 
		{
			if( !bz_inst.freq[bz_inst.idx] ) return;
			uint32_t period = (BUZZ_PERIOD*BUZZ_PERIOD) / bz_inst.freq[bz_inst.idx];
			TIM_SetAutoreload( BUZZ_TIMER, (uint16_t)(period) );
			bz_inst.count_for_cur_duration = bz_inst.dura[bz_inst.idx];
			TIM_Cmd(BUZZ_TIMER, ENABLE);
			TIM_ITConfig(BUZZ_TIMER_DURA, TIM_IT_Update, ENABLE);
		}
		
		if( ++(bz_inst.idx) > bz_inst.node_lenght ) 
		{
			bz_inst.cur_theme = BZT_None;
			bz_set_sound(&bz_inst, 0, NULL, NULL, NULL );
			TIM_Cmd(BUZZ_TIMER, DISABLE);
			TIM_ITConfig(BUZZ_TIMER_DURA, TIM_IT_Update, DISABLE);
		}
	}
}
void bz_init()
{
	bz_inst.volumn_max = 100;
	bz_inst.volumn_cur = 20;
	bz_drive_init();

	bz_inst.count_for_next_tone = 0;
	bz_inst.count_for_cur_duration = 0;
	bz_inst.node_lenght = 0;
	bz_inst.idx = 0;
	
	bz_inst.start = NULL;
	bz_inst.freq = NULL;
	bz_inst.dura = NULL;

	bz_inst.cur_theme = BZT_None;
	bz_inst.enable = 1;
	bz_play(BZT_Init);
}

void bz_tone(uint16_t frequence, uint16_t duration)
{
	if( !frequence ) return;
	uint32_t period = (BUZZ_PERIOD*BUZZ_PERIOD) / frequence;
	TIM_SetAutoreload( BUZZ_TIMER, (uint16_t)(period) );
	bz_inst.count_for_cur_duration = duration;
	TIM_ITConfig(BUZZ_TIMER_DURA, TIM_IT_Update, ENABLE);
}

void bz_play (const tie_bz_theme theme)
{
	bz_inst.cur_theme = theme;
	switch (theme)
	{
	case BZT_Init: bz_set_sound(&bz_inst, size_init, fre_init, dura_init, start_init ); break;
	case BZT_BornFire: bz_set_sound(&bz_inst, size_born_fire, fre_born_fire, dura_born_fire, start_born_fire ); break;
	case BZT_TakeOnMe: bz_set_sound(&bz_inst, size_d, fre_d, dura_d, start_d ); break;
	case BZT_OP1: bz_set_sound(&bz_inst, size_op1, fre_op1, dura_op1, start_op1 ); break;
	case BZT_Guy: bz_set_sound(&bz_inst, size_guy, fre_guy, dura_guy, start_guy ); break;

	default: bz_stop(); break;
	}
	TIM_Cmd(BUZZ_TIMER, ENABLE);
	TIM_ITConfig(BUZZ_TIMER_DURA, TIM_IT_Update, ENABLE);
}
void bz_stop ()
{
	bz_inst.cur_theme = BZT_None;
	bz_set_sound(&bz_inst, 0, NULL, NULL, NULL );
	TIM_ITConfig(BUZZ_TIMER_DURA, TIM_IT_Update, DISABLE);
}
void bz_pause()
{
	bz_inst.cur_theme = BZT_None;
	TIM_ITConfig(BUZZ_TIMER_DURA, TIM_IT_Update, DISABLE);
}
uint8_t bz_is_running(const tie_bz_theme theme)
{
	return bz_inst.cur_theme == theme;
}
uint16_t bz_cur_duration()
{
	return bz_inst.count_for_cur_duration;
}
uint8_t bz_volumn_incr()
{	
	bz_inst.volumn_cur += 10;
	if ( bz_inst.volumn_cur > 10 && bz_inst.volumn_cur < 20 )
		bz_inst.volumn_cur = 10;
	if ( bz_inst.volumn_cur > bz_inst.volumn_max )
		bz_inst.volumn_cur = bz_inst.volumn_max;
	
	//Change Pulse in channel 1
	BUZZ_TIMER->CCR1 = bz_inst.volumn_cur;
	return bz_inst.volumn_cur;
}
uint8_t bz_volumn_decr()
{
	if ( bz_inst.volumn_cur <= 10 ) bz_inst.volumn_cur = 1;
	else bz_inst.volumn_cur -= 10;

	//Change Pulse in channel 1
	BUZZ_TIMER->CCR1 = bz_inst.volumn_cur;
	return bz_inst.volumn_cur;
}
uint8_t bz_volumn_perc()
{ return (100*bz_inst.volumn_cur) / bz_inst.volumn_max; }
