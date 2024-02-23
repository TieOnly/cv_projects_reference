#include "tie_button.h"

#define BTN_TIME_TICK_PRESS_ACCEPT 100//tick

static void IRAM_ATTR TieBTN_pullup_irtr_cback(void* arg)
{
	TieButton* btn = (TieButton*)arg;
	uint32_t curTick = xTaskGetTickCountFromISR();
	
	if( gpio_get_level( btn->pin ) == 0 )
	{
		if( btn->markTick == 0 ) btn->markTick = curTick;
		//Start Button Timer
	
	}
	else 
	{
		//Stop Button Timer

		//Prcess Noise Pressing
		if( curTick - btn->markTick >= BTN_TIME_TICK_PRESS_ACCEPT )
		{
			btn->markTick = 0;
			btn->check++;
			btn->cback((uint32_t)btn->pin);
		}
	}
}
static void IRAM_ATTR TieBTN_pulldown_irtr_cback(void* arg)
{}

void TieBTN_Init( TieButton* btn, gpio_num_t pin, gpio_pull_mode_t pull, void* cback )
{
	gpio_config_t gpio;
	gpio.mode = GPIO_MODE_INPUT;
	gpio.pin_bit_mask = BIT((uint32_t)pin);
	gpio.intr_type = GPIO_INTR_ANYEDGE;
	if( pull == GPIO_PULLUP_ONLY )
		gpio.pull_up_en = GPIO_PULLUP_ENABLE;
	else
		gpio.pull_up_en = GPIO_PULLDOWN_ENABLE;
	gpio_config(&gpio);

	btn->gpio = gpio;
	btn->pin = pin;
	btn->markTick = 0;
	btn->cback = cback;
	btn->check = 0;
}
void TieBTN_Set_CBack( TieButton* btn, void* cback )
{
	btn->cback = cback;
}
void TieBTN_Install_INTR_Service(TieButton* btns, uint8_t amount)
{
	gpio_uninstall_isr_service();
	gpio_install_isr_service(0);
	for( uint8_t i = 0; i < amount; ++i )
	{
		if( btns[i].gpio.pull_up_en == GPIO_PULLUP_ENABLE )
			gpio_isr_handler_add( btns[i].pin, TieBTN_pullup_irtr_cback, (void*)(&btns[i]) );
		else
			gpio_isr_handler_add( btns[i].pin, TieBTN_pulldown_irtr_cback, (void*)(&btns[i]) );
	}
}