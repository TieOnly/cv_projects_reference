#include "tie_st7735.h"

static uint16_t tft_light_max = 200;
static uint16_t tft_light_range = 20;

///////////////////////////////////////////////////////////////////////////////////////
void tft_cs_high()
{
	GPIO_SetBits( TFT_CS_PORT, TFT_CS_PIN );
} 	
void tft_cs_low()
{
	GPIO_ResetBits( TFT_CS_PORT, TFT_CS_PIN );
} 	
void tft_dc_high()
{
	GPIO_SetBits( TFT_DC_PORT, TFT_DC_PIN );
} 	
void tft_dc_low()
{
	GPIO_ResetBits( TFT_DC_PORT, TFT_DC_PIN );
} 	
void tft_rst_low()
{
	GPIO_ResetBits( TFT_RST_PORT, TFT_RST_PIN );
} 	
void tft_rst_high()
{
	GPIO_SetBits( TFT_RST_PORT, TFT_RST_PIN );
} 
void tft_delay_manual( uint16_t ms )
{
	// DCore_Delay( ms );
	ft_delay(ms);
}
static void tft_spi_send( uint8_t data )
{
	SPI_I2S_SendData( TFT_SPI, data );
	while (SPI_I2S_GetFlagStatus(TFT_SPI, SPI_I2S_FLAG_BSY) == SET) {}
}

static void st7735_driver_init()
{
	GPIO_InitTypeDef gpio_init;
	SPI_InitTypeDef  spi_init;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE );

	//CS
	gpio_init.GPIO_Mode  = GPIO_Mode_Out_PP;
	gpio_init.GPIO_Pin   = TFT_CS_PIN;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( TFT_CS_PORT, &gpio_init );
	GPIO_ResetBits(TFT_CS_PORT, TFT_CS_PIN);

	//DC
	gpio_init.GPIO_Mode  = GPIO_Mode_Out_PP;
	gpio_init.GPIO_Pin   = TFT_DC_PIN;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( TFT_DC_PORT, &gpio_init );
	GPIO_ResetBits(TFT_DC_PORT, TFT_DC_PIN);

	//RST
	gpio_init.GPIO_Mode  = GPIO_Mode_Out_PP;
	gpio_init.GPIO_Pin   = TFT_RST_PIN;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( TFT_RST_PORT, &gpio_init );
	GPIO_ResetBits(TFT_RST_PORT, TFT_RST_PIN);
	
	
	//PB13 - SCK2
	gpio_init.GPIO_Mode  = GPIO_Mode_AF_PP;
	gpio_init.GPIO_Pin   = GPIO_Pin_13;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOB, &gpio_init );
	
	//PB14 - MISO2

	//PB15 - MOSI2
	gpio_init.GPIO_Mode  = GPIO_Mode_AF_PP;
	gpio_init.GPIO_Pin   = GPIO_Pin_15;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOB, &gpio_init );
	
	//TIELCD SPI 
	spi_init.SPI_BaudRatePrescaler  = SPI_BaudRatePrescaler_2;
	spi_init.SPI_CPHA				= SPI_CPHA_2Edge;
	spi_init.SPI_CPOL				= SPI_CPOL_High;
	spi_init.SPI_DataSize			= SPI_DataSize_8b;
	spi_init.SPI_Direction			= SPI_Direction_2Lines_FullDuplex;
	spi_init.SPI_FirstBit			= SPI_FirstBit_MSB;
	spi_init.SPI_Mode				= SPI_Mode_Master;
	spi_init.SPI_NSS				= SPI_NSS_Soft;
	SPI_Init( TFT_SPI, &spi_init );
	SPI_Cmd( TFT_SPI, ENABLE );

	//PWM
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseInitTypeDef tim3_init;
	tim3_init.TIM_ClockDivision = TIM_CKD_DIV1;
	tim3_init.TIM_CounterMode	= TIM_CounterMode_Up;
	tim3_init.TIM_Period		= tft_light_max;
	tim3_init.TIM_Prescaler		= 72 - 1;
	TIM_TimeBaseInit( TIM3, &tim3_init );

	gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_init.GPIO_Pin = TFT_LIGHT_PIN;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TFT_LIGHT_PORT, &gpio_init);

	TIM_OCInitTypeDef	pwm;
	pwm.TIM_OCMode = TIM_OCMode_PWM1;
	pwm.TIM_OutputState = TIM_OutputState_Enable;
	pwm.TIM_Pulse = tft_light_range;
	pwm.TIM_OCPolarity = TIM_OCNPolarity_High;
	TIM_OC1Init(TIM3, &pwm);
	TIM_Cmd(TIM3, ENABLE);
}

