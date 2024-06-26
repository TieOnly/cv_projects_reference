#include "tie_gui.h"
#include "jsmn.h"

void gui__rect(vec topleft, vec dimens, COLOR c_main, COLOR c_side, COLOR c_bg)
{
	// gfx__rect_fill(topleft, dimens, c_main);
	gfx__rect_thin(topleft, dimens, c_side);
	gfx_drawpixel(topleft.x, topleft.y, c_bg);
	gfx_drawpixel(topleft.x, topleft.y+dimens.y, c_bg);
	gfx_drawpixel(topleft.x+dimens.x, topleft.y, c_bg);
	gfx_drawpixel(topleft.x+dimens.x, topleft.y+dimens.y, c_bg);
}
void gui__rect_thin(vec topleft, vec dimens, COLOR c, uint8_t thick)
{
	vec tl, dm;
	for (int i = 0; i < thick; i++)
	{
		tl.x = topleft.x+i;
		tl.y = topleft.y+i;
		dm.x = dimens.x-(i*2);
		dm.y = dimens.y-(i*2);
		gfx__rect_thin( tl, dm, c );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

static struct gui_menu_t
{
	#define APP_AMOUNT 4
	
	vec app_bg_dimens;
	vec app_bg_offs[APP_AMOUNT];
	entity app_surf[APP_AMOUNT];
	entity app_side;
	entity app_side_sel;
	
	vec app_sel_offs;
	vec app_sel_dimens;
	uint8_t app_sel_idx;

} gui_menu;

void gui_menu_sel(uint8_t idx);

void gui_menu_init()
{
	gui_menu.app_bg_dimens = vec_inst(80, 80);
	gui_menu.app_bg_offs[0] = vec_inst(10, 10);
	gui_menu.app_bg_offs[1] = vec_inst(10+10+80, 10);
	gui_menu.app_bg_offs[2] = vec_inst(10, 10+80+10);
	gui_menu.app_bg_offs[3] = vec_inst(10+10+80, 10+80+10);

	gui_menu.app_sel_offs.x = gui_menu.app_bg_offs[0].x;
	gui_menu.app_sel_offs.y = gui_menu.app_bg_offs[0].y;
	gui_menu.app_sel_dimens.x = gui_menu.app_bg_dimens.x;
	gui_menu.app_sel_dimens.y = gui_menu.app_bg_dimens.y;

	ent_init(&gui_menu.app_surf[0], &gui_app_tet_anim);
	gui_menu.app_surf[0].topleft = vec_inst(18, 34);
	ent_init(&gui_menu.app_surf[1], &gui_app_setting_anim);
	gui_menu.app_surf[1].topleft = vec_inst(18+80+12, 20);
	ent_init(&gui_menu.app_surf[2], &gui_app_weather_anim);
	gui_menu.app_surf[2].topleft = vec_inst(18+80+12, 20+94);
	ent_init(&gui_menu.app_surf[3], &gui_app_clock_anim);
	gui_menu.app_surf[3].topleft = vec_inst(18+2, 20+90);

	ent_init(&gui_menu.app_side, &gui_app_side_anim);
	ent_init(&gui_menu.app_side_sel, &gui_app_side_sel_anim);

}
void gui_menu_update(void* p)
{
	//Update select app
	int sel = 0;
	if (joy_is_down()) sel = 2;
	else if (joy_is_up()) sel = -2;
	else if (joy_is_left()) sel = -1;
	else if (joy_is_right()) sel = 1;
	
	if ( sel != 0 )
	{
		int idx = gui_menu.app_sel_idx;
		idx += sel;
		idx %= 4;
		if ( idx < 0 ) idx = APP_AMOUNT + idx;
		gui_menu_sel(idx);
		bz_play(BZT_Init);
	}

	//Process choose app
	if ( !joy_is_click() ) return;

	bz_play(BZT_Init);
	p_gui_t* gui = (p_gui_t*)p;
	switch (gui_menu.app_sel_idx)
	{
	case 0: gui_switch(gui, GUI_M_APP1); break;
	case 1: gui_switch(gui, GUI_M_APP2); break;
	case 2: gui_switch(gui, GUI_M_APP3); break;
	case 3: gui_switch(gui, GUI_M_APP4); break;
	
	default: break;
	}
}
void gui_menu_draw(void* p)
{
	for (int i = 0; i < APP_AMOUNT; i++)
	{
		ent_draw(&gui_menu.app_surf[i]);
		ent_draw_at(gui_menu.app_bg_offs[i].x, gui_menu.app_bg_offs[i].y, &gui_menu.app_side);
	}
	ent_draw_at(gui_menu.app_sel_offs.x, gui_menu.app_sel_offs.y, &gui_menu.app_side_sel);
}
void gui_menu_sel(uint8_t idx)
{
	if (idx >= APP_AMOUNT) return;
	gui_menu.app_sel_idx = idx;
	vec_copy( &gui_menu.app_sel_offs, gui_menu.app_bg_offs[idx] ); 
}



static struct gui_sett_t
{
	entity title;
	entity vol;
	entity light;

	vec bar_offs[2];
	vec bar_dimens[2];
	vec bar_cont[2];

	vec bar_sel_offs;
	vec bar_sel_dimens;
	uint8_t bar_sel_idx;
} gui_sett;

void gui_setting_init()
{
	ent_init(&gui_sett.title, &setting_title_anim);
	ent_init(&gui_sett.vol, &setting_vol_anim);
	ent_init(&gui_sett.light, &setting_light_anim);
	gui_sett.title.topleft = vec_inst(40, 10);
	gui_sett.vol.topleft = vec_inst(20, 80);
	gui_sett.light.topleft = vec_inst(20, 80+32+10);

	gui_sett.bar_offs[0] = vec_inst(20+32+4, 90);
	gui_sett.bar_offs[1] = vec_inst(20+32+4, 90+32+10);
	gui_sett.bar_dimens[0] = vec_inst(100, 12);
	gui_sett.bar_dimens[1] = vec_inst(100, 12);
	gui_sett.bar_cont[0] = vec_inst(bz_volumn_perc(), 12);
	gui_sett.bar_cont[1] = vec_inst(tie_tft_light_perc(), 12);
	
	gui_sett.bar_sel_dimens = vec_inst(100, 12);
	gui_sett.bar_sel_idx = 0;
	gui_sett.bar_sel_offs = gui_sett.bar_offs[0];
}
void gui_setting_update(void* p)
{
	if (!bz_is_running(BZT_BornFire)) bz_play(BZT_BornFire);

	if (joy_is_click()) gui_switch((p_gui_t*)p, GUI_M_MENU);
	if (joy_is_down() || joy_is_up())
	{
		if (gui_sett.bar_sel_idx == 0)
		{
			gui_sett.bar_sel_idx = 1;
			gui_sett.bar_sel_offs.y = gui_sett.bar_offs[1].y;
		}
		else
		{
			gui_sett.bar_sel_idx = 0;
			gui_sett.bar_sel_offs.y = gui_sett.bar_offs[0].y;
		}
	}
	if (joy_is_right())
	{
		if (gui_sett.bar_sel_idx == 0)
		{
			bz_volumn_incr();
			gui_sett.bar_cont[0].x = bz_volumn_perc();
		}
		else 
		{
			tie_tft_light_incr();
			gui_sett.bar_cont[1].x = tie_tft_light_perc();
		}
	}
	if (joy_is_left())
	{
		if (gui_sett.bar_sel_idx == 0)
		{
			bz_volumn_decr();
			gui_sett.bar_cont[0].x = bz_volumn_perc();
		}
		else 
		{
			tie_tft_light_decr();
			gui_sett.bar_cont[1].x = tie_tft_light_perc();
		}
	}
}
void gui_setting_draw(void* p)
{
	ent_draw(&gui_sett.title);
	ent_draw(&gui_sett.vol);
	ent_draw(&gui_sett.light);

	gfx__rect_fill(gui_sett.bar_offs[0], gui_sett.bar_cont[0], COLOR_YELLOW);
	gfx__rect_fill(gui_sett.bar_offs[1], gui_sett.bar_cont[1], COLOR_YELLOW);
	gui__rect_thin(gui_sett.bar_offs[0], gui_sett.bar_dimens[0], COLOR_DGREEN, 2);
	gui__rect_thin(gui_sett.bar_offs[1], gui_sett.bar_dimens[1], COLOR_DGREEN, 2);
	gui__rect_thin(gui_sett.bar_sel_offs, gui_sett.bar_sel_dimens, COLOR_RED, 2);

	gfx_draw_string(40, 90+(32+10)*2, "Back by Click", COLOR_DGREEN, &font_microsoft_16);
}



static struct gui_clock_t
{
	entity num_h_10;
	entity num_h_1;
	entity num_m_10;
	entity num_m_1;
	entity num_s_10;
	entity num_s_1;
	vec clock_offs;

	char label_day_main[20];
	char label_day_mon[4];
	uint8_t label_day_day;
	uint16_t label_day_year;

	uint8_t time_h;
	uint8_t time_m;
	uint8_t time_s;
	uint16_t time_cnt;
	
	bool is_show;
	int8_t label_sel;
	char label_back[5];
	char label_cf[7];

	bool is_config;
	uint8_t cf_time[3];
	uint16_t cf_day[3];
	char cf_title_time[5];
	char cf_title_day[5];
	char cf_title_net[20];
	char cf_net_state[20];
	char cf_save[5];
	int8_t cf_label_sel;
	COLOR cf_tt_time_col;
	COLOR cf_tt_day_col;
	COLOR cf_tt_net_col;
	COLOR cf_tt_save_col;
	COLOR cf_tt_net_state_col;
	bool cf_is_sub_cf;
	int8_t cf_time_sel;
	int8_t cf_day_sel;

	bool is_net_proc;
	uint16_t net_timeout;
} gui_clock;

static struct net_t
{
	uint8_t data_rx[128];
	uint8_t data_tx[20];
	jsmn_parser js_parse;
	jsmntok_t js_toks[20];
} net;

void gui_clock_config_init();
void gui_clock_config_update();
void gui_clock_config_draw();
void gui_clock_config_get_net();
void gui_clock_config_init()
{
	gui_clock.is_config = 1;
	gui_clock.cf_label_sel = 0;
	gui_clock.cf_tt_time_col = COLOR_YELLOW;
	gui_clock.cf_tt_day_col = COLOR_LGREEN;
	gui_clock.cf_tt_net_col = COLOR_LGREEN;
	gui_clock.cf_tt_net_state_col = COLOR_LIGHT;
	gui_clock.cf_tt_save_col = COLOR_LGREEN;
	strcpy(gui_clock.cf_title_time, "Time");
	strcpy(gui_clock.cf_title_day, "Day");
	strcpy(gui_clock.cf_title_net, "From Internet");
	strcpy(gui_clock.cf_net_state, "Get");
	strcpy(gui_clock.cf_save, "Save");
	gui_clock.cf_day[0] = 10;
	gui_clock.cf_day[1] = 10;
	gui_clock.cf_day[2] = 2000;

	gui_clock.is_net_proc = 0;
	gui_clock.net_timeout = 0;
}
void gui_clock_config_get_net()
{
	if (gui_clock.net_timeout == 0)
	{
		strcpy(gui_clock.cf_net_state, "Waiting...");
		rs232_clear_buff();
		rs232_send("{\"getTime\":\"1\"}");
	}

	gui_clock.net_timeout += ft_dtime();
	gui_clock.cf_tt_net_state_col = gui_clock.cf_tt_net_state_col == COLOR_LIGHT ? COLOR_YELLOW : COLOR_LIGHT;
	if (gui_clock.net_timeout > 2000)
	{
		strcpy(gui_clock.cf_net_state, "Re-Get");
		gui_clock.cf_is_sub_cf = 0;
		gui_clock.net_timeout = 0;
		gui_clock.cf_tt_net_state_col = COLOR_LIGHT;
	}

	if (rs232_available( '\0' ) > 0) 
	{
		memset(net.data_rx, 0, 128);
		rs232_get_buff(net.data_rx);

		jsmn_init(&net.js_parse);
		const char* js_string = (const char*)net.data_rx;
		rs232_send(js_string);
		int r = jsmn_parse(
			&net.js_parse, js_string, strlen(js_string), net.js_toks,
			sizeof(net.js_toks) / sizeof(net.js_toks[0])
		);
		
		if (r < 0) 
		{
			strcpy(gui_clock.cf_net_state, "error");
			gui_clock.net_timeout = 1500;
		}
		else if (r < 1 || net.js_toks[0].type != JSMN_OBJECT)
		{
			strcpy(gui_clock.cf_net_state, "error");
			gui_clock.net_timeout = 1500;
		}	

		else 
		{
			uint64_t h, m, s = 100;
			uint64_t da, mo, ye = 0;
			for ( int i = 1; i < r; i++) 
			{
				if (jsoneq(js_string, &net.js_toks[i], "h") == 0) 
				{
					json_parse_uint64(js_string, &(net.js_toks[++i]), &h);
				}
				else if (jsoneq(js_string, &net.js_toks[i], "m") == 0) 
				{
					json_parse_uint64(js_string, &(net.js_toks[++i]), &m);
				}
				else if (jsoneq(js_string, &net.js_toks[i], "s") == 0) 
				{
					json_parse_uint64(js_string, &(net.js_toks[++i]), &s);
				}
				else if (jsoneq(js_string, &net.js_toks[i], "da") == 0) 
				{
					json_parse_uint64(js_string, &(net.js_toks[++i]), &da);
				}
				else if (jsoneq(js_string, &net.js_toks[i], "mo") == 0) 
				{
					json_parse_uint64(js_string, &(net.js_toks[++i]), &mo);
				}
				else if (jsoneq(js_string, &net.js_toks[i], "ye") == 0) 
				{
					json_parse_uint64(js_string, &(net.js_toks[++i]), &ye);
				}
			}
			
			if (h != 100 && m != 100 && s != 100)
			{
				//rs232_send("Done Time");
				gui_clock.time_h = gui_clock.cf_time[0] = h;
				gui_clock.time_m = gui_clock.cf_time[1] = m;
				gui_clock.time_s = gui_clock.cf_time[2] = s;
			}
			if (da != 0 && mo != 0 && ye != 0)
			{
				//rs232_send("Done Day");
				gui_clock_set_day(da, mo, ye);
				gui_clock.cf_day[0] = da;
				gui_clock.cf_day[1] = mo;
				gui_clock.cf_day[2] = ye;

				gui_clock.net_timeout = 0;
				gui_clock.cf_tt_net_state_col = COLOR_LIGHT;
				strcpy(gui_clock.cf_net_state, "Get");
				gui_clock.is_config = 0;
				gui_clock.cf_is_sub_cf = 0;
				gui_clock.is_show = 1;
			}
		}
	}	
}
void gui_clock_config_update()
{
	gui_clock.cf_tt_time_col = gui_clock.cf_label_sel == 0 ? COLOR_YELLOW : COLOR_LGREEN;
	gui_clock.cf_tt_day_col = gui_clock.cf_label_sel == 1 ? COLOR_YELLOW : COLOR_LGREEN;
	gui_clock.cf_tt_net_col = gui_clock.cf_label_sel == 2 ? COLOR_YELLOW : COLOR_LGREEN;
	gui_clock.cf_tt_save_col = gui_clock.cf_label_sel == 3 ? COLOR_YELLOW : COLOR_LGREEN;

	if ( !gui_clock.cf_is_sub_cf )
	{
		if (joy_is_up()) 
		{
			gui_clock.cf_label_sel -= 1;
			if ( gui_clock.cf_label_sel == -1 ) gui_clock.cf_label_sel = 3;
		}
		if (joy_is_down()) 
		{
			gui_clock.cf_label_sel += 1;
			if ( gui_clock.cf_label_sel == 4 ) gui_clock.cf_label_sel = 0;
		}
		if (joy_is_click()) gui_clock.cf_is_sub_cf = 1;
		return;
	}

	switch (gui_clock.cf_label_sel)
	{
	case 0:
	{
		if ( joy_is_right() ) gui_clock.cf_time_sel += 1;
		gui_clock.cf_time_sel %= 3;
		if (joy_is_up()) gui_clock.cf_time[gui_clock.cf_time_sel] += 1;
		if (joy_is_down() && gui_clock.cf_time[gui_clock.cf_time_sel] > 0)
			gui_clock.cf_time[gui_clock.cf_time_sel] -= 1;
	} break;

	case 1:
	{
		if ( joy_is_right() ) gui_clock.cf_day_sel += 1;
		gui_clock.cf_day_sel %= 3;
		if (joy_is_up()) gui_clock.cf_day[gui_clock.cf_day_sel] += 1;
		if (joy_is_down() && gui_clock.cf_day[gui_clock.cf_day_sel] > 0)
			gui_clock.cf_day[gui_clock.cf_day_sel] -= 1;
	} break;

	case 2:
	{
		gui_clock_config_get_net();
	} break;

	case 3:
	{
		gui_clock.is_config = 0;
		gui_clock.time_cnt = 0;
		gui_clock.time_h = gui_clock.cf_time[0];
		gui_clock.time_m = gui_clock.cf_time[1];
		gui_clock.time_s = gui_clock.cf_time[2];
		gui_clock_set_day(gui_clock.cf_day[0], gui_clock.cf_day[1], gui_clock.cf_day[2]);
		strcpy(gui_clock.cf_net_state, "Get");
	} break;
	
	default: break;
	}
	if (joy_is_click()) gui_clock.cf_is_sub_cf = 0;
}
void gui_clock_config_draw()
{
	gfx_draw_string(0, 0, gui_clock.cf_title_time, gui_clock.cf_tt_time_col, &font_microsoft_16);
	gfx_draw_string(0, 60, gui_clock.cf_title_day, gui_clock.cf_tt_day_col, &font_microsoft_16);
	gfx_draw_string(0, 120, gui_clock.cf_title_net, gui_clock.cf_tt_net_col, &font_microsoft_16);
	gfx_draw_string(0, 140, gui_clock.cf_net_state, gui_clock.cf_tt_net_state_col, &font_microsoft_16);
	gfx_draw_string(0, 170, gui_clock.cf_save, gui_clock.cf_tt_save_col, &font_microsoft_16);
	
	if ( gui_clock.cf_is_sub_cf )
	{
		for ( int i = 0; i < 160; ++i ) gfx_drawpixel(i, 40, gui_clock.cf_tt_time_col);
		for ( int i = 0; i < 160; ++i ) gfx_drawpixel(i, 100, gui_clock.cf_tt_day_col);
		for ( int i = 0; i < 160; ++i ) gfx_drawpixel(i, 160, gui_clock.cf_tt_net_col);
		for ( int i = 0; i < 3; ++i )
		{
			if ( gui_clock.cf_label_sel == 0 && gui_clock.cf_time_sel == i )
				gfx_draw_number(20*i, 20, gui_clock.cf_time[i], COLOR_YELLOW);
			else
				gfx_draw_number(20*i, 20, gui_clock.cf_time[i], COLOR_LIGHT);

			if ( gui_clock.cf_label_sel == 1 && gui_clock.cf_day_sel == i )
				gfx_draw_number(30*i, 80, gui_clock.cf_day[i], COLOR_YELLOW);
			else
				gfx_draw_number(30*i, 80, gui_clock.cf_day[i], COLOR_LIGHT);
		}
	}
	else
	{
		for ( int i = 0; i < 160; ++i ) gfx_drawpixel(i, 40, COLOR_LGREEN);
		for ( int i = 0; i < 160; ++i ) gfx_drawpixel(i, 100, COLOR_LGREEN);
		for ( int i = 0; i < 160; ++i ) gfx_drawpixel(i, 160, COLOR_LGREEN);
		for ( int i = 0; i < 3; ++i )
		{
			gfx_draw_number(20*i, 20, gui_clock.cf_time[i], COLOR_LIGHT);
			gfx_draw_number(30*i, 80, gui_clock.cf_day[i], COLOR_LIGHT);
		}
	}

}

void gui_clock_init()
{
	gui_clock_config_init();
	gui_clock.is_show = 1;
	strcpy(gui_clock.label_back, "Back");
	strcpy(gui_clock.label_cf, "Config");

	ent_init(&gui_clock.num_h_10, &clock_num_anim);
	ent_init(&gui_clock.num_h_1, &clock_num_anim);
	ent_init(&gui_clock.num_m_10, &clock_num_anim);
	ent_init(&gui_clock.num_m_1, &clock_num_anim);
	ent_init(&gui_clock.num_s_10, &clock_num_anim);
	ent_init(&gui_clock.num_s_1, &clock_num_anim);

	gui_clock.clock_offs = vec_inst(10, 20);
	int16_t offs_x = gui_clock.clock_offs.x;
	int16_t offs_y = gui_clock.clock_offs.y;
	gui_clock.num_h_10.topleft = vec_inst(offs_x + 21*0, offs_y);
	gui_clock.num_h_1.topleft  = vec_inst(offs_x + 21*1 + 5, offs_y);
	gui_clock.num_m_10.topleft = vec_inst(offs_x + 21*2 + 14, offs_y);
	gui_clock.num_m_1.topleft  = vec_inst(offs_x + 21*3 + 14+5, offs_y);
	gui_clock.num_s_10.topleft  = vec_inst(offs_x + 21*4 + 28, offs_y);
	gui_clock.num_s_1.topleft  = vec_inst(offs_x + 21*5 + 28, offs_y);

	gui_clock_set_day(10, 5, 2024);
}
void gui_clock_irq_service(void)
{
	DCore_GPIO_Toggle(GPIOC, GPIO_Pin_13);
	gui_clock.time_s += 1;
	if (gui_clock.time_s > 59)
	{
		gui_clock.time_s = 0;
		gui_clock.time_m += 1;
	}
	if(gui_clock.time_m > 59)
	{
		gui_clock.time_m = 0;
		gui_clock.time_h += 1;
	}
	if (gui_clock.time_h > 23) gui_clock.time_h = 0;
}
void gui_clock_set_day(uint8_t day, uint8_t month, uint16_t year)
{
	if (day > 31 || day == 0) day = 1;;
	if (month > 12 || month == 0) month = 1;
	switch (month)
	{
	case 1: strcpy(gui_clock.label_day_mon, "Jan"); break;
	case 2: strcpy(gui_clock.label_day_mon, "Feb"); break;
	case 3: strcpy(gui_clock.label_day_mon, "Mar"); break;
	case 4: strcpy(gui_clock.label_day_mon, "Apr"); break;
	case 5: strcpy(gui_clock.label_day_mon, "May"); break;
	case 6: strcpy(gui_clock.label_day_mon, "Jun"); break;
	case 7: strcpy(gui_clock.label_day_mon, "Jul"); break;
	case 8: strcpy(gui_clock.label_day_mon, "Arg"); break;
	case 9: strcpy(gui_clock.label_day_mon, "Sep"); break;
	case 10: strcpy(gui_clock.label_day_mon, "Oct"); break;
	case 11: strcpy(gui_clock.label_day_mon, "Nov"); break;
	case 12: strcpy(gui_clock.label_day_mon, "Dec"); break;
	default: break;
	}
	gui_clock.label_day_day = day;
	gui_clock.label_day_year = year;
}
void gui_clock_update(void* p)
{
	if (gui_clock.is_config)
	{
		gui_clock_config_update();
		return;
	}

	if (gui_clock.is_show == 0)
	{
		if (btn_press(&button[0]))
		{
			gui_clock.is_show = 1;
			gui_switch((p_gui_t*)p, GUI_M_MENU);
			return;
		}
		if (btn_press(&button[2]))
		{
			gui_clock.is_config = 1;
			gui_clock.cf_label_sel = 0;
			gui_clock.is_show = 1;
		}
	}
	ent_set_idx(&gui_clock.num_h_1, gui_clock.time_h % 10);
	ent_set_idx(&gui_clock.num_h_10, gui_clock.time_h / 10);
	ent_set_idx(&gui_clock.num_m_1, gui_clock.time_m % 10);
	ent_set_idx(&gui_clock.num_m_10, gui_clock.time_m / 10);
	ent_set_idx(&gui_clock.num_s_1, gui_clock.time_s % 10);
	ent_set_idx(&gui_clock.num_s_10, gui_clock.time_s / 10);

	if (joy_is_click()) gui_clock.is_show = !gui_clock.is_show;
}
void gui_clock_draw(void* p)
{
	if (gui_clock.is_config) 
	{
		gui_clock_config_draw();
		return;
	}

	if (gui_clock.is_show == 0)
	{
		gfx_draw_string(0, 160, gui_clock.label_back, COLOR_LGREEN, &font_microsoft_16 );
		gfx_draw_string(120, 160, gui_clock.label_cf, COLOR_LGREEN, &font_microsoft_16 );
	}

	ent_draw(&gui_clock.num_h_10);
	ent_draw(&gui_clock.num_h_1);
	ent_draw(&gui_clock.num_m_10);
	ent_draw(&gui_clock.num_m_1);
	ent_draw(&gui_clock.num_s_10);
	ent_draw(&gui_clock.num_s_1);

	vec topleft, offs, dimens = vec_inst(4, 4);
	offs = gui_clock.clock_offs;
	topleft = vec_inst(offs.x + 21*2 + 8, offs.y + 10);
	gfx__rect_fill(topleft, dimens, COLOR_YELLOW);
	topleft = vec_inst(offs.x + 21*2 + 8, offs.y + 20);
	gfx__rect_fill(topleft, dimens, COLOR_YELLOW);
	topleft = vec_inst(offs.x + 21*5 + 2, offs.y + 10);
	gfx__rect_fill(topleft, dimens, COLOR_YELLOW);
	topleft = vec_inst(offs.x + 21*5 + 2, offs.y + 20);
	gfx__rect_fill(topleft, dimens, COLOR_YELLOW);

	gfx_draw_string(40, 60, gui_clock.label_day_mon, COLOR_YELLOW, &font_microsoft_16);
	gfx_draw_number(76, 60, gui_clock.label_day_day, COLOR_YELLOW);
	gfx_draw_string(94, 60, ",", COLOR_YELLOW, &font_microsoft_16);
	gfx_draw_number(104, 60, gui_clock.label_day_year, COLOR_YELLOW);
}



static struct gui_wt_t
{
	entity sun;
	entity cloud;
	entity rain;
	entity label;

	entity degree_n10;
	entity degree_n1;
	entity degree_o;
	int16_t degree_num;

	entity moisture_n10;
	entity moisture_n1;
	int16_t moisture_num;

	bool is_get_net;
	uint16_t net_timeout;
	char net_state[20];
} gui_wt;

void gui_weather_get_net();

void gui_weather_init()
{
	ent_init(&gui_wt.label, &weather_title_anim);
	
	ent_init(&gui_wt.degree_n10, &clock_num_anim);
	ent_init(&gui_wt.degree_n1, &clock_num_anim);
	ent_init(&gui_wt.degree_o, &weather_o_anim);

	ent_init(&gui_wt.moisture_n10, &clock_num_anim);
	ent_init(&gui_wt.moisture_n1, &clock_num_anim);

	ent_init(&gui_wt.sun, &weather_sun_anim);
	ent_init(&gui_wt.cloud, &weather_cloud_anim);
	ent_init(&gui_wt.rain, &weather_rain_anim);

	gui_wt.label.topleft = vec_inst(40, 0);
	gui_wt.degree_n10.topleft = vec_inst(60, 70);
	gui_wt.degree_n1.topleft = vec_inst(85, 70);
	gui_wt.degree_o.topleft = vec_inst(110, 70);

	gui_wt.moisture_n10.topleft = vec_inst(60, 140);
	gui_wt.moisture_n1.topleft = vec_inst(85, 140);

	gui_wt.sun.topleft = vec_inst(0, 0);
	gui_wt.cloud.topleft = vec_inst(150, 0);
	gui_wt.rain.topleft = vec_inst(160, 26);

	gui_wt.degree_num = 36;
	gui_wt.moisture_num = 80;
	gui_wt.net_timeout = 0;
	gui_wt.is_get_net = 1;
}
void gui_weather_update(void* p)
{
	ent_update(&gui_wt.sun, ft_dtime());
	ent_update(&gui_wt.cloud, ft_dtime());
	ent_update(&gui_wt.rain, ft_dtime());
	
	if (gui_wt.is_get_net)
	{
		gui_weather_get_net();
		return;
	}

	ent_set_idx(&gui_wt.degree_n10, gui_wt.degree_num / 10);
	ent_set_idx(&gui_wt.degree_n1, gui_wt.degree_num % 10);
	ent_set_idx(&gui_wt.moisture_n10, gui_wt.moisture_num / 10);
	ent_set_idx(&gui_wt.moisture_n1, gui_wt.moisture_num % 10);

}
void gui_weather_get_net()
{
	if (gui_wt.net_timeout == 0)
	{
		strcpy(gui_wt.net_state, "Waiting...");
		rs232_clear_buff();
		rs232_send("{\"getTemp\":\"1\"}");
	}

	gui_wt.net_timeout += ft_dtime();
	if (gui_wt.net_timeout > 3000)
	{
		gui_wt.net_timeout = 3000;
		strcpy(gui_wt.net_state, "Click to re-get");
		if (joy_is_click()) gui_wt.net_timeout = 0;
	}

	if (rs232_available( '\0' ) > 0) 
	{
		memset(net.data_rx, 0, 128);
		rs232_get_buff(net.data_rx);

		jsmn_init(&net.js_parse);
		const char* js_string = (const char*)net.data_rx;
		rs232_send(js_string);
		int r = jsmn_parse(
			&net.js_parse, js_string, strlen(js_string), net.js_toks,
			sizeof(net.js_toks) / sizeof(net.js_toks[0])
		);
		
		if (r < 0 || r < 1 || net.js_toks[0].type != JSMN_OBJECT)
		{
			strcpy(gui_wt.net_state, "error data");
			gui_wt.net_timeout = 2500;
		}
		else 
		{
			uint64_t nd, dm = 1000;
			for ( int i = 1; i < r; i++) 
			{
				if (jsoneq(js_string, &net.js_toks[i], "ND") == 0) 
				{
					json_parse_uint64(js_string, &(net.js_toks[++i]), &nd);
				}
				else if (jsoneq(js_string, &net.js_toks[i], "DM") == 0) 
				{
					json_parse_uint64(js_string, &(net.js_toks[++i]), &dm);
				}
			}

			if ( nd != 1000 && dm != 1000 )
			{
				gui_wt.is_get_net = false;
				gui_wt.net_timeout = 0;
				gui_wt.degree_num = nd;
				gui_wt.moisture_num = dm;
			}
		}
	}
}
void gui_weather_ready_get_net()
{
	gui_wt.is_get_net = 1;
}
void gui_weather_draw(void* p)
{
	ent_draw(&gui_wt.label);
	ent_draw(&gui_wt.sun);
	ent_draw(&gui_wt.cloud);
	ent_draw(&gui_wt.rain);

	if (gui_wt.is_get_net)
	{
		gfx_draw_string(40, 60, gui_wt.net_state, COLOR_LGREEN, &font_microsoft_16);
		return;
	}

	gfx_draw_string(30, 40, "Nhiet do hien tai", COLOR_YELLOW, &font_microsoft_16);
	ent_draw(&gui_wt.degree_n10);
	ent_draw(&gui_wt.degree_n1);
	ent_draw(&gui_wt.degree_o);

	gfx_draw_string(30, 110, "Do am hien tai", COLOR_YELLOW, &font_microsoft_16);
	ent_draw(&gui_wt.moisture_n10);
	ent_draw(&gui_wt.moisture_n1);
}



void gui_init(p_gui_t* gui, void (*p_switch)(void*))
{
	gui_menu_init();
	gui_setting_init();
	gui_clock_init();
	gui_weather_init();

	gui->mode = GUI_M_MENU;
	gui->update = &gui_menu_update;
	gui->draw = &gui_menu_draw;
	gui->p_switch = p_switch;
	gui_switch(gui, GUI_M_MENU);
}
void gui_update(p_gui_t* gui) { gui->update((void*)(gui)); }
void gui_draw(p_gui_t* gui) { gui->draw((void*)(gui)); }
void gui_switch(p_gui_t* gui, GUI_MODE mode)
{
	if (gui->p_switch == NULL) return;
	gui->mode = mode;
	gui->p_switch((void*)(gui));
}

