#pragma once
#include <time.h>

#include "tie_define.h"
#include "tie_st7735.h"
#include "pool.h"

typedef struct
{
	uint16_t w;
	uint64_t amount;
	uint16_t *pixels;
} t_bmp;

typedef struct 
{
	uint16_t *pixels;
} t_bmp_data;

typedef enum
{
	IDA_CLOUD,
	IDA_YTB,

	IDA_TET_TITLE,
	IDA_TET_START_BTN,
	IDA_TET_OPT_BTN,
	IDA_TET_BACK_BTN,
	IDA_TET_FRAME_BTN,
	IDA_TET_ARROW
} id_anim;

typedef struct 
{
	uint16_t w, h;
	uint64_t len_of_frame;//amount element each of bmp
	uint16_t frame;//amount frame of animation
	t_bmp_data *anim;//duration and counting active of each frame
	id_anim id;//enum id of animation
} t_anim;

#define COLOR_BLACK 	0x0001
#define COLOR_BG 		0x0000
#define COLOR_DGREEN 	0x8430
#define COLOR_LGREEN 	0x07E0
#define COLOR_LIGHT 	0x7D7C
#define COLOR_RED 		0xF820
#define COLOR_ORANGE 	0xfdc4
#define COLOR_YELLOW 	0xFFE0

typedef struct
{
	int16_t x;
	int16_t y;
} vec;
vec vec_inst( int16_t x, int16_t y );
void vec_copy( vec* own, vec other );
void vec_add( vec* own, vec* other );

typedef struct 
{
	vec topleft;
	vec dimens;
	uint8_t scale;
	const t_anim* sAnim;
	uint32_t anim_cnt;
	uint32_t anim_dura;
	uint8_t anim_idx;
} entity;

void ent_init( entity* ent, const t_anim* sAnim );
void ent_scale( entity* ent, uint8_t scale );
void ent_update( entity* ent, uint32_t dtime );
void ent_draw( entity* ent );
void ent_draw_at( vec topleft, entity* ent );
void ent_set_idx( entity* ent, uint8_t idx );

void gfx__rect_thin( vec topleft, vec dimens, uint16_t color );
void gfx__rect_fill( vec topleft, vec dimens, uint16_t color );
void gfx__draw_pixels( vec topleft, uint16_t width, const uint16_t* pixels, uint64_t len, uint8_t scale );
// void gfx__line( int16_t x1, int16_t y1, int16_t x2, int16_t y2, COLOR color );

//Lấy số ngẫu nhiên dựa trên timer
uint16_t util_random( uint16_t begin, uint16_t end );
//Convert string về số nguyên lớn
bool str_to_u64(const char* str, uint64_t* num);
//Cập nhật thời gian
uint32_t tick_dtime();

typedef enum
{
    S_CTRL_UP, S_CTRL_DN, S_CTRL_RT, S_CTRL_LT,
    S_CTRL_ENT,
    S_CTRL_COUNT
} t_ctrl;
void signal_ctrl_set( t_ctrl ctrl );
bool signal_ctrl_get( t_ctrl ctrl );
void signal_ctrl_clear();
void signal_mtx_lock();
void signal_mtx_unlock();



#define bit16Read(value, bit)   (((value) << (bit)) & 0x8000)
	//Read Left -> Right increasing
#define bit16Set(value, bit)    ((value) |= (0x8000 >> (bit)))
#define bit16Clear(value, bit)  ((value) &= ~(0x8000 >> (bit))) 
#define bit16Write(value, bit, bitvalue)  ((bitvalue) ? bit16Set(value, bit) : bit16Clear(value, bit))

void assets_load();
t_anim* assets_tetris_get( id_anim id );
t_anim* load_anim( id_anim id, const char* filename, int num_of_crop );

#pragma once
#include "utils.h"