///////////////////////////////////////////////////////////////////////////////////////////////

void st7735_init();
void tft_write_reg(uint16_t reg);
void tft_write_data(uint16_t data);
void tft_write_data8(uint8_t data);
void tft_write_reg_data(uint8_t reg, uint16_t data);

#define TFT_CMD_RAM_PREPARE		0X2C
#define TFT_CMD_SET_XCMD		0X2A
#define TFT_CMD_SET_YCMD		0X2B

void tft_write_reg(uint16_t reg)
{
	tft_cs_low();
	tft_dc_low();
	tft_spi_send(reg & 0x00ff);
	tft_cs_high();
}
void tft_write_data(uint16_t data)
{
	tft_cs_low();
	tft_dc_high();
	tft_spi_send(data>>8);
	tft_spi_send(data);
	tft_cs_high();
}
void tft_write_data8(uint8_t data)
{
	tft_cs_low();
	tft_dc_high();
	tft_spi_send(data);
	tft_cs_high();
}
void tft_write_reg_data(uint8_t reg, uint16_t data)
{
	tft_write_reg(reg);
	tft_write_data(data);
}
void tft_set_cursor(uint16_t x, uint16_t y)
{
	tft_write_reg(TFT_CMD_SET_XCMD);
	tft_write_data8(x >> 8);
	tft_write_data8(x & 0xff);
	tft_write_reg(TFT_CMD_SET_YCMD);
	tft_write_data8(y >> 8);
	tft_write_data8(y & 0xff);
	//
}


