#include "tie_button.h"

void btn_init( tie_btn_t* btn, GPIO_TypeDef* gpio, uint16_t pin )
{
	btn->pin = pin;
	btn->gpio = gpio;
	if( gpio == GPIOA )
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	else if( gpio == GPIOB )
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	else if( gpio == GPIOC )
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Mode  = GPIO_Mode_IPU;
	gpio_init.GPIO_Pin   = pin;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( gpio, &gpio_init ); 

	btn->cnt_press  = 0;	//40ms
	btn->cnt_double = 0;
	
	btn->is_down    = 0;
	btn->is_press   = 0;
	btn->state_last = 0;
	btn->state_old  = 0;
}
void btn_update( tie_btn_t* btn, uint16_t dtime )
{
	//Process Event
	if ( dtime >= 40 )			
	{
		btn->is_down = !GPIO_ReadInputDataBit(btn->gpio, btn->pin);
	}
	else if ( 0 == GPIO_ReadInputDataBit( btn->gpio, btn->pin ) )
	{
		if ( btn->is_down == 0 && btn->cnt_delay == 0 ) 
		{
			btn->is_down = 1;
			btn->cnt_delay = 40;	//40ms
		}
		else if ( btn->cnt_delay > dtime ) btn->cnt_delay -= dtime;
		else btn->cnt_delay = 0;

		btn->cnt_press += dtime;
	}
	else
	{
		if ( btn->is_down == 1 && btn->cnt_delay == 0 )
		{
			btn->is_down = 0;
			btn->cnt_delay = 40;	//40ms
		}
		else if ( btn->cnt_delay > dtime ) btn->cnt_delay -= dtime;
		else btn->cnt_delay = 0;

		btn->cnt_press = 0;
	}

	//Process Press
	btn->state_old = btn->state_last;
	btn->state_last = btn->is_down;
	if ( btn->state_old == 1 && btn->state_last == 0 ) btn->is_press = 1;
	else btn->is_press = 0;
	if ( btn->cnt_double >= dtime ) btn->cnt_double -= dtime;
	else btn->cnt_double = 0;
}
uint8_t btn_press( tie_btn_t* btn ) { return btn->is_press; }
uint8_t btn_down( tie_btn_t* btn ) { return btn->is_down; }
uint8_t btn_dou_press( tie_btn_t* btn )
{
	if ( btn->is_press == 1 )
	{
		if ( btn->cnt_double == 0 )
		{
			btn->cnt_double = 250;	//500ms
			return 0;
		}
		else
		{
			btn->cnt_double = 0;
			return 1;
		}
	}
	return 0;
}

tie_btn_t button[3];

void button_init()
{
	btn_init(&button[0], BTN_PORT, BTN_1_PIN);
	btn_init(&button[1], BTN_PORT, BTN_2_PIN);
	btn_init(&button[2], BTN_PORT, BTN_3_PIN);
}
void button_update( uint16_t dtime )
{
	for (int i = 0; i < 3; ++i) btn_update(&button[i], dtime);
}
