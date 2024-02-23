#include "../include/Teris.h"

Teris::Teris(): 
    pos_default( {(board_w-1)/2, 0} ),
    pos( pos_default ),
    size_shape(0)
{}
void Teris::Init()
{
    pos_default = {(board_w-1)/2, 0};
    pos = pos_default;
    size_shape = 0;
    SpawnBlock();
}
const _ui& Teris::GetNextShape()
{
    _u8 idx = random(0, 70) % 7;
    switch (idx)
    {
        case 0: size_next_shape = 3; return I;
        case 1: size_next_shape = 3; return L;
        case 2: size_next_shape = 3; return J;
        case 3: size_next_shape = 2; return O;
        case 4: size_next_shape = 3; return S;
        case 5: size_next_shape = 3; return T;
        case 6: size_next_shape = 3; return Z;
        default: size_next_shape = 0; return None;
    }
}
void Teris::SetTypeShape( const _ui& type )
{
    //Update New Shape
    shape = type;

    //Update New Origin Shape to Roll
    origin_shape = shape;

    //Update Size Shape
    shape == O ? size_shape = 2 :  size_shape = 3;  
}
void Teris::InitSpawn()
{
    pos = pos_default;
    isLanding = false;

    angles[0] = {0, 0};
    angles[1] = {size_shape-1, 0};
    angles[2] = {size_shape-1, size_shape-1};
    angles[3] = {0, size_shape-1};
}
void Teris::SpawnBlock()
{
    //Set Shape
    if( size_shape == 0 ) 
    {
        next_shape = GetNextShape();
        SetTypeShape( next_shape );
    }
    else SetTypeShape( next_shape );

    //Update next
    next_shape = GetNextShape();
    //Init
    InitSpawn();
}
void Teris::Roll( Board& board )
{
    // Next roll
    char xD, yD;
    if( angles[3].y == 0 ) yD = 1;
    else yD = -1;
    if( angles[3].x == 0 ) xD = 1;
    else xD = -1;

    char i = 0;
    if( angles[3].x == angles[2].x )
    {
        for( char iY = angles[3].y; iY != -1 && iY != size_shape; iY+=yD )
        {
            for( char iX = angles[3].x; iX != -1 && iX != size_shape; iX+=xD, i++ )
            {
                bit16Write( roll_shape, i, bit16Read(origin_shape, (iX + iY*size_shape)) );
            }
        }
    }
    else
    {
        for( char iX = angles[3].x; iX != -1 && iX != size_shape; iX+=xD )
        {
            for( char iY = angles[3].y; iY != -1 && iY != size_shape; iY+=yD, i++ )
            {
                bit16Write( roll_shape, i, bit16Read(origin_shape, (iX + iY*size_shape)) );
            }
        }
    }

    // Check next roll
    if( IsValidToRoll( board ) ) 
    {
        shape = roll_shape;
        const Vec tmp = angles[0];
        angles[0] = angles[3];
        angles[3] = angles[2];
        angles[2] = angles[1];
        angles[1] = tmp;
    }
}
bool Teris::IsValidToRoll( Board& board )
{
    ClearShape( board );
    char i = 0;
    for( char iY = pos.y; iY < pos.y + size_shape; iY++ )
    {
        for( char iX = pos.x; iX < pos.x + size_shape; iX++, i++ )
        {
            if( bit16Read( roll_shape, i ) )
            {
                if( !board.IsCover( {iX, iY} ) || board.CellAt( {iX, iY} ) ) return 0;
            }
        }
    } 
    return 1;
}
void Teris::ClearShape( Board& board )
{
    char i = 0;
    for( char iY = pos.y; iY < pos.y + size_shape; iY++ )
    {
        for( char iX = pos.x; iX < pos.x + size_shape; iX++, i++ )
        {
            if( bit16Read(shape, i) ) board.ClearCell( {iX, iY} );
        }
    }
}
void Teris::Moving( Board& board, Vec& dir )
{
    Vec newDir = dir;
    if( !isLanding )
    {
        ClearShape( board );
        //DirY
        if( newDir.y != 0 )     //Falling
        {
            char i = 0;
            for( char iY = pos.y + newDir.y; iY < pos.y + newDir.y + size_shape; iY++ )
            {
                for( char iX = pos.x; iX < pos.x + size_shape; iX++, i++ )
                {
                    if( bit16Read( shape, i ) )
                    {
                        if( board.CellAt( {iX, iY} ) || iY >= board_h )
                        {
                            newDir = {0, 0};
                            isLanding = true;
                            //Break Loop Check
                            iY = pos.y + newDir.y + size_shape;
                            break;
                        }
                    }
                }
            }
        }
        //DirX
        if( newDir.x != 0 )
        {
            char i = 0;
            for( char iY = pos.y + newDir.y; iY < pos.y + newDir.y + size_shape; iY++ )
            {
                for( char iX = pos.x + newDir.x; iX < pos.x + newDir.x + size_shape; iX++, i++ )
                {
                    if( bit16Read( shape, i ) )
                    {
                        if( board.CellAt( {iX, iY} ) || iX < 0 || iX >= board_w )
                        {
                            newDir.x = 0;
                            //Break Loop Check
                            iY = pos.y + newDir.y + size_shape;
                            break;
                        }
                    }
                }
            }
        }
    }

    //Update pos and isTopping
    if( !isLanding )
        pos = pos + newDir; 
    else if( pos.y == 0) 
        isTopping = true;
    //Set new shape
    char i = 0;
    for( char iY = pos.y; iY < pos.y + size_shape; iY++ )
    {
        for( char iX = pos.x; iX < pos.x + size_shape; iX++, i++ )
        {
            if( bit16Read( shape, i ) ) board.SetCell( {iX, iY} );
        }
    }
}
void Teris::ProcessGoal( TieSystem& TieSys, Board& board )
{
    for( char iY = 0; iY < board_h; iY++ )
    {
        bool isGoal = true;
        for( char iX = 0; iX < board_w; iX++ )
        {
            if( board.CellAt( {iX, iY} ) == 0 )
            {
                isGoal = false;
                break;
            }
        }
        if( isGoal )
        {
            if( iY == 0 )
            {
                for( char i = 0; i < board_w; i++ )
                {
                    board.ClearCell( {i, 0} );
                }
            }
            else
            {
                for( char iiY = iY; iiY > 0; iiY-- )
                {
                    for( char iiX = 0; iiX < board_w; iiX++ )
                    {
                        board.CellAt({iiX, char(iiY-1)})
                            ? board.SetCell(  {iiX, iiY})
                            : board.ClearCell({iiX, iiY});
                    }
                }
            }
            //Sound Eff
            TieSys.BZ.Stop();
            TieSys.BZ.Play( TieBuzzer::Theme::TakeOnMe );
        }
    }
}
void Teris::Update( TieSystem& TieSys, Board& board )
{
    //when game is over --> break update game play
    if( CheckGameOver( TieSys, board ) ) return;

    Vec dir;
    fallCount += TieSys.DTime();
    moveCount += TieSys.DTime();
    
    //Falling
    if( fallCount >= duraRest )
    {
        fallCount = 0;
        dir.y = 1;
    }
    //Event moving
    if( moveCount >= duraRest )
    {
        _ui last_count = moveCount;
        moveCount = 0;
        if( TieSys.MTX.IsDown( TieMTXBTN::Name::UP ) )
            dir.x = -1;
        else if( TieSys.MTX.IsDown( TieMTXBTN::Name::DN ) )
            dir.x = 1;
        else moveCount = last_count % 60000;
    }
    if( TieSys.MTX.IsDown( TieMTXBTN::Name::LT ) )
        fallCount += TieSys.DTime() * 6;
    if( TieSys.MTX.IsDown( TieMTXBTN::Name::DN ) || TieSys.MTX.IsDown( TieMTXBTN::Name::UP ) )
        moveCount += TieSys.DTime() * 2;

    if( isLanding )
    {
        ProcessGoal( TieSys, board );
        SpawnBlock();
    }
    else if( TieSys.MTX.IsPress( TieMTXBTN::Name::RT ) ) Roll( board );
    
    Moving( board, dir );
}

bool Teris::CheckGameOver( TieSystem& TieSys, Board& board )
{
    if( isTopping )
    {
        if( !TieSys.BZ.IsRunning( TieBuzzer::Theme::RickRoll ) )
            TieSys.BZ.Play( TieBuzzer::Theme::RickRoll );
        if( TieSys.LED.DrawEff1() )
        {
            board.Clear();
            isTopping = false;
            isLanding = false;
        }
        return 1;
    }
    return 0;
}
const Vec& Teris::GetPos() const { return pos; }
bool Teris::IsBlockLanding() const { return isLanding; }
bool Teris::IsBlockTopping() const { return isTopping; }