void st7735_init()
{
	tft_rst_low();
 	tft_delay_manual(50); 
	tft_rst_high();
 	tft_delay_manual(50);

	tft_rst_low();
	tft_delay_manual(20);  
	tft_rst_high();
	tft_delay_manual(20);


	tft_write_reg(0x11); //Sleep out
	tft_delay_manual(120); //Delay 120ms
	//------------------------------------ST7735S Frame Rate-----------------------------------------//
	tft_write_reg(0xB1);
	tft_write_data8(0x05);
	tft_write_data8(0x3C);
	tft_write_data8(0x3C);
	tft_write_reg(0xB2);
	tft_write_data8(0x05);
	tft_write_data8(0x3C);
	tft_write_data8(0x3C);
	tft_write_reg(0xB3);
	tft_write_data8(0x05);
	tft_write_data8(0x3C);
	tft_write_data8(0x3C);
	tft_write_data8(0x05);
	tft_write_data8(0x3C);
	tft_write_data8(0x3C);
	//------------------------------------End ST7735S Frame Rate-----------------------------------------//
	tft_write_reg(0xB4); //Dot inversion
	tft_write_data8(0x03);
	tft_write_reg(0xC0);
	tft_write_data8(0x28);
	tft_write_data8(0x08);
	tft_write_data8(0x04);
	tft_write_reg(0xC1);
	tft_write_data8(0XC0);
	tft_write_reg(0xC2);
	tft_write_data8(0x0D);
	tft_write_data8(0x00);
	tft_write_reg(0xC3);
	tft_write_data8(0x8D);
	tft_write_data8(0x2A);
	tft_write_reg(0xC4);
	tft_write_data8(0x8D);
	tft_write_data8(0xEE);
	//---------------------------------End ST7735S Power Sequence-------------------------------------//
	tft_write_reg(0xC5); //VCOM
	tft_write_data8(0x1A);
	tft_write_reg(0x36); //MX, MY, RGB mode
	tft_write_data8(0xC0);
	//------------------------------------ST7735S Gamma Sequence-----------------------------------------//
	tft_write_reg(0xE0);
	tft_write_data8(0x04);
	tft_write_data8(0x22);
	tft_write_data8(0x07);
	tft_write_data8(0x0A);
	tft_write_data8(0x2E);
	tft_write_data8(0x30);
	tft_write_data8(0x25);
	tft_write_data8(0x2A);
	tft_write_data8(0x28);
	tft_write_data8(0x26);
	tft_write_data8(0x2E);
	tft_write_data8(0x3A);
	tft_write_data8(0x00);
	tft_write_data8(0x01);
	tft_write_data8(0x03);
	tft_write_data8(0x13);
	tft_write_reg(0xE1);
	tft_write_data8(0x04);
	tft_write_data8(0x16);
	tft_write_data8(0x06);
	tft_write_data8(0x0D);
	tft_write_data8(0x2D);
	tft_write_data8(0x26);
	tft_write_data8(0x23);
	tft_write_data8(0x27);
	tft_write_data8(0x27);
	tft_write_data8(0x25);
	tft_write_data8(0x2D);
	tft_write_data8(0x3B);
	tft_write_data8(0x00);
	tft_write_data8(0x01);
	tft_write_data8(0x04);
	tft_write_data8(0x13);
	//------------------------------------End ST7735S Gamma Sequence-----------------------------------------//
	tft_write_reg(0x3A); //65k mode
	tft_write_data8(0x05);
	tft_write_reg(0x29); //Display on
}


void tie_tft_init()
{
	st7735_driver_init();
	st7735_init();
	tie_tft_clear(0xffff);
}
void tie_tft_clear(uint16_t color)
{
	tft_set_cursor(0, 0);
	tft_write_reg(TFT_CMD_RAM_PREPARE);
	uint32_t i = ST7735_TFTWIDTH * ST7735_TFTHEIGHT;
	while (i--) tft_write_data(color);
}
void tie_tft_draw_point(uint16_t x, uint16_t y, uint16_t color)
{
	tft_set_cursor(x, y);
	tft_write_reg(TFT_CMD_RAM_PREPARE);
	tft_write_data(color);
}
uint8_t tie_tft_light_incr()
{
	tft_light_range += 20;
	if (tft_light_range > 20 && tft_light_range < 40)
		tft_light_range = 20;
	if ( tft_light_range > tft_light_max) 
		tft_light_range = tft_light_max;

	//Change Pulse in channel 1
	TIM3->CCR1 = tft_light_range;
	return tft_light_range;
}
uint8_t tie_tft_light_decr()
{
	if ( tft_light_range <= 20) tft_light_range = 1;
	else tft_light_range -= 20;

	//Change Pulse in channel 1
	TIM3->CCR1 = tft_light_range;
	return tft_light_range;
}
uint8_t tie_tft_light_perc()
{ return (100 * tft_light_range) / tft_light_max; }

void tie_tft_light_off() { TIM3->CCR1 = 0; }
void tie_tft_light_on() { TIM3->CCR1 = tft_light_range; }
//////////////////////////////////////////////////////////////////////////////////////////////////

static uint8_t buff[GFX_HEIGHT*3][GFX_WIDTH/8] = {0};
static uint16_t buff_color[8] = { TFT_BLACK, TFT_GREEN, 17670, 0x37e9, TFT_CYAN, TFT_RED, 0xfc04, TFT_YELLOW };
static char debug_str[30];

