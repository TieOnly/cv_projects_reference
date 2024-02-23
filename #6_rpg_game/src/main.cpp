#include "Game.h"
#include "Settings.h"

int main()
{
    assert(!GetWindowHandle());
    InitWindow(settings::screenW, settings::screenH, "Raylib Game");
    InitAudioDevice();

    Game game(settings::fps);

    while(!game.GameShouldClose())
    {
        game.Tick();
    }

    return 0;
}