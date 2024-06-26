#include "tetris.h"

static const uint16_t TET_I = 0x0f00;	// 0, 0, 0, 0,
										// 1, 1, 1, 1,
										// 0, 0, 0, 0,
										// 0, 0, 0, 0  
    
static const uint16_t TET_L = 0x9C00;   // 1, 0, 0,
										// 1, 1, 1,
										// 0, 0, 0

static const uint16_t TET_J = 0x3C00;   // 0, 0, 1,
										// 1, 1, 1,
										// 0, 0, 0

static const uint16_t TET_O = 0xf000;   // 1, 1,
										// 1, 1

static const uint16_t TET_S = 0x7800;   // 0, 1, 1,
										// 1, 1, 0,
										// 0, 0, 0

static const uint16_t TET_T = 0xE800;   // 1, 1, 1,
										// 0, 1, 0,
										// 0, 0, 0

static const uint16_t TET_Z = 0xCC00;   // 1, 1, 0,
										// 0, 1, 1,
										// 0, 0, 0

typedef enum {
	TET_MODE_MENU = 0, TET_MODE_GAME, TET_MODE_OPTION
} TET_MODE;

static struct tetris
{
	vec pos_default;
	vec pos;
	vec pos_land;

	vec angles[4];

	uint16_t shape;
	uint16_t roll_shape;
	uint16_t origin_shape;
	uint16_t shape_color;

	uint16_t next_shape;
	uint8_t size_next_shape : 3;
	uint8_t size_shape : 3;

	bool is_landing : 1;
	bool is_topping : 1;
	bool is_avail_roll : 1;
	bool is_goal	: 1;

	uint16_t fall_count;
	uint16_t move_count;
	uint16_t dura_rest ;

	uint16_t score;
	entity title;
	entity start_btn;
	entity option_btn;
	entity back_btn;
	entity frame_btn;
	entity arrow_sel;
	TET_MODE mode;
	int8_t sel_idx;
	bool is_quit : 1;
	bool x_control : 1;
	bool y_control : 1;
} tet;

static uint16_t GetNextShape();
static uint16_t GetNextColor();
static void SetTypeShape( uint16_t type );
static void InitSpawn();
static void ClearShape();
static void ProcessGoal();
static bool IsValidToRoll();
static void Eff_NewGame( uint16_t dtime );
static void DrawShape( uint16_t shape, uint8_t size, vec pos_OnScreen );
static void InitMenu();
static TET_MODE ProcessMenu( uint16_t dtime );