void render_glyph( uint16_t x, uint16_t y, COLOR fore_color, COLOR back_color, const tImage *glyph, uint8_t is_bg  )
{
	uint16_t width = 0, height = 0;

	width = glyph->width;
	height = glyph->height;

	uint16_t temp_x = x;
	uint16_t temp_y = y;

	uint8_t mask = 0x80;
	uint8_t bit_counter = 0;

	const uint8_t *glyph_data_ptr = (const uint8_t *)(glyph->data);
	uint8_t glyph_data = 0;

	// font bitmaps are stored in column major order (scanned from left-to-right, not the conventional top-to-bottom)
	// as font glyphs have heigher height than width, this scanning saves some storage.
	// So, we also render in left-to-right manner.

	// Along x axis (width)
	for (int i = 0; i < width; i++)
	{
		// Along y axis (height)
		for (int j = 0; j < height; j++)
		{

			// load new data only when previous byte (or word, depends on glyph->dataSize) is completely traversed by the mask
			// bit_counter = 0 means glyph_data is completely traversed by the mask
			if (bit_counter == 0)
			{
				glyph_data = *glyph_data_ptr++;
				bit_counter = glyph->dataSize;
			}
			// Decrement bit counter
			bit_counter--;

			//If pixel is blank
			if (glyph_data & mask)
			{
				//Has background color (not transparent bg)
				if (is_bg)
				{
					gfx_drawpixel( temp_x, temp_y, back_color );
				}
				// drawPixel(temp_x, temp_y, WHITE);
			}

			//if pixel is not blank
			else
			{
				gfx_drawpixel( temp_x, temp_y, fore_color );
			}

			glyph_data <<= 1;
			temp_y++;
		}

		//New col starts. So, row is set to initial value and col is increased by one
		temp_y = y;
		temp_x++;

		//Reset the bit counter cause we're moving to next column, so we start with a new byte
		bit_counter = 0;
	}
}
void gfx_clear_full( COLOR color )
{
	tie_tft_clear(buff_color[(uint8_t)color]);
}
void gfx_clear( COLOR color )
{	
	uint8_t fCol = 1&((uint8_t)color>>2) ? 0xff : 0x00;
	uint8_t sCol = 1&((uint8_t)color>>1) ? 0xff : 0x00;
	uint8_t tCol = 1&((uint8_t)color) ? 0xff : 0x00;
	memset(buff, fCol, GFX_HEIGHT*(GFX_WIDTH/8));
	memset(buff+GFX_HEIGHT, sCol, GFX_HEIGHT*(GFX_WIDTH/8));
	memset(buff+2*GFX_HEIGHT, tCol, GFX_HEIGHT*(GFX_WIDTH/8));
}
void gfx_display()
{
	uint8_t bit_col = 0x00;
	for( uint8_t i = 0; i < GFX_HEIGHT; ++i )
	{
		tft_set_cursor(GFX_OFFSET_X, GFX_OFFSET_Y+i);
		tft_write_reg(TFT_CMD_RAM_PREPARE);
		tft_cs_low();
		tft_dc_high();
		for( uint8_t j = 0; j < GFX_WIDTH / 8; ++j )
		{
			for( int8_t k = 7; k >= 0; --k )
			{
				bitWrite( bit_col, 2, bitRead( buff[i][j], k ) );
				bitWrite( bit_col, 1, bitRead( buff[i+GFX_HEIGHT][j], k ) );
				bitWrite( bit_col, 0, bitRead( buff[i+2*GFX_HEIGHT][j], k ) );
				
				tft_spi_send( (uint8_t)(buff_color[bit_col] >> 8) );
				tft_spi_send( (uint8_t)(buff_color[bit_col]) );
				// SPI_I2S_SendData( TFT_SPI, (uint8_t)(buff_color[bit_col]>>8) );
				// SPI_I2S_SendData( TFT_SPI, (uint8_t)(buff_color[bit_col]) );
			}
		}
		tft_cs_high();
	}
}
COLOR gfx_get_pixel( int16_t x, int16_t y )
{
	if ( x < 0 || y < 0 || x >= GFX_WIDTH || y >= GFX_HEIGHT ) return COLOR_COUNT;
	uint8_t x_col = x / 8;
	uint8_t x_idx = x % 8;

	uint8_t bit_col = 0x00;
	bitWrite( bit_col, 2, 1&bitRead(buff[y][x_col], x_idx) );
	bitWrite( bit_col, 1, 1&bitRead(buff[y+1][x_col], x_idx) );
	bitWrite( bit_col, 0, 1&bitRead(buff[y+2][x_col], x_idx) );

	COLOR result = COLOR_COUNT;
	switch (bit_col)
	{
	case 0: result = COLOR_BLACK; break;
	case 1: result = COLOR_BLACK; break;
	case 2: result = COLOR_BLACK; break;
	case 3: result = COLOR_BLACK; break;
	case 4: result = COLOR_BLACK; break;
	case 5: result = COLOR_BLACK; break;
	case 6: result = COLOR_BLACK; break;
	case 7: result = COLOR_BLACK; break;
	default: break;
	}
	return result;
}
void gfx_drawpixel( int16_t x, int16_t y, COLOR color )
{
	if ( x < 0 || y < 0 || x >= GFX_WIDTH || y >= GFX_HEIGHT ) return;
	if ( color > (uint8_t)COLOR_COUNT ) return;
	uint8_t x_col = x / 8;
	uint8_t x_idx = x % 8;
	
	bitWrite( buff[y][x_col], 7-x_idx, 1&(color>>2) );
	bitWrite( buff[y+GFX_HEIGHT][x_col], 7-x_idx, 1&(color>>1) );
	bitWrite( buff[y+2*GFX_HEIGHT][x_col], 7-x_idx, 1&color );
}
void gfx_drawpixel_scale( int16_t x, int16_t y, uint8_t color, uint8_t scale )
{
	x = x*scale; y = y*scale;
	if ( x < 0 || y < 0 || x >= GFX_WIDTH || y >= GFX_HEIGHT ) return;
	if ( color > (uint8_t)COLOR_COUNT ) return;
	uint8_t x_col = x / 8;
	uint8_t x_idx = x % 8;
	
	for( uint8_t i = 0; i < scale; ++i )
	{
		for( uint8_t j = 0; j < scale; ++j )
		{
			int8_t idx = x_idx+j;	//for checking if next colum or not
			bitWrite( buff[y+i][x_col+(idx/8)], 7-(idx%8), 1&(color>>2) );
			bitWrite( buff[y+i+GFX_HEIGHT][x_col+(idx/8)], 7-(idx%8), 1&(color>>1) );
			bitWrite( buff[y+i+2*GFX_HEIGHT][x_col+(idx/8)], 7-(idx%8), 1&color );
		}
	}
}

