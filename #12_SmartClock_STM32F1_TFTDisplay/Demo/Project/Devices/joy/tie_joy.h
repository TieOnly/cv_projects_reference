#pragma once

#include "tie_define.h"
#include "tie_button.h"

#define JOY_PIN_X	GPIO_Pin_1
#define JOY_PIN_Y	GPIO_Pin_2
#define JOY_PORT	GPIOA
#define JOY_PIN_BTN		GPIO_Pin_3
#define JOY_PORT_BTN	GPIOA

void joy_init();
void joy_enable();
void joy_disable();
void joy_update(uint16_t dtime);

uint16_t joy_get_vx();
uint16_t joy_get_vy();

bool joy_is_click();

bool joy_is_idle_x();
bool joy_is_idle_y();

bool joy_is_up();
bool joy_is_down();
bool joy_is_right();
bool joy_is_left();

bool joy_is_up_release();
bool joy_is_down_release();
bool joy_is_right_release();
bool joy_is_left_release();
