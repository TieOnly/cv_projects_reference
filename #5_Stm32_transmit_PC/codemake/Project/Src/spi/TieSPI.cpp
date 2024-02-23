#include "TieSPI.h"

#define NSS2_1 GPIO_SetBits( GPIOB, GPIO_Pin_12 );
#define NSS2_0 GPIO_ResetBits( GPIOB, GPIO_Pin_12 );

TieSPI::TieSPI() {}
void TieSPI::pindef( SPI_TypeDef* SPIx )
{
	if( SPIx == SPI2 )
	{
		//init
		spix = SPI2;
		cs   = GPIO_Pin_12;
		sck  = GPIO_Pin_13; 
		miso = GPIO_Pin_14;
		mosi = GPIO_Pin_15;

		GPIO_InitTypeDef gpio_init;
		SPI_InitTypeDef  spi_init;
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE );

		//PB12 - NSS2
		gpio_init.GPIO_Mode  = GPIO_Mode_Out_PP;
		gpio_init.GPIO_Pin   = GPIO_Pin_12;
		gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init( GPIOB, &gpio_init );
		
		//PB13 - SCK2
		gpio_init.GPIO_Mode  = GPIO_Mode_AF_PP;
		gpio_init.GPIO_Pin   = GPIO_Pin_13;
		gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init( GPIOB, &gpio_init );
		
		//PB14 - MISO2
		gpio_init.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
		gpio_init.GPIO_Pin   = GPIO_Pin_14;
		gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init( GPIOB, &gpio_init );

		//PB15 - MOSI2
		gpio_init.GPIO_Mode  = GPIO_Mode_AF_PP;
		gpio_init.GPIO_Pin   = GPIO_Pin_15;
		gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init( GPIOB, &gpio_init );
		
		//SPI2
		spi_init.SPI_BaudRatePrescaler  = SPI_BaudRatePrescaler_2;
		spi_init.SPI_CPHA				= SPI_CPHA_1Edge;
		spi_init.SPI_CPOL				= SPI_CPOL_Low;
		spi_init.SPI_DataSize			= SPI_DataSize_8b;
		spi_init.SPI_Direction			= SPI_Direction_2Lines_FullDuplex;
		spi_init.SPI_FirstBit			= SPI_FirstBit_MSB;
		spi_init.SPI_Mode				= SPI_Mode_Master;
		spi_init.SPI_NSS				= SPI_NSS_Soft;
		SPI_Init( SPI2, &spi_init );
		SPI_Cmd( SPI2, ENABLE );
	}
}
bool TieSPI::begin()
{
	if( spix == SPI2 )
	{
		GPIO_SetBits(GPIOB, cs);
		GPIO_ResetBits(GPIOB, sck);
		GPIO_SetBits(GPIOB, mosi);
		return true;
	}
	return false;
}

#define NSS2_1 GPIO_SetBits( GPIOB, GPIO_Pin_12 )
#define NSS2_0 GPIO_ResetBits( GPIOB, GPIO_Pin_12 )
//Ham gui du lieu qua SPI
void TieSPI::write( const uint8_t *buffer, uint16_t len )
{
	if( spix == SPI2 )
	{
		for( int i = 0; i < len; i++ )
		{
			NSS2_0;
			SPI_I2S_SendData( spix, *(buffer+i) );
			while( SPI_I2S_GetFlagStatus(spix, SPI_I2S_FLAG_BSY) == SET ){}
			NSS2_1;
		}
	} 
}