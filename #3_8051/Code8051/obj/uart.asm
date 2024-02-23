;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 4.2.0 #13081 (MINGW64)
;--------------------------------------------------------
	.module uart
	.optsdcc -mmcs51 --model-small
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _BTN_Init_PARM_2
	.globl _main
	.globl _uart_println
	.globl _CY
	.globl _AC
	.globl _F0
	.globl _RS1
	.globl _RS0
	.globl _OV
	.globl _FL
	.globl _P
	.globl _TF2
	.globl _EXF2
	.globl _RCLK
	.globl _TCLK
	.globl _EXEN2
	.globl _TR2
	.globl _C_T2
	.globl _CP_RL2
	.globl _T2CON_7
	.globl _T2CON_6
	.globl _T2CON_5
	.globl _T2CON_4
	.globl _T2CON_3
	.globl _T2CON_2
	.globl _T2CON_1
	.globl _T2CON_0
	.globl _PT2
	.globl _PS
	.globl _PT1
	.globl _PX1
	.globl _PT0
	.globl _PX0
	.globl _RD
	.globl _WR
	.globl _T1
	.globl _T0
	.globl _INT1
	.globl _INT0
	.globl _TXD
	.globl _RXD
	.globl _P3_7
	.globl _P3_6
	.globl _P3_5
	.globl _P3_4
	.globl _P3_3
	.globl _P3_2
	.globl _P3_1
	.globl _P3_0
	.globl _EA
	.globl _ET2
	.globl _ES
	.globl _ET1
	.globl _EX1
	.globl _ET0
	.globl _EX0
	.globl _P2_7
	.globl _P2_6
	.globl _P2_5
	.globl _P2_4
	.globl _P2_3
	.globl _P2_2
	.globl _P2_1
	.globl _P2_0
	.globl _SM0
	.globl _SM1
	.globl _SM2
	.globl _REN
	.globl _TB8
	.globl _RB8
	.globl _TI
	.globl _RI
	.globl _T2EX
	.globl _T2
	.globl _P1_7
	.globl _P1_6
	.globl _P1_5
	.globl _P1_4
	.globl _P1_3
	.globl _P1_2
	.globl _P1_1
	.globl _P1_0
	.globl _TF1
	.globl _TR1
	.globl _TF0
	.globl _TR0
	.globl _IE1
	.globl _IT1
	.globl _IE0
	.globl _IT0
	.globl _P0_7
	.globl _P0_6
	.globl _P0_5
	.globl _P0_4
	.globl _P0_3
	.globl _P0_2
	.globl _P0_1
	.globl _P0_0
	.globl _B
	.globl _A
	.globl _ACC
	.globl _PSW
	.globl _TH2
	.globl _TL2
	.globl _RCAP2H
	.globl _RCAP2L
	.globl _T2MOD
	.globl _T2CON
	.globl _IP
	.globl _P3
	.globl _IE
	.globl _P2
	.globl _SBUF
	.globl _SCON
	.globl _P1
	.globl _TH1
	.globl _TH0
	.globl _TL1
	.globl _TL0
	.globl _TMOD
	.globl _TCON
	.globl _PCON
	.globl _DPH
	.globl _DPL
	.globl _SP
	.globl _P0
	.globl _cmd_end
	.globl _cmd
	.globl _cols
	.globl _count_100
	.globl _BTN_Update_PARM_2
	.globl _uart_println_PARM_2
	.globl _dtime
	.globl _delay
	.globl _timer0_init
	.globl _timer0_update
	.globl _timer2_38khz_init
	.globl _uart_init
	.globl _uart_write
	.globl _BTN_Init
	.globl _BTN_Update
	.globl _BTN_IsDown
	.globl _BTN_IsPress
;--------------------------------------------------------
; special function registers
;--------------------------------------------------------
	.area RSEG    (ABS,DATA)
	.org 0x0000
_P0	=	0x0080
_SP	=	0x0081
_DPL	=	0x0082
_DPH	=	0x0083
_PCON	=	0x0087
_TCON	=	0x0088
_TMOD	=	0x0089
_TL0	=	0x008a
_TL1	=	0x008b
_TH0	=	0x008c
_TH1	=	0x008d
_P1	=	0x0090
_SCON	=	0x0098
_SBUF	=	0x0099
_P2	=	0x00a0
_IE	=	0x00a8
_P3	=	0x00b0
_IP	=	0x00b8
_T2CON	=	0x00c8
_T2MOD	=	0x00c9
_RCAP2L	=	0x00ca
_RCAP2H	=	0x00cb
_TL2	=	0x00cc
_TH2	=	0x00cd
_PSW	=	0x00d0
_ACC	=	0x00e0
_A	=	0x00e0
_B	=	0x00f0
;--------------------------------------------------------
; special function bits
;--------------------------------------------------------
	.area RSEG    (ABS,DATA)
	.org 0x0000