typedef struct 
{
	uint8_t is_fill;
	uint16_t color;
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

void board_set_cell( vec loca, uint16_t color );
void board_clear_cell( vec loca );
bool board_cell_at( vec loca );
bool board_is_cover( vec loca );
uint16_t board_color_at( vec loca );

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

void tet_init();
void tet_refresh();
void tet_spawn_block();
void tet_roll();
void tet_moving( vec dir );
void tet_update( uint16_t dtime );
void tet_draw();
bool tet_is_landing();
bool tet_is_topping();
bool tet_is_quit();

void tet_update( uint16_t dtime )
{
	if( tet.is_quit ) return;
	if( ProcessMenu( dtime ) != TET_MODE_GAME ) return;
	if ( signal_ctrl_get(S_CTRL_ENT) ) {
		tet.mode = TET_MODE_MENU;
		return;
	}

	vec dir = vec_inst(0, 0);
	tet.fall_count += dtime;
	tet.move_count += dtime;
	//Falling
	if( tet.fall_count >= tet.dura_rest ) {
		tet.fall_count = 0;
		dir.y = 1;
	}
	//Event moving
	if( tet.move_count >= tet.dura_rest ) {
		uint16_t last_count = tet.move_count;
		tet.move_count = 0;
		if 		( signal_ctrl_get(S_CTRL_LT) ) 	dir.x = -1;
		else if ( signal_ctrl_get(S_CTRL_RT) )	dir.x = 1;
		else	tet.move_count = last_count % 60000;
	}
	if ( signal_ctrl_get(S_CTRL_DN) )
		tet.fall_count += dtime * 10;
	if ( signal_ctrl_get(S_CTRL_LT) || signal_ctrl_get(S_CTRL_RT) )
		tet.move_count += dtime * 2;

	if( tet.is_topping ) Eff_NewGame( dtime );
	else {
		if( tet.is_landing ) {
			ProcessGoal();
			tet_spawn_block();
		}
		else if( signal_ctrl_get(S_CTRL_UP) ) {
			if ( tet.is_avail_roll ) {
				tet.is_avail_roll = 0;
				tet_roll();
			}
		}
		else tet.is_avail_roll = 1;
		
		tet_moving( dir );
	}
}

#include "tie_define.h"
#include "tie_st7735.h"
#include "utils.h"
#include "tetris.h"
#include "tcp_ser.h"

void tcp_task_ctrl_cb(const TieRecv* sk)
{
	TieDataType data_type = sk->data_desc.data_type;

	if (data_type == DT_CMD)
	{
		TieDataCMD cmd = *(TieDataCMD*)sk->data_cont;
		signal_mtx_lock();
		switch (cmd)
		{
		case CMD_CTRL_UP: signal_ctrl_set(S_CTRL_UP); break;
		case CMD_CTRL_DN: signal_ctrl_set(S_CTRL_DN); break;
		case CMD_CTRL_RT: signal_ctrl_set(S_CTRL_RT); break;
		case CMD_CTRL_LT: signal_ctrl_set(S_CTRL_LT); break;
		case CMD_CTRL_ENT: signal_ctrl_set(S_CTRL_ENT); break;
		
		default:
		{
			printf("[USER-AT-%d] --- cmd recv %d\n", sk->sock, cmd);
			tcp_sk_send_data_cmd(sk, CMD_NONE);
		} break;
		}
		signal_mtx_unlock();
		return;
	}

	tcp_sk_send_data_cmd(sk, CMD_NONE);
}

void* thread_tcp_ser( void* agrs )
{
	tcp_man_init();
	tcp_set_task_proc_cback("task-ctrl", tcp_task_ctrl_cb);
	tcp_server_main();
	return agrs;
}

int main()
{
	//Khởi tạo màn hình
	tft_init();
	tft_clear(0xf1f1);

	//Tạo thread cho TCP server
	pthread_t t_tcp_ser;
	pthread_create(&t_tcp_ser, NULL, thread_tcp_ser, NULL);
	pthread_detach(t_tcp_ser);

	assets_load();	//Convert các file hình ảnh của trò chơi
	tet_init();		//Khởi tạo trò chơi

	uint32_t dtime = 0;
	while (1)
	{
		dtime = tick_dtime();	//Cập nhật thời gian
		tft_clear(COLOR_BLACK);
		
		//Cập nhật trạng thái trò chơi
		signal_mtx_lock();
		tet_update(dtime);
		signal_ctrl_clear();
		signal_mtx_unlock();
		
		//Hiển thị trạng thái lên TFT
		tet_draw();
		tft_display();
		
		//Tốc độ trò chơi
		usleep(1000*100);
	}

	return 0;
}