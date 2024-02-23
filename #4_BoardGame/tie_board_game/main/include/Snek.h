#pragma once
#include "TieSys.h"

class Snek
{
public:
    Snek() = default;
    void Init( TieSystem& TieSys );
    void Update( TieSystem& TieSys );
    void Draw( TieSystem& TieSys ) const;
    void SpawnFruit();
    void Replay();
    _uc dead = 0;
private:
    _uc snek[DISPLAY_X*DISPLAY_Y] = {0};
    _uc pos_fruit;
    
    _uc lenght = 0;
    
    Vec dir;
    Vec lastdir;

    _ui dura_rest = 120;
    _ui count_rest = 0;

    _ui dura_blink = 500;
    _ui count_blink = 0;
    _u8 blink = 0;
};