_P0_0	=	0x0080
_P0_1	=	0x0081
_P0_2	=	0x0082
_P0_3	=	0x0083
_P0_4	=	0x0084
_P0_5	=	0x0085
_P0_6	=	0x0086
_P0_7	=	0x0087
_IT0	=	0x0088
_IE0	=	0x0089
_IT1	=	0x008a
_IE1	=	0x008b
_TR0	=	0x008c
_TF0	=	0x008d
_TR1	=	0x008e
_TF1	=	0x008f
_P1_0	=	0x0090
_P1_1	=	0x0091
_P1_2	=	0x0092
_P1_3	=	0x0093
_P1_4	=	0x0094
_P1_5	=	0x0095
_P1_6	=	0x0096
_P1_7	=	0x0097
_T2	=	0x0090
_T2EX	=	0x0091
_RI	=	0x0098
_TI	=	0x0099
_RB8	=	0x009a
_TB8	=	0x009b
_REN	=	0x009c
_SM2	=	0x009d
_SM1	=	0x009e
_SM0	=	0x009f
_P2_0	=	0x00a0
_P2_1	=	0x00a1
_P2_2	=	0x00a2
_P2_3	=	0x00a3
_P2_4	=	0x00a4
_P2_5	=	0x00a5
_P2_6	=	0x00a6
_P2_7	=	0x00a7
_EX0	=	0x00a8
_ET0	=	0x00a9
_EX1	=	0x00aa
_ET1	=	0x00ab
_ES	=	0x00ac
_ET2	=	0x00ad
_EA	=	0x00af
_P3_0	=	0x00b0
_P3_1	=	0x00b1
_P3_2	=	0x00b2
_P3_3	=	0x00b3
_P3_4	=	0x00b4
_P3_5	=	0x00b5
_P3_6	=	0x00b6
_P3_7	=	0x00b7
_RXD	=	0x00b0
_TXD	=	0x00b1
_INT0	=	0x00b2
_INT1	=	0x00b3
_T0	=	0x00b4
_T1	=	0x00b5
_WR	=	0x00b6
_RD	=	0x00b7
_PX0	=	0x00b8
_PT0	=	0x00b9
_PX1	=	0x00ba
_PT1	=	0x00bb
_PS	=	0x00bc
_PT2	=	0x00bd
_T2CON_0	=	0x00c8
_T2CON_1	=	0x00c9
_T2CON_2	=	0x00ca
_T2CON_3	=	0x00cb
_T2CON_4	=	0x00cc
_T2CON_5	=	0x00cd
_T2CON_6	=	0x00ce
_T2CON_7	=	0x00cf
_CP_RL2	=	0x00c8
_C_T2	=	0x00c9
_TR2	=	0x00ca
_EXEN2	=	0x00cb
_TCLK	=	0x00cc
_RCLK	=	0x00cd
_EXF2	=	0x00ce
_TF2	=	0x00cf
_P	=	0x00d0
_FL	=	0x00d1
_OV	=	0x00d2
_RS0	=	0x00d3
_RS1	=	0x00d4
_F0	=	0x00d5
_AC	=	0x00d6
_CY	=	0x00d7
;--------------------------------------------------------
; overlayable register banks
;--------------------------------------------------------
	.area REG_BANK_0	(REL,OVR,DATA)
	.ds 8
;--------------------------------------------------------
; internal ram data
;--------------------------------------------------------
	.area DSEG    (DATA)
_dtime::
	.ds 1
_uart_println_PARM_2:
	.ds 1
_BTN_Update_PARM_2:
	.ds 1
_BTN_Update_btn_65536_26:
	.ds 3
_count_100::
	.ds 2
_cols::
	.ds 5
_cmd::
	.ds 1
_cmd_end::
	.ds 1
_main_keys_65537_40:
	.ds 30
;--------------------------------------------------------
; overlayable items in internal ram
;--------------------------------------------------------
	.area	OSEG    (OVR,DATA)
	.area	OSEG    (OVR,DATA)
	.area	OSEG    (OVR,DATA)
_BTN_Init_PARM_2:
	.ds 1
	.area	OSEG    (OVR,DATA)
	.area	OSEG    (OVR,DATA)
;--------------------------------------------------------
; Stack segment in internal ram
;--------------------------------------------------------
	.area	SSEG
__start__stack:
	.ds	1

;--------------------------------------------------------
; indirectly addressable internal ram data
;--------------------------------------------------------
	.area ISEG    (DATA)
;--------------------------------------------------------
; absolute internal ram data
;--------------------------------------------------------
	.area IABS    (ABS,DATA)
	.area IABS    (ABS,DATA)
;--------------------------------------------------------
; bit data
;--------------------------------------------------------
	.area BSEG    (BIT)
;--------------------------------------------------------
; paged external ram data
;--------------------------------------------------------
	.area PSEG    (PAG,XDATA)
;--------------------------------------------------------
; external ram data
;--------------------------------------------------------
	.area XSEG    (XDATA)
