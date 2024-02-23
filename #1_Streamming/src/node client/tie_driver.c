#include "include/tie_driver.h"

//Don't directly change args of this struct, should use extern api or init in source implementation
static TieDriver sTieDriver = {
	.gpio_control = 0,
	.isInited = 0,
	.gpio_input_handeler = NULL
};

void TD_gpio_input_create( uint32_t pin_num, gpio_int_type_t type );

static void IRAM_ATTR gpio_input_irs_handler(void* args)
{
	uint32_t pin_num = (uint32_t)args;
	sTieDriver.gpio_input_handeler(pin_num);
}

void TDriver_Init()
{
	if( sTieDriver.isInited ) return;

	TD_gpio_output_normal_create(GPIO_NUM_2);

	sTieDriver.isInited = 1;
}

void TD_gpio_output_normal_create( gpio_num_t pin_num )
{
	gpio_config_t gpio;
	gpio.intr_type		= GPIO_INTR_DISABLE;
	gpio.mode			= GPIO_MODE_OUTPUT;
	gpio.pin_bit_mask	= BIT((uint32_t)pin_num);
	gpio_config( &gpio );
}
void TD_gpio_out_level( gpio_num_t pin_num, uint32_t level )
{
	gpio_set_level( pin_num, level );
	bitWrite( sTieDriver.gpio_control, (uint8_t)pin_num, level );
}
void TD_gpio_out_toggle( gpio_num_t pin_num )
{
	gpio_set_level( pin_num, (uint32_t)bitRead(sTieDriver.gpio_control, (uint8_t)pin_num) );
	bitToggle( sTieDriver.gpio_control, (uint8_t)pin_num );
}

void TD_gpio_input_create( uint32_t pin_num, gpio_int_type_t type )
{
	gpio_config_t gpio;
	gpio.intr_type		= type;
	gpio.mode			= GPIO_MODE_INPUT;
	gpio.pin_bit_mask	= BIT(pin_num);
	gpio.pull_up_en		= GPIO_PULLUP_ENABLE;
	gpio_config( &gpio );
}
void TD_gpio_input_setup( gpio_num_t pin_num, gpio_int_type_t type, void* cback )
{
	TD_gpio_input_create( pin_num, type );
	gpio_install_isr_service(0);
	gpio_isr_handler_add( pin_num, gpio_input_irs_handler, (void*)pin_num );
	sTieDriver.gpio_input_handeler = cback;
}

void TD_uart_config( uart_port_t uart_port )
{
	// Configure parameters of an UART driver,
	// communication pins and install the driver
	uart_config_t uart_config = {
		.baud_rate = 115200,
		.data_bits = UART_DATA_8_BITS,
		.parity    = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE
	};
	uart_param_config(uart_port, &uart_config);
	uart_driver_install(uart_port, BUFF_SIZE_UART * 2, 0, 0, NULL, 0);
}
