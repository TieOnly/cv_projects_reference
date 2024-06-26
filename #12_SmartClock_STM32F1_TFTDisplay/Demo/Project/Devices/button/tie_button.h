#pragma once

#include "tie_define.h"

#define BTN_PORT	GPIOB
#define BTN_1_PIN	GPIO_Pin_7
#define BTN_2_PIN	GPIO_Pin_6
#define BTN_3_PIN	GPIO_Pin_8

typedef struct {
	GPIO_TypeDef*	gpio;
	uint16_t		pin;

	uint8_t		is_down    : 1;
	uint8_t		is_press   : 1;
	uint8_t		state_last : 1;
	uint8_t		state_old  : 1;
	uint16_t	cnt_double : 8;		//<512ms
	uint16_t	cnt_delay  : 6;		//<64ms
	uint16_t	cnt_press  : 14;	//<16s
} tie_btn_t;

void btn_init( tie_btn_t* btn, GPIO_TypeDef* gpio, uint16_t pin );
void btn_update( tie_btn_t* btn, uint16_t dtime );
uint8_t btn_down( tie_btn_t* btn );
uint8_t btn_press( tie_btn_t* btn );
uint8_t btn_dou_press( tie_btn_t* btn );

extern tie_btn_t button[3];
void button_init();
void button_update( uint16_t dtime );