;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area XABS    (ABS,XDATA)
;--------------------------------------------------------
; external initialized ram data
;--------------------------------------------------------
	.area XISEG   (XDATA)
	.area HOME    (CODE)
	.area GSINIT0 (CODE)
	.area GSINIT1 (CODE)
	.area GSINIT2 (CODE)
	.area GSINIT3 (CODE)
	.area GSINIT4 (CODE)
	.area GSINIT5 (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	.area CSEG    (CODE)
;--------------------------------------------------------
; interrupt vector
;--------------------------------------------------------
	.area HOME    (CODE)
__interrupt_vect:
	ljmp	__sdcc_gsinit_startup
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area HOME    (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	.area GSINIT  (CODE)
	.globl __sdcc_gsinit_startup
	.globl __sdcc_program_startup
	.globl __start__stack
	.globl __mcs51_genXINIT
	.globl __mcs51_genXRAMCLEAR
	.globl __mcs51_genRAMCLEAR
;	uart.c:10: extern uint8_t dtime = 0;
	mov	_dtime,#0x00
;	uart.c:162: uint16_t count_100 = 0;
	clr	a
	mov	_count_100,a
	mov	(_count_100 + 1),a
;	uart.c:163: uint8_t cols[5] = {1, 2, 3, 4, 0};
	mov	_cols,#0x01
	mov	(_cols + 0x0001),#0x02
	mov	(_cols + 0x0002),#0x03
	mov	(_cols + 0x0003),#0x04
;	1-genFromRTrack replaced	mov	(_cols + 0x0004),#0x00
	mov	(_cols + 0x0004),a
;	uart.c:164: uint8_t cmd = 0;
;	1-genFromRTrack replaced	mov	_cmd,#0x00
	mov	_cmd,a
;	uart.c:165: uint8_t cmd_end = 0;
;	1-genFromRTrack replaced	mov	_cmd_end,#0x00
	mov	_cmd_end,a
	.area GSFINAL (CODE)
	ljmp	__sdcc_program_startup
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area HOME    (CODE)
	.area HOME    (CODE)
__sdcc_program_startup:
	ljmp	_main
;	return from main will return to caller
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG    (CODE)
;------------------------------------------------------------
;Allocation info for local variables in function 'delay'
;------------------------------------------------------------
;ms                        Allocated to registers r6 r7 
;i                         Allocated to registers r4 r5 
;------------------------------------------------------------
;	uart.c:13: void delay(uint16_t ms)
;	-----------------------------------------
;	 function delay
;	-----------------------------------------
_delay:
	ar7 = 0x07
	ar6 = 0x06
	ar5 = 0x05
	ar4 = 0x04
	ar3 = 0x03
	ar2 = 0x02
	ar1 = 0x01
	ar0 = 0x00
	mov	r6,dpl
	mov	r7,dph
;	uart.c:16: for( i = 0; i < ms; i++ )
	mov	r4,#0x00
	mov	r5,#0x00
00106$:
	clr	c
	mov	a,r4
	subb	a,r6
	mov	a,r5
	subb	a,r7
	jnc	00108$
;	uart.c:18: TR0 = 0;
;	assignBit
	clr	_TR0
;	uart.c:19: TH0 = 0xFC;
	mov	_TH0,#0xfc
;	uart.c:20: TL0 = 0x18;
	mov	_TL0,#0x18
;	uart.c:21: TF0 = 0;
;	assignBit
	clr	_TF0
;	uart.c:22: TR0 = 1;
;	assignBit
	setb	_TR0
;	uart.c:23: while( !TF0 ) { }
00101$:
	jnb	_TF0,00101$
;	uart.c:16: for( i = 0; i < ms; i++ )
	inc	r4
	cjne	r4,#0x00,00106$
	inc	r5
	sjmp	00106$
00108$:
;	uart.c:25: }
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'timer0_init'
;------------------------------------------------------------
;	uart.c:34: void timer0_init()
;	-----------------------------------------
;	 function timer0_init
;	-----------------------------------------
_timer0_init:
;	uart.c:36: TMOD &= ~0xF0;
	anl	_TMOD,#0x0f
;	uart.c:37: TMOD |= (1 << 0);
	orl	_TMOD,#0x01
;	uart.c:39: TR0 = 0;
;	assignBit
	clr	_TR0
;	uart.c:40: TH0 = 0x00;
	mov	_TH0,#0x00
;	uart.c:41: TL0 = 0x00;
	mov	_TL0,#0x00
;	uart.c:42: TR0 = 1;
;	assignBit
	setb	_TR0
;	uart.c:43: }
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'timer0_update'
;------------------------------------------------------------
;	uart.c:44: void timer0_update()
;	-----------------------------------------
;	 function timer0_update
;	-----------------------------------------
_timer0_update:
;	uart.c:46: dtime = 0;
	mov	_dtime,#0x00
;	uart.c:47: if( TH0 > 0x03 || (TH0 == 0x03 && TL0 >= 0x41) )
	mov	a,_TH0
	add	a,#0xff - 0x03
	jc	00101$
	mov	a,#0x03
	cjne	a,_TH0,00105$
	mov	a,#0x100 - 0x41
	add	a,_TL0
	jnc	00105$
00101$:
;	uart.c:49: TL0 = (TH0 > 0x03) ? 0x00 : (TL0 - 0x41);
	mov	a,_TH0
	add	a,#0xff - 0x03
	jnc	00107$
	mov	r6,#0x00
	mov	r7,#0x00
	sjmp	00108$