void gfx_drawbitmap( int16_t x, int16_t y, const Tie_BMP *sBmp, COLOR bg, uint8_t scale )
{
	if ( scale != 1 ) { x /= scale; y /= scale; }

	uint16_t idx = 0;
	uint8_t bit_col = 0x00;
	for( uint16_t i = 0; i < sBmp->amount; ++i )
	{
		for( int8_t j = 7; j >= 0; --j, ++idx )
		{
			if( idx >= sBmp->w ) {
				y++; idx = 0;
			}
			bitWrite( bit_col, 1, 1&(sBmp->bmp[i]>>j) );
			bitWrite( bit_col, 0, 1&(sBmp->bmp[i+sBmp->amount]>>j) );
			if( (uint8_t)bg != bit_col ) 
			{
				if( scale == 1 )
					gfx_drawpixel( x+idx, y, bit_col );
				else
					gfx_drawpixel_scale( x+idx, y, bit_col, scale );
			}
			
		}
	}
}
void gfx_draw_by_arr( 
	int16_t x, int16_t y, uint16_t w, const uint8_t* arr, const uint16_t size,  
	COLOR bg, uint8_t scale )
{
	if ( scale != 1 ) { x /= scale; y /= scale; }

	uint8_t fData = 0x00;
	uint8_t sData = 0x00;
	uint8_t tData = 0x00;

	uint16_t idx = 0;
	uint8_t bit_col = 0x00;
	for( uint16_t i = 0; i < size; ++i )
	{
		for( int8_t j = 7; j >= 0; --j, ++idx )
		{
			if( idx >= w ) { y++; idx = 0; }

			fData = arr[i]; sData = arr[i+size]; tData = arr[i+2*size]; 

			bitWrite( bit_col, 2, 1&(fData>>j) );
			bitWrite( bit_col, 1, 1&(sData>>j) );
			bitWrite( bit_col, 0, 1&(tData>>j) );

			if( (uint8_t)bg != bit_col ) 
				scale == 1 ? gfx_drawpixel( x+idx, y, bit_col ) : gfx_drawpixel_scale( x+idx, y, bit_col, scale );
		}
	}
}

