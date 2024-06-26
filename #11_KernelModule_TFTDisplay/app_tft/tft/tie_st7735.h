#pragma once

#include "tie_define.h"
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

//Hàm gửi các tập lệnh cấu hình 
void tft_init();

//Hàm gửi mảng dữ liệu trong bảng vẽ (buffer)
void tft_display();
//Hàm reset dữ liệu trong bảng vẽ
void tft_clear(uint16_t color);

//Các hàm thao tác với bảng vẽ
uint16_t tft_get_point(int16_t x, int16_t y);
void tft_draw_point(int16_t x, int16_t y, uint16_t color);
void tft_draw_point_scale(int16_t x, int16_t y, uint16_t color, uint8_t scale);

void tft_draw_string_main( int16_t x, int16_t y, char *str, uint16_t fore_color, uint16_t back_color, const tFont *font, uint8_t is_bg );
void tft_draw_string( int16_t x, int16_t y, char *str, uint16_t color, const tFont *font );
void tft_draw_string_withbg( int16_t x, int16_t y, char *str, uint16_t fore_color, uint16_t back_color, const tFont *font );
void tft_draw_number( int16_t x, int16_t y, int16_t num, uint16_t color );

