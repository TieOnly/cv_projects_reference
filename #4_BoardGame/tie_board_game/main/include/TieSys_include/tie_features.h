#pragma once
#include "tie_define.h"                  
#include "tie_data_progmem.h"

class TieBTN
{
public:
  TieBTN() = default;
  TieBTN( const _uc& pin );
  void Update( const _ul& dTime );
  bool IsDown() const;
  bool IsPress() const;
private:
  _uc pin;
    
  //control - [7]:assert, [6]:flag_state, [5:0]:count//
  _uc control = 0b10000000;
    
  //flag    - [2:0]- Press Sate | [2]: last, [1]: old, [0]: isPress//
  _uc flag = 0b00000000;
};

class TieMTXBTN
{
public:
  enum class Name : _uc
  {
    UP = 0, DN, LT, RT
  };
  TieMTXBTN();
  void Update( const _ul& dTime );
  bool IsDown( const Name& name_btn ) const;
  bool IsPress( const Name& name_btn ) const;
private:
  //Scan COLS
  //Array Linear express matrix ( RR-RR-RR )
  TieBTN buttons[BTN_COLS*BTN_ROWS] = {TieBTN()};

  _uc  cols[BTN_COLS] = { BTN_PORT_COL0, BTN_PORT_COL1 };
  _uc  rows[BTN_ROWS] = { BTN_PORT_ROW0, BTN_PORT_ROW1 };
};

class TieBuzzer
{
public:
  enum class Theme : _uc
  {
    BornFire = 0, TakeOnMe, Howls_0, RickRoll, Eat, Guy, FeelGood, COUNT
  };
  TieBuzzer();
  void Update( const _ul& dTime );
  void Play( const Theme& theme );
  void Play( const _ui& idx_theme ) {};
  void Stop();
  void Pause();
  bool IsRunning();
  bool IsRunning( const Theme& curTheme );
  bool IsPause();
  _u8 enable = 1;
private:
  _ui count = 0;
  _ui idx = 0;
  _ui node_lenght = 0;

  Theme curTheme = Theme::COUNT;

  const _ui* freq = nullptr;
  const _ui* dura = nullptr;
  const _ui* start = nullptr;
  void SetSound( const _ui& node_lenght, const _ui* freq, const _ui* dura, const _ui* start );
};

class TieLED
{
public:
  TieLED();
  void Bright( _uc val = 0 );
  void Update( const _ul& dtime );
  void DrawPoint( _uc x, _uc y );
  void Draw();
private:
  _ui board[DISPLAY_Y];
  void hc595_send( _uc addr, _ui data );
  _uc bright = 0;
//Effects
public:
  void Clear();
  void Fill();
  // void Draw8x8( int offsetx, _uc* arr );
  void Draw8x8( int offsetx, const _uc* arr );
  void DrawText( int offsetx, const char* arr );
  void DrawText( int offsetx, const char* arr, int n );
  bool DrawScroll( const char* arr );
  bool DrawEff1();
  bool DrawEff1( const _ui& duration );
  void ResetScroll();
  void ResetEff1();
private:
  _ui count_srcoll = 0;
  _ui i_scroll = DISPLAY_X;
  
  _ui count_eff1 = 0;
  char eff1_dir = 1;
  char eff1_x = 0;
  char eff1_y = 0;
};
