#include "tie_rs232.h"

static uint8_t buff[RS232_SIZE_BUFF];
static uint8_t* ring_p_fst;
static uint8_t* ring_p_snd;
static uint32_t ring_size;
static uint8_t done_frame = 1;

uint8_t* rs232_buff()
{
	return buff;
}
void rs232_init()
{
	rs232_clear_buff();
	ring_p_fst = buff;
	ring_p_snd = buff;
	ring_size = 0;

	USART_InitTypeDef	uart;
	GPIO_InitTypeDef 	gpio;
	RS232_ENABLE_RCC_GPIO;
	RS232_ENABLE_RCC_USART;

	//PB10 - TX
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Pin = RS232_TX_PIN;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RS232_TX_PORT, &gpio);

	//PB11 - RX
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Pin = RS232_RX_PIN;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RS232_RX_PORT, &gpio);

	//Cau hinh RS232_USART
	uart.USART_BaudRate = RS232_BAUD;
	uart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	uart.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	uart.USART_Parity = USART_Parity_No;
	uart.USART_StopBits = USART_StopBits_1;
	uart.USART_WordLength = USART_WordLength_8b;
	USART_Init(RS232_USART, &uart);
	USART_Cmd(RS232_USART, ENABLE);

	//DMA
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_InitTypeDef dma_init;
	dma_init.DMA_BufferSize = RS232_SIZE_BUFF;
	dma_init.DMA_DIR 		= DMA_DIR_PeripheralSRC;
	dma_init.DMA_M2M		= DMA_MemoryInc_Disable;
	dma_init.DMA_MemoryBaseAddr = (uint32_t)(buff);
	dma_init.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	dma_init.DMA_MemoryInc		= DMA_MemoryInc_Enable;
	dma_init.DMA_Mode			= DMA_Mode_Circular;
	dma_init.DMA_PeripheralBaseAddr = (uint32_t)(&RS232_USART->DR);
	dma_init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	dma_init.DMA_PeripheralInc		= DMA_PeripheralInc_Disable;
	dma_init.DMA_Priority			= DMA_Priority_High;
	DMA_Init(DMA1_Channel5, &dma_init);
	DMA_Cmd(DMA1_Channel5, ENABLE);
	USART_DMACmd(RS232_USART, USART_DMAReq_Rx, ENABLE);
}
void rs232_send( const char* data )
{
	while (*data != '\0')
	{
		USART_SendData( RS232_USART, *(data++) );
		while( USART_GetFlagStatus( RS232_USART, USART_FLAG_TXE ) == RESET ) {}
	}	
}
void rs232_send_val( uint32_t value )
{
	if ( value <= 0 ) 
	{
		rs232_send("0");
		return;
	}

	char tmp[20];
	int len = 0;
	while( value > 0 )
	{
		tmp[len++] = (value % 10) + 48;
		value /= 10;		
	}
	tmp[len--] = '\0';
	int l = 0, r = len;
	while( l < r )
	{
		char t = tmp[l];
		tmp[l] = tmp[r];
		tmp[r] = t;
		l++;r--;
	}
	rs232_send(tmp);
}
void rs232_enable( uint8_t is_enable )
{
	USART_Cmd(RS232_USART, is_enable ? ENABLE : DISABLE);
}
uint32_t rs232_available( const char end_char )
{
	if (done_frame) ring_size = 0;
	done_frame = 0;
	while( *ring_p_snd != '\0' && ring_size < RS232_SIZE_BUFF )
	{
		ring_size++;
		if (*ring_p_snd == end_char) done_frame = 1;
		if ( ring_p_snd == (buff + RS232_SIZE_BUFF) ) ring_p_snd = buff;
		else ring_p_snd++;
	}
	if (done_frame) return ring_size;
	if (ring_size > 0 && end_char == '\0')
	{
		done_frame = 1;
		return ring_size;
	}
	return 0;
}
void rs232_get_buff( uint8_t* dest )
{
	while( ring_size-- > 0 )
	{
		*(dest++) = *(ring_p_fst);
		*ring_p_fst = '\0';
		if ( ring_p_fst == (buff + RS232_SIZE_BUFF) ) ring_p_fst = buff;
		else ring_p_fst++;
	}
	*dest = '\0';
	ring_p_fst = ring_p_snd;
}
void rs232_clear_buff()
{
	memset( buff, '\0', RS232_SIZE_BUFF );
}