static uint16_t GetNextShape()
{
	uint8_t idx = util_random(0, 7);
	switch (idx)
	{
		case 0: tet.size_next_shape = 4; return TET_I;
		case 1: tet.size_next_shape = 3; return TET_L;
		case 2: tet.size_next_shape = 3; return TET_J;
		case 3: tet.size_next_shape = 2; return TET_O;
		case 4: tet.size_next_shape = 3; return TET_S;
		case 5: tet.size_next_shape = 3; return TET_T;
		case 6: tet.size_next_shape = 3; return TET_Z;
		default: tet.size_next_shape = 0; return 0;
	}
}
static uint16_t GetNextColor()
{
	uint8_t idx = util_random(0, 5);
	switch (idx)
	{
		case 0: return COLOR_LGREEN;
		case 1: return COLOR_ORANGE;
		case 2: return COLOR_RED;
		case 3: return COLOR_YELLOW;
		case 4: return COLOR_LIGHT;
		default: return COLOR_BLACK;
	}
}
static void SetTypeShape( uint16_t type )
{
	// Update New Shape
	tet.shape = type;
	//Update New Origin Shape to Roll
	tet.origin_shape = type;

	//Update Size Shape
	if( tet.shape == TET_I ) 
		tet.size_shape = 4;
	else if( tet.shape == TET_O ) 
		tet.size_shape = 2;
	else 
		tet.size_shape = 3;
}
static void InitSpawn()
{
	tet.is_landing = 0;
	tet.pos.y = 0;
	tet.pos.x = tet.pos_default.x - tet.size_shape/2;
	if ( tet.shape == TET_I ) tet.pos.y = -1;


	tet.angles[0] = vec_inst(0, 0);
	tet.angles[1] = vec_inst(tet.size_shape-1, 0);
	tet.angles[2] = vec_inst(tet.size_shape-1, tet.size_shape-1);
	tet.angles[3] = vec_inst(0, tet.size_shape-1);
}
static void ClearShape()
{
	int i = 0;
	for( int iY = tet.pos.y; iY < tet.pos.y + tet.size_shape; iY++ )
	{
		for( int iX = tet.pos.x; iX < tet.pos.x + tet.size_shape; iX++, i++ )
		{
			if( bit16Read(tet.shape, i) ) board_clear_cell( vec_inst(iX, iY) );
		}
	}
}
static void ProcessGoal()
{
	for( int iY = 0; iY < board_h; iY++ )
	{
		tet.is_goal = true;
		for( int iX = 0; iX < board_w; iX++ )
		{
			if( board_cell_at( vec_inst(iX, iY) ) == 0 )
			{
				tet.is_goal = false;
				break;
			}
		}
		if( tet.is_goal )
		{
			if( iY == 0 )
			{
				for( int i = 0; i < board_w; i++ )
					board_clear_cell( vec_inst(i, 0) );
			}
			else
			{
				for( int iiY = iY; iiY > 0; iiY-- )
				{
					for( int iiX = 0; iiX < board_w; iiX++ )
					{
						board_cell_at( vec_inst(iiX, iiY-1) )
							? board_set_cell( vec_inst(iiX, iiY), board_color_at( vec_inst(iiX, iiY-1) ) )
							: board_clear_cell( vec_inst(iiX, iiY) );
					}
				}
			}
			tet.score += 10;
		}
	}
}
static bool IsValidToRoll()
{
	ClearShape();
	int i = 0;
	for( int iY = tet.pos.y; iY < tet.pos.y + tet.size_shape; iY++ )
	{
		for( int iX = tet.pos.x; iX < tet.pos.x + tet.size_shape; iX++, i++ )
		{
			if( bit16Read( tet.roll_shape, i ) )
			{
				if ( !board_is_cover( vec_inst(iX, iY) ) ) return 0;
				if ( board_cell_at( vec_inst(iX, iY) ) ) return 0;
			}
		}
	} 
	return 1;
}

