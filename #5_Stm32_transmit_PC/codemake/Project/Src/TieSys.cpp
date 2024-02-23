#include "TieSys.h"

TieSystem::TieSystem()
	: LCD(),
	button( GPIOB, GPIO_Pin_9 ), 
	btn_sub_up( GPIOB, GPIO_Pin_0 ),
	btn_sub_dn( GPIOA, GPIO_Pin_6 ),
	btn_sub_rt( GPIOB, GPIO_Pin_1 ),
	btn_sub_lt( GPIOA, GPIO_Pin_7 )
{}
void TieSystem::Delay( uint32_t ms )
{
	ft.Delay(ms);
}
const uint16_t& TieSystem::DTime() const
{
	return ft.dtime;
}
const uint16_t& TieSystem::DTime_Micro() const
{
	return ft.dtime_micro;
}
void TieSystem::Tick()
{
	TieSystem::Update();
	#ifdef HAS_DISPLAY
	LCD.clearDisplay();
	TieSystem::Draw();
	LCD.display();
	#endif
}
void TieSystem::Init()
{
	ft.Init();
	LCD.Init();
	GPIO_InitTypeDef 	gpio_init;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio_init.GPIO_Pin = GPIO_Pin_13;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &gpio_init);
	
    GPIO_SetBits(GPIOC, GPIO_Pin_13);

	//PB10 - TX
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_init.GPIO_Pin = GPIO_Pin_10;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_init);

	//PB11 - RX
	gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_init.GPIO_Pin = GPIO_Pin_11;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_init);

	//Cau hinh USART3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	app_uart_init.USART_BaudRate = 1000000;
	app_uart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	app_uart_init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	app_uart_init.USART_Parity = USART_Parity_No;
	app_uart_init.USART_StopBits = USART_StopBits_1;
	app_uart_init.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &app_uart_init);
	USART_Cmd(USART3, ENABLE);
	Init_DMA_UART();
}
void TieSystem::Update()
{
	ft.Tick();
	Update_BTN();

	NextLayer();

	Process_DMA_UART();
}
void TieSystem::Draw()
{
	ProcessLayer();
}

void TieSystem::Update_BTN()
{
	button.Update( DTime() );
	btn_sub_up.Update( DTime() );
	btn_sub_dn.Update( DTime() );
	btn_sub_rt.Update( DTime() );
	btn_sub_lt.Update( DTime() );
}
void TieSystem::Init_DMA_UART()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_InitTypeDef dma_init;
	dma_init.DMA_BufferSize = 528;	//So luong byte cho mang buff du dieu LCD5110
	dma_init.DMA_DIR 		= DMA_DIR_PeripheralSRC;
	dma_init.DMA_M2M		= DMA_MemoryInc_Disable;
	dma_init.DMA_MemoryBaseAddr = uint32_t(uart_data);
	dma_init.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	dma_init.DMA_MemoryInc		= DMA_MemoryInc_Enable;
	dma_init.DMA_Mode			= DMA_Mode_Circular;
	dma_init.DMA_PeripheralBaseAddr = uint32_t(&USART3->DR);
	dma_init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	dma_init.DMA_PeripheralInc		= DMA_PeripheralInc_Disable;
	dma_init.DMA_Priority			= DMA_Priority_Medium;
	DMA_Init(DMA1_Channel3, &dma_init);
	DMA_Cmd(DMA1_Channel3, ENABLE);
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
}
void TieSystem::Process_DMA_UART()
{
	if( DMA_GetFlagStatus(DMA1_FLAG_TC3) == SET 
		&& (curLayer == Layer::RXgif || curLayer == Layer::RXspeed) )
	{
		TieFunc::GPIO_TOGGLE(GPIOC, GPIO_Pin_13);
		DMA_ClearFlag(DMA1_FLAG_TC3);
		cnt_speed_rx_dtime = cnt_speed_rx_last;
		cnt_speed_rx_last = 0;
	}
}
