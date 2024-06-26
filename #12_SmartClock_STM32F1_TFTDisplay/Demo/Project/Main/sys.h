#pragma once

#include "tie_define.h"
#include "tie_devices.h"
#include "utils.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

typedef struct user_t user_t;
struct user_t
{
	void (*init)(user_t* self);
	// void (*loop)(user_t* self);

	const char* name;
};
extern void user_inst(user_t* self, const char* name);

extern void app_init();
extern void app_loop();
