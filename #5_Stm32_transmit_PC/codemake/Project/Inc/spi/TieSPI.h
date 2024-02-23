#pragma once
#include "tie_define.h"

class TieSPI
{
public:
	TieSPI();
	void pindef( SPI_TypeDef* SPIx );
	bool begin();
	void write( const uint8_t *buffer, uint16_t len );
private:
	SPI_TypeDef* spix;
	uint16_t cs, sck, mosi, miso; 
};
