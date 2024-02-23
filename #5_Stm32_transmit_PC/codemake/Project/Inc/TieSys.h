#pragma once
#include "tie_define.h"
#include "tie_feature.h"
#include "tie_mem.h"

class TieSystem
{
public:
	enum class Layer : uint8_t
	{
		Main = 0, RXtext, RXgif, TXgame, RXspeed, Count
	};
public:
	TieLCD LCD;
	TieBTN button;
	TieBTN btn_sub_up;
	TieBTN btn_sub_dn;
	TieBTN btn_sub_rt;
	TieBTN btn_sub_lt;

	TieSystem();
	void Init();
	void Tick();
	void Delay( uint32_t ms );
	const uint16_t& DTime() const;
	const uint16_t& DTime_Micro() const;
private:
	void Update();
	void Draw();

	TieFT ft;
	void Update_BTN();

	//Layer
	Layer curLayer = Layer::Main;
	uint8_t idx_layer = 0;
	void NextLayer();
	void ProcessLayer();
	char eff_display[20] = "Main";
	int eff_cnt = 0;
	static const uint16_t eff_dura = 1000;

	void Init_Main();

	void Init_RXtext();
	uint8_t rx_text_count = 0;
	static constexpr uint8_t rx_text_max_size = 100;
	static constexpr char rx_text_end_rx = '\n';
	char rx_text_buff[rx_text_max_size];
	uint16_t rx_text_cnt_dura = 0;
	int rx_text_offset_y = 0;
	int rx_text_heightofbuff = 0;

	void Init_RXgif();
	
	void Init_TXgame();
	char u8_gamedraw = 's';
	// uint16_t cnt_tx = 0;

	void Init_RXspeed();
	uint16_t cnt_speed_dura = 0;
	uint16_t cnt_speed_frame = 0;
	uint16_t cnt_speed_rx_last = 0;
	uint16_t cnt_speed_rx_dtime = 0;
	uint16_t cnt_speed_rx_frame = 0;
	uint8_t  speed_rx_color = 1;
	/////////////////

	//Variable
	uint16_t cnt_13 = 0;

	// char u8Data[100] = "";
	/////////////////

	//UART 
    USART_InitTypeDef app_uart_init;
	void Init_DMA_UART();
	void Process_DMA_UART();
	uint8_t uart_data[528] = {1};
	uint16_t i_uart = 0;
	uint8_t done = 0;
	/////////////////
};