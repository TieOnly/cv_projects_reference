#pragma once

#include "board.h"

void tet_init();
void tet_spawn_block();
void tet_roll();
void tet_moving( vec dir );
void tet_update();
void tet_draw();
bool tet_is_landing();
bool tet_is_topping();
