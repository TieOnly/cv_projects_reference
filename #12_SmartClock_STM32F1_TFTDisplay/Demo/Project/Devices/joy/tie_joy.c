#include "tie_joy.h"

static struct tie_joy_t
{
	uint16_t adc_vals[2];
	uint8_t x_is_idle;
	uint8_t y_is_idle;
	tie_btn_t btn;
} tie_joy;

static void joy_driver_init()
{
	//Init ADC Driver
	GPIO_InitTypeDef 	gpio_init;
	ADC_InitTypeDef		adc_init;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	gpio_init.GPIO_Mode = GPIO_Mode_AIN;
	gpio_init.GPIO_Pin = JOY_PIN_X | JOY_PIN_Y;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(JOY_PORT, &gpio_init);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	adc_init.ADC_ContinuousConvMode = ENABLE;
	adc_init.ADC_DataAlign = ADC_DataAlign_Right;
	adc_init.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	adc_init.ADC_Mode = ADC_Mode_Independent;
	adc_init.ADC_NbrOfChannel = 2;
	adc_init.ADC_ScanConvMode = ENABLE;
	ADC_Init(ADC1, &adc_init);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_41Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_41Cycles5);

	ADC_Cmd(ADC1, ENABLE);

	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1));

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	//Init ADC DMA
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_InitTypeDef dma_init;
	dma_init.DMA_BufferSize = 2;
	dma_init.DMA_DIR 		= DMA_DIR_PeripheralSRC;
	dma_init.DMA_M2M		= DMA_MemoryInc_Disable;
	dma_init.DMA_MemoryBaseAddr = (uint32_t)(tie_joy.adc_vals);
	dma_init.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	dma_init.DMA_MemoryInc		= DMA_MemoryInc_Enable;
	dma_init.DMA_Mode			= DMA_Mode_Circular;
	dma_init.DMA_PeripheralBaseAddr = (uint32_t)(&ADC1->DR);
	dma_init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	dma_init.DMA_PeripheralInc		= DMA_PeripheralInc_Disable;
	dma_init.DMA_Priority			= DMA_Priority_Medium;
	DMA_Init(DMA1_Channel1, &dma_init);
}

void joy_init()
{
	tie_joy.adc_vals[0] = 0;
	tie_joy.adc_vals[1] = 0;

	joy_driver_init();
	btn_init(&tie_joy.btn, JOY_PORT_BTN, JOY_PIN_BTN);
	joy_enable();
}
void joy_enable()
{
	ADC_DMACmd(ADC1, ENABLE);
	DMA_Cmd(DMA1_Channel1, ENABLE);
}
void joy_disable()
{
	ADC_DMACmd(ADC1, DISABLE);
	DMA_Cmd(DMA1_Channel1, DISABLE);
}
void joy_update(uint16_t dtime)
{
	tie_joy.x_is_idle = (tie_joy.adc_vals[1] < 3000) && (tie_joy.adc_vals[1] > 1000);
	tie_joy.y_is_idle = (tie_joy.adc_vals[0] < 3000) && (tie_joy.adc_vals[0] > 1000);
	btn_update(&tie_joy.btn, dtime);
}
uint16_t joy_get_vx()
{ return tie_joy.adc_vals[0]; }
uint16_t joy_get_vy()
{ return tie_joy.adc_vals[1]; }

bool joy_is_click()
{ return btn_press(&tie_joy.btn); }

bool joy_is_right()
{ return tie_joy.adc_vals[1] > 3000; }
bool joy_is_left()
{ return tie_joy.adc_vals[1] < 1000; }
bool joy_is_down()
{ return tie_joy.adc_vals[0] < 1000; }
bool joy_is_up()
{ return tie_joy.adc_vals[0] > 3000; }

bool joy_is_idle_x()
{ return !joy_is_left() && !joy_is_right(); }
bool joy_is_idle_y()
{ return !joy_is_up() && !joy_is_down(); }

bool joy_is_up_release()
{ 
	if (tie_joy.y_is_idle && joy_is_up())
	{
		tie_joy.y_is_idle = 0;
		return true;
	}
	return false;
}
bool joy_is_down_release()
{
	if (tie_joy.y_is_idle && joy_is_down())
	{
		tie_joy.y_is_idle = 0;
		return true;
	}
	return false;
}
bool joy_is_right_release()
{
	if (tie_joy.x_is_idle && joy_is_right())
	{
		tie_joy.x_is_idle = 0;
		return true;
	}
	return false;
}
bool joy_is_left_release()
{
	if (tie_joy.x_is_idle && joy_is_left())
	{
		tie_joy.x_is_idle = 0;
		return true;
	}
	return false;
}
