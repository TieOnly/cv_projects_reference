#pragma once
#include "TieSys.h"
#include "Snek.h"
#include "Board.h"
#include "Teris.h"

class App
{
public:
    App();
    void Tick();
private:
    void Update();
    void Draw();

    TieSystem TieSys;
    Snek snek;
    Board board;
    Teris teris;
};
