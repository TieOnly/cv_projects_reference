/**************************************************************************/
/*!
  @file Adafruit_PCD8544.cpp

  @mainpage Adafruit PCD8544 Nokia 5110 LCD Library

  @section intro Introduction

  This is a library for our Monochrome Nokia 5110 LCD Displays

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/338

  These displays use SPI to communicate, 4 or 5 pins are required to
  interface

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  @section author Author

  Written by Limor Fried/Ladyada  for Adafruit Industries.

  @section license License

  BSD license, check license.txt for more information
  All text above, and the splash screen below must be included in any
  redistribution

 */
/**************************************************************************/

#include "LCD_5110.h"
#include <stdlib.h>
#include <string.h>

#define min(i, j) (((i) < (j)) ? (i) : (j))
#define max(i, j) (((i) > (j)) ? (i) : (j))

#ifndef pgm_read_byte
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif
#ifndef pgm_read_word
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif
/** the memory buffer for the LCD */
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

void Adafruit_PCD8544::updateBoundingBox(uint8_t xmin, uint8_t ymin,
                                         uint8_t xmax, uint8_t ymax) {
  xUpdateMin = min(xUpdateMin, xmin);
  xUpdateMax = max(xUpdateMax, xmax);
  yUpdateMin = min(yUpdateMin, ymin);
  yUpdateMax = max(yUpdateMax, ymax);
}

Adafruit_PCD8544::Adafruit_PCD8544( SPI_TypeDef* SPIx, GPIO_TypeDef* GPIOx, uint16_t dc_pin, uint16_t rst_pin )
{
	//SPI_CPP
	spi_dev.pindef( SPI2 );

	_dcpin = dc_pin;
  	_rstpin = rst_pin;

	//Init mode pin
	gpiox = GPIOx;
	if( GPIOx == GPIOA )
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	else if( GPIOx == GPIOB )
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	else if( GPIOx == GPIOC )
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Mode  = GPIO_Mode_Out_PP;
	gpio_init.GPIO_Pin   = _rstpin | _dcpin;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( gpiox, &gpio_init );
}

void Adafruit_PCD8544::drawPixel(int16_t x, int16_t y, uint16_t color) {
  setPixel(x, y, color, pcd8544_buffer);
}

void Adafruit_PCD8544::setPixel(int16_t x, int16_t y, bool color,
                                uint8_t *buffer) {
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

bool Adafruit_PCD8544::getPixel(int16_t x, int16_t y, uint8_t *buffer) {
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

void Adafruit_PCD8544::drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color)
{
	int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
  	uint8_t b = 0;

  	for (int16_t j = 0; j < h; j++, y++) {
    	for (int16_t i = 0; i < w; i++) {
      		if (i & 7) b <<= 1;
      		else
       			b = bitmap[j * byteWidth + i / 8];
      		if (b & 0x80)
        		drawPixel(x + i, y, color);
    }
  }
}

void Adafruit_PCD8544::initDisplay() {

  // toggle RST low to reset
	GPIO_ResetBits( gpiox, _rstpin );
	GPIO_SetBits( gpiox, _rstpin );

  setBias(_bias);
  setContrast(_contrast);

  // normal mode
  command(PCD8544_FUNCTIONSET);

  // Set display to Normal
  command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);
}

bool Adafruit_PCD8544::begin(uint8_t contrast, uint8_t bias) {

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

void Adafruit_PCD8544::command(uint8_t c) {
	GPIO_ResetBits(gpiox, _dcpin);
	spi_dev.write(&c, 1);
}

void Adafruit_PCD8544::data(uint8_t c) {
	GPIO_SetBits(gpiox, _dcpin);
	spi_dev.write(&c, 1);
}

void Adafruit_PCD8544::setContrast(uint8_t val) {
  if (val > 0x7f) {
    val = 0x7f;
  }
  _contrast = val;
  command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION);
  command(PCD8544_SETVOP | val);
  command(PCD8544_FUNCTIONSET);
}

void Adafruit_PCD8544::setBias(uint8_t val) {
  if (val > 0x07) {
    val = 0x07;
  }
  _bias = val;
  command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION);
  command(PCD8544_SETBIAS | val);
  command(PCD8544_FUNCTIONSET);
}

uint8_t Adafruit_PCD8544::getBias() { return _bias; }
uint8_t Adafruit_PCD8544::getContrast() { return _contrast; }


void Adafruit_PCD8544::setReinitInterval(uint8_t val) {
  _reinit_interval = val;
}

uint8_t Adafruit_PCD8544::getReinitInterval() { return _reinit_interval; }

void Adafruit_PCD8544::display(void) {
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

	GPIO_SetBits(gpiox, _dcpin);
	spi_dev.write(pcd8544_buffer + (LCDWIDTH * page) + startcol, endcol - startcol + 1);
  }

  command(PCD8544_SETYADDR); // no idea why this is necessary but it is to
                             // finish the last byte?

  xUpdateMin = LCDWIDTH - 1;
  xUpdateMax = 0;
  yUpdateMin = LCDHEIGHT - 1;
  yUpdateMax = 0;
}

void Adafruit_PCD8544::clearDisplay(void) {
  memset(pcd8544_buffer, 0, LCDWIDTH * LCDHEIGHT / 8);
  updateBoundingBox(0, 0, LCDWIDTH - 1, LCDHEIGHT - 1);
}

void Adafruit_PCD8544::invertDisplay(bool i) {
  command(PCD8544_FUNCTIONSET);
  command(PCD8544_DISPLAYCONTROL |
          (i ? PCD8544_DISPLAYINVERTED : PCD8544_DISPLAYNORMAL));
}

void Adafruit_PCD8544::scroll(int8_t xpixels, int8_t ypixels) {
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