00107$:
	mov	a,_TL0
	add	a,#0xbf
	mov	r5,a
	mov	r6,a
	rlc	a
	subb	a,acc
	mov	r7,a
00108$:
	mov	_TL0,r6
;	uart.c:50: dtime = TH0 / 0x03;
	mov	r6,_TH0
	mov	r7,#0x00
	mov	__divsint_PARM_2,#0x03
;	1-genFromRTrack replaced	mov	(__divsint_PARM_2 + 1),#0x00
	mov	(__divsint_PARM_2 + 1),r7
	mov	dpl,r6
	mov	dph,r7
	lcall	__divsint
	mov	r6,dpl
	mov	_dtime,r6
;	uart.c:51: TH0 -= dtime*0x03;
	mov	a,_dtime
	mov	b,#0x03
	mul	ab
	mov	r7,a
	mov	a,_TH0
	mov	r6,a
	clr	c
	subb	a,r7
	mov	_TH0,a
00105$:
;	uart.c:53: }
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'timer2_38khz_init'
;------------------------------------------------------------
;	uart.c:54: void timer2_38khz_init()
;	-----------------------------------------
;	 function timer2_38khz_init
;	-----------------------------------------
_timer2_38khz_init:
;	uart.c:56: T2CON = 0x00;
	mov	_T2CON,#0x00
;	uart.c:57: T2MOD = 0x02;
	mov	_T2MOD,#0x02
;	uart.c:59: RCAP2H = 0xFF;
	mov	_RCAP2H,#0xff
;	uart.c:60: RCAP2L = 0xBE;
	mov	_RCAP2L,#0xbe
;	uart.c:64: TR2 = 1;
;	assignBit
	setb	_TR2
;	uart.c:65: }
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'uart_init'
;------------------------------------------------------------
;	uart.c:66: void uart_init()
;	-----------------------------------------
;	 function uart_init
;	-----------------------------------------
_uart_init:
;	uart.c:68: SM0 = 0;		//uart mode 1
;	assignBit
	clr	_SM0
;	uart.c:69: SM1 = 1;
;	assignBit
	setb	_SM1
;	uart.c:71: TMOD |= 0x20;	//timer1 8bit, auto reload
	orl	_TMOD,#0x20
;	uart.c:72: TH1 = 0xF5;		//2400 b/s
	mov	_TH1,#0xf5
;	uart.c:75: TR1 = 1;
;	assignBit
	setb	_TR1
;	uart.c:76: TI = 1;
;	assignBit
	setb	_TI
;	uart.c:77: }
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'uart_write'
;------------------------------------------------------------
;c                         Allocated to registers r7 
;------------------------------------------------------------
;	uart.c:78: void uart_write( uint8_t c )
;	-----------------------------------------
;	 function uart_write
;	-----------------------------------------
_uart_write:
	mov	r7,dpl
;	uart.c:80: while ( TI == 0 );
00101$:
;	uart.c:81: TI = 0;
;	assignBit
	jbc	_TI,00114$
	sjmp	00101$
00114$:
;	uart.c:82: SBUF = c;	
	mov	_SBUF,r7
;	uart.c:83: }
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'uart_println'
;------------------------------------------------------------
;size                      Allocated with name '_uart_println_PARM_2'
;s                         Allocated to registers r5 r6 r7 
;i                         Allocated to registers r4 
;------------------------------------------------------------
;	uart.c:84: void uart_println( uint8_t* s, uint8_t size )
;	-----------------------------------------
;	 function uart_println
;	-----------------------------------------
_uart_println:
	mov	r5,dpl
	mov	r6,dph
	mov	r7,b
;	uart.c:86: for( uint8_t i = 0; i < size; i++ )
	mov	r4,#0x00
00103$:
	clr	c
	mov	a,r4
	subb	a,_uart_println_PARM_2
	jnc	00101$
;	uart.c:88: uart_write(s[i]);
	mov	a,r4
	add	a,r5
	mov	r1,a
	clr	a
	addc	a,r6
	mov	r2,a
	mov	ar3,r7
	mov	dpl,r1
	mov	dph,r2
	mov	b,r3
	lcall	__gptrget
	mov	dpl,a
	push	ar7
	push	ar6
	push	ar5
	push	ar4
	lcall	_uart_write
	pop	ar4
	pop	ar5
	pop	ar6
	pop	ar7
;	uart.c:86: for( uint8_t i = 0; i < size; i++ )
	inc	r4
	sjmp	00103$
00101$:
;	uart.c:90: uart_write('\n');
	mov	dpl,#0x0a
;	uart.c:91: }
	ljmp	_uart_write
;------------------------------------------------------------
;Allocation info for local variables in function 'BTN_Init'
;------------------------------------------------------------
;pin_of_p1                 Allocated with name '_BTN_Init_PARM_2'
;button                    Allocated to registers r5 r6 r7 
;------------------------------------------------------------
;	uart.c:107: void BTN_Init( BTN* button, uint8_t pin_of_p1 )
;	-----------------------------------------
;	 function BTN_Init
;	-----------------------------------------
_BTN_Init:
;	uart.c:109: button->pin_of_p1 = pin_of_p1;
	mov	r5,dpl
	mov	r6,dph
	mov	r7,b
	mov	a,_BTN_Init_PARM_2
	lcall	__gptrput