void gfx_draw_string_main( uint16_t x, uint16_t y, char *str, COLOR fore_color, COLOR back_color, const tFont *font, uint8_t is_bg )
{
	uint16_t x_temp = x;
	uint16_t y_temp = y;

	uint8_t x_padding = 0;
	uint8_t y_padding = 0;
	const tImage *img = NULL;
	uint16_t width = 0, height = 0;


	while (*str)
	{
		if (*str == '\n')
		{
			x_temp = x;					//go to first col
			y_temp += (font->chars[0].image->height + y_padding);	//go to next row (row height = height of space)
		}

		else if (*str == '\t')
		{
			x_temp += 4 * (font->chars[0].image->height + y_padding);	//Skip 4 spaces (width = width of space)
		}
		else
		{
			for (uint8_t i = 0; i < font->length; i++)
			{
				if (font->chars[i].code == *str)
				{
					img = font->chars[i].image;
					break;
				}
			}
			// No glyph (img) found, so return from this function
			if (img == NULL)
			{
				return;
			}

			width = img->width;
			height = img->height;

			if(y_temp + (height + y_padding) > GFX_HEIGHT - 1)	//not enough space available at the bottom
				return;
			if (x_temp + (width + x_padding) > GFX_WIDTH - 1)		//not enough space available at the right side
			{
				x_temp = x;					//go to first col
				y_temp += (height + y_padding);	//go to next row
			}


			if (is_bg)
				render_glyph(x_temp, y_temp, fore_color, back_color, img, 1);
			else
				render_glyph(x_temp, y_temp, fore_color, back_color, img, 0);
			x_temp += (width + x_padding);		//next char position
		}


		str++;
	}
}
void gfx_draw_string( uint16_t x, uint16_t y, char *str, COLOR color, const tFont *font )
{
	gfx_draw_string_main(x, y, str, color, COLOR_BLACK, font, 0);
}
void gfx_draw_string_withbg( uint16_t x, uint16_t y, char *str, COLOR fore_color, COLOR back_color, const tFont *font )
{
	gfx_draw_string_main(x, y, str, fore_color, back_color, font, 1);
}
void gfx_draw_number( uint16_t x, uint16_t y, int16_t num, COLOR color )
{
	if( num == 0 ) {
		debug_str[0] = '0'; debug_str[1] = '\0';
		gfx_draw_string(x, y, debug_str, color, &font_microsoft_16); 
		return;
	}

	uint8_t i = 0;
	if( num < 0 ) {
		debug_str[i++] = '-';
		num *= -1;
	}
	
	uint16_t thres = 10000;
	while( num < thres ) thres /= 10;
	
	while( thres > 0 )
	{
		debug_str[i] = (char)('0' + num/thres);
		num -= (debug_str[i]-'0') * thres;
		thres /= 10;
		i++;
	}
	
	debug_str[i] = '\0';
	gfx_draw_string(x, y, debug_str, color, &font_microsoft_16);
}
