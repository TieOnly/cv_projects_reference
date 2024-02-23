#pragma once
#include "TieSys_include/tie_define.h"
#include "TieSys_include/tie_features.h"
#include "TieSys_include/tie_frametime.h"

class TieSystem
{
public:
    enum class Mode: _u8
    {
        Main, Scroll, Snek, Teris, Count
    };
public:
    TieSystem();
    TieMTXBTN MTX;
    TieBuzzer BZ;
    TieLED    LED;

    void Init();
    void Update();
    const _ul& DTime();
    const Mode& GetMode();

    _uc Point( _uc x, _uc y );
    const char* GetPText() const;
    void PlayTheme();
private:
    Mode mode = Mode::Main;
    _u8 idx_mode = (_u8)mode;
    void SetMode( const _u8 idx_mode );
    TieFT FT;
    _uc bright = 0;

    //Text scroll
    const char text_0[3][32] = {
        "Hello 2023", "Made By Duy DT01", "Do an thiet ke mach dien tu d20"
    };
    _u8 idx_text = 0;
    const char* pText = text_0[idx_text];

    //Theme 
    TieBuzzer::Theme theme = TieBuzzer::Theme::BornFire;
    _u8 idx_theme = 0;
    void SetTheme();
};

