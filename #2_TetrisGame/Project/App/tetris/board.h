#pragma once

#include "utils.h"

typedef struct 
{
	uint8_t is_fill : 1;
	COLOR	color	: 7;
} cell;

#define	board_w		10
#define	board_h		20
#define cell_d		8
#define board_pad	1
typedef struct
{
	vec pos;
	cell cells[board_h][board_w];
} board;
void board_init( vec pos );
void board_draw();
void board_clear();

void board_set_cell( vec loca, COLOR color );
void board_clear_cell( vec loca );
bool board_cell_at( vec loca );
bool board_is_cover( vec loca );
COLOR board_color_at( vec loca );
