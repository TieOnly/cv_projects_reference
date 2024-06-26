#pragma once
#include "board.h"

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
