#include "../../include/TieSys_include/tie_features.h"

/////////////////////////////////////////////////////////////////////////////////////////
TieBTN::TieBTN( const _uc& _pin ) : pin(_pin)
{ 
    pinMode( pin, INPUT );
}
void TieBTN::Update( const _ul& dTime )
{
    //Process Event
    if( dTime > 0x80 )                  //greater bit[7] assert -> state just depend on pin, no count
    {
        digitalRead(pin) ? control = 0b10000000 : control = 0b11000000;
        //upping         //down = 0             //down = 1                           
    }
    else if( !digitalRead(pin) )        //Downing
    {
        if( control == 0b10000000 )     //Down real
            control = 0b11010100;       //Is dowing, Count == 20ms
        else if( control > 0b11000000 ) //Down noise, decrease count for ready to upping
        {
            control -= dTime;
            if( control < 0b11000000 ) control = 0b11000000;    //ready to upping
        } 
    }
    else
    {
        if( control == 0b11000000 )     //Up real
            control = 0b10010100;       //Is upping, Count == 20ms
        else if( control > 0b10000000 ) //Up noise, decrease count for ready to downing
        {
            control -= dTime;
            if( control < 0b10000000 ) control = 0b10000000;    //ready to downing
        }
    }

    //Process Press
    bitWrite( flag, 1, bitRead(flag, 2) );          //old  = last;
    bitWrite( flag, 2, IsDown() );                  //last = IsDown();
    if( bitRead( flag, 1 ) && !bitRead( flag, 2 ) ) //old:down && last:up
        bitSet( flag, 0 );                          //isPress
    else 
        bitClear( flag, 0 );                        //isPress yet
}
bool TieBTN::IsDown()  const { return bitRead( control, 6 ); }
bool TieBTN::IsPress() const { return bitRead( flag, 0 ); }

/////////////////////////////////////////////////////////////////////////////////////////
TieMTXBTN::TieMTXBTN()
{
    //Button: read for ROW, control by COL
//    buttons[0] = TieBTN{ BTN_PORT_ROW0 };
//    buttons[1] = TieBTN{ BTN_PORT_ROW1 };
//    buttons[2] = TieBTN{ BTN_PORT_ROW0 };
//    buttons[3] = TieBTN{ BTN_PORT_ROW1 }; 

    for( int i = 0; i < BTN_COLS; i++ )
    {
        pinMode( cols[i], OUTPUT );
        for( int j = 0; j < BTN_ROWS; j++ )
        {
            buttons[i*BTN_COLS + j] = TieBTN( rows[j] );
        }
    }
}
void TieMTXBTN::Update( const _ul& dTime )
{
    for( int i = 0; i < BTN_COLS; i++ )
    {
        digitalWrite( cols[i], 0 );
        for( int j = 0; j < BTN_ROWS; j++ )
        {
            buttons[i*BTN_COLS + j].Update( dTime );
        }
        digitalWrite( cols[i], 1 );
    }
}
bool TieMTXBTN::IsDown( const Name& name_btn ) const
{
    return buttons[(_uc)name_btn].IsDown();
}
bool TieMTXBTN::IsPress( const Name& name_btn ) const
{
    return buttons[(_uc)name_btn].IsPress();
}

