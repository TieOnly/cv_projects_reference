#pragma once
#include "utils.h"
#include "asset.h"

void gui__rect(vec topleft, vec dimens, COLOR c_main, COLOR c_side, COLOR c_bg);
void gui__rect_thin(vec topleft, vec dimens, COLOR c, uint8_t thick);


typedef enum 
{
	GUI_M_MENU, GUI_M_APP1, GUI_M_APP2, GUI_M_APP3, GUI_M_APP4
} GUI_MODE;

typedef struct 
{
	void (*update)(void*);
	void (*draw)(void*);
	void (*p_switch)(void*);
	GUI_MODE mode;
} p_gui_t;

void gui_init(p_gui_t* gui, void (*p_switch)(void*));
void gui_update(p_gui_t* gui);
void gui_draw(p_gui_t* gui);
void gui_switch(p_gui_t* gui, GUI_MODE mode);

void gui_menu_init();
void gui_menu_update(void* p);
void gui_menu_draw(void* p);

void gui_tet_update(void* p);
void gui_tet_draw(void* p);

void gui_setting_init();
void gui_setting_update(void* p);
void gui_setting_draw(void* p);

void gui_clock_init();
void gui_clock_irq_service(void);
void gui_clock_update(void* p);
void gui_clock_draw(void* p);
void gui_clock_set_day(uint8_t day, uint8_t month, uint16_t year);

void gui_weather_init();
void gui_weather_update(void* p);
void gui_weather_draw(void* p);
void gui_weather_ready_get_net();

