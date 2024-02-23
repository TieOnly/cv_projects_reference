#include "../include/TieSys.h"

TieSystem::TieSystem()
{
    // TieUtil::Init();    
}
void TieSystem::Init()
{
    // TieUtil::Init();
}
void TieSystem::Update()
{
    FT.Tick();

    MTX.Update( FT.DTime() );
    BZ.Update( FT.DTime() );
    LED.Update( FT.DTime() );

    //=====CMD button control system=====//
    //[AllMode] Bright                --> LT + RT
    //[AllMode] Enable/Disable Buzzer --> LT + UP
    //[AllMode] Change Mode           --> UP + DN
    //[AllMode] Change Text Scroll    --> UP + RT
    //[ScrollMode] Change Theme       --> DN + RT

    if( MTX.IsDown(TieMTXBTN::Name::LT) )
    {
        if( MTX.IsPress(TieMTXBTN::Name::RT) )
            LED.Bright(++bright % 5);
        else if( MTX.IsPress(TieMTXBTN::Name::UP) )
            BZ.enable = !BZ.enable;
    }
    if( MTX.IsDown(TieMTXBTN::Name::UP) )
    {            
        if( MTX.IsPress(TieMTXBTN::Name::DN) )
        {
            ++idx_mode %= _u8(Mode::Count);
            SetMode(idx_mode);
            BZ.Stop();
            LED.ResetEff1();
        }
        else if( MTX.IsPress(TieMTXBTN::Name::RT) )
        {
            ++idx_text %= 3;
            pText = text_0[idx_text];
            LED.ResetScroll();
        }
    }

    if( mode == Mode::Scroll )
    {
        if( MTX.IsDown(TieMTXBTN::Name::DN) && MTX.IsPress(TieMTXBTN::Name::RT) )
        {
            ++idx_theme %= 4;
            BZ.Stop();
            SetTheme();
        }
    }
}
const _ul& TieSystem::DTime() { return FT.DTime(); }
const TieSystem::Mode& TieSystem::GetMode() { return mode; }
void TieSystem::SetMode( const _u8 idx_mode )
{
    switch (idx_mode)
    {
    case 0: mode = Mode::Main; break;
    case 1: mode = Mode::Scroll; break;
    case 2: mode = Mode::Snek; break;
    case 3: mode = Mode::Teris; break;
    default: mode = Mode::Count; break;
    }
}
void TieSystem::SetTheme()
{
    switch (idx_theme)
    {
    case 0: theme = TieBuzzer::Theme::BornFire; break;
    case 1: theme = TieBuzzer::Theme::Guy; break;
    case 2: theme = TieBuzzer::Theme::Howls_0; break;
    case 3: theme = TieBuzzer::Theme::FeelGood; break;
    
    default: theme = TieBuzzer::Theme::COUNT; break;
    }
}
_uc TieSystem::Point( _uc x, _uc y )
{
    return y*DISPLAY_X + x;
}
const char* TieSystem::GetPText() const { return pText; }
void TieSystem::PlayTheme() { if( !BZ.IsRunning() ) BZ.Play( theme ); }