#pragma once
#include "tie_define.h"

#define RS232_SIZE_BUFF		128
#define RS232_USART			USART1
#define RS232_TX_PORT		GPIOA
#define RS232_RX_PORT		GPIOA
#define RS232_TX_PIN		GPIO_Pin_9
#define RS232_RX_PIN		GPIO_Pin_10
#define RS232_BAUD			115200

//NOTE: RCC must enble exactly
#define RS232_ENABLE_RCC_USART		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE)
#define RS232_ENABLE_RCC_GPIO		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)
// #define RS232_ENABLE_RCC_USART		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE)
// #define RS232_ENABLE_RCC_GPIO		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE)

void rs232_init();
void rs232_enable( uint8_t is_enable );
void rs232_send( const char* data );
void rs232_send_val( uint32_t value );
uint32_t rs232_available( const char end_char );
void rs232_get_buff( uint8_t* dest );
void rs232_clear_buff();
uint8_t* rs232_buff();