void tet_init()
{
	tet_refresh();
	tet.mode = TET_MODE_MENU;
	InitMenu();
}
void tet_refresh()
{
	board_clear();
	for ( int i = 0; i < 4; ++i ) tet.angles[i] = vec_inst(0,0);

	tet.shape = 0;
	tet.roll_shape = 0;
	tet.origin_shape = 0;

	tet.next_shape = 0;
	tet.size_next_shape = 0;

	tet.is_landing = 0;
	tet.is_topping = 0;
	tet.is_avail_roll = 1;
	tet.is_goal = 0;

	tet.fall_count = 0;
	tet.move_count = 0;
	tet.dura_rest  = 300;

	
	tet.pos_default = vec_inst(board_w/2, 0);
	tet.pos = tet.pos_default;
	tet.size_shape = 0;
	
	tet.score = 0;
}
void tet_spawn_block()
{
	//Set Shape
	if( tet.size_shape == 0 ) 
	{
		tet.next_shape = GetNextShape();
		SetTypeShape( tet.next_shape );
	}
	else SetTypeShape( tet.next_shape );
	tet.shape_color = GetNextColor();

	//Update next
	tet.next_shape = GetNextShape();
	//Init
	InitSpawn();
}
void tet_roll()
{
	// Next roll
	int xD = -1, yD = -1;
	if( tet.angles[3].y == 0 ) yD = 1;
	if( tet.angles[3].x == 0 ) xD = 1;

	int i = 0;
	if( tet.angles[3].x == tet.angles[2].x )
	{
		for( int iY = tet.angles[3].y; iY != -1 && iY != tet.size_shape; iY+=yD )
		{
			for( int iX = tet.angles[3].x; iX != -1 && iX != tet.size_shape; iX+=xD, i++ )
			{
				bit16Write( 
					tet.roll_shape, i, 
					bit16Read(tet.origin_shape, (iX + iY*tet.size_shape)) 
				);
			}
		}
	}
	else
	{
		for( int iX = tet.angles[3].x; iX != -1 && iX != tet.size_shape; iX+=xD )
		{
			for( int iY = tet.angles[3].y; iY != -1 && iY != tet.size_shape; iY+=yD, i++ )
			{
				bit16Write( 
					tet.roll_shape, i, 
					bit16Read(tet.origin_shape, (iX + iY*tet.size_shape)) 
				);
			}
		}
	}

	// Check next roll
	if( IsValidToRoll() ) 
	{
		tet.shape = tet.roll_shape;
		vec tmp = tet.angles[0];
		tet.angles[0] = tet.angles[3];
		tet.angles[3] = tet.angles[2];
		tet.angles[2] = tet.angles[1];
		tet.angles[1] = tmp;
	}
}
void tet_moving( vec dir )
{
	vec newDir = dir;
	if( !tet.is_landing )
	{
		ClearShape();
		//DirY
		if( newDir.y != 0 )     //Falling
		{
			int i = 0;
			for( int iY = tet.pos.y + newDir.y; iY < tet.pos.y + newDir.y + tet.size_shape; iY++ )
			{
				for( int iX = tet.pos.x; iX < tet.pos.x + tet.size_shape; iX++, i++ )
				{
					if( bit16Read( tet.shape, i ) )
					{
						if( board_cell_at( vec_inst(iX, iY) ) || iY >= board_h )
						{
							newDir = vec_inst(0, 0);
							tet.is_landing = 1;
							//Break Loop Check
							iY = tet.pos.y + newDir.y + tet.size_shape;
							break;
						}
					}
				}
			}
		}
		//DirX
		if( newDir.x != 0 )
		{
			int i = 0;
			for( int iY = tet.pos.y + newDir.y; iY < tet.pos.y + newDir.y + tet.size_shape; iY++ )
			{
				for( int iX = tet.pos.x + newDir.x; iX < tet.pos.x + newDir.x + tet.size_shape; iX++, i++ )
				{
					if( bit16Read( tet.shape, i ) )
					{
						if( board_cell_at( vec_inst(iX, iY) ) || iX < 0 || iX >= board_w )
						{
							newDir.x = 0;
							//Break Loop Check
							iY = tet.pos.y + newDir.y + tet.size_shape;
							break;
						}
					}
				}
			}
		}
	}

	//Update pos and isTopping
	if( !tet.is_landing ) 
		tet.pos = vec_inst( tet.pos.x+newDir.x, tet.pos.y+newDir.y );
	else if( tet.pos.y == 0) 
		tet.is_topping = 1;
	//Set new shape
	int i = 0;
	for( int iY = tet.pos.y; iY < tet.pos.y + tet.size_shape; iY++ )
	{
		for( int iX = tet.pos.x; iX < tet.pos.x + tet.size_shape; iX++, i++ )
		{
			if( bit16Read( tet.shape, i ) ) 
				board_set_cell( vec_inst(iX, iY), tet.shape_color );
		}
	}
}
void tet_update( uint16_t dtime )
{
	if( tet.is_quit ) return;
	if( ProcessMenu( dtime ) != TET_MODE_GAME ) return;
	if ( signal_ctrl_get(S_CTRL_ENT) ) 
	{
		tet.mode = TET_MODE_MENU;
		return;
	}

	vec dir = vec_inst(0, 0);
	tet.fall_count += dtime;
	tet.move_count += dtime;

	//Falling
	if( tet.fall_count >= tet.dura_rest )
	{
		tet.fall_count = 0;
		dir.y = 1;
	}
	//Event moving
	if( tet.move_count >= tet.dura_rest )
	{
		uint16_t last_count = tet.move_count;
		tet.move_count = 0;
		if 		( signal_ctrl_get(S_CTRL_LT) ) 	dir.x = -1;
		else if ( signal_ctrl_get(S_CTRL_RT) )	dir.x = 1;
		else	tet.move_count = last_count % 60000;
	}
	if ( signal_ctrl_get(S_CTRL_DN) )
		tet.fall_count += dtime * 10;
	if ( signal_ctrl_get(S_CTRL_LT) || signal_ctrl_get(S_CTRL_RT) )
	{
		tet.move_count += dtime * 2;
	}

	if( tet.is_topping ) Eff_NewGame( dtime );
	else
	{
		if( tet.is_landing )
		{
			ProcessGoal();
			tet_spawn_block();
		}
		else if( signal_ctrl_get(S_CTRL_UP) ) 
		{
			if ( tet.is_avail_roll ) 
			{
				tet.is_avail_roll = 0;
				tet_roll();
			}
		}
		else tet.is_avail_roll = 1;
		
		tet_moving( dir );
	}
}
bool tet_is_landing() { return tet.is_landing; }
bool tet_is_topping() { return tet.is_topping; }
bool tet_is_quit() 
{
	if (tet.is_quit)
	{
		tet.is_quit = 0;
		tet.sel_idx = 0;
		// bz_play(BZT_Init);
		return true;
	} 
	return false; 
}

