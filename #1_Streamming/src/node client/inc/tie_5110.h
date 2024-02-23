#pragma once

#include "tie_define.h"

#define BLACK 1 ///< Black pixel
#define WHITE 0 ///< White pixel

#define LCDWIDTH 84  ///< LCD is 84 pixels wide
#define LCDHEIGHT 48 ///< 48 pixels high

#define PCD8544_POWERDOWN 0x04 ///< Function set, Power down mode
#define PCD8544_ENTRYMODE 0x02 ///< Function set, Entry mode
#define PCD8544_EXTENDEDINSTRUCTION                                            \
  0x01 ///< Function set, Extended instruction set control

#define PCD8544_DISPLAYBLANK 0x0    ///< Display control, blank
#define PCD8544_DISPLAYNORMAL 0x4   ///< Display control, normal mode
#define PCD8544_DISPLAYALLON 0x1    ///< Display control, all segments on
#define PCD8544_DISPLAYINVERTED 0x5 ///< Display control, inverse mode

#define PCD8544_FUNCTIONSET 0x20 ///< Basic instruction set
#define PCD8544_DISPLAYCONTROL                                                 \
  0x08 ///< Basic instruction set - Set display configuration
#define PCD8544_SETYADDR                                                       \
  0x40 ///< Basic instruction set - Set Y address of RAM, 0 <= Y <= 5
#define PCD8544_SETXADDR                                                       \
  0x80 ///< Basic instruction set - Set X address of RAM, 0 <= X <= 83

#define PCD8544_SETTEMP                                                        \
  0x04 ///< Extended instruction set - Set temperature coefficient
#define PCD8544_SETBIAS 0x10 ///< Extended instruction set - Set bias system
#define PCD8544_SETVOP                                                         \
  0x80 ///< Extended instruction set - Write Vop to register

/**************************************************************************/

#define SPI_DEV		HSPI_HOST
#define DC_GPIO		4
#define RST_GPIO	5
#define CE_GPIO		15
#define PCD_PIN_SEL  (1ULL<<DC_GPIO) | (1ULL<<RST_GPIO) | (1ULL<<CE_GPIO)

void init_pcd8544();
void clearDisplay(void);
void display();
void pcd_drawPixel(int16_t x, int16_t y, uint16_t color);
void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);

void DrawChar( char c, int pos_x, int pos_y, uint8_t color );
void DrawNumber( long num, int pos_x, int pos_y, uint8_t color );
void DrawText( const char* text, int pos_x, int pos_y, uint8_t color );
void DrawTextInBound( const char* text, int pos_x, int pos_y, uint8_t color );
void DrawTextInBound_NegativeAble( const char* text, int16_t pos_x, int16_t pos_y, uint8_t color );
uint16_t GetHeightOf( const char* text );