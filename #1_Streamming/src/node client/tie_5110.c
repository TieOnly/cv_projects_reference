#include "tie_5110.h"

#define DC_HIGH 	GPIO.out_w1ts |= (0x1 << DC_GPIO)
#define DC_LOW 		GPIO.out_w1tc |= (0x1 << DC_GPIO)
#define RST_HIGH 	GPIO.out_w1ts |= (0x1 << RST_GPIO)
#define RST_LOW 	GPIO.out_w1tc |= (0x1 << RST_GPIO)
#define CE_HIGH 	GPIO.out_w1ts |= (0x1 << CE_GPIO)
#define CE_LOW 		GPIO.out_w1tc |= (0x1 << CE_GPIO)

#define min(i, j) (((i) < (j)) ? (i) : (j))
#define max(i, j) (((i) > (j)) ? (i) : (j))

static uint32_t IRAM_ATTR pay_load = 0;
static spi_trans_t IRAM_ATTR trans8 = {
	.mosi = &pay_load,
	.bits.mosi = 8
};
static void IRAM_ATTR spi_send( uint8_t data )
{
	CE_LOW;
	pay_load = data;
	spi_trans(SPI_DEV, &trans8);
	CE_HIGH;
}

uint8_t pcd8544_buffer[LCDWIDTH * LCDHEIGHT / 8] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFC, 0xFE, 0xFF, 0xFC, 0xE0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8,
    0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF0, 0xF0, 0xE0, 0xE0, 0xC0, 0x80, 0xC0,
    0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xC7, 0xC7, 0x87, 0x8F, 0x9F,
    0x9F, 0xFF, 0xFF, 0xFF, 0xC1, 0xC0, 0xE0, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFC, 0xFC, 0xFC, 0xFC, 0xFE, 0xFE, 0xFE, 0xFC, 0xFC, 0xF8, 0xF8,
    0xF0, 0xE0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0,
    0xF1, 0xFB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x1F, 0x0F, 0x0F, 0x87,
    0xE7, 0xFF, 0xFF, 0xFF, 0x1F, 0x1F, 0x3F, 0xF9, 0xF8, 0xF8, 0xF8, 0xF8,
    0xF8, 0xF8, 0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x7F, 0x3F, 0x0F, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xFE, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x7E, 0x3F, 0x3F, 0x0F,
    0x1F, 0xFF, 0xFF, 0xFF, 0xFC, 0xF0, 0xE0, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFC, 0xF0, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x03, 0x0F, 0x1F, 0x3F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF,
    0x7F, 0x7F, 0x1F, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

void init_pcd8544();
// bool begin(uint8_t contrast = 100, uint8_t bias = 0x04);
bool begin(uint8_t contrast, uint8_t bias);

void command(uint8_t c);
void data(uint8_t c);

void setContrast(uint8_t val);
uint8_t getContrast(void);

uint8_t getBias(void);
void setBias(uint8_t val);

void updateBoundingBox(uint8_t xmin, uint8_t ymin, uint8_t xmax,
						uint8_t ymax);

void setReinitInterval(uint8_t val);
uint8_t getReinitInterval(void);

void setPixel(int16_t x, int16_t y, bool color, uint8_t *buffer);
bool getPixel(int16_t x, int16_t y, uint8_t *buffer);
void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);

void initDisplay();
void scroll(int8_t vpixels, int8_t hpixels);

static uint8_t xUpdateMin, xUpdateMax, yUpdateMin, yUpdateMax;
static uint8_t rotation = 0;
static uint8_t _contrast = 58;        ///< Contrast level, Vop
static uint8_t _bias = 0x06;            ///< Bias value
static uint8_t _reinit_interval; ///< Reinitialize the display after this many calls to display()
static uint8_t _display_count;   ///< Count for reinit interval


//////////////////////////////////////////////////////////////////////////////////////////
static const char* TAG = "spi_5110";
void init_pcd8544()
{
	//SPI init
	ESP_LOGI(TAG, "init hspi");
    spi_config_t spi_config;
    // Load default interface parameters
    // CS_EN:1, MISO_EN:1, MOSI_EN:1, BYTE_TX_ORDER:1, BYTE_TX_ORDER:1, BIT_RX_ORDER:0, BIT_TX_ORDER:0, CPHA:0, CPOL:0
    spi_config.interface.val = SPI_DEFAULT_INTERFACE;
    // Load default interrupt enable
    // TRANS_DONE: true, WRITE_STATUS: false, READ_STATUS: false, WRITE_BUFFER: false, READ_BUFFER: false
    // spi_config.intr_enable.val = SPI_MASTER_DEFAULT_INTR_ENABLE;
    // spi_config.intr_enable.val = 0;
    // spi_config.interface.miso_en = 0;
    // spi_config.interface.cs_en = 0;
    // spi_config.interface.cpol = 0;
    // spi_config.interface.cpha = 0;
    spi_config.mode = SPI_MASTER_MODE;
    spi_config.clk_div = SPI_20MHz_DIV;
    // spi_config.clk_div = SPI_16MHz_DIV;
    spi_config.event_cb = NULL;
    spi_init(SPI_DEV, &spi_config);

	//GPIO
	ESP_LOGI(TAG, "init gpio");
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = PCD_PIN_SEL;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);

	//PCD
	begin(58, 0x04);
	//setContrast(58);
}

