#pragma once
#include "tie_define.h"
#include "tie_mem.h"
#include "LCD_5110.h"

//Frame Time Feature////////////////////////////////////////////////////////////////////////////////////////
class TieFT
{
public:
	TieFT() = default;
	void Init();
	void Tick();
	void Delay( uint32_t ms );

	uint16_t dtime;
	uint16_t dtime_micro;
};

//Button Feature////////////////////////////////////////////////////////////////////////////////////////
class TieBTN
{
public:
	TieBTN() = default;
	TieBTN( GPIO_TypeDef* GPIOx, uint16_t Pin );
	void Update( const uint8_t dTime );
	bool IsDown();
	bool IsPress();
private:
	GPIO_TypeDef* mGPIO;
	uint16_t mPin;

	//control - [7]:assert, [6]:flag_state, [5:0]:count//
	uint8_t control = 0b10000000;
	
	//flag    - [2:0]- Press Sate | [2]: last, [1]: old, [0]: isPress//
	uint8_t flag = 0b00000000;
};
//Button Feature////////////////////////////////////////////////////////////////////////////////////////
#define HAS_DISPLAY
class TieLCD : public Adafruit_PCD8544
{
public:
	TieLCD();
	void Init();
	void DrawChar( char c, int pos_x, int pos_y, _u8 color = BLACK );
	void DrawNumber( long num, int pos_x, int pos_y, _u8 color = BLACK );
	void DrawText( const char* text, int pos_x, int pos_y, _u8 color = BLACK );
	void DrawTextInBound( const char* text, int pos_x, int pos_y, _u8 color = BLACK );
	void DrawTextInBound_NegativeAble( const char* text, int16_t pos_x, int16_t pos_y, _u8 color = BLACK );
	static uint16_t GetHeightOf( const char* text );
private:
	static constexpr uint8_t glyphWidth = font_w;
	static constexpr uint8_t glyphHeight = font_h;
	static constexpr char fisrtChar = ' ';
	static constexpr char lastChar = '~';
};