;	uart.c:110: button->control = 0b10000000;
	mov	a,#0x01
	add	a,r5
	mov	r2,a
	clr	a
	addc	a,r6
	mov	r3,a
	mov	ar4,r7
	mov	dpl,r2
	mov	dph,r3
	mov	b,r4
	mov	a,#0x80
	lcall	__gptrput
;	uart.c:111: button->flag = 0x00;
	mov	a,#0x02
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	clr	a
;	uart.c:112: }
	ljmp	__gptrput
;------------------------------------------------------------
;Allocation info for local variables in function 'BTN_Update'
;------------------------------------------------------------
;dtime                     Allocated with name '_BTN_Update_PARM_2'
;btn                       Allocated with name '_BTN_Update_btn_65536_26'
;res                       Allocated to registers r4 
;------------------------------------------------------------
;	uart.c:113: uint8_t BTN_Update( BTN* btn, uint8_t dtime )
;	-----------------------------------------
;	 function BTN_Update
;	-----------------------------------------
_BTN_Update:
	mov	_BTN_Update_btn_65536_26,dpl
	mov	(_BTN_Update_btn_65536_26 + 1),dph
	mov	(_BTN_Update_btn_65536_26 + 2),b
;	uart.c:115: uint8_t res = 0;
	mov	r4,#0x00
;	uart.c:117: if( dtime > 0x80 )                  //overflow control -> khong dem
	mov	a,_BTN_Update_PARM_2
	add	a,#0xff - 0x80
	jnc	00122$
;	uart.c:119: if( bitread(P1, btn->pin_of_p1) ) 		//Nut nha?	
	mov	dpl,_BTN_Update_btn_65536_26
	mov	dph,(_BTN_Update_btn_65536_26 + 1)
	mov	b,(_BTN_Update_btn_65536_26 + 2)
	lcall	__gptrget
	mov	r3,a
	mov	b,r3
	inc	b
	mov	a,_P1
	sjmp	00188$
00187$:
	clr	c
	rrc	a
00188$:
	djnz	b,00187$
	jnb	acc.0,00102$
;	uart.c:120: btn->control = 0b10000000; 														//down = 0
	mov	a,#0x01
	add	a,_BTN_Update_btn_65536_26
	mov	r1,a
	clr	a
	addc	a,(_BTN_Update_btn_65536_26 + 1)
	mov	r2,a
	mov	r3,(_BTN_Update_btn_65536_26 + 2)
	mov	dpl,r1
	mov	dph,r2
	mov	b,r3
	mov	a,#0x80
	lcall	__gptrput
	ljmp	00123$
00102$:
;	uart.c:122: btn->control = 0b11000000;														//down = 1
	mov	a,#0x01
	add	a,_BTN_Update_btn_65536_26
	mov	r1,a
	clr	a
	addc	a,(_BTN_Update_btn_65536_26 + 1)
	mov	r2,a
	mov	r3,(_BTN_Update_btn_65536_26 + 2)
	mov	dpl,r1
	mov	dph,r2
	mov	b,r3
	mov	a,#0xc0
	lcall	__gptrput
	ljmp	00123$
00122$:
;	uart.c:124: else if( 0 == bitread(P1, btn->pin_of_p1) )	//Nut an?
	mov	dpl,_BTN_Update_btn_65536_26
	mov	dph,(_BTN_Update_btn_65536_26 + 1)
	mov	b,(_BTN_Update_btn_65536_26 + 2)
	lcall	__gptrget
	mov	r3,a
	mov	b,r3
	inc	b
	mov	a,_P1
	sjmp	00191$
00190$:
	clr	c
	rrc	a
00191$:
	djnz	b,00190$
	jb	acc.0,00119$
;	uart.c:127: if( btn->control == 0b10000000 )		//down real
	mov	a,#0x01
	add	a,_BTN_Update_btn_65536_26
	mov	r1,a
	clr	a
	addc	a,(_BTN_Update_btn_65536_26 + 1)
	mov	r2,a
	mov	r3,(_BTN_Update_btn_65536_26 + 2)
	mov	dpl,r1
	mov	dph,r2
	mov	b,r3
	lcall	__gptrget
	mov	r0,a
	cjne	r0,#0x80,00109$
;	uart.c:129: btn->control = 0b11010100;			//trang thai dang nhan, dem == 20ms
	mov	dpl,r1
	mov	dph,r2
	mov	b,r3
	mov	a,#0xd4
	lcall	__gptrput
;	uart.c:130: res = BTN_IsDown(btn);
	mov	dpl,_BTN_Update_btn_65536_26
	mov	dph,(_BTN_Update_btn_65536_26 + 1)
	mov	b,(_BTN_Update_btn_65536_26 + 2)
	lcall	_BTN_IsDown
	mov	r4,dpl
	ljmp	00123$
