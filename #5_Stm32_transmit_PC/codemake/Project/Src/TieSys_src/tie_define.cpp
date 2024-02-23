#include "tie_define.h"

uint8_t TieFunc::Arr_BeSame( const char* a, const char* b )
{
	_ui i = 0;
	while( *(a+i) != '\0' && *(b+i) != '\0' )
	{
		if( *(a+i) != *(b+i) )
			return 0;
		++i;
	}
	return 1;
}
void TieFunc::Arr_Flush( char* a, uint16_t size )
{
	_ui i = 0;
	while ( *(a+i) != '\0' && i < size )
	{
		*(a+i) = 0;
		++i;
	}
}
void TieFunc::GPIO_TOGGLE( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin )
{
	GPIO_WriteBit( GPIOx, GPIO_Pin, GPIO_ReadInputDataBit(GPIOx, GPIO_Pin)?Bit_RESET:Bit_SET );
}
void TieFunc::Reset_Sys_DMA_USART()
{
	// USART_SendData( USART3, uart_data[0] );
	// while( USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET ){}
	DMA_Cmd(DMA1_Channel3, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel3, 528);
	DMA_Cmd(DMA1_Channel3, ENABLE);
}