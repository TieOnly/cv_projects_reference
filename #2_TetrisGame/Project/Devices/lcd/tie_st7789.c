#include "tie_st7789.h"

void dc_data()
{
	GPIO_SetBits( DC_PORT, DC_PIN );
} 	
void dc_cmd()
{
	GPIO_ResetBits( DC_PORT, DC_PIN );
} 	
void rst_low()
{
	GPIO_ResetBits( RST_PORT, RST_PIN );
} 	
void rst_high()
{
	GPIO_SetBits( RST_PORT, RST_PIN );
} 
void delay_manual( uint16_t ms )
{
	DCore_Delay( ms );
}
static void spi_send( uint8_t data )
{
	SPI_I2S_SendData( TIELCD_SPI, data );
	// TIELCD_SPI->DR = (uint8_t)data;
	// TIELCD_SPI->DR = data;
}

// #define DMA_RANGE_BUFF 480
// static uint8_t u8_dma_base[DMA_RANGE_BUFF];

static void st7789_driver_init()
{
	GPIO_InitTypeDef gpio_init;
	SPI_InitTypeDef  spi_init;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE );

	//DC
	gpio_init.GPIO_Mode  = GPIO_Mode_Out_PP;
	gpio_init.GPIO_Pin   = DC_PIN;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( DC_PORT, &gpio_init );

	//RST
	gpio_init.GPIO_Mode  = GPIO_Mode_Out_PP;
	gpio_init.GPIO_Pin   = RST_PIN;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( RST_PORT, &gpio_init );
	
	//PB12 - NSS2
	
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
	spi_init.SPI_CPHA				= SPI_CPHA_1Edge;
	spi_init.SPI_CPOL				= SPI_CPOL_High;
	spi_init.SPI_DataSize			= SPI_DataSize_8b;
	spi_init.SPI_Direction			= SPI_Direction_1Line_Tx;
	spi_init.SPI_FirstBit			= SPI_FirstBit_MSB;
	spi_init.SPI_Mode				= SPI_Mode_Master;
	spi_init.SPI_NSS				= SPI_NSS_Soft;
	SPI_Init( TIELCD_SPI, &spi_init );
	SPI_Cmd( TIELCD_SPI, ENABLE );

	//DMA TIELCD SPI
	// RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	// DMA_InitTypeDef dma_init;
	// dma_init.DMA_BufferSize = DMA_RANGE_BUFF;
	// dma_init.DMA_DIR = DMA_DIR_PeripheralDST;
	// dma_init.DMA_M2M = DMA_M2M_Disable;
	// dma_init.DMA_MemoryBaseAddr = (uint32_t)(u8_dma_base);
	// dma_init.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	// dma_init.DMA_MemoryInc = DMA_MemoryInc_Enable;
	// dma_init.DMA_Mode = DMA_Mode_Circular;
	// dma_init.DMA_PeripheralBaseAddr = (uint32_t)(&SPI2->DR);
	// dma_init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	// dma_init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	// dma_init.DMA_Priority = DMA_Priority_Medium;
	// DMA_Init(DMA1_Channel5, &dma_init);
	// DMA_Cmd(DMA1_Channel5, DISABLE);
}

#define ST7789_NOP     0x00
#define ST7789_SWRESET 0x01

#define ST7789_SLPIN   0x10  // sleep on
#define ST7789_SLPOUT  0x11  // sleep off
#define ST7789_PTLON   0x12  // partial on
#define ST7789_NORON   0x13  // partial off
#define ST7789_INVOFF  0x20  // invert off
#define ST7789_INVON   0x21  // invert on
#define ST7789_DISPOFF 0x28  // display off
#define ST7789_DISPON  0x29  // display on
#define ST7789_IDMOFF  0x38  // idle off
#define ST7789_IDMON   0x39  // idle on

#define ST7789_CASET   0x2A
#define ST7789_RASET   0x2B
#define ST7789_RAMWR   0x2C
#define ST7789_RAMRD   0x2E

#define ST7789_COLMOD  0x3A
#define ST7789_MADCTL  0x36