00109$:
;	uart.c:132: else if( btn->control > 0b11000000 )	//down noise
	mov	a,r0
	add	a,#0xff - 0xc0
	jnc	00123$
;	uart.c:134: btn->control -= dtime;						//giam thoi gian cho de thuc hien nha nut
	mov	a,r0
	clr	c
	subb	a,_BTN_Update_PARM_2
	mov	r0,a
	mov	dpl,r1
	mov	dph,r2
	mov	b,r3
	lcall	__gptrput
;	uart.c:135: if( btn->control < 0b11000000 ) btn->control = 0b11000000;    //san sang de nha
	cjne	r0,#0xc0,00196$
00196$:
	jnc	00123$
	mov	dpl,r1
	mov	dph,r2
	mov	b,r3
	mov	a,#0xc0
	lcall	__gptrput
	sjmp	00123$
00119$:
;	uart.c:140: res = 0;
	mov	r4,#0x00
;	uart.c:141: if( btn->control == 0b11000000 )     		//up real
	mov	a,#0x01
	add	a,_BTN_Update_btn_65536_26
	mov	r1,a
	clr	a
	addc	a,(_BTN_Update_btn_65536_26 + 1)
	mov	r2,a
	mov	r3,(_BTN_Update_btn_65536_26 + 2)
	mov	dpl,r1
	mov	dph,r2
	mov	b,r3
	lcall	__gptrget
	mov	r0,a
	cjne	r0,#0xc0,00116$
;	uart.c:142: btn->control = 0b10010100;       //trang thai khong nhan, dem == 20ms
	mov	dpl,r1
	mov	dph,r2
	mov	b,r3
	mov	a,#0x94
	lcall	__gptrput
	sjmp	00123$
00116$:
;	uart.c:143: else if( btn->control > 0b10000000 )//up noise
	mov	a,r0
	add	a,#0xff - 0x80
	jnc	00123$
;	uart.c:145: btn->control -= dtime;						//giam thoi gian cho de thuc hien an nut
	mov	a,r0
	clr	c
	subb	a,_BTN_Update_PARM_2
	mov	r0,a
	mov	dpl,r1
	mov	dph,r2
	mov	b,r3
	lcall	__gptrput
;	uart.c:146: if( btn->control < 0b10000000 ) btn->control = 0b10000000;    //san sang de an
	cjne	r0,#0x80,00201$
00201$:
	jnc	00123$
	mov	dpl,r1
	mov	dph,r2
	mov	b,r3
	mov	a,#0x80
	lcall	__gptrput
00123$:
;	uart.c:151: bitwrite( btn->flag, 1, bitread(btn->flag, 2) );           	//old  = last;
	mov	a,#0x02
	add	a,_BTN_Update_btn_65536_26
	mov	r1,a
	clr	a
	addc	a,(_BTN_Update_btn_65536_26 + 1)
	mov	r2,a
	mov	r3,(_BTN_Update_btn_65536_26 + 2)
	mov	dpl,r1
	mov	dph,r2
	mov	b,r3
	lcall	__gptrget
	mov	r0,a
	rr	a
	rr	a
	anl	a,#0x01
	mov	r7,a
	jz	00130$
	mov	a,#0x02
	orl	a,r0
	mov	r7,a
	mov	dpl,r1
	mov	dph,r2
	mov	b,r3
	lcall	__gptrput
	sjmp	00131$
00130$:
	anl	ar0,#0xfd
	mov	dpl,r1
	mov	dph,r2
	mov	b,r3
	mov	a,r0
	lcall	__gptrput
00131$:
;	uart.c:152: bitwrite( btn->flag, 2, BTN_IsDown( btn ) );                //last = IsDown();
	mov	dpl,_BTN_Update_btn_65536_26
	mov	dph,(_BTN_Update_btn_65536_26 + 1)
	mov	b,(_BTN_Update_btn_65536_26 + 2)
	push	ar4
	lcall	_BTN_IsDown
	mov	a,dpl
	pop	ar4
	jz	00132$
	mov	a,#0x02
	add	a,_BTN_Update_btn_65536_26
	mov	r5,a
	clr	a
	addc	a,(_BTN_Update_btn_65536_26 + 1)
	mov	r6,a
	mov	r7,(_BTN_Update_btn_65536_26 + 2)
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	r3,a
	orl	ar3,#0x04
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	mov	a,r3
	lcall	__gptrput
	sjmp	00133$
00132$:
	mov	a,#0x02
	add	a,_BTN_Update_btn_65536_26
	mov	r5,a
	clr	a
	addc	a,(_BTN_Update_btn_65536_26 + 1)
	mov	r6,a
	mov	r7,(_BTN_Update_btn_65536_26 + 2)
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	r3,a
	anl	ar3,#0xfb
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	mov	a,r3
	lcall	__gptrput