static void Eff_NewGame( uint16_t dtime )
{
	static uint16_t eff_newgame_count = 0;
	static uint8_t eff_iY = board_h;
	eff_newgame_count += dtime;
	if( eff_newgame_count > 80 )
	{
		eff_newgame_count -= 80;
		eff_iY -= 1;
		for( int iX = 0; iX < board_w; iX++ )
			board_set_cell(vec_inst(iX, eff_iY), COLOR_RED);
		if( eff_iY <= 0 )
		{
			board_clear();
			tet.score = 0;
			eff_newgame_count = 0;
			eff_iY = board_h;
			tet.is_topping = 0;
			tet.is_landing = 0;
			tet.mode = TET_MODE_MENU;
		}
	}
}
static void DrawShape( uint16_t shape, uint8_t size, vec pos_on_screen )
{
	int i = 0;
	for( int iY = 0; iY < size; iY++ )
	{
		for( int iX = 0; iX < size; iX++, i++ )
		{
			if( bit16Read( shape, i ) )
			{
				gfx__rect_fill(
					vec_inst(
						pos_on_screen.x + iX*(cell_d+board_pad), 
						pos_on_screen.y + iY*(cell_d+board_pad)
					),
					vec_inst(cell_d, cell_d), COLOR_LIGHT
				);
			}
		}
	}
}
static void InitMenu()
{
	// tft_clear(COLOR_BLACK);

	ent_init(&tet.title, assets_tetris_get(IDA_TET_TITLE));
	ent_scale(&tet.title, 2);
	tet.title.topleft = vec_inst(60, 40);

	ent_init(&tet.start_btn, assets_tetris_get(IDA_TET_START_BTN));
	ent_scale(&tet.start_btn, 2);
	tet.start_btn.topleft = vec_inst(60, 20 + tet.title.topleft.y + tet.title.dimens.y);
	
	ent_init(&tet.option_btn, assets_tetris_get(IDA_TET_OPT_BTN));
	ent_scale(&tet.option_btn, 2);
	tet.option_btn.topleft = vec_inst(60, 10 + tet.start_btn.topleft.y + tet.start_btn.dimens.y);
	
	ent_init(&tet.back_btn, assets_tetris_get(IDA_TET_BACK_BTN));
	ent_scale(&tet.back_btn, 2);
	tet.back_btn.topleft = vec_inst(60, 10 + tet.option_btn.topleft.y + tet.option_btn.dimens.y);
	
	ent_init(&tet.frame_btn, assets_tetris_get(IDA_TET_FRAME_BTN));
	ent_scale(&tet.frame_btn, 2);
	tet.frame_btn.topleft = tet.start_btn.topleft;
	
	ent_init(&tet.arrow_sel, assets_tetris_get(IDA_TET_ARROW));
	tet.arrow_sel.topleft.x = 30;
	tet.arrow_sel.topleft.y = 10 + tet.frame_btn.topleft.y;
	tet.sel_idx = 0;
	tet.is_quit = 0;
}
static TET_MODE ProcessMenu( uint16_t dtime )
{
	if( tet.mode == TET_MODE_MENU )
	{
		bool is_up = signal_ctrl_get(S_CTRL_UP);
		bool is_dn = signal_ctrl_get(S_CTRL_DN);
		bool is_click = signal_ctrl_get(S_CTRL_ENT);

		switch (tet.sel_idx)
		{
		case 0:	//START
		{
			if (is_click)
			{
				tet.mode = TET_MODE_GAME;
				tet_refresh();
				tet_spawn_block();
				return TET_MODE_MENU;
			}
			else if (is_up)
			{
				tet.frame_btn.topleft.y = tet.back_btn.topleft.y;
				tet.sel_idx = 2;
			}
			else if (is_dn) 
			{
				tet.frame_btn.topleft.y = tet.option_btn.topleft.y;
				tet.sel_idx = 1;
			}
		} break;
		case 1:	//OPTION
		{
			if (is_up) 
			{
				tet.frame_btn.topleft.y = tet.start_btn.topleft.y;
				tet.sel_idx = 0;
			}
			else if (is_dn) 
			{
				tet.frame_btn.topleft.y = tet.back_btn.topleft.y;
				tet.sel_idx = 2;
			}	
		} break;
		case 2:	//BACK
		{
			if (is_up) 
			{
				tet.frame_btn.topleft.y = tet.option_btn.topleft.y;
				tet.sel_idx = 1;
			}
			else if (is_dn) 
			{
				tet.frame_btn.topleft.y = tet.start_btn.topleft.y;
				tet.sel_idx = 0;
			}
		} break;
		
		default: break;
		}
		tet.arrow_sel.topleft.y = 10 + tet.frame_btn.topleft.y;
	}
	else return TET_MODE_GAME;
	
	ent_update(&tet.arrow_sel, dtime);
	return tet.mode;
}

void tet_draw()
{
	if( tet.mode == TET_MODE_MENU )
	{
		ent_draw(&tet.title);
		ent_draw(&tet.start_btn);
		ent_draw(&tet.option_btn);
		ent_draw(&tet.back_btn);
		ent_draw(&tet.frame_btn);
		ent_draw(&tet.arrow_sel);
	}
	else if( tet.mode == TET_MODE_GAME )
	{
		board_draw();
		tft_draw_string( 102, 6, "Next", COLOR_LIGHT, &font_microsoft_16 );
		gfx__rect_thin( vec_inst(100, 30), vec_inst(40, 30), COLOR_RED );
		if( tet.next_shape == TET_I ) 
			DrawShape( tet.next_shape, tet.size_next_shape, vec_inst(103, 34) );
		else 
			DrawShape( tet.next_shape, tet.size_next_shape, vec_inst(108, 36) );
		tft_draw_string( 99, 80, "Score", COLOR_LIGHT, &font_microsoft_16 );
		tft_draw_number( 99, 100, tet.score, COLOR_YELLOW );
		tft_draw_string( 99, 140, "Exit = Click", COLOR_DGREEN, &font_microsoft_16 );
	}
}

