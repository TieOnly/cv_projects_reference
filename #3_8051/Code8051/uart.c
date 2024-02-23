#include <8051_redefine.h>	//thư viện định nghĩa các thanh ghi
#include <stdint.h>

#define bitset(reg, bit) 		(reg |= (1 << bit))
#define bitclear(reg, bit)		(reg &= ~(1 << bit))
#define bitread(reg, bit)		(1 & (reg >> bit))
#define bitwrite(reg, bit, val)	( val ? bitset(reg, bit) : bitclear(reg, bit) )
#define bittoggle(reg, bit)		( bitwrite(reg, bit, !bitread(reg, bit)) )

extern uint8_t dtime = 0;	//biến toàn cục có nhiệm vụ lập lịch

void delay(uint16_t ms);
void delay(uint16_t ms)
{
	uint16_t i;
	for( i = 0; i < ms; i++ )
	{
		TR0 = 0;			//tắt timer0
		TH0 = 0xFC;			//lập giá trị đếm lên 0xFFFF đủ 1ms
		TL0 = 0x18;
		TF0 = 0;
		TR0 = 1;			//bật timer0
		while( !TF0 ) { }	//lặp khi chưa đủ 1ms
	}
}

void timer0_init();				//hàm cài đặt timer0
void timer0_update();			//hàm cập nhật timer0
void timer2_38khz_init();		//hàm cài đặt timer2 tạo xung 38khz
void uart_init();				//hàm cài đặt uart
void uart_write( uint8_t c );
void uart_print( uint8_t* s, uint8_t size );

void timer0_init()
{
	TMOD &= ~0xF0;				//chế độ timer0 8bit đếm lên
	TMOD |= (1 << 0);

	TR0 = 0;					//reset thanh ghi đếm
	TH0 = 0x00;
	TL0 = 0x00;
	TR0 = 1;
}
void timer0_update()
{
	dtime = 0;
	if( TH0 > 0x03 || (TH0 == 0x03 && TL0 >= 0x41) )	//khi thanh ghi đếm đạt 1ms
	{
		TL0 = (TH0 > 0x03) ? 0x00 : (TL0 - 0x41);
		dtime = TH0 / 0x03;
		TH0 -= dtime*0x03;
	}
}
void timer2_38khz_init()
{
	T2CON = 0x00;	
	T2MOD = 0x02;	//timer2 tự động nạp lại và chế độ clock output

	RCAP2H = 0xFF;	//giá trị nạp lại để tạo tần số 38Khz
	RCAP2L = 0xBE;

	TR2 = 1;		//bật timer2
}
void uart_init()
{
	SM0 = 0;		//uart mode 1
	SM1 = 1;

	TMOD |= 0x20;	//timer1 8bit, auto reload
	TH1 = 0xF5;		//2400 b/s
	
	TR1 = 1;
	TI = 1;
}
void uart_write( uint8_t c )
{
	while ( TI == 0 );
	TI = 0;
	SBUF = c;
}
void uart_println( uint8_t* s, uint8_t size )
{
	for( uint8_t i = 0; i < size; i++ )
	{
		uart_write(s[i]);
	}
	uart_write('\n');
}

struct Button
{
	uint8_t pin_of_p1;
	//control - [7]:assert, [6]:flag_state, [5:0]:count//
	uint8_t control;
	//flag    - [2:0]- Press Sate | [2]: last, [1]: old, [0]: isPress//
	uint8_t flag;
};
typedef struct Button BTN;
void BTN_Init( BTN* button, uint8_t pin_of_p1 );
void BTN_Update( BTN* button, uint8_t dtime );
uint8_t BTN_IsDown( BTN* button );
uint8_t BTN_IsPress( BTN* button );