00133$:
;	uart.c:153: if( bitread( btn->flag, 1 ) && !bitread( btn->flag, 2 ) )  	//old:down && last:up
	mov	a,#0x02
	add	a,_BTN_Update_btn_65536_26
	mov	r5,a
	clr	a
	addc	a,(_BTN_Update_btn_65536_26 + 1)
	mov	r6,a
	mov	r7,(_BTN_Update_btn_65536_26 + 2)
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	mov	r3,a
	rr	a
	anl	a,#0x01
	mov	r2,a
	jz	00125$
	mov	a,r3
	rr	a
	rr	a
	anl	a,#0x01
	mov	r2,a
	jnz	00125$
;	uart.c:154: bitset( btn->flag, 0 );                          					//isPress
	mov	a,#0x01
	orl	a,r3
	mov	r2,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrput
	sjmp	00126$
00125$:
;	uart.c:156: bitclear( btn->flag, 0 );                        					//isPress yet
	anl	ar3,#0xfe
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	mov	a,r3
	lcall	__gptrput
00126$:
;	uart.c:157: return res;
	mov	dpl,r4
;	uart.c:158: }
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'BTN_IsDown'
;------------------------------------------------------------
;btn                       Allocated to registers r5 r6 r7 
;------------------------------------------------------------
;	uart.c:159: uint8_t BTN_IsDown( BTN* btn )  { return bitread( btn->control, 6 ); }
;	-----------------------------------------
;	 function BTN_IsDown
;	-----------------------------------------
_BTN_IsDown:
	mov	r5,dpl
	mov	r6,dph
	mov	r7,b
	inc	r5
	cjne	r5,#0x00,00103$
	inc	r6
00103$:
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	rl	a
	rl	a
	anl	a,#0x01
	mov	dpl,a
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'BTN_IsPress'
;------------------------------------------------------------
;btn                       Allocated to registers r5 r6 r7 
;------------------------------------------------------------
;	uart.c:160: uint8_t BTN_IsPress( BTN* btn ) { return bitread( btn->flag, 0 ); }
;	-----------------------------------------
;	 function BTN_IsPress
;	-----------------------------------------
_BTN_IsPress:
	mov	r5,dpl
	mov	r6,dph
	mov	r7,b
	mov	a,#0x02
	add	a,r5
	mov	r5,a
	clr	a
	addc	a,r6
	mov	r6,a
	mov	dpl,r5
	mov	dph,r6
	mov	b,r7
	lcall	__gptrget
	anl	a,#0x01
	mov	dpl,a
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'main'
;------------------------------------------------------------
;keys                      Allocated with name '_main_keys_65537_40'
;i                         Allocated to registers r7 
;i                         Allocated to registers r7 
;j                         Allocated to registers r6 
;i                         Allocated to registers r7 
;------------------------------------------------------------
;	uart.c:167: void main(void)
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
;	uart.c:169: timer0_init();
	lcall	_timer0_init
;	uart.c:170: timer2_38khz_init();
	lcall	_timer2_38khz_init
;	uart.c:171: uart_init();
	lcall	_uart_init
;	uart.c:174: for( uint8_t i = 0; i < 10; i++ )
	mov	r7,#0x00
00126$:
	cjne	r7,#0x0a,00232$
00232$:
	jnc	00123$
;	uart.c:176: BTN_Init(&keys[i], (i%2)+2);
	mov	a,r7
	mov	b,#0x03
	mul	ab
	add	a,#_main_keys_65537_40
	mov	r6,a
	mov	r5,#0x00
	mov	r4,#0x40
	mov	a,#0x01
	anl	a,r7
	add	a,#0x02
	mov	_BTN_Init_PARM_2,a
	mov	dpl,r6
	mov	dph,r5
	mov	b,r4
	push	ar7
	lcall	_BTN_Init
	pop	ar7
;	uart.c:174: for( uint8_t i = 0; i < 10; i++ )
	inc	r7
;	uart.c:179: while(1)
	sjmp	00126$
00123$:
;	uart.c:181: timer0_update();
	lcall	_timer0_update
;	uart.c:182: count_100 += dtime;
	mov	r6,_dtime
	mov	r7,#0x00
	mov	r4,_count_100
	mov	r5,(_count_100 + 1)
	mov	a,r6
	add	a,r4
	mov	r4,a
	mov	a,r7
	addc	a,r5
	mov	r5,a
	mov	_count_100,r4
	mov	(_count_100 + 1),r5
;	uart.c:183: bitset(P3, 2);
	orl	_P3,#0x04
;	uart.c:185: for( uint8_t i = 0; i < 5; i++ )
	mov	r7,#0x00
00132$:
	cjne	r7,#0x05,00234$
00234$:
	jnc	00103$
;	uart.c:187: bitclear(P2, cols[i]);
	mov	a,r7
	add	a,#_cols
	mov	r1,a
	mov	ar6,@r1
	mov	b,r6
	inc	b
	mov	a,#0x01
	sjmp	00238$
00236$:
	add	a,acc
00238$:
	djnz	b,00236$
	cpl	a
	anl	_P2,a
;	uart.c:188: for( uint8_t j = 0; j < 2; j++ )
	mov	r6,#0x00
00129$:
	cjne	r6,#0x02,00239$
00239$:
	jnc	00102$
