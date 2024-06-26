#include "utils.h"
#include "tie_gui.h"
#include "board.h"
#include "tetris.h"
#include "jsmn.h"

static struct app_t
{
	bool is_sleep;
	uint16_t cnt_press;
	uint16_t dtime;
	vec board_offs;
	p_gui_t gui;
} app;

void gui_tet_update(void* p) 
{ 
	tet_update(); 
	if ( tet_is_quit() ) gui_switch((p_gui_t*)p, GUI_M_MENU);
}
void gui_tet_draw(void* p) { tet_draw(); }

void gui_switch_proc(void* p)
{
	p_gui_t* gui = (p_gui_t*)p;
	switch (gui->mode)
	{
	case GUI_M_MENU:
	{
		gui->update = &gui_menu_update;
		gui->draw = &gui_menu_draw;
	} break;

	case GUI_M_APP1:
	{
		gui->update = &gui_tet_update;
		gui->draw = &gui_tet_draw;
	} break;

	case GUI_M_APP2:
	{
		gui->update = &gui_setting_update;
		gui->draw = &gui_setting_draw;
	} break;

	case GUI_M_APP3:
	{
		gui->update = &gui_clock_update;
		gui->draw = &gui_clock_draw;
	} break;

	case GUI_M_APP4:
	{
		gui_weather_ready_get_net();
		gui->update = &gui_weather_update;
		gui->draw = &gui_weather_draw;
	}
	
	default: break;
	}
}

bool sleep_update()
{
	if (app.is_sleep)
	{
		if (btn_down(&button[1]))
		{
			app.cnt_press += app.dtime;
			if (app.cnt_press > 1000)
			{
				app.cnt_press = 0;
				app.is_sleep = 0;
				tie_tft_light_on();
				bz_play(BZT_OP1);
			}
		}
		else app.cnt_press = 0;
	}
	else if (btn_down(&button[1]))
	{
		app.cnt_press += app.dtime;
		if (app.cnt_press > 3000)
		{
			app.cnt_press = 0;
			app.is_sleep = 1;
			tie_tft_light_off();
		}
	}
	else app.cnt_press = 0;

	return app.is_sleep;
}

void app_init()
{
	app.is_sleep = 0;
	app.board_offs = vec_inst(0, 0);
	board_init( app.board_offs );
	tet_init();
	rtc_init(gui_clock_irq_service);
	gui_init(&app.gui, &gui_switch_proc);
}

void app_loop()
{
	app.dtime = ft_dtime();
	if ( sleep_update() ) return;

	gui_update(&app.gui);
	
	if ( btn_press(&button[1]) )
	{
		if (app.gui.mode != GUI_M_MENU)
			gui_switch(&app.gui, GUI_M_MENU);
		else
			gui_switch(&app.gui, GUI_M_APP3);
	}
	
	
	//Render
	gfx_clear(COLOR_BLACK);
	gui_draw(&app.gui);
	// gfx_draw_number(0, 0, ft_dtime(), COLOR_LGREEN);
	// gfx_draw_number(0, 10, joy_get_vx(), COLOR_LGREEN);
	// gfx_draw_number(0, 20, joy_get_vy(), COLOR_LGREEN);
	// gfx_draw_string(0, 10, (char*)app.data_rx, COLOR_LGREEN, &font_microsoft_16);
	gfx_display();
}