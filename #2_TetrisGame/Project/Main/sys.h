#pragma once

#include "tie_define.h"
#include "tie_devices.h"

typedef struct user_t user_t;
struct user_t
{
	void (*init)(user_t* self);
	void (*loop)(user_t* self);
	const char* name;

	uint16_t dtime;

	uint16_t cnt_13;
};

extern void user_inst(user_t* self, const char* name);
extern void app_init();
extern void app_loop();
