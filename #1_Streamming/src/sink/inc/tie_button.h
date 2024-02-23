#pragma once
#include "tie_define.h"
#include "tie_driver.h"

typedef void (*tie_btn_cback_t)(uint32_t);

typedef struct {
	gpio_config_t gpio;
	gpio_num_t pin;
	uint32_t markTick;
	tie_btn_cback_t cback;
	uint32_t check;
} TieButton;

void TieBTN_Init( TieButton* btn, gpio_num_t pin, gpio_pull_mode_t pull, void* cback );
void TieBTN_Set_CBack( TieButton* btn, void* cback );

//Install for all Button should be register
void TieBTN_Install_INTR_Service(TieButton* btns, uint8_t amount);