#define ST7789_PTLAR    0x30   // partial start/end
#define ST7789_VSCRDEF  0x33   // SETSCROLLAREA
#define ST7789_VSCRSADD 0x37

#define ST7789_WRDISBV  0x51
#define ST7789_WRCTRLD  0x53
#define ST7789_WRCACE   0x55
#define ST7789_WRCABCMB 0x5e

#define ST7789_POWSAVE    0xbc
#define ST7789_DLPOFFSAVE 0xbd

// bits in MADCTL
#define ST7789_MADCTL_MY  0x80
#define ST7789_MADCTL_MX  0x40
#define ST7789_MADCTL_MV  0x20
#define ST7789_MADCTL_ML  0x10
#define ST7789_MADCTL_RGB 0x00

#define ST7789_240x240_XSTART 0
#define ST7789_240x240_YSTART 0

#define ST_CMD_DELAY   0x80

static const uint8_t init_240x240[] = {
    9,                       				// 9 commands in list:
    ST7789_SWRESET,   ST_CMD_DELAY,  		// 1: Software reset, no args, w/delay
      150,                     				// 150 ms delay
    ST7789_SLPOUT ,   ST_CMD_DELAY,  		// 2: Out of sleep mode, no args, w/delay
      255,                    				// 255 = 500 ms delay
    ST7789_COLMOD , 1+ST_CMD_DELAY,  		// 3: Set color mode, 1 arg + delay:
      0x55,                   				// 16-bit color
      10,                     				// 10 ms delay
    ST7789_MADCTL , 1,  					// 4: Memory access ctrl (directions), 1 arg:
      0x00,                   				// Row addr/col addr, bottom to top refresh
    ST7789_CASET  , 4,  					// 5: Column addr set, 4 args, no delay:
      0x00, ST7789_240x240_XSTART,          // XSTART = 0
	    (ST7789_TFTWIDTH+ST7789_240x240_XSTART) >> 8,
	    (ST7789_TFTWIDTH+ST7789_240x240_XSTART) & 0xFF,   // XEND = 240
    ST7789_RASET  , 4,  					// 6: Row addr set, 4 args, no delay:
      0x00, ST7789_240x240_YSTART,          // YSTART = 0
      (ST7789_TFTHEIGHT+ST7789_240x240_YSTART) >> 8,
	    (ST7789_TFTHEIGHT+ST7789_240x240_YSTART) & 0xFF,	// YEND = 240
    ST7789_INVON ,   ST_CMD_DELAY,  		// 7: Inversion ON
      10,
    ST7789_NORON  ,   ST_CMD_DELAY,  		// 8: Normal display on, no args, w/delay
      10,                     				// 10 ms delay
    ST7789_DISPON ,   ST_CMD_DELAY,  		// 9: Main screen turn on, no args, w/delay
      20
};

static uint8_t _colstart, _rowstart, _xstart, _ystart, rotation, _width, _height;
void st_init(uint16_t width, uint16_t height);
void setRotation(uint8_t m);
void setAddrWindow(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye);
void gfx_set_addr_window(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye);
void pushColor(uint16_t color);
void drawPixel(int16_t x, int16_t y, uint16_t color);
void fillScreen(uint16_t color);
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

void invertDisplay(uint8_t mode);
void partialDisplay(uint8_t mode);
void sleepDisplay(uint8_t mode);
void enableDisplay(uint8_t mode);
void idleDisplay(uint8_t mode);
void resetDisplay();

uint16_t Color565(uint8_t r, uint8_t g, uint8_t b);
uint16_t color565(uint8_t r, uint8_t g, uint8_t b) { return Color565(r, g, b); } 
void displayInit(const uint8_t *addr);
void writeSPI(uint8_t c);
void writeCmd(uint8_t c);
void writeMulti(uint16_t color, uint16_t num);
void copyMulti(uint8_t *img, uint16_t num);
void writeData(uint8_t d8);
void writeData16(uint16_t d16);
void commonST7789Init(const uint8_t *cmdList);

