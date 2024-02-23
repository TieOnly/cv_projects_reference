#include "../include/Snek.h"

void Snek::Init( TieSystem& TieSys )
{
    lenght = 3;
    snek[0] = TieSys.Point(2, 2);
    snek[1] = TieSys.Point(1, 2);
    snek[2] = TieSys.Point(0, 2);
    dir.x = 1;
    dir.y = 0;
    lastdir.x = dir.x;
    lastdir.y = dir.y;
    SpawnFruit();
}
void Snek::SpawnFruit()
{
    int bound = DISPLAY_X*DISPLAY_Y;
    pos_fruit = random( 0, bound );
    for( int i = 0; i < lenght; i++ )
    {
        if( snek[i] == pos_fruit )
        {
            pos_fruit = (pos_fruit+1) % bound;
        }
    }
}
void Snek::Update( TieSystem& TieSys )
{
    //Event Control
    Vec newdir;
    if( TieSys.MTX.IsPress( TieMTXBTN::Name::UP ) )
    {
        newdir.y = -1;
        // Serial.println("Up");
    }    
    if( TieSys.MTX.IsPress( TieMTXBTN::Name::DN ) )
    {
        newdir.y = 1;
        // Serial.println("DN");
    }    
    if( TieSys.MTX.IsPress( TieMTXBTN::Name::RT ) )
    {
        newdir.x = 1;
        // Serial.println("RT");
    }    
    if( TieSys.MTX.IsPress( TieMTXBTN::Name::LT ) )
    {
        newdir.x = -1;
        // Serial.println("LT");
    }      
    if( newdir.x != 0 || newdir.y != 0 )
    {
        lastdir = newdir; 
    }     
    //Update
    if( dead )
    {
        if( !TieSys.BZ.IsRunning( TieBuzzer::Theme::RickRoll ) )
            TieSys.BZ.Play( TieBuzzer::Theme::RickRoll );
        if( TieSys.LED.DrawEff1() )
        {
            for( int i = 0; i < lenght; i++ )
                snek[i] = 0;
            Init( TieSys );
            dead = 0;
        }
        return;
    }
    count_rest  += TieSys.DTime();
    count_blink += TieSys.DTime();

    if( count_rest >= dura_rest )
    {
        count_rest -= dura_rest;

        if( (abs(dir.y*lastdir.y) == 0) && (abs(dir.x*lastdir.x) == 0) )
            dir = lastdir;

        _uc tail = snek[lenght-1];
        char head_x = snek[0] % DISPLAY_X;
        char head_y = snek[0] / DISPLAY_X;
        
        for( int i = lenght-1; i > 0; i-- )
        {
            snek[i] = snek[i-1];
        }
        
        head_x += dir.x;
        head_y += dir.y;
        if( head_x == DISPLAY_X ) head_x = 0;
        else if( head_x < 0) head_x = DISPLAY_X - 1;
        if( head_y == DISPLAY_Y ) head_y = 0;
        else if( head_y < 0 ) head_y = DISPLAY_Y - 1;
        
        snek[0] = head_x + head_y*DISPLAY_X;

        if( snek[0] == pos_fruit )
        {
            lenght += 1;
            snek[lenght-1] = tail;
            SpawnFruit();
            TieSys.BZ.Stop();
            TieSys.BZ.Play( TieBuzzer::Theme::Eat );
        }
        for( int i = 1; i < lenght; i++ )
        {
            if( snek[0] == snek[i] ) 
            {
                dead = 1; return;
            }
        }
    }
    if( count_blink >= dura_blink )
    {
        count_blink -= dura_blink;
        if( blink ) count_blink += 300;
        blink = !blink;
    }
}
void Snek::Draw( TieSystem& TieSys ) const
{
    for( int i = 0; i < lenght; i++ )
    {
        TieSys.LED.DrawPoint( snek[i] % DISPLAY_X, snek[i] / DISPLAY_X );
    }
    if( blink ) 
        TieSys.LED.DrawPoint( pos_fruit % DISPLAY_X, pos_fruit / DISPLAY_X );
}