void updateBoundingBox(uint8_t xmin, uint8_t ymin, uint8_t xmax, uint8_t ymax) {
	xUpdateMin = min(xUpdateMin, xmin);
	xUpdateMax = max(xUpdateMax, xmax);
	yUpdateMin = min(yUpdateMin, ymin);
	yUpdateMax = max(yUpdateMax, ymax);
}
void pcd_drawPixel(int16_t x, int16_t y, uint16_t color) 
{
	setPixel(x, y, color, pcd8544_buffer);
}

void setPixel(int16_t x, int16_t y, bool color, uint8_t *buffer) 
{
	if ((x < 0) || (x >= LCDWIDTH) || (y < 0) || (y >= LCDHEIGHT))
	return;

	int16_t t;
	switch (rotation) {
	case 1:
	t = x;
	x = y;
	y = LCDHEIGHT - 1 - t;
	break;
	case 2:
	x = LCDWIDTH - 1 - x;
	y = LCDHEIGHT - 1 - y;
	break;
	case 3:
	t = x;
	x = LCDWIDTH - 1 - y;
	y = t;
	break;
	}
	updateBoundingBox(x, y, x, y);

	// x is which column
	if (color)
	buffer[x + (y / 8) * LCDWIDTH] |= 1 << (y % 8);
	else
	buffer[x + (y / 8) * LCDWIDTH] &= ~(1 << (y % 8));
}

bool getPixel(int16_t x, int16_t y, uint8_t *buffer) 
{
	if ((x < 0) || (x >= LCDWIDTH) || (y < 0) || (y >= LCDHEIGHT))
	return false;

	int16_t t;
	switch (rotation) {
	case 1:
	t = x;
	x = y;
	y = LCDHEIGHT - 1 - t;
	break;
	case 2:
	x = LCDWIDTH - 1 - x;
	y = LCDHEIGHT - 1 - y;
	break;
	case 3:
	t = x;
	x = LCDWIDTH - 1 - y;
	y = t;
	break;
	}

	return (buffer[x + (y / 8) * LCDWIDTH] >> (y % 8)) & 0x1;
}

void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color)
{
	int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
	uint8_t b = 0;

	for (int16_t j = 0; j < h; j++, y++) {
		for (int16_t i = 0; i < w; i++) {
			if (i & 7) b <<= 1;
			else
				b = bitmap[j * byteWidth + i / 8];
			if (b & 0x80)
				pcd_drawPixel(x + i, y, color);
		}
	}
}

void initDisplay() 
{
	// toggle RST low to reset
	RST_LOW;
	RST_HIGH;

	setBias(_bias);
	setContrast(_contrast);

	// normal mode
	command(PCD8544_FUNCTIONSET);

	// Set display to Normal
	command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);
}

bool begin(uint8_t contrast, uint8_t bias) 
{
	_bias = bias;
	_contrast = contrast;
	_reinit_interval = 0;
	_display_count = 0;
	initDisplay();

	updateBoundingBox(0, 0, LCDWIDTH - 1, LCDHEIGHT - 1);
	// Push out pcd8544_buffer to the Display (will show the AFI logo)
	display();

	return true;
}

void command(uint8_t c) 
{
	// GPIO_ResetBits(gpiox, _dcpin);
	DC_LOW;
	// spi_dev.write(&c, 1);
	spi_send(c);
}

void data(uint8_t c) 
{
	// GPIO_SetBits(gpiox, _dcpin);
	DC_HIGH;
	// spi_dev.write(&c, 1);
	spi_send(c);
}

void setContrast(uint8_t val) 
{
	if (val > 0x7f) {
		val = 0x7f;
	}
	_contrast = val;
	command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION);
	command(PCD8544_SETVOP | val);
	command(PCD8544_FUNCTIONSET);
}

void setBias(uint8_t val) {
	if (val > 0x07) {
		val = 0x07;
	}
	_bias = val;
	command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION);
	command(PCD8544_SETBIAS | val);
	command(PCD8544_FUNCTIONSET);
}

uint8_t getBias() { return _bias; }
uint8_t getContrast() { return _contrast; }
uint8_t getReinitInterval() { return _reinit_interval; }

void setReinitInterval(uint8_t val) 
{
	_reinit_interval = val;
}

