#include "board.h"

static board bd;

void board_init( vec pos )
{
	bd.pos = vec_inst(pos.x, pos.y);
	memset(bd.cells, 0, board_w*board_h);
}
void board_draw()
{
	gfx__rect_thin(
		bd.pos, vec_inst(
			board_w*(cell_d+board_pad)+2*board_pad, 
			board_h*(cell_d+board_pad)+2*board_pad
		), COLOR_RED
	);
	vec cell_dimen = vec_inst(cell_d, cell_d);
	for ( int y = 0; y < board_h; ++y )
	{
		for ( int x = 0; x < board_w; ++x )
		{
			if ( bd.cells[y][x].is_fill )
			{
				vec loca = vec_inst( 
					bd.pos.x + 2*board_pad + x*(cell_d + board_pad) , 
					bd.pos.y + 2*board_pad + y*(cell_d + board_pad) 
				);
				gfx__rect_fill( loca, cell_dimen, bd.cells[y][x].color );
			}
		}
	}
}
void board_clear()
{
	memset(bd.cells, 0, board_w*board_h);
}

void board_set_cell( vec loca, COLOR color )
{
	if ( loca.x < 0 || loca.x >= board_w ) return;
	if ( loca.y < 0 || loca.y >= board_h ) return;
	bd.cells[loca.y][loca.x].is_fill = 1;
	bd.cells[loca.y][loca.x].color = color;
}
void board_clear_cell( vec loca )
{
	if ( loca.x < 0 || loca.x >= board_w ) return;
	if ( loca.y < 0 || loca.y >= board_h ) return;
	bd.cells[loca.y][loca.x].is_fill = 0;
	bd.cells[loca.y][loca.x].color = COLOR_BLACK;
}
bool board_cell_at( vec loca )
{
	if ( loca.x < 0 || loca.x >= board_w ) return 0;
	if ( loca.y < 0 || loca.y >= board_h ) return 0;
	return bd.cells[loca.y][loca.x].is_fill;
}
bool board_is_cover( vec loca )
{
	if ( loca.x < 0 || loca.x >= board_w ) return 0;
	if ( loca.y < 0 || loca.y >= board_h ) return 0;
	return 1;
}
COLOR board_color_at( vec loca )
{
	if ( loca.x < 0 || loca.x >= board_w ) return COLOR_BLACK;
	if ( loca.y < 0 || loca.y >= board_h ) return COLOR_BLACK;
	return bd.cells[loca.y][loca.x].color;
}