/////////////////////////////////////////////////////////////////////////////////////////
TieBuzzer::TieBuzzer()
{
    pinMode( BUZZER, OUTPUT );
}
void TieBuzzer::Update( const _ul& dTime )
{
    if( curTheme == Theme::COUNT || enable == 0 ) return;

    count += dTime;
    if( count >= pgm_read_word(start+idx) )
    {
        if( idx < node_lenght  )
        {
            const _ui fre_val = pgm_read_word(freq+idx);
            if( fre_val > 260 && fre_val < 1047 )
                tone( BUZZER, fre_val, pgm_read_word(dura+idx) );
        } 

        idx += 1;
        if( idx > node_lenght ) Stop();
    }
}
void TieBuzzer::SetSound( const _ui& _node_lenght, const _ui* _freq, const _ui* _dura, const _ui* _start )
{
    count = 0;
    idx = 0;
    node_lenght = _node_lenght-1;
    freq = _freq;
    dura = _dura;
    start = _start;
}
void TieBuzzer::Play( const Theme& theme )
{
    curTheme = theme;
    switch (curTheme)
    {
    case Theme::BornFire: SetSound( size_bornfire, fre_bornfire, dura_bornfire, start_bornfire ); break;
    case Theme::TakeOnMe: SetSound( size_takeonme, fre_takeonme, dura_takeonme, start_takeonme ); break;
    case Theme::Howls_0 : SetSound( size_howls_0, fre_howls_0, dura_howls_0, start_howls_0 ); break;
    case Theme::RickRoll: SetSound( size_rickroll, fre_rickroll, dura_rickroll, start_rickroll ); break;
    case Theme::Eat     : SetSound( size_eat, fre_eat, dura_eat, start_eat ); break;
    case Theme::Guy     : SetSound( size_guy, fre_guy, dura_guy, start_guy ); break;
    case Theme::FeelGood: SetSound( size_feelgood, fre_feelgood, dura_feelgood, start_feelgood ); break;
    
    default: Stop(); break;
    }
}
void TieBuzzer::Stop()
{
    curTheme = Theme::COUNT;
    SetSound( 0, nullptr, nullptr, nullptr );
}
void TieBuzzer::Pause()
{
    curTheme = Theme::COUNT;
}
bool TieBuzzer::IsRunning()
{
    return curTheme != Theme::COUNT;
}
bool TieBuzzer::IsRunning( const Theme& _curTheme )
{
    return curTheme == _curTheme;
}
bool TieBuzzer::IsPause()
{
    return !IsRunning() && count != 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
TieLED::TieLED()
{
    pinMode( DATA, OUTPUT );
    pinMode( ST_CP, OUTPUT );
    pinMode( SH_CP, OUTPUT );
    pinMode( OE, OUTPUT );
    digitalWrite(OE, 0);
    Bright();
}
void TieLED::Bright( _uc val )
{
    if( val == 0 || val > 4 )
    {
        bright = 0; 
        digitalWrite(OE, 0);
        return;
    }
    bright = 250 - ((val-1)*20);
        //250 200 150 100
}
void TieLED::Update( const _ul& dtime )
{
    //warning: overflow
    count_srcoll += dtime;
    count_eff1   += dtime;
    if( bright ) analogWrite( OE, bright );
}
void TieLED::DrawPoint( _uc x, _uc y )
{
    if( x > 15 || x < 0 || y > 7 || y < 0 ) return;
    // hc595_send( 0x80 >> y, ~(0x8000>>x) );
    board[y] &= ~(0x8000 >> x); 
}
void TieLED::Clear()
{
    for( int i = 0; i < DISPLAY_Y; i++ )
        board[i] = 0xffff;
}
void TieLED::Fill()
{
    for( int i = 0; i < DISPLAY_Y; i++ )
        board[i] = 0x0000;
}
void TieLED::Draw()
{
    for( int i = 0; i < DISPLAY_Y; i++ )
        hc595_send( 0x80 >> i, board[i] );
    Clear();
}
void TieLED::hc595_send( _uc addr, _ui data )
{ 
    for( int i = 0; i < 16; i++ )
    {
        digitalWrite( DATA, (data>>i) & 1 );
        digitalWrite( SH_CP, 1 );
        digitalWrite( SH_CP, 0 );
    }
    
    for( int i = 0; i < 8; i++ )
    {
        digitalWrite( DATA, (addr>>i) & 1 );
        digitalWrite( SH_CP, 1 );
        digitalWrite( SH_CP, 0 );
    }
    digitalWrite( ST_CP, 1 );
    digitalWrite( ST_CP, 0 );
}
// void TieLED::Draw8x8( int offsetx, _uc* arr )
void TieLED::Draw8x8( int offsetx, const _uc* arr )
{
    if( offsetx < 0 )
    {
        offsetx *= -1;
        if( offsetx > 8 ) return;
        for( int i = 0; i < 8; i++ )
        {
            // board[i] &= ~((*(arr+i)<<8) << offsetx);
            board[i] &= ~((pgm_read_byte(arr+i)<<8) << offsetx);
        }
    }
    else
    {
        if( offsetx > 16 ) return;
        for( int i = 0; i < 8; i++ )
        {
            board[i] &= ~((pgm_read_byte(arr+i)<<8) >> offsetx) | ~(0xffff >> offsetx);
        }
    }
}
void TieLED::DrawText( int offsetx, const char* arr, int n ) 
{
    for( int i = 0; i < n; i++ )
    {
        //idx = 26 --> space
        _u8 idx = 26;
        
        //draw case number
        if( *(arr+i) >= '0' && *(arr+i) <= '9' )
        {
            idx = _u8( *(arr+i) - '0' );
            Draw8x8( offsetx + (8*i), *(prog_number+idx) );
            continue;
        }

        //draw case text
        if( *(arr+i) != ' ' ) 
        {
            if( *(arr+i) >= 'a' && *(arr+i) <= 'z' )
                idx = _u8( *(arr+i) - 'a' );
            else
                idx = _u8( *(arr+i) - 'A' );
        }
        Draw8x8( offsetx + (8*i), *(prog_font+idx) );
    }
}
void TieLED::DrawText( int offsetx, const char* arr )
{
    int m = 0;
    while( *(arr+m) != '\0' ) ++m; 
    DrawText( offsetx, arr, m );
}
bool TieLED::DrawScroll( const char* arr )
{
    bool done = false;

    int lenght = 0;
    while( *(arr+lenght) != '\0' ) ++lenght; 

    if( count_srcoll >= 40 )
    {
        count_srcoll %= 40;
        i_scroll--;
        if( i_scroll == -(8*lenght) ) 
        {
            i_scroll = DISPLAY_X;
            done = true;
        }
    }
    DrawText( i_scroll, arr, lenght );
    return done;
}
bool TieLED::DrawEff1()
{
    return DrawEff1(30);
}
bool TieLED::DrawEff1( const _ui& duration )
{
    if( eff1_x > 16 ) 
    {
        eff1_x = 0;
        return true;
    }

    if( count_eff1 >= duration )
    {
        count_eff1 %= duration;
        
        eff1_y += eff1_dir;
        if( eff1_y <= 0 || eff1_y >= 8 )
        {
            eff1_dir *= -1;
            eff1_x++;
        }
    }
    if( eff1_dir == 1 )
        for( int i = 0; i <= eff1_y; i++ ) DrawPoint(0, i);
    else
        for( int i = eff1_y; i < 8; i++ ) DrawPoint(0, i);

    for( int i = 0; i < 8; i++ )
        board[i] = board[i] >> eff1_x;
    return false;
}
void TieLED::ResetScroll()
{
    i_scroll = DISPLAY_X;
    count_srcoll = 0;
}
void TieLED::ResetEff1()
{
    count_eff1 = 0;
    eff1_dir = 1;
    eff1_x = 0;
    eff1_y = 0;
}