void display(void) 
{
	if (_reinit_interval) {
		_display_count++;
		if (_display_count >= _reinit_interval) {
			_display_count = 0;
			initDisplay();
		}
	}

	for (uint8_t page = (yUpdateMin / 8); page < (yUpdateMax / 8) + 1; page++) {
		command(PCD8544_SETYADDR | page);

		uint8_t startcol = xUpdateMin;
		uint8_t endcol = xUpdateMax;

		command(PCD8544_SETXADDR | startcol);

		// GPIO_SetBits(gpiox, _dcpin);
		DC_HIGH;
		// spi_dev.write(pcd8544_buffer + (LCDWIDTH * page) + startcol, endcol - startcol + 1);
		for (uint8_t i = 0; i < endcol - startcol + 1; i++)
		{
			spi_send((uint8_t)*(pcd8544_buffer + (LCDWIDTH * page) + startcol + i));
		}
	}

	command(PCD8544_SETYADDR); // no idea why this is necessary but it is to
								// finish the last byte?

	xUpdateMin = LCDWIDTH - 1;
	xUpdateMax = 0;
	yUpdateMin = LCDHEIGHT - 1;
	yUpdateMax = 0;
}

void clearDisplay(void) {
	memset(pcd8544_buffer, 0, LCDWIDTH * LCDHEIGHT / 8);
	updateBoundingBox(0, 0, LCDWIDTH - 1, LCDHEIGHT - 1);
}

void invertDisplay(bool i) {
	command(PCD8544_FUNCTIONSET);
	command(PCD8544_DISPLAYCONTROL | (i ? PCD8544_DISPLAYINVERTED : PCD8544_DISPLAYNORMAL));
}

void scroll(int8_t xpixels, int8_t ypixels) {
	uint8_t new_buffer[LCDWIDTH * LCDHEIGHT / 8];
	memset(new_buffer, 0, LCDWIDTH * LCDHEIGHT / 8);

	// negative pixels wrap around
	while (ypixels < 0) {
	ypixels += LCDHEIGHT;
	}
	ypixels %= LCDHEIGHT;
	while (xpixels < 0) {
	xpixels += LCDWIDTH;
	}
	xpixels %= LCDWIDTH;

	for (int x = 0; x < LCDWIDTH; x++) {
	for (int y = 0; y < LCDHEIGHT; y++) {
		if (getPixel(x, y, pcd8544_buffer)) {
		int new_x = (x + xpixels) % LCDWIDTH;
		int new_y = (y + ypixels) % LCDHEIGHT;
		setPixel(new_x, new_y, true, new_buffer);
		}
	}
	}
	memcpy(pcd8544_buffer, new_buffer, LCDWIDTH * LCDHEIGHT / 8);
	updateBoundingBox(0, 0, LCDWIDTH - 1, LCDHEIGHT - 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////
static const uint8_t glyphWidth = font_w;
static const uint8_t glyphHeight = font_h;
static const char fisrtChar = ' ';
static const char lastChar = '~';

void DrawChar( char c, int pos_x, int pos_y, uint8_t color )
{
	if( c >= fisrtChar && c <= lastChar )
	{
		drawBitmap( pos_x, pos_y, font[(uint8_t)(c-fisrtChar)], glyphWidth, glyphHeight, color );
	}
}
void DrawNumber( long num, int pos_x, int pos_y, uint8_t color )
{
	int16_t curPos_x = pos_x;
	int16_t curPos_y = pos_y;
	if( num == 0 )
	{
		drawBitmap(curPos_x, curPos_y, font[(uint8_t)('0'-fisrtChar)], glyphWidth, glyphHeight, color);
		return;
	}
	if( num < 0 )
	{
		drawBitmap(curPos_x, curPos_y, font[(uint8_t)('-'-fisrtChar)], glyphWidth, glyphHeight, color);
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
			drawBitmap(curPos_x, curPos_y, font[(uint8_t)(c-fisrtChar)], glyphWidth, glyphHeight, color);
		}
		curPos_x += glyphWidth;
	}
}
void DrawText( const char* text, int pos_x, int pos_y, uint8_t color )
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
			drawBitmap(curPos_x, curPos_y, font[(uint8_t)(c-fisrtChar)], glyphWidth, glyphHeight, color);
		}
		curPos_x += glyphWidth;
	}
}
void DrawTextInBound( const char* text, int pos_x, int pos_y, uint8_t color )
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
					drawBitmap(curPos_x, curPos_y, font[(uint8_t)(text[l]-fisrtChar)], glyphWidth, glyphHeight, color);
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
				drawBitmap(curPos_x, curPos_y, font[(uint8_t)(text[l]-fisrtChar)], glyphWidth, glyphHeight, color);
			l++;
			curPos_x += glyphWidth;
		}
	}
}
uint16_t GetHeightOf( const char* text )
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