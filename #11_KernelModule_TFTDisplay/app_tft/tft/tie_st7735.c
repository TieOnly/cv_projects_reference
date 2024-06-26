#include "tie_st7735.h"

//////////////////////////////////////////////////////////////////////////////////////
const char* device_file_path= "/dev/my-spi-driver";
// const char* device_file_path= "./txt";

#define CMD_LIGHT (0)
#define CMD_RST (1)
#define CMD_COMMAND (2)
#define CMD_DATA (3)

static void write_to_device_file(uint8_t command, uint8_t data)
{
	int f= open(device_file_path, O_RDWR);
	char charint[2];
	charint[0] = command;
	charint[1] = data;
	write(f, charint, sizeof(charint));
	close(f);
}
static uint8_t arr[1+2*320*240];
static void write_arr_to_devfile(uint8_t* data, uint32_t size)
{
	int f= open(device_file_path, O_RDWR);
	arr[0] = CMD_DATA;
	memmove(arr+1, data, size);
	write(f, arr, size+1);
	close(f);
}

void tft_delay_manual( uint16_t ms )
{
	usleep(1000*ms);
}
///////////////////////////////////////////////////////////////////////////////////////////////

void st7735_init();
void tft_write_reg(uint8_t reg);
void tft_write_data(uint8_t data);
void tft_write_data8(uint8_t data);

#define TFT_CMD_RAM_PREPARE		0X2C
#define TFT_CMD_SET_XCMD		0X2A
#define TFT_CMD_SET_YCMD		0X2B

void tft_write_reg(uint8_t reg)
{
	write_to_device_file(CMD_COMMAND, reg);
}
void tft_write_data(uint8_t data)
{
	write_to_device_file(CMD_DATA, data);
}
void tft_set_cursor(uint16_t x, uint16_t y)
{
	tft_write_reg(TFT_CMD_SET_XCMD);
	tft_write_data(x >> 8);
	tft_write_data(x & 0xff);
	tft_write_reg(TFT_CMD_SET_YCMD);
	tft_write_data(y >> 8);
	tft_write_data(y & 0xff);
}

void st7735_init()
{
	write_to_device_file(CMD_RST, 1);

	tft_write_reg(0x11); //Sleep out
	tft_delay_manual(120); //Delay 120ms
	//------------------------------------ST7735S Frame Rate-----------------------------------------//
	tft_write_reg(0xB1);
	tft_write_data(0x05);
	tft_write_data(0x3C);
	tft_write_data(0x3C);
	tft_write_reg(0xB2);
	tft_write_data(0x05);
	tft_write_data(0x3C);
	tft_write_data(0x3C);
	tft_write_reg(0xB3);
	tft_write_data(0x05);
	tft_write_data(0x3C);
	tft_write_data(0x3C);
	tft_write_data(0x05);
	tft_write_data(0x3C);
	tft_write_data(0x3C);
	//------------------------------------End ST7735S Frame Rate-----------------------------------------//
	tft_write_reg(0xB4); //Dot inversion
	tft_write_data(0x03);
	tft_write_reg(0xC0);
	tft_write_data(0x28);
	tft_write_data(0x08);
	tft_write_data(0x04);
	tft_write_reg(0xC1);
	tft_write_data(0XC0);
	tft_write_reg(0xC2);
	tft_write_data(0x0D);
	tft_write_data(0x00);
	tft_write_reg(0xC3);
	tft_write_data(0x8D);
	tft_write_data(0x2A);
	tft_write_reg(0xC4);
	tft_write_data(0x8D);
	tft_write_data(0xEE);
	//---------------------------------End ST7735S Power Sequence-------------------------------------//
	tft_write_reg(0xC5); //VCOM
	tft_write_data(0x1A);
	tft_write_reg(0x36); //MX, MY, RGB mode
	tft_write_data(0xC0);
	//------------------------------------ST7735S Gamma Sequence-----------------------------------------//
	tft_write_reg(0xE0);
	tft_write_data(0x04);
	tft_write_data(0x22);
	tft_write_data(0x07);
	tft_write_data(0x0A);
	tft_write_data(0x2E);
	tft_write_data(0x30);
	tft_write_data(0x25);
	tft_write_data(0x2A);
	tft_write_data(0x28);
	tft_write_data(0x26);
	tft_write_data(0x2E);
	tft_write_data(0x3A);
	tft_write_data(0x00);
	tft_write_data(0x01);
	tft_write_data(0x03);
	tft_write_data(0x13);
	tft_write_reg(0xE1);
	tft_write_data(0x04);
	tft_write_data(0x16);
	tft_write_data(0x06);
	tft_write_data(0x0D);
	tft_write_data(0x2D);
	tft_write_data(0x26);
	tft_write_data(0x23);
	tft_write_data(0x27);
	tft_write_data(0x27);
	tft_write_data(0x25);
	tft_write_data(0x2D);
	tft_write_data(0x3B);
	tft_write_data(0x00);
	tft_write_data(0x01);
	tft_write_data(0x04);
	tft_write_data(0x13);
	//------------------------------------End ST7735S Gamma Sequence-----------------------------------------//
	tft_write_reg(0x3A); //65k mode
	tft_write_data(0x05);
	tft_write_reg(0x29); //Display on
}

