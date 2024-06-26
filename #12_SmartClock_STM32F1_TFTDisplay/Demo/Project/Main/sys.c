#include "sys.h"

#define ref_t user_t* ref

static void Init(ref_t);
// static void Loop(ref_t);

void user_inst(user_t* self, const char* name)
{
	self->name = name;
	self->init = Init;
	// self->loop = Loop;
}
///////////////////////////////////////////////////////////////////////////////////////////
static void MAIN_TASK( void *p );
static void SUB_TASK( void *p );
///////////////////////////////////////////////////////////////////////////////////////////

static void Init(ref_t)
{
	DCore_Init();
	ft_init();
	joy_init();
	button_init();
	rs232_init();
	bz_init();
	tie_tft_init();
	app_init();

	xTaskCreate(MAIN_TASK, "T1", 70, NULL, 5, NULL);
}

static void MAIN_TASK( void *p )
{
	while(1)
	{
		ft_tick();
		uint16_t dtime = ft_dtime();
		app_loop();
		joy_update(dtime);
		button_update(dtime);
	}
}

static void SUB_TASK( void *p )
{
	
}