void BTN_Init( BTN* button, uint8_t pin_of_p1 )
{
	button->pin_of_p1 = pin_of_p1;
	button->control = 0b10000000;
	button->flag = 0x00;
}
void BTN_Update( BTN* btn, uint8_t dtime )
{
	//Process Event
	if( dtime > 0x80 )                  //overflow control -> khong dem
	{                    
		if( bitread(P1, btn->pin_of_p1) ) 		//Nut nha?	
			btn->control = 0b10000000; 			//down = 0
		else
			btn->control = 0b11000000;			//down = 1
	}
	else if( 0 == bitread(P1, btn->pin_of_p1) )	//Nut an?
	{
		if( btn->control == 0b10000000 )		//down real
		{
			btn->control = 0b11010100;			//trang thai dang nhan, dem == 20ms
		}
		else if( btn->control > 0b11000000 )	//down noise
		{
			btn->control -= dtime;				//giam thoi gian cho de thuc hien nha nut
			if( btn->control < 0b11000000 ) btn->control = 0b11000000;    //san sang de nha
		} 
	}
	else
	{
		if( btn->control == 0b11000000 )		//up real
			btn->control = 0b10010100;			//trang thai khong nhan, dem == 20ms
		else if( btn->control > 0b10000000 )	//up noise
		{
			btn->control -= dtime;				//giam thoi gian cho de thuc hien an nut
			if( btn->control < 0b10000000 ) btn->control = 0b10000000;    //san sang de an
		}
	}

	//Process Press
	bitwrite( btn->flag, 1, bitread(btn->flag, 2) );           	//old  = last;
	bitwrite( btn->flag, 2, BTN_IsDown( btn ) );                //last = IsDown();
	if( bitread( btn->flag, 1 ) && !bitread( btn->flag, 2 ) )  	//old:down && last:up
		bitset( btn->flag, 0 );                          		//isPress
	else 
		bitclear( btn->flag, 0 );                        		//isPress yet
}
uint8_t BTN_IsDown( BTN* btn )  { return bitread( btn->control, 6 ); }
uint8_t BTN_IsPress( BTN* btn ) { return bitread( btn->flag, 0 ); }

uint16_t count_60 = 0;

uint8_t cols[5] = {1, 2, 3, 4, 0};	//Quét các chân P2_1, P2_2, P2_3, P2_4, P2_0
uint8_t cmd = 0;
uint8_t cmd_end = 0;

void main(void)
{
	timer0_init();
	timer2_38khz_init();
	uart_init();

	//Khởi tạo và cấu hình nút nhấn tại P1_2 và P1_3
	BTN keys[10];
	for( uint8_t i = 0; i < 10; i++ )
	{
		BTN_Init(&keys[i], (i%2)+2);
	}

	while(1)
	{	
		timer0_update();
		count_60 += dtime;
		bitset(P3, 2);

		for( uint8_t i = 0; i < 5; i++ )
		{
			bitclear(P2, cols[i]);
			for( uint8_t j = 0; j < 2; j++ )
			{
				BTN_Update(&keys[i*2 + j], dtime);
			}
			bitset(P2, cols[i]);
		}

		//Gui du lieu dieu khien moi 60ms
		for( uint8_t i = 0; i < 10; i++ )
		{
			if( BTN_IsDown(&keys[i]) )
			{
				bitclear(P3, 2);
				//Gán giá trị cho cmd tùy vào mỗi phím được nhấn
				if( i >= 7 )
				{
					switch (i)
					{
					case 7: cmd = 0b01111110;break;
					case 8: cmd = 0b00111100;break;
					case 9: cmd = 0b01010101;break;
					default:break;
					}
					break;
				}
				cmd |= (1 << i);
			}
		}
		if( count_60 >= 60 )
		{
			count_60 %= 60;
			if( cmd_end && cmd == 0 )
			{
				uart_write( 0x00 );
				cmd_end = 0;
			}
			else if( cmd != 0 )
			{
				uart_write( cmd );
				cmd = 0;
				cmd_end = 1;
			}
		}
	}
}
