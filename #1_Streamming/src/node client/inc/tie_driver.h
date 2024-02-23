#pragma once
#include "tie_define.h"

typedef void (*gpio_handel_f)(uint32_t);

typedef struct {
	uint8_t isInited;
	uint32_t gpio_control;
	gpio_handel_f gpio_input_handeler;
} TieDriver;

void TDriver_Init();
void TD_gpio_output_normal_create( gpio_num_t pin_num );
void TD_gpio_out_level( gpio_num_t pin_num, uint32_t level );
void TD_gpio_out_toggle( gpio_num_t pin_num );

void TD_gpio_input_setup( gpio_num_t pin_num, gpio_int_type_t type, void* cback );

#define BUFF_SIZE_UART	(1024)

void TD_uart_config( uart_port_t uart_port );
