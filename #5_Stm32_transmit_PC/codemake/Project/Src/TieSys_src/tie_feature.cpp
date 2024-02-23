#include "tie_feature.h"
#include "tie_mem.h"

//////////////////////////////////////////////////////////////////////////////////////////
void TieFT::Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	TIM_TimeBaseInitTypeDef tim1_init;
	tim1_init.TIM_ClockDivision 	= TIM_CKD_DIV1;
	tim1_init.TIM_CounterMode	= TIM_CounterMode_Up;
	tim1_init.TIM_Period		= 0xffff;
	tim1_init.TIM_Prescaler		= 72 - 1;
	TIM_TimeBaseInit( TIM1, &tim1_init );
	TIM_Cmd( TIM1, ENABLE );
}
void TieFT::Tick()
{
	uint16_t count = TIM_GetCounter( TIM1 );
	dtime = 0;
	dtime_micro = count % 1000;

	if( count >= 1000 )
	{
		TIM_SetCounter(TIM1, dtime_micro);
		dtime = count / 1000;
	}
}
void TieFT::Delay( uint32_t ms )
{
	while(ms--)
	{
		TIM_SetCounter(TIM1, 0);
		while( TIM_GetCounter(TIM1) < 1000 ){}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
TieBTN::TieBTN( GPIO_TypeDef* _GPIOx, uint16_t _Pin )
{
	mPin = _Pin;
	mGPIO = _GPIOx;
	if( mGPIO == GPIOA )
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	else if( mGPIO == GPIOB )
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	else if( mGPIO == GPIOC )
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Mode  = GPIO_Mode_IPU;
	gpio_init.GPIO_Pin   = mPin;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( mGPIO, &gpio_init ); 
}
void TieBTN::Update( const uint8_t dTime )
{
    //Process Event
    if( dTime > 0x80 )                  //greater bit[7] assert -> state just depend on pin, no count
    {                    
		GPIO_ReadInputDataBit( mGPIO, mPin ) ? control = 0b10000000 : control = 0b11000000;
		//upping?							 //down = 0				//down = 1
    }
    else if( 0 == GPIO_ReadInputDataBit( mGPIO, mPin ) )        //Downing
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
    u8Write( flag, 1, u8Read(flag, 2) );           //old  = last;
    u8Write( flag, 2, IsDown() );                  //last = IsDown();
    if( u8Read( flag, 1 ) && !u8Read( flag, 2 ) )  //old:down && last:up
        u8Set( flag, 0 );                          //isPress
    else 
        u8Clear( flag, 0 );                        //isPress yet
}
bool TieBTN::IsDown()  { return u8Read( control, 6 ); }
bool TieBTN::IsPress() { return u8Read( flag, 0 ); }

//////////////////////////////////////////////////////////////////////////////////////////
TieLCD::TieLCD() : Adafruit_PCD8544( SPI2, GPIOA, GPIO_Pin_8, GPIO_Pin_9) 
{}
void TieLCD::Init()
{
    begin();
    setContrast(58);
}
void TieLCD::DrawChar( char c, int pos_x, int pos_y, _u8 color )
{
	if( c >= fisrtChar && c <= lastChar )
	{
		drawBitmap( pos_x, pos_y, font[_u8(c-fisrtChar)], glyphWidth, glyphHeight, color );
	}
}
void TieLCD::DrawNumber( long num, int pos_x, int pos_y, _u8 color )
{
	int16_t curPos_x = pos_x;
	int16_t curPos_y = pos_y;
	if( num == 0 )
	{
		drawBitmap(curPos_x, curPos_y, font[_u8('0'-fisrtChar)], glyphWidth, glyphHeight, color);
		return;
	}
	if( num < 0 )
	{
		drawBitmap(curPos_x, curPos_y, font[_u8('-'-fisrtChar)], glyphWidth, glyphHeight, color);
		curPos_x += pos_x;
		num *= -1;
	}
	

	char tmp[20];
	int length = 0;
	while( num > 0 )
	{
		tmp[length++] = (num % 10) + 48;
		num /= 10;		
	}

	for( int i = length-1; i >= 0; --i )
	{
		char c = tmp[i];
		if( c == '\n' )
		{
			curPos_x = pos_x;
			curPos_y += glyphHeight;
			continue;
		}
		else if( c >= fisrtChar && c <= lastChar )
		{
			drawBitmap(curPos_x, curPos_y, font[_u8(c-fisrtChar)], glyphWidth, glyphHeight, color);
		}
		curPos_x += glyphWidth;
	}
}
void TieLCD::DrawText( const char* text, int pos_x, int pos_y, _u8 color )
{
	int16_t curPos_x = pos_x;
	int16_t curPos_y = pos_y;
	uint16_t idx = 0;
	while( text[idx] != '\0' )
	{
		char c = text[idx++];
		if( c == '\n' )
		{
			curPos_x = pos_x;
			curPos_y += glyphHeight;
			continue;
		}
		else if( c >= fisrtChar && c <= lastChar )
		{
			drawBitmap(curPos_x, curPos_y, font[_u8(c-fisrtChar)], glyphWidth, glyphHeight, color);
		}
		curPos_x += glyphWidth;
	}
}
void TieLCD::DrawTextInBound( const char* text, int pos_x, int pos_y, _u8 color )
{
	int16_t curPos_x = pos_x;
	int16_t curPos_y = pos_y;
	uint16_t l = 0;
	uint16_t r = 0;
	while( text[r] != '\0' )
	{
		if( text[r] == ' ' || text[r] == '\n' )
		{
			if( curPos_x + (r-l)*glyphWidth > LCDWIDTH )
			{
				curPos_x = pos_x;
				curPos_y += glyphHeight;
			}
			while( l <= r ) 
			{
				if( text[l] == '\n' )
				{
					curPos_x = pos_x;
					curPos_y += glyphHeight;					
				}
				else if( text[l] >= fisrtChar && text[l] <= lastChar )
				{
					drawBitmap(curPos_x, curPos_y, font[_u8(text[l]-fisrtChar)], glyphWidth, glyphHeight, color);
					curPos_x += glyphWidth;
				}
				l++;
			}
		}
		r++;
	}
	if( l < r )
	{
		if( curPos_x + (r-l)*glyphWidth > LCDWIDTH )
		{
			curPos_x = pos_x;
			curPos_y += glyphHeight;
		}
		while( l <= r ) 
		{
			if( text[l] > fisrtChar && text[l] <= lastChar )
				drawBitmap(curPos_x, curPos_y, font[_u8(text[l]-fisrtChar)], glyphWidth, glyphHeight, color);
			l++;
			curPos_x += glyphWidth;
		}
	}
}
uint16_t TieLCD::GetHeightOf( const char* text )
{
	uint16_t curPos_x = 0;
	uint16_t curPos_y = 0;
	uint16_t l = 0;
	uint16_t r = 0;
	while( text[r] != '\0' )
	{
		if( text[r] == ' ' || text[r] == '\n' )
		{
			if( curPos_x + (r-l)*glyphWidth > LCDWIDTH )
			{
				curPos_x = 0;
				curPos_y += glyphHeight;
			}
			while( l <= r ) 
			{
				if( text[l] == '\n' )
				{
					curPos_x = 0;
					curPos_y += glyphHeight;					
				}
				else if( text[l] >= fisrtChar && text[l] <= lastChar ) curPos_x += glyphWidth;
				l++;
			}
		}
		r++;
	}
	if( l < r && ( curPos_x + (r-l)*glyphWidth > LCDWIDTH ) ) curPos_y += glyphHeight;
	return curPos_y + glyphHeight;
}
