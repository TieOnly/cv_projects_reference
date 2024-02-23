#include "sys.h"

#define ref_t user_t* ref

static void Init(ref_t);
static void Loop(ref_t);

void user_inst(user_t* self, const char* name)
{
	self->name = name;
	self->init = Init;
	self->loop = Loop;
}

///////////////////////////////////////////////////////////////////////////////////////////

static void Init(ref_t)
{
	DCore_Init();
	ft_init();

	tie_lcd_init();
	tie_lcd_clear(COLOR_BLACK);
	
	bz_init();
	joy_init();

	app_init();
}

static void Loop(ref_t)
{
	ft_tick();
	ref->dtime = ft_dtime();
	
	ref->cnt_13 += ref->dtime;
	if (ref->cnt_13 >= 1000)
	{
		DCore_GPIO_Toggle(GPIOC, GPIO_Pin_13);
		ref->cnt_13 %= 1000;
		// if (!bz_is_running(BZT_OP1)) bz_play(BZT_OP1);
	}
	
	app_loop();
}