;	uart.c:190: BTN_Update(&keys[i*2 + j], dtime);
	mov	ar5,r7
	mov	a,r5
	add	a,r5
	mov	r5,a
	mov	ar4,r6
	mov	a,r4
	add	a,r5
	mov	b,#0x03
	mul	ab
	add	a,#_main_keys_65537_40
	mov	r5,a
	mov	r4,#0x00
	mov	r3,#0x40
	mov	_BTN_Update_PARM_2,_dtime
	mov	dpl,r5
	mov	dph,r4
	mov	b,r3
	push	ar7
	push	ar6
	lcall	_BTN_Update
	pop	ar6
	pop	ar7
;	uart.c:188: for( uint8_t j = 0; j < 2; j++ )
	inc	r6
	sjmp	00129$
00102$:
;	uart.c:192: bitset(P2, cols[i]);
	mov	a,r7
	add	a,#_cols
	mov	r1,a
	mov	ar6,@r1
	mov	b,r6
	inc	b
	mov	a,#0x01
	sjmp	00243$
00241$:
	add	a,acc
00243$:
	djnz	b,00241$
	mov	r6,a
	orl	_P2,a
;	uart.c:185: for( uint8_t i = 0; i < 5; i++ )
	inc	r7
	sjmp	00132$
00103$:
;	uart.c:196: for( uint8_t i = 0; i < 10; i++ )
	mov	r7,#0x00
	mov	r6,#0x00
00135$:
	cjne	r6,#0x0a,00244$
00244$:
	jnc	00113$
;	uart.c:198: if( BTN_IsDown(&keys[i]) )
	mov	a,r6
	mov	b,#0x03
	mul	ab
	add	a,#_main_keys_65537_40
	mov	r5,a
	mov	r4,#0x00
	mov	r3,#0x40
	mov	dpl,r5
	mov	dph,r4
	mov	b,r3
	push	ar7
	push	ar6
	lcall	_BTN_IsDown
	mov	a,dpl
	pop	ar6
	pop	ar7
	jz	00136$
;	uart.c:200: bitclear(P3, 2);
	anl	_P3,#0xfb
;	uart.c:201: if( i >= 7 )
	cjne	r6,#0x07,00247$
00247$:
	jc	00110$
;	uart.c:203: switch (i)
	cjne	r7,#0x07,00249$
	sjmp	00104$
00249$:
	cjne	r7,#0x08,00250$
	sjmp	00105$
00250$:
;	uart.c:205: case 7: cmd = 0b01111110;break;
	cjne	r7,#0x09,00113$
	sjmp	00106$
00104$:
	mov	_cmd,#0x7e
;	uart.c:206: case 8: cmd = 0b00111100;break;
	sjmp	00113$
00105$:
	mov	_cmd,#0x3c
;	uart.c:207: case 9: cmd = 0b01010101;break;
	sjmp	00113$
00106$:
	mov	_cmd,#0x55
;	uart.c:210: break;
	sjmp	00113$
00110$:
;	uart.c:212: cmd |= (1 << i);
	mov	ar5,r6
	mov	b,r5
	inc	b
	mov	a,#0x01
	sjmp	00254$
00252$:
	add	a,acc
00254$:
	djnz	b,00252$
	mov	r5,a
	orl	_cmd,a
00136$:
;	uart.c:196: for( uint8_t i = 0; i < 10; i++ )
	inc	r6
	mov	ar7,r6
	sjmp	00135$
00113$:
;	uart.c:215: if( count_100 >= 60 )
	mov	r6,_count_100
	mov	r7,(_count_100 + 1)
	clr	c
	mov	a,r6
	subb	a,#0x3c
	mov	a,r7
	subb	a,#0x00
	jnc	00255$
	ljmp	00123$
00255$:
;	uart.c:217: count_100 %= 60;
	mov	__moduint_PARM_2,#0x3c
	mov	(__moduint_PARM_2 + 1),#0x00
	mov	dpl,r6
	mov	dph,r7
	lcall	__moduint
	mov	r6,dpl
	mov	r7,dph
	mov	_count_100,r6
	mov	(_count_100 + 1),r7
;	uart.c:218: if( cmd_end && cmd == 0 )
	mov	a,_cmd_end
	jz	00117$
	mov	a,_cmd
;	uart.c:220: uart_write( 0x00 );
	jnz	00117$
	mov	dpl,a
	lcall	_uart_write
;	uart.c:221: cmd_end = 0;
	mov	_cmd_end,#0x00
	ljmp	00123$
00117$:
;	uart.c:223: else if( cmd != 0 )
	mov	a,_cmd
	jnz	00258$
	ljmp	00123$
00258$:
;	uart.c:225: uart_write( cmd );
	mov	dpl,_cmd
	lcall	_uart_write
;	uart.c:226: cmd = 0;
	mov	_cmd,#0x00
;	uart.c:227: cmd_end = 1;
	mov	_cmd_end,#0x01
;	uart.c:231: }
	ljmp	00123$
	.area CSEG    (CODE)
	.area CONST   (CODE)
	.area XINIT   (CODE)
	.area CABS    (ABS,CODE)
