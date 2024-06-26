#pragma once
#include "tie_define.h"

#define FT_TIMER	TIM1

void ft_init(void);
void ft_tick(void);
void ft_delay(uint16_t ms);
uint16_t ft_dtime(void);
uint16_t ft_dtime_micro(void);
void rtc_init(void (*irq_service)(void));