void writeSPI( uint8_t c )
{
	spi_send( c );
}
void writeMulti(uint16_t color, uint16_t num)
{
	while(num--) { spi_send((uint8_t)(color>>8)); spi_send((uint8_t)(color)); }
}
void copyMulti(uint8_t *img, uint16_t num)
{
	while(num--) { spi_send(*(img+1)); spi_send(*(img+0)); img+=2; }
}
void st_init(uint16_t width, uint16_t height)
{
	commonST7789Init(NULL);

	if(width==240 && height==240) {
		_colstart = 0;
		_rowstart = 80;
	} else {
		_colstart = 0;
		_rowstart = 0;
	}
	_ystart = _xstart = 0;
	_width  = width;
	_height = height;

	displayInit(init_240x240);
	setRotation(2);
}
void writeCmd(uint8_t c)
{
	dc_cmd();
	writeSPI(c);
}
void writeData(uint8_t d8)
{
	dc_data();
	writeSPI(d8);
}
void writeData16(uint16_t d16)
{
	dc_data();
	writeMulti(d16,1);
}

void displayInit(const uint8_t *addr)
{
	uint8_t  numCommands, numArgs;
	uint16_t ms;
	numCommands = *addr++;
	while(numCommands--) {
		writeCmd( *addr++ );
		numArgs  = *addr++;
		ms       = numArgs & ST_CMD_DELAY;
		numArgs &= ~ST_CMD_DELAY;
		while(numArgs--) writeData( *addr++ );

		if(ms) {
			ms = *addr++;
			if(ms==255) ms=500;
			delay_manual( ms );
		}
	}
}
void commonST7789Init(const uint8_t *cmdList) 
{
	// on AVR ST7789 works correctly in MODE2 and MODE3 but for STM32 only MODE3 seems to be working
	#ifdef COMPATIBILITY_MODE
	// spiSettings = SPISettings(16000000, MSBFIRST, SPI_MODE3);  // 8000000 gives max speed on AVR 16MHz
	#endif

	rst_high();
	delay_manual(50);
	rst_low();
	delay_manual(50);
	rst_high();
	delay_manual(50);

	if(cmdList) displayInit(cmdList);
}
void setRotation(uint8_t m)
{
	writeCmd(ST7789_MADCTL);
	rotation = m & 3;
	switch (rotation) {
	case 0:
		writeData(ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_RGB);
		_xstart = _colstart;
		_ystart = _rowstart;
		break;
	case 1:
		writeData(ST7789_MADCTL_MY | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);
		_ystart = _colstart;
		_xstart = _rowstart;
		break;
	case 2:
		writeData(ST7789_MADCTL_RGB);
		_xstart = 0;
		_ystart = 0;
		break;
	case 3:
		writeData(ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);
		_xstart = 0;
		_ystart = 0;
		break;
	}
}
void setAddrWindow(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye)
{
	xs+=_xstart; xe+=_xstart;
	ys+=_ystart; ye+=_ystart;

	dc_cmd();
	writeSPI(ST7789_CASET);

	dc_data();
	writeSPI(xs >> 8); writeSPI(xs);
	writeSPI(xe >> 8); writeSPI(xe);

	dc_cmd();
	writeSPI(ST7789_RASET);

	dc_data();
	writeSPI(ys >> 8); writeSPI(ys);
	writeSPI(ye >> 8); writeSPI(ye);

	dc_cmd();
	writeSPI(ST7789_RAMWR);

	dc_data();
	// no CS_IDLE + SPI_END, DC_DATA to save memory
}
void gfx_set_addr_window(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye)
{
	setAddrWindow(xs, ys, xe, ye);
}

void pushColor(uint16_t color) 
{
	writeSPI(color>>8); writeSPI(color);
}
void drawPixel(int16_t x, int16_t y, uint16_t color) 
{
	if(x<0 ||x>=_width || y<0 || y>=_height) return;
	setAddrWindow(x,y,x+1,y+1);

	writeSPI(color>>8); writeSPI(color);
}
void fillScreen(uint16_t color) 
{
	fillRect(0, 0, _width, _height, color);
}
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) 
{
	if(x>=_width || y>=_height || w<=0 || h<=0) return;
	if(x+w-1>=_width)  w=_width -x;
	if(y+h-1>=_height) h=_height-y;
	setAddrWindow(x, y, x+w-1, y+h-1);

	writeMulti(color,w*h);
}

