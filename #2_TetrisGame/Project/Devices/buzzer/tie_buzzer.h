#pragma once

#include "tie_define.h"

#define BUZZER_GPIO_PORT 	GPIOA
#define BUZZER_GPIO_PIN 	GPIO_Pin_0
#define BUZZ_TIMER			TIM2
#define BUZZ_TIMER_DURA		TIM4
#define BUZZ_PERIOD 		1000

typedef enum {
	BZT_None, BZT_Init, BZT_BornFire, BZT_TakeOnMe, BZT_OP1
} tie_bz_theme;

typedef struct {
	uint16_t count_for_next_tone;
	uint16_t count_for_cur_duration;
	uint16_t idx;
	uint16_t node_lenght;
	
	const uint16_t* freq;
	const uint16_t* dura;
	const uint16_t* start;

	tie_bz_theme cur_theme;
	uint8_t enable;
} tie_buzzer;

void bz_init();

void bz_tone (uint16_t frequence, uint16_t duration);
void bz_play (const tie_bz_theme theme);
void bz_stop ();
void bz_pause();
uint8_t bz_is_running(const tie_bz_theme theme);
uint16_t bz_cur_duration();