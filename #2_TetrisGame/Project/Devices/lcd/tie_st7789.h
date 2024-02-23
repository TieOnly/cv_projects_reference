#pragma once
#include "tie_define.h"
#include "bmp_types.h"
#include "font_microsoft_16.h"
#include "bmp_2bit.h"

#define TIELCD_SPI 		SPI2
#define DC_PIN 			GPIO_Pin_10
#define DC_PORT			GPIOA
#define RST_PIN 		GPIO_Pin_11
#define RST_PORT 		GPIOA

#define ST7789_TFTWIDTH 	240
#define ST7789_TFTHEIGHT 	240

#ifdef  MODE_MAX_WIDTH
#define GFX_WIDTH		240
#define GFX_HEIGHT		160
#define	GFX_OFFSET_X		0
#define	GFX_OFFSET_Y		20
#else
#define GFX_WIDTH		144
#define GFX_HEIGHT		183
#define	GFX_OFFSET_X	50
#define	GFX_OFFSET_Y	30
#endif

typedef enum {
	COLOR_BLACK = 0, COLOR_BG, COLOR_DGREEN, COLOR_LGREEN, COLOR_LIGHT, COLOR_RED, COLOR_ORANGE, COLOR_YELLOW, COLOR_COUNT
} COLOR;

void tie_lcd_init();
void tie_lcd_clear( COLOR color );

void gfx_clear( COLOR color );
void gfx_display();
void gfx_set_addr_window(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye);

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