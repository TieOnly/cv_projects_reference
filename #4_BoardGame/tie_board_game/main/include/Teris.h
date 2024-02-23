#pragma once
#include "TieSys.h"
#include "Board.h"

class Teris
{
public:
    // Teris() = default;
    Teris();
    void Init();
    void SpawnBlock();
    void Roll( Board& board );
    void Moving( Board& board, Vec& dir );
    void Update( TieSystem& TieSys, Board& board );

    const Vec& GetPos() const;
    bool IsBlockLanding() const;
    bool IsBlockTopping() const;
private:
    const _ui& GetNextShape();
    void SetTypeShape( const _ui& type );
    void InitSpawn();
    void ClearShape( Board& board );
    void ProcessGoal( TieSystem& TieSys, Board& board );
    bool IsValidToRoll( Board& board );
    
    bool CheckGameOver( TieSystem& TieSys, Board& board );
    _ui score = 0;
private:
    Vec pos_default;
    Vec pos;
    Vec pos_land;

    Vec angles[4] = { {0, 0} };

    _ui shape = 0;
    _ui roll_shape = 0;
    _ui origin_shape = 0;

    _ui next_shape = 0;
    char size_next_shape = 0;

    bool isLanding = false;
    bool isTopping = true;

    _ui fallCount = 0;
    _ui moveCount = 0;
    _ui duraRest  = 500;

    char size_shape = 3; 
        //size square
    const _ui I = 0x1C00;   // 0, 0, 0,
                            // 1, 1, 1,
                            // 0, 0, 0,
    
    const _ui L = 0x9C00;   // 1, 0, 0,
                            // 1, 1, 1,
                            // 0, 0, 0
    
    const _ui J = 0x3C00;   // 0, 0, 1,
                            // 1, 1, 1,
                            // 0, 0, 0
    
    const _ui O = 0xf000;   // 1, 1,
                            // 1, 1
    
    const _ui S = 0x7800;   // 0, 1, 1,
                            // 1, 1, 0,
                            // 0, 0, 0
    
    const _ui T = 0xE800;   // 1, 1, 1,
                            // 0, 1, 0,
                            // 0, 0, 0
    const _ui Z = 0xCC00;   // 1, 1, 0,
                            // 0, 1, 1,
                            // 0, 0, 0
    const _ui None = 0x0000;
};