void tft_init()
{
	st7735_init();
	tft_clear(0xffff);
	tft_set_cursor(0, 0);
	tft_write_reg(TFT_CMD_RAM_PREPARE);
	write_arr_to_devfile(buff, 2*320*240);
}

static uint8_t buff[2*320*240];

void tft_display()
{
	tft_set_cursor(0, 0);
	tft_write_reg(TFT_CMD_RAM_PREPARE);
	write_arr_to_devfile(buff, 2*320*240);
}
void tft_clear(uint16_t color)
{
	uint32_t i = 320 * 240;
	while (i--)
	{
		buff[2*i]   = (uint8_t)(color>>8);
		buff[2*i+1] = (uint8_t)(color);
	}
}
void tft_draw_point_raw(uint16_t x, uint16_t y, uint16_t color)
{
	tft_set_cursor(x, y);
	tft_write_reg(TFT_CMD_RAM_PREPARE);
	tft_write_data((uint8_t)(color >> 8));
	tft_write_data((uint8_t)color);
}
void tft_draw_point(int16_t x, int16_t y, uint16_t color)
{
	if ( x < 0 || y < 0 || x >= ST7735_TFTWIDTH || y >= ST7735_TFTHEIGHT ) return;
	uint32_t idx = (2*x) + (y*2*ST7735_TFTWIDTH);
	buff[idx] = (uint8_t)(color >> 8);
	buff[idx+1] = (uint8_t)(color);
}
void tft_draw_point_scale(int16_t x, int16_t y, uint16_t color, uint8_t scale)
{
	x = x+scale; y = y+scale;
	if ( x < 0 || y < 0 || x >= ST7735_TFTWIDTH || y >= ST7735_TFTHEIGHT ) return;
	x -= scale;
	y -= scale;

	for( uint32_t i = 0; i < scale; ++i )
	{
		for( uint32_t j = 0; j < scale; ++j )
		{
			uint32_t idx = (2*(x+j)) + ((y+i)*2*ST7735_TFTWIDTH);
			buff[idx] = (uint8_t)(color >> 8);
			buff[idx+1] = (uint8_t)(color);
		}
	}
}
uint16_t tft_get_point(int16_t x, int16_t y)
{
	uint16_t idx = (2*x) + (y*2*ST7735_TFTWIDTH);
	uint16_t color;
	color = (uint16_t)(buff[idx]);
	color = color << 8;
	color = (uint16_t)(buff[idx+1]);
	return color;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
static char debug_str[30];

void render_glyph( uint16_t x, uint16_t y, uint16_t fore_color, uint16_t back_color, const tImage *glyph, uint8_t is_bg  )
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

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (bit_counter == 0)
			{
				glyph_data = *glyph_data_ptr++;
				bit_counter = glyph->dataSize;
			}
			// Decrement bit counter
			bit_counter--;

			if (glyph_data & mask)
			{
				if (is_bg) tft_draw_point( temp_x, temp_y, back_color );
			}
			else tft_draw_point( temp_x, temp_y, fore_color );

			glyph_data <<= 1;
			temp_y++;
		}

		temp_y = y;
		temp_x++;
		bit_counter = 0;
	}
}

void tft_draw_string_main( int16_t x, int16_t y, char *str, uint16_t fore_color, uint16_t back_color, const tFont *font, uint8_t is_bg )
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
			x_temp = x;
			y_temp += (font->chars[0].image->height + y_padding);
		}

		else if (*str == '\t')
		{
			x_temp += 4 * (font->chars[0].image->height + y_padding);
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
			
			if (img == NULL)
			{
				return;
			}

			width = img->width;
			height = img->height;

			if(y_temp + (height + y_padding) > ST7735_TFTHEIGHT - 1)
				return;
			if (x_temp + (width + x_padding) > ST7735_TFTWIDTH - 1)
			{
				x_temp = x;
				y_temp += (height + y_padding);
			}


			if (is_bg)
				render_glyph(x_temp, y_temp, fore_color, back_color, img, 1);
			else
				render_glyph(x_temp, y_temp, fore_color, back_color, img, 0);
			x_temp += (width + x_padding);
		}
		str++;
	}
}
void tft_draw_string( int16_t x, int16_t y, char *str, uint16_t color, const tFont *font )
{
	tft_draw_string_main(x, y, str, color, 0, font, 0);
}
void tft_draw_string_withbg( int16_t x, int16_t y, char *str, uint16_t fore_color, uint16_t back_color, const tFont *font )
{
	tft_draw_string_main(x, y, str, fore_color, back_color, font, 1);
}
void tft_draw_number( int16_t x, int16_t y, int16_t num, uint16_t color )
{
	if( num == 0 ) {
		debug_str[0] = '0'; debug_str[1] = '\0';
		tft_draw_string(x, y, debug_str, color, &font_microsoft_16); 
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
	tft_draw_string(x, y, debug_str, color, &font_microsoft_16);
}
