#pragma once
#include "TieSys.h"

#define board_w DISPLAY_Y
#define board_h DISPLAY_X

class Board
{
public:
    Board() = default;
    void Update();
    void Clear();
    void Draw( TieSystem& TieSys );

    void SetCell( const Vec& loca );
    void ClearCell( const Vec& loca );
    void SetCells( const Vec* list, const int& start, const int& end );
    void ClearCells( const Vec* list, const int& start, const int& end );

    bool IsCover( const Vec& loca ) const;
    bool CellAt( const Vec& loca ) const;
    // _ui score = 0;
private:
    //board 8x16
    row_8 board[board_h];
};
