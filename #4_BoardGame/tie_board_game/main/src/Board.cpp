#include "../include/Board.h"

void Board::Update() {}
void Board::Clear()
{
    for( uint8_t y = 0; y < board_h; y++ )
    {
        for( uint8_t x = 0; x < board_w; x++ )
        {
            board[y].set( x, 0 );
        }
    }
}
void Board::Draw( TieSystem& TieSys )
{
    for( uint8_t y = 0; y < board_h; y++ )
    {
        for( uint8_t x = 0; x < board_w; x++ )
        {
            if( board[y].at(x) )
            {
                TieSys.LED.DrawPoint( (DISPLAY_X-1) - y, x );
            }
        }
    }
}
bool Board::IsCover( const Vec& loca ) const
{
    if( loca.x < 0 || loca.y < 0 ) 
        return false;
    if( loca.x >= board_w || loca.y >= board_h ) 
        return false;
    return true;
}
bool Board::CellAt( const Vec& loca ) const
{
    if( !IsCover( loca ) ) 
        return false;
    return board[loca.y].at(loca.x);
}
void Board::SetCell( const Vec& loca )
{
    if( loca.x < 0 || loca.y < 0 ) return;
    if( loca.x >= board_w || loca.y >= board_h ) return;
    board[loca.y].set( loca.x, 1 );
}
void Board::ClearCell( const Vec& loca )
{
    if( loca.x < 0 || loca.y < 0 ) return;
    if( loca.x >= board_w || loca.y >= board_h ) return;
    board[loca.y].set( loca.x, 0 );
}
void Board::SetCells( const Vec* list, const int& start, const int& end )
{
    for( int i = start; i <= end; i++ )
    {
        SetCell( list[i] );
    }
}
void Board::ClearCells( const Vec* list, const int& start, const int& end )
{
    for( int i = start; i <= end; i++ )
    {
        ClearCell( list[i] );
    }
}
