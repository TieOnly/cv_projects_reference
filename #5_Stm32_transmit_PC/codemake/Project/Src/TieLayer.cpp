#include "TieSys.h"

void TieSystem::NextLayer()
{
	if( button.IsPress() )
	{
		eff_cnt = eff_dura;
		memset(eff_display, 0, 20);
		++idx_layer %= uint8_t(Layer::Count);
		switch (idx_layer)
		{
		case 0: Init_Main(); break;
		case 1: Init_RXtext(); break;
		case 2: Init_RXgif(); break;
		case 3: Init_TXgame(); break;
		case 4: Init_RXspeed(); break;
		default: curLayer = Layer::Count; break;
		}
	}
	if( eff_cnt > 0 )
	{
		eff_cnt -= DTime();
		if( eff_cnt <= 0 ) eff_cnt = 0;
	}
}

void TieSystem::Init_Main()
{
	memcpy(eff_display, "Main", 4);
	curLayer = Layer::Main;
}
void TieSystem::Init_RXtext()
{
	memcpy(eff_display, "Text", 4);
	curLayer = Layer::RXtext;
	//Reset DMA_UART
	memset(uart_data, 0x00, rx_text_max_size);
	TieFunc::Reset_Sys_DMA_USART();
}
void TieSystem::Init_RXgif()
{
	memcpy(eff_display, "Gif", 3);
	curLayer = Layer::RXgif;

	//Reset baudrate
	USART_Cmd(USART3, DISABLE);
	app_uart_init.USART_BaudRate = 1000000;
	USART_Init(USART3, &app_uart_init);
	USART_Cmd(USART3, ENABLE);

	//Reset DMA_UART
	memset(uart_data, 0xff, 528);
	TieFunc::Reset_Sys_DMA_USART();
}
void TieSystem::Init_TXgame()
{
	memcpy(eff_display, "Game", 4);
	curLayer = Layer::TXgame;

	//Reset baudrate
	USART_Cmd(USART3, DISABLE);
	app_uart_init.USART_BaudRate = 115200;
	USART_Init(USART3, &app_uart_init);
	USART_Cmd(USART3, ENABLE);
}
void TieSystem::Init_RXspeed()
{
	memcpy(eff_display, "Speed", 5);
	curLayer = Layer::RXspeed;

	//Reset baudrate
	USART_Cmd(USART3, DISABLE);
	app_uart_init.USART_BaudRate = 1000000;
	USART_Init(USART3, &app_uart_init);
	USART_Cmd(USART3, ENABLE);

	//Reset buff
	memset(uart_data, 0xff, 528);
	cnt_speed_rx_dtime = 0;
}

void TieSystem::ProcessLayer()
{
	if( eff_cnt > 0 )
	{
		LCD.DrawTextInBound(eff_display, 0, 0, BLACK);
		return;
	}
	
	if( curLayer == Layer::RXgif )
	{
		LCD.drawBitmap(0, 0, uart_data, 84, 48, BLACK);
	}
	else if( curLayer == Layer::Main )
	{
		LCD.DrawText( "STM32\nRS232", 0, 0 );
	}
	else if( curLayer == Layer::RXtext )
	{
		if( uart_data[0] != 0x00 )
		{
			for( int i = 0; i < rx_text_max_size; i++ )
			{
				if( uart_data[i] == '\n' || i == rx_text_max_size-1 )
				{
					memcpy(rx_text_buff, uart_data, i);
					rx_text_buff[i] = '\0';
					memset(uart_data, 0x00, rx_text_max_size);
					TieFunc::Reset_Sys_DMA_USART();
					rx_text_heightofbuff = (int)TieLCD::GetHeightOf( rx_text_buff );
					rx_text_offset_y = 0;
					TieFunc::GPIO_TOGGLE(GPIOC, GPIO_Pin_13);
					break;
				}
			}
		}
		
		rx_text_cnt_dura += DTime();
		if( rx_text_cnt_dura >= 10 )
		{
			rx_text_cnt_dura %= 10;
			if( btn_sub_dn.IsDown() )
			{
				if( (rx_text_offset_y + (rx_text_heightofbuff - LCDHEIGHT)) > 0 )
					rx_text_offset_y -= 1;
			} 
			else if( btn_sub_up.IsDown() && rx_text_offset_y < 0 )
				rx_text_offset_y += 1;
		}

		LCD.DrawTextInBound(rx_text_buff, 0, rx_text_offset_y);
	}
	else if( curLayer == Layer::TXgame )
	{
		uint8_t flag = 1;
		if( btn_sub_up.IsPress() ) u8_gamedraw = 'w';
		else if( btn_sub_dn.IsPress() ) u8_gamedraw = 's';
		else if( btn_sub_rt.IsPress() ) u8_gamedraw = 'd';
		else if( btn_sub_lt.IsPress() ) u8_gamedraw = 'a';
		else flag = 0;
		
		if( flag ) USART_SendData( USART3, u8_gamedraw );
		LCD.DrawChar(u8_gamedraw, 0, 0);
	}
	else if( curLayer == Layer::RXspeed )
	{
		cnt_speed_dura += DTime();
		cnt_speed_rx_last += DTime();
		if( cnt_speed_dura >= 1000 )
		{
			cnt_speed_dura %= 1000;
			cnt_speed_frame = DTime();
			cnt_speed_rx_frame = cnt_speed_rx_dtime;
		}
		if( btn_sub_up.IsPress() ) speed_rx_color = !speed_rx_color;
		if( speed_rx_color ) 
			LCD.drawBitmap(font_w * 2, 0, uart_data, 84, 48, BLACK);
		LCD.DrawNumber(cnt_speed_frame, 0, 0);
		LCD.DrawNumber(cnt_speed_rx_frame, 0, font_h*1);
	}
}
