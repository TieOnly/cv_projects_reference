#pragma once
#include "raylibCpp.h"
#include <assert.h>
#include "FrameTime.h"
#include "FontC.h"
#include "Cam2D.h"
#include "LevelPro.h"
#include "SrcItems.h"
#include "TilesMap.h"
#include "Character.h"
#include "Enemies.h"
#include "GUI.h"
#include "GUI_Table_Content.h"

class Game
{
public:
    Game(int fps);
    ~Game() noexcept;
    Game (const Game& another) = delete;
    Game& operator = (const Game& another) = delete;
    bool GameShouldClose() const;
    void Tick();
private:
    void Update();
    void Draw();

    FrameTime ft;
    std::mt19937 rng;
    
    FontC font;
    LevelPro level;
    TilesMap tileMap;

    Character tie;
    Enemies enemies;

    GUI gui;
    GUI_Table_Content gui_table_content;

    Cam2D cam;
    Vec2 targetCam;
    RectF camRect;
};
