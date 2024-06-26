#pragma once

#include "tie_define.h"
#include "tie_ft.h"
#include "bmp_types.h"
#include "font_microsoft_16.h"


#define TFT_SPI 		SPI2
#define TFT_CS_PIN 			GPIO_Pin_12
#define TFT_CS_PORT			GPIOB
#define TFT_DC_PIN 			GPIO_Pin_11
#define TFT_DC_PORT			GPIOB
#define TFT_RST_PIN 		GPIO_Pin_10
#define TFT_RST_PORT 		GPIOB
#define TFT_LIGHT_PIN		GPIO_Pin_6
#define TFT_LIGHT_PORT		GPIOA

#define ST7735_TFTWIDTH 	240
#define ST7735_TFTHEIGHT 	320

#define TFT_WHITE         	 0xFFFF
#define TFT_BLACK         	 0x0000
#define TFT_BLUE         	 0x001F
#define TFT_BRED             0XF81F
#define TFT_GRED 			 0XFFE0
#define TFT_GBLUE			 0X07FF
#define TFT_RED           	 0xF820
#define TFT_MAGENTA       	 0xF81F
#define TFT_GREEN         	 0x07E0
#define TFT_CYAN          	 0x7FFF
#define TFT_YELLOW        	 0xFFE0
#define TFT_BROWN 			 0XBC40 
#define TFT_BRRED 			 0XFC07 
#define TFT_GRAY  			 0X8430 
#define TFT_DARKBLUE      	 0X01CF	
#define TFT_LIGHTBLUE      	 0X7D7C	
#define TFT_GRAYBLUE       	 0X5458 
#define TFT_LIGHTGREEN     	 0X841F 
#define TFT_LGRAY 			 0XC618 
#define TFT_LGRAYBLUE        0XA651 
#define TFT_LBBLUE           0X2B12 

void tie_tft_init();
void tie_tft_clear(uint16_t color);
void tie_tft_draw_point(uint16_t x, uint16_t y, uint16_t color);
uint8_t tie_tft_light_incr();
uint8_t tie_tft_light_decr();
uint8_t tie_tft_light_perc();
void tie_tft_light_off();
void tie_tft_light_on();


#define GFX_WIDTH		200
#define GFX_HEIGHT		200
#define	GFX_OFFSET_X	20
#define	GFX_OFFSET_Y	60

typedef enum {
	COLOR_BLACK = 0, COLOR_BG, COLOR_DGREEN, COLOR_LGREEN, COLOR_LIGHT, COLOR_RED, COLOR_ORANGE, COLOR_YELLOW, COLOR_COUNT
} COLOR;

void gfx_clear( COLOR color );
void gfx_clear_full( COLOR color );
void gfx_display();
// void gfx_set_addr_window(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye);

COLOR gfx_get_pixel( int16_t x, int16_t y );
void gfx_drawpixel( int16_t x, int16_t y, COLOR color );
void gfx_drawpixel_scale( int16_t x, int16_t y, uint8_t color, uint8_t scale );

void gfx_drawbitmap( int16_t x, int16_t y, const Tie_BMP *sBmp, COLOR bg, uint8_t scale );
void gfx_draw_by_arr( 
	int16_t x, int16_t y, uint16_t w, const uint8_t* arr, const uint16_t size, 
	COLOR bg, uint8_t scale
);

void gfx_draw_string_main( uint16_t x, uint16_t y, char *str, COLOR fore_color, COLOR back_color, const tFont *font, uint8_t is_bg );
void gfx_draw_string( uint16_t x, uint16_t y, char *str, COLOR color, const tFont *font );
void gfx_draw_string_withbg( uint16_t x, uint16_t y, char *str, COLOR fore_color, COLOR back_color, const tFont *font );
void gfx_draw_number( uint16_t x, uint16_t y, int16_t num, COLOR color );