uint16_t Color565(uint8_t r, uint8_t g, uint8_t b) 
{
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
void invertDisplay(uint8_t mode) 
{
  writeCmd(!mode ? ST7789_INVON : ST7789_INVOFF);  // modes inverted?
}
void partialDisplay(uint8_t mode) 
{
  writeCmd(mode ? ST7789_PTLON : ST7789_NORON);
}
void sleepDisplay(uint8_t mode) 
{
  writeCmd(mode ? ST7789_SLPIN : ST7789_SLPOUT);
  delay_manual(5);
}
void enableDisplay(uint8_t mode) 
{
  writeCmd(mode ? ST7789_DISPON : ST7789_DISPOFF);
}
void idleDisplay(uint8_t mode) 
{
  writeCmd(mode ? ST7789_IDMON : ST7789_IDMOFF);
}
void resetDisplay() 
{
  writeCmd(ST7789_SWRESET);
  delay_manual(5);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

static uint8_t buff[GFX_HEIGHT*3][GFX_WIDTH/8] = {0};
static char debug_str[30];
static uint16_t buff_color[8] = { 2177, 3840, 19816, 24393, 51199, 64201, 64648, 65451 };
void render_glyph( uint16_t x, uint16_t y, COLOR fore_color, COLOR back_color, const tImage *glyph, uint8_t is_bg );

void tie_lcd_init()
{
	st7789_driver_init();
	st_init( 240, 240 );
	// delay_manual(100);
	tie_lcd_clear(COLOR_BLACK);
	// setAddrWindow(0, 0, _width-1, _height-1);
}

void tie_lcd_clear( COLOR color )
{
	setAddrWindow(0, 0, ST7789_TFTWIDTH-1, ST7789_TFTHEIGHT-1);
	for( uint8_t y = 0; y < ST7789_TFTHEIGHT; y++ )
	{
		for( uint8_t x = 0; x < ST7789_TFTWIDTH; x++ )
		{
			spi_send( (uint8_t)(buff_color[(uint8_t)color]>>8) );
			spi_send( (uint8_t)(buff_color[(uint8_t)color]) );
			// spi_send_16b( buff_color[(uint8_t)color] );
		}
	}
}
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
					// drawPixel(temp_x, temp_y, BLACK);
					// drawPixel(temp_x, temp_y, back_color);
					gfx_drawpixel( temp_x, temp_y, back_color );
				}
				// drawPixel(temp_x, temp_y, WHITE);
			}

			//if pixel is not blank
			else
			{
				// drawPixel(temp_x, temp_y, BLACK);
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
  #ifndef MODE_MAX_WIDTH
	setAddrWindow(GFX_OFFSET_X, GFX_OFFSET_Y, GFX_OFFSET_X+GFX_WIDTH-1, GFX_OFFSET_Y+GFX_HEIGHT-1);
  #endif

	uint8_t bit_col = 0x00;
	for( uint8_t i = 0; i < GFX_HEIGHT; ++i )
	{
		for( uint8_t j = 0; j < GFX_WIDTH / 8; ++j )
		{
			for( int8_t k = 7; k >= 0; --k )
			{
				bitWrite( bit_col, 2, bitRead( buff[i][j], k ) );
				bitWrite( bit_col, 1, bitRead( buff[i+GFX_HEIGHT][j], k ) );
				bitWrite( bit_col, 0, bitRead( buff[i+2*GFX_HEIGHT][j], k ) );
				
				spi_send( (uint8_t)(buff_color[bit_col]>>8) );
				spi_send( (uint8_t)(buff_color[bit_col]) );
				// spi_send_16b( buff_color[bit_col] );
			}
		}
	}
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

			if(y_temp + (height + y_padding) > ST7789_TFTHEIGHT - 1)	//not enough space available at the bottom
				return;
			if (x_temp + (width + x_padding) > ST7789_TFTWIDTH - 1)		//not enough space available at the right side
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
