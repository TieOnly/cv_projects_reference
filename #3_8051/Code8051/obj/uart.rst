                                      1 ;--------------------------------------------------------
                                      2 ; File Created by SDCC : free open source ANSI-C Compiler
                                      3 ; Version 4.2.0 #13081 (MINGW64)
                                      4 ;--------------------------------------------------------
                                      5 	.module uart
                                      6 	.optsdcc -mmcs51 --model-small
                                      7 	
                                      8 ;--------------------------------------------------------
                                      9 ; Public variables in this module
                                     10 ;--------------------------------------------------------
                                     11 	.globl _BTN_Init_PARM_2
                                     12 	.globl _main
                                     13 	.globl _uart_println
                                     14 	.globl _CY
                                     15 	.globl _AC
                                     16 	.globl _F0
                                     17 	.globl _RS1
                                     18 	.globl _RS0
                                     19 	.globl _OV
                                     20 	.globl _FL
                                     21 	.globl _P
                                     22 	.globl _TF2
                                     23 	.globl _EXF2
                                     24 	.globl _RCLK
                                     25 	.globl _TCLK
                                     26 	.globl _EXEN2
                                     27 	.globl _TR2
                                     28 	.globl _C_T2
                                     29 	.globl _CP_RL2
                                     30 	.globl _T2CON_7
                                     31 	.globl _T2CON_6
                                     32 	.globl _T2CON_5
                                     33 	.globl _T2CON_4
                                     34 	.globl _T2CON_3
                                     35 	.globl _T2CON_2
                                     36 	.globl _T2CON_1
                                     37 	.globl _T2CON_0
                                     38 	.globl _PT2
                                     39 	.globl _PS
                                     40 	.globl _PT1
                                     41 	.globl _PX1
                                     42 	.globl _PT0
                                     43 	.globl _PX0
                                     44 	.globl _RD
                                     45 	.globl _WR
                                     46 	.globl _T1
                                     47 	.globl _T0
                                     48 	.globl _INT1
                                     49 	.globl _INT0
                                     50 	.globl _TXD
                                     51 	.globl _RXD
                                     52 	.globl _P3_7
                                     53 	.globl _P3_6
                                     54 	.globl _P3_5
                                     55 	.globl _P3_4
                                     56 	.globl _P3_3
                                     57 	.globl _P3_2
                                     58 	.globl _P3_1
                                     59 	.globl _P3_0
                                     60 	.globl _EA
                                     61 	.globl _ET2
                                     62 	.globl _ES
                                     63 	.globl _ET1
                                     64 	.globl _EX1
                                     65 	.globl _ET0
                                     66 	.globl _EX0
                                     67 	.globl _P2_7
                                     68 	.globl _P2_6
                                     69 	.globl _P2_5
                                     70 	.globl _P2_4
                                     71 	.globl _P2_3
                                     72 	.globl _P2_2
                                     73 	.globl _P2_1
                                     74 	.globl _P2_0
                                     75 	.globl _SM0
                                     76 	.globl _SM1
                                     77 	.globl _SM2
                                     78 	.globl _REN
                                     79 	.globl _TB8
                                     80 	.globl _RB8
                                     81 	.globl _TI
                                     82 	.globl _RI
                                     83 	.globl _T2EX
                                     84 	.globl _T2
                                     85 	.globl _P1_7
                                     86 	.globl _P1_6
                                     87 	.globl _P1_5
                                     88 	.globl _P1_4
                                     89 	.globl _P1_3
                                     90 	.globl _P1_2
                                     91 	.globl _P1_1
                                     92 	.globl _P1_0
                                     93 	.globl _TF1
                                     94 	.globl _TR1
                                     95 	.globl _TF0
                                     96 	.globl _TR0
                                     97 	.globl _IE1
                                     98 	.globl _IT1
                                     99 	.globl _IE0
                                    100 	.globl _IT0
                                    101 	.globl _P0_7
                                    102 	.globl _P0_6
                                    103 	.globl _P0_5
                                    104 	.globl _P0_4
                                    105 	.globl _P0_3
                                    106 	.globl _P0_2
                                    107 	.globl _P0_1
                                    108 	.globl _P0_0
                                    109 	.globl _B
                                    110 	.globl _A
                                    111 	.globl _ACC
                                    112 	.globl _PSW
                                    113 	.globl _TH2
                                    114 	.globl _TL2
                                    115 	.globl _RCAP2H
                                    116 	.globl _RCAP2L
                                    117 	.globl _T2MOD
                                    118 	.globl _T2CON
                                    119 	.globl _IP
                                    120 	.globl _P3
                                    121 	.globl _IE
                                    122 	.globl _P2
                                    123 	.globl _SBUF
                                    124 	.globl _SCON
                                    125 	.globl _P1
                                    126 	.globl _TH1
                                    127 	.globl _TH0
                                    128 	.globl _TL1
                                    129 	.globl _TL0
                                    130 	.globl _TMOD
                                    131 	.globl _TCON
                                    132 	.globl _PCON
                                    133 	.globl _DPH
                                    134 	.globl _DPL
                                    135 	.globl _SP
                                    136 	.globl _P0
                                    137 	.globl _cmd_end
                                    138 	.globl _cmd
                                    139 	.globl _cols
                                    140 	.globl _count_100
                                    141 	.globl _BTN_Update_PARM_2
                                    142 	.globl _uart_println_PARM_2
                                    143 	.globl _dtime
                                    144 	.globl _delay
                                    145 	.globl _timer0_init
                                    146 	.globl _timer0_update
                                    147 	.globl _timer2_38khz_init
                                    148 	.globl _uart_init
                                    149 	.globl _uart_write
                                    150 	.globl _BTN_Init
                                    151 	.globl _BTN_Update
                                    152 	.globl _BTN_IsDown
                                    153 	.globl _BTN_IsPress
                                    154 ;--------------------------------------------------------
                                    155 ; special function registers
                                    156 ;--------------------------------------------------------
                                    157 	.area RSEG    (ABS,DATA)
      000000                        158 	.org 0x0000
                           000080   159 _P0	=	0x0080
                           000081   160 _SP	=	0x0081
                           000082   161 _DPL	=	0x0082
                           000083   162 _DPH	=	0x0083
                           000087   163 _PCON	=	0x0087
                           000088   164 _TCON	=	0x0088
                           000089   165 _TMOD	=	0x0089
                           00008A   166 _TL0	=	0x008a
                           00008B   167 _TL1	=	0x008b
                           00008C   168 _TH0	=	0x008c
                           00008D   169 _TH1	=	0x008d
                           000090   170 _P1	=	0x0090
                           000098   171 _SCON	=	0x0098
                           000099   172 _SBUF	=	0x0099
                           0000A0   173 _P2	=	0x00a0
                           0000A8   174 _IE	=	0x00a8
                           0000B0   175 _P3	=	0x00b0
                           0000B8   176 _IP	=	0x00b8
                           0000C8   177 _T2CON	=	0x00c8
                           0000C9   178 _T2MOD	=	0x00c9
                           0000CA   179 _RCAP2L	=	0x00ca
                           0000CB   180 _RCAP2H	=	0x00cb
                           0000CC   181 _TL2	=	0x00cc
                           0000CD   182 _TH2	=	0x00cd
                           0000D0   183 _PSW	=	0x00d0
                           0000E0   184 _ACC	=	0x00e0
                           0000E0   185 _A	=	0x00e0
                           0000F0   186 _B	=	0x00f0
                                    187 ;--------------------------------------------------------
                                    188 ; special function bits
                                    189 ;--------------------------------------------------------
                                    190 	.area RSEG    (ABS,DATA)
      000000                        191 	.org 0x0000
                           000080   192 _P0_0	=	0x0080
                           000081   193 _P0_1	=	0x0081
                           000082   194 _P0_2	=	0x0082
                           000083   195 _P0_3	=	0x0083
                           000084   196 _P0_4	=	0x0084
                           000085   197 _P0_5	=	0x0085
                           000086   198 _P0_6	=	0x0086
                           000087   199 _P0_7	=	0x0087
                           000088   200 _IT0	=	0x0088
                           000089   201 _IE0	=	0x0089
                           00008A   202 _IT1	=	0x008a
                           00008B   203 _IE1	=	0x008b
                           00008C   204 _TR0	=	0x008c
                           00008D   205 _TF0	=	0x008d
                           00008E   206 _TR1	=	0x008e
                           00008F   207 _TF1	=	0x008f
                           000090   208 _P1_0	=	0x0090
                           000091   209 _P1_1	=	0x0091
                           000092   210 _P1_2	=	0x0092
                           000093   211 _P1_3	=	0x0093
                           000094   212 _P1_4	=	0x0094
                           000095   213 _P1_5	=	0x0095
                           000096   214 _P1_6	=	0x0096
                           000097   215 _P1_7	=	0x0097
                           000090   216 _T2	=	0x0090
                           000091   217 _T2EX	=	0x0091
                           000098   218 _RI	=	0x0098
                           000099   219 _TI	=	0x0099
                           00009A   220 _RB8	=	0x009a
                           00009B   221 _TB8	=	0x009b
                           00009C   222 _REN	=	0x009c
                           00009D   223 _SM2	=	0x009d
                           00009E   224 _SM1	=	0x009e
                           00009F   225 _SM0	=	0x009f
                           0000A0   226 _P2_0	=	0x00a0
                           0000A1   227 _P2_1	=	0x00a1
                           0000A2   228 _P2_2	=	0x00a2
                           0000A3   229 _P2_3	=	0x00a3
                           0000A4   230 _P2_4	=	0x00a4
                           0000A5   231 _P2_5	=	0x00a5
                           0000A6   232 _P2_6	=	0x00a6
                           0000A7   233 _P2_7	=	0x00a7
                           0000A8   234 _EX0	=	0x00a8
                           0000A9   235 _ET0	=	0x00a9
                           0000AA   236 _EX1	=	0x00aa
                           0000AB   237 _ET1	=	0x00ab
                           0000AC   238 _ES	=	0x00ac
                           0000AD   239 _ET2	=	0x00ad
                           0000AF   240 _EA	=	0x00af
                           0000B0   241 _P3_0	=	0x00b0
                           0000B1   242 _P3_1	=	0x00b1
                           0000B2   243 _P3_2	=	0x00b2
                           0000B3   244 _P3_3	=	0x00b3
                           0000B4   245 _P3_4	=	0x00b4
                           0000B5   246 _P3_5	=	0x00b5
                           0000B6   247 _P3_6	=	0x00b6
                           0000B7   248 _P3_7	=	0x00b7
                           0000B0   249 _RXD	=	0x00b0
                           0000B1   250 _TXD	=	0x00b1
                           0000B2   251 _INT0	=	0x00b2
                           0000B3   252 _INT1	=	0x00b3
                           0000B4   253 _T0	=	0x00b4
                           0000B5   254 _T1	=	0x00b5
                           0000B6   255 _WR	=	0x00b6
                           0000B7   256 _RD	=	0x00b7
                           0000B8   257 _PX0	=	0x00b8
                           0000B9   258 _PT0	=	0x00b9
                           0000BA   259 _PX1	=	0x00ba
                           0000BB   260 _PT1	=	0x00bb
                           0000BC   261 _PS	=	0x00bc
                           0000BD   262 _PT2	=	0x00bd
                           0000C8   263 _T2CON_0	=	0x00c8
                           0000C9   264 _T2CON_1	=	0x00c9
                           0000CA   265 _T2CON_2	=	0x00ca
                           0000CB   266 _T2CON_3	=	0x00cb
                           0000CC   267 _T2CON_4	=	0x00cc
                           0000CD   268 _T2CON_5	=	0x00cd
                           0000CE   269 _T2CON_6	=	0x00ce
                           0000CF   270 _T2CON_7	=	0x00cf
                           0000C8   271 _CP_RL2	=	0x00c8
                           0000C9   272 _C_T2	=	0x00c9
                           0000CA   273 _TR2	=	0x00ca
                           0000CB   274 _EXEN2	=	0x00cb
                           0000CC   275 _TCLK	=	0x00cc
                           0000CD   276 _RCLK	=	0x00cd
                           0000CE   277 _EXF2	=	0x00ce
                           0000CF   278 _TF2	=	0x00cf
                           0000D0   279 _P	=	0x00d0
                           0000D1   280 _FL	=	0x00d1
                           0000D2   281 _OV	=	0x00d2
                           0000D3   282 _RS0	=	0x00d3
                           0000D4   283 _RS1	=	0x00d4
                           0000D5   284 _F0	=	0x00d5
                           0000D6   285 _AC	=	0x00d6
                           0000D7   286 _CY	=	0x00d7
                                    287 ;--------------------------------------------------------
                                    288 ; overlayable register banks
                                    289 ;--------------------------------------------------------
                                    290 	.area REG_BANK_0	(REL,OVR,DATA)
      000000                        291 	.ds 8
                                    292 ;--------------------------------------------------------
                                    293 ; internal ram data
                                    294 ;--------------------------------------------------------
                                    295 	.area DSEG    (DATA)
      000008                        296 _dtime::
      000008                        297 	.ds 1
      000009                        298 _uart_println_PARM_2:
      000009                        299 	.ds 1
      00000A                        300 _BTN_Update_PARM_2:
      00000A                        301 	.ds 1
      00000B                        302 _BTN_Update_btn_65536_26:
      00000B                        303 	.ds 3
      00000E                        304 _count_100::
      00000E                        305 	.ds 2
      000010                        306 _cols::
      000010                        307 	.ds 5
      000015                        308 _cmd::
      000015                        309 	.ds 1
      000016                        310 _cmd_end::
      000016                        311 	.ds 1
      000017                        312 _main_keys_65537_40:
      000017                        313 	.ds 30
                                    314 ;--------------------------------------------------------
                                    315 ; overlayable items in internal ram
                                    316 ;--------------------------------------------------------
                                    317 	.area	OSEG    (OVR,DATA)
                                    318 	.area	OSEG    (OVR,DATA)
                                    319 	.area	OSEG    (OVR,DATA)
      000035                        320 _BTN_Init_PARM_2:
      000035                        321 	.ds 1
                                    322 	.area	OSEG    (OVR,DATA)
                                    323 	.area	OSEG    (OVR,DATA)
                                    324 ;--------------------------------------------------------
                                    325 ; Stack segment in internal ram
                                    326 ;--------------------------------------------------------
                                    327 	.area	SSEG
      000037                        328 __start__stack:
      000037                        329 	.ds	1
                                    330 
                                    331 ;--------------------------------------------------------
                                    332 ; indirectly addressable internal ram data
                                    333 ;--------------------------------------------------------
                                    334 	.area ISEG    (DATA)
                                    335 ;--------------------------------------------------------
                                    336 ; absolute internal ram data
                                    337 ;--------------------------------------------------------
                                    338 	.area IABS    (ABS,DATA)
                                    339 	.area IABS    (ABS,DATA)
                                    340 ;--------------------------------------------------------
                                    341 ; bit data
                                    342 ;--------------------------------------------------------
                                    343 	.area BSEG    (BIT)
                                    344 ;--------------------------------------------------------
                                    345 ; paged external ram data
                                    346 ;--------------------------------------------------------
                                    347 	.area PSEG    (PAG,XDATA)
                                    348 ;--------------------------------------------------------
                                    349 ; external ram data
                                    350 ;--------------------------------------------------------
                                    351 	.area XSEG    (XDATA)
                                    352 ;--------------------------------------------------------
                                    353 ; absolute external ram data
                                    354 ;--------------------------------------------------------
                                    355 	.area XABS    (ABS,XDATA)
                                    356 ;--------------------------------------------------------
                                    357 ; external initialized ram data
                                    358 ;--------------------------------------------------------
                                    359 	.area XISEG   (XDATA)
                                    360 	.area HOME    (CODE)
                                    361 	.area GSINIT0 (CODE)
                                    362 	.area GSINIT1 (CODE)
                                    363 	.area GSINIT2 (CODE)
                                    364 	.area GSINIT3 (CODE)
                                    365 	.area GSINIT4 (CODE)
                                    366 	.area GSINIT5 (CODE)
                                    367 	.area GSINIT  (CODE)
                                    368 	.area GSFINAL (CODE)
                                    369 	.area CSEG    (CODE)
                                    370 ;--------------------------------------------------------
                                    371 ; interrupt vector
                                    372 ;--------------------------------------------------------
                                    373 	.area HOME    (CODE)
      000000                        374 __interrupt_vect:
      000000 02 00 06         [24]  375 	ljmp	__sdcc_gsinit_startup
                                    376 ;--------------------------------------------------------
                                    377 ; global & static initialisations
                                    378 ;--------------------------------------------------------
                                    379 	.area HOME    (CODE)
                                    380 	.area GSINIT  (CODE)
                                    381 	.area GSFINAL (CODE)
                                    382 	.area GSINIT  (CODE)
                                    383 	.globl __sdcc_gsinit_startup
                                    384 	.globl __sdcc_program_startup
                                    385 	.globl __start__stack
                                    386 	.globl __mcs51_genXINIT
                                    387 	.globl __mcs51_genXRAMCLEAR
                                    388 	.globl __mcs51_genRAMCLEAR
                                    389 ;	uart.c:10: extern uint8_t dtime = 0;
      00005F 75 08 00         [24]  390 	mov	_dtime,#0x00
                                    391 ;	uart.c:162: uint16_t count_100 = 0;
      000062 E4               [12]  392 	clr	a
      000063 F5 0E            [12]  393 	mov	_count_100,a
      000065 F5 0F            [12]  394 	mov	(_count_100 + 1),a
                                    395 ;	uart.c:163: uint8_t cols[5] = {1, 2, 3, 4, 0};
      000067 75 10 01         [24]  396 	mov	_cols,#0x01
      00006A 75 11 02         [24]  397 	mov	(_cols + 0x0001),#0x02
      00006D 75 12 03         [24]  398 	mov	(_cols + 0x0002),#0x03
      000070 75 13 04         [24]  399 	mov	(_cols + 0x0003),#0x04
                                    400 ;	1-genFromRTrack replaced	mov	(_cols + 0x0004),#0x00
      000073 F5 14            [12]  401 	mov	(_cols + 0x0004),a
                                    402 ;	uart.c:164: uint8_t cmd = 0;
                                    403 ;	1-genFromRTrack replaced	mov	_cmd,#0x00
      000075 F5 15            [12]  404 	mov	_cmd,a
                                    405 ;	uart.c:165: uint8_t cmd_end = 0;
                                    406 ;	1-genFromRTrack replaced	mov	_cmd_end,#0x00
      000077 F5 16            [12]  407 	mov	_cmd_end,a
                                    408 	.area GSFINAL (CODE)
      000079 02 00 03         [24]  409 	ljmp	__sdcc_program_startup
                                    410 ;--------------------------------------------------------
                                    411 ; Home
                                    412 ;--------------------------------------------------------
                                    413 	.area HOME    (CODE)
                                    414 	.area HOME    (CODE)
      000003                        415 __sdcc_program_startup:
      000003 02 03 C1         [24]  416 	ljmp	_main
                                    417 ;	return from main will return to caller
                                    418 ;--------------------------------------------------------
                                    419 ; code
                                    420 ;--------------------------------------------------------
                                    421 	.area CSEG    (CODE)
                                    422 ;------------------------------------------------------------
                                    423 ;Allocation info for local variables in function 'delay'
                                    424 ;------------------------------------------------------------
                                    425 ;ms                        Allocated to registers r6 r7 
                                    426 ;i                         Allocated to registers r4 r5 
                                    427 ;------------------------------------------------------------
                                    428 ;	uart.c:13: void delay(uint16_t ms)
                                    429 ;	-----------------------------------------
                                    430 ;	 function delay
                                    431 ;	-----------------------------------------
      00007C                        432 _delay:
                           000007   433 	ar7 = 0x07
                           000006   434 	ar6 = 0x06
                           000005   435 	ar5 = 0x05
                           000004   436 	ar4 = 0x04
                           000003   437 	ar3 = 0x03
                           000002   438 	ar2 = 0x02
                           000001   439 	ar1 = 0x01
                           000000   440 	ar0 = 0x00
      00007C AE 82            [24]  441 	mov	r6,dpl
      00007E AF 83            [24]  442 	mov	r7,dph
                                    443 ;	uart.c:16: for( i = 0; i < ms; i++ )
      000080 7C 00            [12]  444 	mov	r4,#0x00
      000082 7D 00            [12]  445 	mov	r5,#0x00
      000084                        446 00106$:
      000084 C3               [12]  447 	clr	c
      000085 EC               [12]  448 	mov	a,r4
      000086 9E               [12]  449 	subb	a,r6
      000087 ED               [12]  450 	mov	a,r5
      000088 9F               [12]  451 	subb	a,r7
      000089 50 16            [24]  452 	jnc	00108$
                                    453 ;	uart.c:18: TR0 = 0;
                                    454 ;	assignBit
      00008B C2 8C            [12]  455 	clr	_TR0
                                    456 ;	uart.c:19: TH0 = 0xFC;
      00008D 75 8C FC         [24]  457 	mov	_TH0,#0xfc
                                    458 ;	uart.c:20: TL0 = 0x18;
      000090 75 8A 18         [24]  459 	mov	_TL0,#0x18
                                    460 ;	uart.c:21: TF0 = 0;
                                    461 ;	assignBit
      000093 C2 8D            [12]  462 	clr	_TF0
                                    463 ;	uart.c:22: TR0 = 1;
                                    464 ;	assignBit
      000095 D2 8C            [12]  465 	setb	_TR0
                                    466 ;	uart.c:23: while( !TF0 ) { }
      000097                        467 00101$:
      000097 30 8D FD         [24]  468 	jnb	_TF0,00101$
                                    469 ;	uart.c:16: for( i = 0; i < ms; i++ )
      00009A 0C               [12]  470 	inc	r4
      00009B BC 00 E6         [24]  471 	cjne	r4,#0x00,00106$
      00009E 0D               [12]  472 	inc	r5
      00009F 80 E3            [24]  473 	sjmp	00106$
      0000A1                        474 00108$:
                                    475 ;	uart.c:25: }
      0000A1 22               [24]  476 	ret
                                    477 ;------------------------------------------------------------
                                    478 ;Allocation info for local variables in function 'timer0_init'
                                    479 ;------------------------------------------------------------
                                    480 ;	uart.c:34: void timer0_init()
                                    481 ;	-----------------------------------------
                                    482 ;	 function timer0_init
                                    483 ;	-----------------------------------------
      0000A2                        484 _timer0_init:
                                    485 ;	uart.c:36: TMOD &= ~0xF0;
      0000A2 53 89 0F         [24]  486 	anl	_TMOD,#0x0f
                                    487 ;	uart.c:37: TMOD |= (1 << 0);
      0000A5 43 89 01         [24]  488 	orl	_TMOD,#0x01
                                    489 ;	uart.c:39: TR0 = 0;
                                    490 ;	assignBit
      0000A8 C2 8C            [12]  491 	clr	_TR0
                                    492 ;	uart.c:40: TH0 = 0x00;
      0000AA 75 8C 00         [24]  493 	mov	_TH0,#0x00
                                    494 ;	uart.c:41: TL0 = 0x00;
      0000AD 75 8A 00         [24]  495 	mov	_TL0,#0x00
                                    496 ;	uart.c:42: TR0 = 1;
                                    497 ;	assignBit
      0000B0 D2 8C            [12]  498 	setb	_TR0
                                    499 ;	uart.c:43: }
      0000B2 22               [24]  500 	ret
                                    501 ;------------------------------------------------------------
                                    502 ;Allocation info for local variables in function 'timer0_update'
                                    503 ;------------------------------------------------------------
                                    504 ;	uart.c:44: void timer0_update()
                                    505 ;	-----------------------------------------
                                    506 ;	 function timer0_update
                                    507 ;	-----------------------------------------
      0000B3                        508 _timer0_update:
                                    509 ;	uart.c:46: dtime = 0;
      0000B3 75 08 00         [24]  510 	mov	_dtime,#0x00
                                    511 ;	uart.c:47: if( TH0 > 0x03 || (TH0 == 0x03 && TL0 >= 0x41) )
      0000B6 E5 8C            [12]  512 	mov	a,_TH0
      0000B8 24 FC            [12]  513 	add	a,#0xff - 0x03
      0000BA 40 0B            [24]  514 	jc	00101$
      0000BC 74 03            [12]  515 	mov	a,#0x03
      0000BE B5 8C 40         [24]  516 	cjne	a,_TH0,00105$
      0000C1 74 BF            [12]  517 	mov	a,#0x100 - 0x41
      0000C3 25 8A            [12]  518 	add	a,_TL0
      0000C5 50 3A            [24]  519 	jnc	00105$
      0000C7                        520 00101$:
                                    521 ;	uart.c:49: TL0 = (TH0 > 0x03) ? 0x00 : (TL0 - 0x41);
      0000C7 E5 8C            [12]  522 	mov	a,_TH0
      0000C9 24 FC            [12]  523 	add	a,#0xff - 0x03
      0000CB 50 06            [24]  524 	jnc	00107$
      0000CD 7E 00            [12]  525 	mov	r6,#0x00
      0000CF 7F 00            [12]  526 	mov	r7,#0x00
      0000D1 80 0A            [24]  527 	sjmp	00108$
      0000D3                        528 00107$:
      0000D3 E5 8A            [12]  529 	mov	a,_TL0
      0000D5 24 BF            [12]  530 	add	a,#0xbf
      0000D7 FD               [12]  531 	mov	r5,a
      0000D8 FE               [12]  532 	mov	r6,a
      0000D9 33               [12]  533 	rlc	a
      0000DA 95 E0            [12]  534 	subb	a,acc
      0000DC FF               [12]  535 	mov	r7,a
      0000DD                        536 00108$:
      0000DD 8E 8A            [24]  537 	mov	_TL0,r6
                                    538 ;	uart.c:50: dtime = TH0 / 0x03;
      0000DF AE 8C            [24]  539 	mov	r6,_TH0
      0000E1 7F 00            [12]  540 	mov	r7,#0x00
      0000E3 75 35 03         [24]  541 	mov	__divsint_PARM_2,#0x03
                                    542 ;	1-genFromRTrack replaced	mov	(__divsint_PARM_2 + 1),#0x00
      0000E6 8F 36            [24]  543 	mov	(__divsint_PARM_2 + 1),r7
      0000E8 8E 82            [24]  544 	mov	dpl,r6
      0000EA 8F 83            [24]  545 	mov	dph,r7
      0000EC 12 05 D6         [24]  546 	lcall	__divsint
      0000EF AE 82            [24]  547 	mov	r6,dpl
      0000F1 8E 08            [24]  548 	mov	_dtime,r6
                                    549 ;	uart.c:51: TH0 -= dtime*0x03;
      0000F3 E5 08            [12]  550 	mov	a,_dtime
      0000F5 75 F0 03         [24]  551 	mov	b,#0x03
      0000F8 A4               [48]  552 	mul	ab
      0000F9 FF               [12]  553 	mov	r7,a
      0000FA E5 8C            [12]  554 	mov	a,_TH0
      0000FC FE               [12]  555 	mov	r6,a
      0000FD C3               [12]  556 	clr	c
      0000FE 9F               [12]  557 	subb	a,r7
      0000FF F5 8C            [12]  558 	mov	_TH0,a
      000101                        559 00105$:
                                    560 ;	uart.c:53: }
      000101 22               [24]  561 	ret
                                    562 ;------------------------------------------------------------
                                    563 ;Allocation info for local variables in function 'timer2_38khz_init'
                                    564 ;------------------------------------------------------------
                                    565 ;	uart.c:54: void timer2_38khz_init()
                                    566 ;	-----------------------------------------
                                    567 ;	 function timer2_38khz_init
                                    568 ;	-----------------------------------------
      000102                        569 _timer2_38khz_init:
                                    570 ;	uart.c:56: T2CON = 0x00;
      000102 75 C8 00         [24]  571 	mov	_T2CON,#0x00
                                    572 ;	uart.c:57: T2MOD = 0x02;
      000105 75 C9 02         [24]  573 	mov	_T2MOD,#0x02
                                    574 ;	uart.c:59: RCAP2H = 0xFF;
      000108 75 CB FF         [24]  575 	mov	_RCAP2H,#0xff
                                    576 ;	uart.c:60: RCAP2L = 0xBE;
      00010B 75 CA BE         [24]  577 	mov	_RCAP2L,#0xbe
                                    578 ;	uart.c:64: TR2 = 1;
                                    579 ;	assignBit
      00010E D2 CA            [12]  580 	setb	_TR2
                                    581 ;	uart.c:65: }
      000110 22               [24]  582 	ret
                                    583 ;------------------------------------------------------------
                                    584 ;Allocation info for local variables in function 'uart_init'
                                    585 ;------------------------------------------------------------
                                    586 ;	uart.c:66: void uart_init()
                                    587 ;	-----------------------------------------
                                    588 ;	 function uart_init
                                    589 ;	-----------------------------------------
      000111                        590 _uart_init:
                                    591 ;	uart.c:68: SM0 = 0;		//uart mode 1
                                    592 ;	assignBit
      000111 C2 9F            [12]  593 	clr	_SM0
                                    594 ;	uart.c:69: SM1 = 1;
                                    595 ;	assignBit
      000113 D2 9E            [12]  596 	setb	_SM1
                                    597 ;	uart.c:71: TMOD |= 0x20;	//timer1 8bit, auto reload
      000115 43 89 20         [24]  598 	orl	_TMOD,#0x20
                                    599 ;	uart.c:72: TH1 = 0xF5;		//2400 b/s
      000118 75 8D F5         [24]  600 	mov	_TH1,#0xf5
                                    601 ;	uart.c:75: TR1 = 1;
                                    602 ;	assignBit
      00011B D2 8E            [12]  603 	setb	_TR1
                                    604 ;	uart.c:76: TI = 1;
                                    605 ;	assignBit
      00011D D2 99            [12]  606 	setb	_TI
                                    607 ;	uart.c:77: }
      00011F 22               [24]  608 	ret
                                    609 ;------------------------------------------------------------
                                    610 ;Allocation info for local variables in function 'uart_write'
                                    611 ;------------------------------------------------------------
                                    612 ;c                         Allocated to registers r7 
                                    613 ;------------------------------------------------------------
                                    614 ;	uart.c:78: void uart_write( uint8_t c )
                                    615 ;	-----------------------------------------
                                    616 ;	 function uart_write
                                    617 ;	-----------------------------------------
      000120                        618 _uart_write:
      000120 AF 82            [24]  619 	mov	r7,dpl
                                    620 ;	uart.c:80: while ( TI == 0 );
      000122                        621 00101$:
                                    622 ;	uart.c:81: TI = 0;
                                    623 ;	assignBit
      000122 10 99 02         [24]  624 	jbc	_TI,00114$
      000125 80 FB            [24]  625 	sjmp	00101$
      000127                        626 00114$:
                                    627 ;	uart.c:82: SBUF = c;	
      000127 8F 99            [24]  628 	mov	_SBUF,r7
                                    629 ;	uart.c:83: }
      000129 22               [24]  630 	ret
                                    631 ;------------------------------------------------------------
                                    632 ;Allocation info for local variables in function 'uart_println'
                                    633 ;------------------------------------------------------------
                                    634 ;size                      Allocated with name '_uart_println_PARM_2'
                                    635 ;s                         Allocated to registers r5 r6 r7 
                                    636 ;i                         Allocated to registers r4 
                                    637 ;------------------------------------------------------------
                                    638 ;	uart.c:84: void uart_println( uint8_t* s, uint8_t size )
                                    639 ;	-----------------------------------------
                                    640 ;	 function uart_println
                                    641 ;	-----------------------------------------
      00012A                        642 _uart_println:
      00012A AD 82            [24]  643 	mov	r5,dpl
      00012C AE 83            [24]  644 	mov	r6,dph
      00012E AF F0            [24]  645 	mov	r7,b
                                    646 ;	uart.c:86: for( uint8_t i = 0; i < size; i++ )
      000130 7C 00            [12]  647 	mov	r4,#0x00
      000132                        648 00103$:
      000132 C3               [12]  649 	clr	c
      000133 EC               [12]  650 	mov	a,r4
      000134 95 09            [12]  651 	subb	a,_uart_println_PARM_2
      000136 50 29            [24]  652 	jnc	00101$
                                    653 ;	uart.c:88: uart_write(s[i]);
      000138 EC               [12]  654 	mov	a,r4
      000139 2D               [12]  655 	add	a,r5
      00013A F9               [12]  656 	mov	r1,a
      00013B E4               [12]  657 	clr	a
      00013C 3E               [12]  658 	addc	a,r6
      00013D FA               [12]  659 	mov	r2,a
      00013E 8F 03            [24]  660 	mov	ar3,r7
      000140 89 82            [24]  661 	mov	dpl,r1
      000142 8A 83            [24]  662 	mov	dph,r2
      000144 8B F0            [24]  663 	mov	b,r3
      000146 12 05 BA         [24]  664 	lcall	__gptrget
      000149 F5 82            [12]  665 	mov	dpl,a
      00014B C0 07            [24]  666 	push	ar7
      00014D C0 06            [24]  667 	push	ar6
      00014F C0 05            [24]  668 	push	ar5
      000151 C0 04            [24]  669 	push	ar4
      000153 12 01 20         [24]  670 	lcall	_uart_write
      000156 D0 04            [24]  671 	pop	ar4
      000158 D0 05            [24]  672 	pop	ar5
      00015A D0 06            [24]  673 	pop	ar6
      00015C D0 07            [24]  674 	pop	ar7
                                    675 ;	uart.c:86: for( uint8_t i = 0; i < size; i++ )
      00015E 0C               [12]  676 	inc	r4
      00015F 80 D1            [24]  677 	sjmp	00103$
      000161                        678 00101$:
                                    679 ;	uart.c:90: uart_write('\n');
      000161 75 82 0A         [24]  680 	mov	dpl,#0x0a
                                    681 ;	uart.c:91: }
      000164 02 01 20         [24]  682 	ljmp	_uart_write
                                    683 ;------------------------------------------------------------
                                    684 ;Allocation info for local variables in function 'BTN_Init'
                                    685 ;------------------------------------------------------------
                                    686 ;pin_of_p1                 Allocated with name '_BTN_Init_PARM_2'
                                    687 ;button                    Allocated to registers r5 r6 r7 
                                    688 ;------------------------------------------------------------
                                    689 ;	uart.c:107: void BTN_Init( BTN* button, uint8_t pin_of_p1 )
                                    690 ;	-----------------------------------------
                                    691 ;	 function BTN_Init
                                    692 ;	-----------------------------------------
      000167                        693 _BTN_Init:
                                    694 ;	uart.c:109: button->pin_of_p1 = pin_of_p1;
      000167 AD 82            [24]  695 	mov	r5,dpl
      000169 AE 83            [24]  696 	mov	r6,dph
      00016B AF F0            [24]  697 	mov	r7,b
      00016D E5 35            [12]  698 	mov	a,_BTN_Init_PARM_2
      00016F 12 05 29         [24]  699 	lcall	__gptrput
                                    700 ;	uart.c:110: button->control = 0b10000000;
      000172 74 01            [12]  701 	mov	a,#0x01
      000174 2D               [12]  702 	add	a,r5
      000175 FA               [12]  703 	mov	r2,a
      000176 E4               [12]  704 	clr	a
      000177 3E               [12]  705 	addc	a,r6
      000178 FB               [12]  706 	mov	r3,a
      000179 8F 04            [24]  707 	mov	ar4,r7
      00017B 8A 82            [24]  708 	mov	dpl,r2
      00017D 8B 83            [24]  709 	mov	dph,r3
      00017F 8C F0            [24]  710 	mov	b,r4
      000181 74 80            [12]  711 	mov	a,#0x80
      000183 12 05 29         [24]  712 	lcall	__gptrput
                                    713 ;	uart.c:111: button->flag = 0x00;
      000186 74 02            [12]  714 	mov	a,#0x02
      000188 2D               [12]  715 	add	a,r5
      000189 FD               [12]  716 	mov	r5,a
      00018A E4               [12]  717 	clr	a
      00018B 3E               [12]  718 	addc	a,r6
      00018C FE               [12]  719 	mov	r6,a
      00018D 8D 82            [24]  720 	mov	dpl,r5
      00018F 8E 83            [24]  721 	mov	dph,r6
      000191 8F F0            [24]  722 	mov	b,r7
      000193 E4               [12]  723 	clr	a
                                    724 ;	uart.c:112: }
      000194 02 05 29         [24]  725 	ljmp	__gptrput
                                    726 ;------------------------------------------------------------
                                    727 ;Allocation info for local variables in function 'BTN_Update'
                                    728 ;------------------------------------------------------------
                                    729 ;dtime                     Allocated with name '_BTN_Update_PARM_2'
                                    730 ;btn                       Allocated with name '_BTN_Update_btn_65536_26'
                                    731 ;res                       Allocated to registers r4 
                                    732 ;------------------------------------------------------------
                                    733 ;	uart.c:113: uint8_t BTN_Update( BTN* btn, uint8_t dtime )
                                    734 ;	-----------------------------------------
                                    735 ;	 function BTN_Update
                                    736 ;	-----------------------------------------
      000197                        737 _BTN_Update:
      000197 85 82 0B         [24]  738 	mov	_BTN_Update_btn_65536_26,dpl
      00019A 85 83 0C         [24]  739 	mov	(_BTN_Update_btn_65536_26 + 1),dph
      00019D 85 F0 0D         [24]  740 	mov	(_BTN_Update_btn_65536_26 + 2),b
                                    741 ;	uart.c:115: uint8_t res = 0;
      0001A0 7C 00            [12]  742 	mov	r4,#0x00
                                    743 ;	uart.c:117: if( dtime > 0x80 )                  //overflow control -> khong dem
      0001A2 E5 0A            [12]  744 	mov	a,_BTN_Update_PARM_2
      0001A4 24 7F            [12]  745 	add	a,#0xff - 0x80
      0001A6 50 4F            [24]  746 	jnc	00122$
                                    747 ;	uart.c:119: if( bitread(P1, btn->pin_of_p1) ) 		//Nut nha?	
      0001A8 85 0B 82         [24]  748 	mov	dpl,_BTN_Update_btn_65536_26
      0001AB 85 0C 83         [24]  749 	mov	dph,(_BTN_Update_btn_65536_26 + 1)
      0001AE 85 0D F0         [24]  750 	mov	b,(_BTN_Update_btn_65536_26 + 2)
      0001B1 12 05 BA         [24]  751 	lcall	__gptrget
      0001B4 FB               [12]  752 	mov	r3,a
      0001B5 8B F0            [24]  753 	mov	b,r3
      0001B7 05 F0            [12]  754 	inc	b
      0001B9 E5 90            [12]  755 	mov	a,_P1
      0001BB 80 02            [24]  756 	sjmp	00188$
      0001BD                        757 00187$:
      0001BD C3               [12]  758 	clr	c
      0001BE 13               [12]  759 	rrc	a
      0001BF                        760 00188$:
      0001BF D5 F0 FB         [24]  761 	djnz	b,00187$
      0001C2 30 E0 19         [24]  762 	jnb	acc.0,00102$
                                    763 ;	uart.c:120: btn->control = 0b10000000; 														//down = 0
      0001C5 74 01            [12]  764 	mov	a,#0x01
      0001C7 25 0B            [12]  765 	add	a,_BTN_Update_btn_65536_26
      0001C9 F9               [12]  766 	mov	r1,a
      0001CA E4               [12]  767 	clr	a
      0001CB 35 0C            [12]  768 	addc	a,(_BTN_Update_btn_65536_26 + 1)
      0001CD FA               [12]  769 	mov	r2,a
      0001CE AB 0D            [24]  770 	mov	r3,(_BTN_Update_btn_65536_26 + 2)
      0001D0 89 82            [24]  771 	mov	dpl,r1
      0001D2 8A 83            [24]  772 	mov	dph,r2
      0001D4 8B F0            [24]  773 	mov	b,r3
      0001D6 74 80            [12]  774 	mov	a,#0x80
      0001D8 12 05 29         [24]  775 	lcall	__gptrput
      0001DB 02 02 B7         [24]  776 	ljmp	00123$
      0001DE                        777 00102$:
                                    778 ;	uart.c:122: btn->control = 0b11000000;														//down = 1
      0001DE 74 01            [12]  779 	mov	a,#0x01
      0001E0 25 0B            [12]  780 	add	a,_BTN_Update_btn_65536_26
      0001E2 F9               [12]  781 	mov	r1,a
      0001E3 E4               [12]  782 	clr	a
      0001E4 35 0C            [12]  783 	addc	a,(_BTN_Update_btn_65536_26 + 1)
      0001E6 FA               [12]  784 	mov	r2,a
      0001E7 AB 0D            [24]  785 	mov	r3,(_BTN_Update_btn_65536_26 + 2)
      0001E9 89 82            [24]  786 	mov	dpl,r1
      0001EB 8A 83            [24]  787 	mov	dph,r2
      0001ED 8B F0            [24]  788 	mov	b,r3
      0001EF 74 C0            [12]  789 	mov	a,#0xc0
      0001F1 12 05 29         [24]  790 	lcall	__gptrput
      0001F4 02 02 B7         [24]  791 	ljmp	00123$
      0001F7                        792 00122$:
                                    793 ;	uart.c:124: else if( 0 == bitread(P1, btn->pin_of_p1) )	//Nut an?
      0001F7 85 0B 82         [24]  794 	mov	dpl,_BTN_Update_btn_65536_26
      0001FA 85 0C 83         [24]  795 	mov	dph,(_BTN_Update_btn_65536_26 + 1)
      0001FD 85 0D F0         [24]  796 	mov	b,(_BTN_Update_btn_65536_26 + 2)
      000200 12 05 BA         [24]  797 	lcall	__gptrget
      000203 FB               [12]  798 	mov	r3,a
      000204 8B F0            [24]  799 	mov	b,r3
      000206 05 F0            [12]  800 	inc	b
      000208 E5 90            [12]  801 	mov	a,_P1
      00020A 80 02            [24]  802 	sjmp	00191$
      00020C                        803 00190$:
      00020C C3               [12]  804 	clr	c
      00020D 13               [12]  805 	rrc	a
      00020E                        806 00191$:
      00020E D5 F0 FB         [24]  807 	djnz	b,00190$
      000211 20 E0 59         [24]  808 	jb	acc.0,00119$
                                    809 ;	uart.c:127: if( btn->control == 0b10000000 )		//down real
      000214 74 01            [12]  810 	mov	a,#0x01
      000216 25 0B            [12]  811 	add	a,_BTN_Update_btn_65536_26
      000218 F9               [12]  812 	mov	r1,a
      000219 E4               [12]  813 	clr	a
      00021A 35 0C            [12]  814 	addc	a,(_BTN_Update_btn_65536_26 + 1)
      00021C FA               [12]  815 	mov	r2,a
      00021D AB 0D            [24]  816 	mov	r3,(_BTN_Update_btn_65536_26 + 2)
      00021F 89 82            [24]  817 	mov	dpl,r1
      000221 8A 83            [24]  818 	mov	dph,r2
      000223 8B F0            [24]  819 	mov	b,r3
      000225 12 05 BA         [24]  820 	lcall	__gptrget
      000228 F8               [12]  821 	mov	r0,a
      000229 B8 80 1C         [24]  822 	cjne	r0,#0x80,00109$
                                    823 ;	uart.c:129: btn->control = 0b11010100;			//trang thai dang nhan, dem == 20ms
      00022C 89 82            [24]  824 	mov	dpl,r1
      00022E 8A 83            [24]  825 	mov	dph,r2
      000230 8B F0            [24]  826 	mov	b,r3
      000232 74 D4            [12]  827 	mov	a,#0xd4
      000234 12 05 29         [24]  828 	lcall	__gptrput
                                    829 ;	uart.c:130: res = BTN_IsDown(btn);
      000237 85 0B 82         [24]  830 	mov	dpl,_BTN_Update_btn_65536_26
      00023A 85 0C 83         [24]  831 	mov	dph,(_BTN_Update_btn_65536_26 + 1)
      00023D 85 0D F0         [24]  832 	mov	b,(_BTN_Update_btn_65536_26 + 2)
      000240 12 03 8B         [24]  833 	lcall	_BTN_IsDown
      000243 AC 82            [24]  834 	mov	r4,dpl
      000245 02 02 B7         [24]  835 	ljmp	00123$
      000248                        836 00109$:
                                    837 ;	uart.c:132: else if( btn->control > 0b11000000 )	//down noise
      000248 E8               [12]  838 	mov	a,r0
      000249 24 3F            [12]  839 	add	a,#0xff - 0xc0
      00024B 50 6A            [24]  840 	jnc	00123$
                                    841 ;	uart.c:134: btn->control -= dtime;						//giam thoi gian cho de thuc hien nha nut
      00024D E8               [12]  842 	mov	a,r0
      00024E C3               [12]  843 	clr	c
      00024F 95 0A            [12]  844 	subb	a,_BTN_Update_PARM_2
      000251 F8               [12]  845 	mov	r0,a
      000252 89 82            [24]  846 	mov	dpl,r1
      000254 8A 83            [24]  847 	mov	dph,r2
      000256 8B F0            [24]  848 	mov	b,r3
      000258 12 05 29         [24]  849 	lcall	__gptrput
                                    850 ;	uart.c:135: if( btn->control < 0b11000000 ) btn->control = 0b11000000;    //san sang de nha
      00025B B8 C0 00         [24]  851 	cjne	r0,#0xc0,00196$
      00025E                        852 00196$:
      00025E 50 57            [24]  853 	jnc	00123$
      000260 89 82            [24]  854 	mov	dpl,r1
      000262 8A 83            [24]  855 	mov	dph,r2
      000264 8B F0            [24]  856 	mov	b,r3
      000266 74 C0            [12]  857 	mov	a,#0xc0
      000268 12 05 29         [24]  858 	lcall	__gptrput
      00026B 80 4A            [24]  859 	sjmp	00123$
      00026D                        860 00119$:
                                    861 ;	uart.c:140: res = 0;
      00026D 7C 00            [12]  862 	mov	r4,#0x00
                                    863 ;	uart.c:141: if( btn->control == 0b11000000 )     		//up real
      00026F 74 01            [12]  864 	mov	a,#0x01
      000271 25 0B            [12]  865 	add	a,_BTN_Update_btn_65536_26
      000273 F9               [12]  866 	mov	r1,a
      000274 E4               [12]  867 	clr	a
      000275 35 0C            [12]  868 	addc	a,(_BTN_Update_btn_65536_26 + 1)
      000277 FA               [12]  869 	mov	r2,a
      000278 AB 0D            [24]  870 	mov	r3,(_BTN_Update_btn_65536_26 + 2)
      00027A 89 82            [24]  871 	mov	dpl,r1
      00027C 8A 83            [24]  872 	mov	dph,r2
      00027E 8B F0            [24]  873 	mov	b,r3
      000280 12 05 BA         [24]  874 	lcall	__gptrget
      000283 F8               [12]  875 	mov	r0,a
      000284 B8 C0 0D         [24]  876 	cjne	r0,#0xc0,00116$
                                    877 ;	uart.c:142: btn->control = 0b10010100;       //trang thai khong nhan, dem == 20ms
      000287 89 82            [24]  878 	mov	dpl,r1
      000289 8A 83            [24]  879 	mov	dph,r2
      00028B 8B F0            [24]  880 	mov	b,r3
      00028D 74 94            [12]  881 	mov	a,#0x94
      00028F 12 05 29         [24]  882 	lcall	__gptrput
      000292 80 23            [24]  883 	sjmp	00123$
      000294                        884 00116$:
                                    885 ;	uart.c:143: else if( btn->control > 0b10000000 )//up noise
      000294 E8               [12]  886 	mov	a,r0
      000295 24 7F            [12]  887 	add	a,#0xff - 0x80
      000297 50 1E            [24]  888 	jnc	00123$
                                    889 ;	uart.c:145: btn->control -= dtime;						//giam thoi gian cho de thuc hien an nut
      000299 E8               [12]  890 	mov	a,r0
      00029A C3               [12]  891 	clr	c
      00029B 95 0A            [12]  892 	subb	a,_BTN_Update_PARM_2
      00029D F8               [12]  893 	mov	r0,a
      00029E 89 82            [24]  894 	mov	dpl,r1
      0002A0 8A 83            [24]  895 	mov	dph,r2
      0002A2 8B F0            [24]  896 	mov	b,r3
      0002A4 12 05 29         [24]  897 	lcall	__gptrput
                                    898 ;	uart.c:146: if( btn->control < 0b10000000 ) btn->control = 0b10000000;    //san sang de an
      0002A7 B8 80 00         [24]  899 	cjne	r0,#0x80,00201$
      0002AA                        900 00201$:
      0002AA 50 0B            [24]  901 	jnc	00123$
      0002AC 89 82            [24]  902 	mov	dpl,r1
      0002AE 8A 83            [24]  903 	mov	dph,r2
      0002B0 8B F0            [24]  904 	mov	b,r3
      0002B2 74 80            [12]  905 	mov	a,#0x80
      0002B4 12 05 29         [24]  906 	lcall	__gptrput
      0002B7                        907 00123$:
                                    908 ;	uart.c:151: bitwrite( btn->flag, 1, bitread(btn->flag, 2) );           	//old  = last;
      0002B7 74 02            [12]  909 	mov	a,#0x02
      0002B9 25 0B            [12]  910 	add	a,_BTN_Update_btn_65536_26
      0002BB F9               [12]  911 	mov	r1,a
      0002BC E4               [12]  912 	clr	a
      0002BD 35 0C            [12]  913 	addc	a,(_BTN_Update_btn_65536_26 + 1)
      0002BF FA               [12]  914 	mov	r2,a
      0002C0 AB 0D            [24]  915 	mov	r3,(_BTN_Update_btn_65536_26 + 2)
      0002C2 89 82            [24]  916 	mov	dpl,r1
      0002C4 8A 83            [24]  917 	mov	dph,r2
      0002C6 8B F0            [24]  918 	mov	b,r3
      0002C8 12 05 BA         [24]  919 	lcall	__gptrget
      0002CB F8               [12]  920 	mov	r0,a
      0002CC 03               [12]  921 	rr	a
      0002CD 03               [12]  922 	rr	a
      0002CE 54 01            [12]  923 	anl	a,#0x01
      0002D0 FF               [12]  924 	mov	r7,a
      0002D1 60 0F            [24]  925 	jz	00130$
      0002D3 74 02            [12]  926 	mov	a,#0x02
      0002D5 48               [12]  927 	orl	a,r0
      0002D6 FF               [12]  928 	mov	r7,a
      0002D7 89 82            [24]  929 	mov	dpl,r1
      0002D9 8A 83            [24]  930 	mov	dph,r2
      0002DB 8B F0            [24]  931 	mov	b,r3
      0002DD 12 05 29         [24]  932 	lcall	__gptrput
      0002E0 80 0D            [24]  933 	sjmp	00131$
      0002E2                        934 00130$:
      0002E2 53 00 FD         [24]  935 	anl	ar0,#0xfd
      0002E5 89 82            [24]  936 	mov	dpl,r1
      0002E7 8A 83            [24]  937 	mov	dph,r2
      0002E9 8B F0            [24]  938 	mov	b,r3
      0002EB E8               [12]  939 	mov	a,r0
      0002EC 12 05 29         [24]  940 	lcall	__gptrput
      0002EF                        941 00131$:
                                    942 ;	uart.c:152: bitwrite( btn->flag, 2, BTN_IsDown( btn ) );                //last = IsDown();
      0002EF 85 0B 82         [24]  943 	mov	dpl,_BTN_Update_btn_65536_26
      0002F2 85 0C 83         [24]  944 	mov	dph,(_BTN_Update_btn_65536_26 + 1)
      0002F5 85 0D F0         [24]  945 	mov	b,(_BTN_Update_btn_65536_26 + 2)
      0002F8 C0 04            [24]  946 	push	ar4
      0002FA 12 03 8B         [24]  947 	lcall	_BTN_IsDown
      0002FD E5 82            [12]  948 	mov	a,dpl
      0002FF D0 04            [24]  949 	pop	ar4
      000301 60 24            [24]  950 	jz	00132$
      000303 74 02            [12]  951 	mov	a,#0x02
      000305 25 0B            [12]  952 	add	a,_BTN_Update_btn_65536_26
      000307 FD               [12]  953 	mov	r5,a
      000308 E4               [12]  954 	clr	a
      000309 35 0C            [12]  955 	addc	a,(_BTN_Update_btn_65536_26 + 1)
      00030B FE               [12]  956 	mov	r6,a
      00030C AF 0D            [24]  957 	mov	r7,(_BTN_Update_btn_65536_26 + 2)
      00030E 8D 82            [24]  958 	mov	dpl,r5
      000310 8E 83            [24]  959 	mov	dph,r6
      000312 8F F0            [24]  960 	mov	b,r7
      000314 12 05 BA         [24]  961 	lcall	__gptrget
      000317 FB               [12]  962 	mov	r3,a
      000318 43 03 04         [24]  963 	orl	ar3,#0x04
      00031B 8D 82            [24]  964 	mov	dpl,r5
      00031D 8E 83            [24]  965 	mov	dph,r6
      00031F 8F F0            [24]  966 	mov	b,r7
      000321 EB               [12]  967 	mov	a,r3
      000322 12 05 29         [24]  968 	lcall	__gptrput
      000325 80 22            [24]  969 	sjmp	00133$
      000327                        970 00132$:
      000327 74 02            [12]  971 	mov	a,#0x02
      000329 25 0B            [12]  972 	add	a,_BTN_Update_btn_65536_26
      00032B FD               [12]  973 	mov	r5,a
      00032C E4               [12]  974 	clr	a
      00032D 35 0C            [12]  975 	addc	a,(_BTN_Update_btn_65536_26 + 1)
      00032F FE               [12]  976 	mov	r6,a
      000330 AF 0D            [24]  977 	mov	r7,(_BTN_Update_btn_65536_26 + 2)
      000332 8D 82            [24]  978 	mov	dpl,r5
      000334 8E 83            [24]  979 	mov	dph,r6
      000336 8F F0            [24]  980 	mov	b,r7
      000338 12 05 BA         [24]  981 	lcall	__gptrget
      00033B FB               [12]  982 	mov	r3,a
      00033C 53 03 FB         [24]  983 	anl	ar3,#0xfb
      00033F 8D 82            [24]  984 	mov	dpl,r5
      000341 8E 83            [24]  985 	mov	dph,r6
      000343 8F F0            [24]  986 	mov	b,r7
      000345 EB               [12]  987 	mov	a,r3
      000346 12 05 29         [24]  988 	lcall	__gptrput
      000349                        989 00133$:
                                    990 ;	uart.c:153: if( bitread( btn->flag, 1 ) && !bitread( btn->flag, 2 ) )  	//old:down && last:up
      000349 74 02            [12]  991 	mov	a,#0x02
      00034B 25 0B            [12]  992 	add	a,_BTN_Update_btn_65536_26
      00034D FD               [12]  993 	mov	r5,a
      00034E E4               [12]  994 	clr	a
      00034F 35 0C            [12]  995 	addc	a,(_BTN_Update_btn_65536_26 + 1)
      000351 FE               [12]  996 	mov	r6,a
      000352 AF 0D            [24]  997 	mov	r7,(_BTN_Update_btn_65536_26 + 2)
      000354 8D 82            [24]  998 	mov	dpl,r5
      000356 8E 83            [24]  999 	mov	dph,r6
      000358 8F F0            [24] 1000 	mov	b,r7
      00035A 12 05 BA         [24] 1001 	lcall	__gptrget
      00035D FB               [12] 1002 	mov	r3,a
      00035E 03               [12] 1003 	rr	a
      00035F 54 01            [12] 1004 	anl	a,#0x01
      000361 FA               [12] 1005 	mov	r2,a
      000362 60 17            [24] 1006 	jz	00125$
      000364 EB               [12] 1007 	mov	a,r3
      000365 03               [12] 1008 	rr	a
      000366 03               [12] 1009 	rr	a
      000367 54 01            [12] 1010 	anl	a,#0x01
      000369 FA               [12] 1011 	mov	r2,a
      00036A 70 0F            [24] 1012 	jnz	00125$
                                   1013 ;	uart.c:154: bitset( btn->flag, 0 );                          					//isPress
      00036C 74 01            [12] 1014 	mov	a,#0x01
      00036E 4B               [12] 1015 	orl	a,r3
      00036F FA               [12] 1016 	mov	r2,a
      000370 8D 82            [24] 1017 	mov	dpl,r5
      000372 8E 83            [24] 1018 	mov	dph,r6
      000374 8F F0            [24] 1019 	mov	b,r7
      000376 12 05 29         [24] 1020 	lcall	__gptrput
      000379 80 0D            [24] 1021 	sjmp	00126$
      00037B                       1022 00125$:
                                   1023 ;	uart.c:156: bitclear( btn->flag, 0 );                        					//isPress yet
      00037B 53 03 FE         [24] 1024 	anl	ar3,#0xfe
      00037E 8D 82            [24] 1025 	mov	dpl,r5
      000380 8E 83            [24] 1026 	mov	dph,r6
      000382 8F F0            [24] 1027 	mov	b,r7
      000384 EB               [12] 1028 	mov	a,r3
      000385 12 05 29         [24] 1029 	lcall	__gptrput
      000388                       1030 00126$:
                                   1031 ;	uart.c:157: return res;
      000388 8C 82            [24] 1032 	mov	dpl,r4
                                   1033 ;	uart.c:158: }
      00038A 22               [24] 1034 	ret
                                   1035 ;------------------------------------------------------------
                                   1036 ;Allocation info for local variables in function 'BTN_IsDown'
                                   1037 ;------------------------------------------------------------
                                   1038 ;btn                       Allocated to registers r5 r6 r7 
                                   1039 ;------------------------------------------------------------
                                   1040 ;	uart.c:159: uint8_t BTN_IsDown( BTN* btn )  { return bitread( btn->control, 6 ); }
                                   1041 ;	-----------------------------------------
                                   1042 ;	 function BTN_IsDown
                                   1043 ;	-----------------------------------------
      00038B                       1044 _BTN_IsDown:
      00038B AD 82            [24] 1045 	mov	r5,dpl
      00038D AE 83            [24] 1046 	mov	r6,dph
      00038F AF F0            [24] 1047 	mov	r7,b
      000391 0D               [12] 1048 	inc	r5
      000392 BD 00 01         [24] 1049 	cjne	r5,#0x00,00103$
      000395 0E               [12] 1050 	inc	r6
      000396                       1051 00103$:
      000396 8D 82            [24] 1052 	mov	dpl,r5
      000398 8E 83            [24] 1053 	mov	dph,r6
      00039A 8F F0            [24] 1054 	mov	b,r7
      00039C 12 05 BA         [24] 1055 	lcall	__gptrget
      00039F 23               [12] 1056 	rl	a
      0003A0 23               [12] 1057 	rl	a
      0003A1 54 01            [12] 1058 	anl	a,#0x01
      0003A3 F5 82            [12] 1059 	mov	dpl,a
      0003A5 22               [24] 1060 	ret
                                   1061 ;------------------------------------------------------------
                                   1062 ;Allocation info for local variables in function 'BTN_IsPress'
                                   1063 ;------------------------------------------------------------
                                   1064 ;btn                       Allocated to registers r5 r6 r7 
                                   1065 ;------------------------------------------------------------
                                   1066 ;	uart.c:160: uint8_t BTN_IsPress( BTN* btn ) { return bitread( btn->flag, 0 ); }
                                   1067 ;	-----------------------------------------
                                   1068 ;	 function BTN_IsPress
                                   1069 ;	-----------------------------------------
      0003A6                       1070 _BTN_IsPress:
      0003A6 AD 82            [24] 1071 	mov	r5,dpl
      0003A8 AE 83            [24] 1072 	mov	r6,dph
      0003AA AF F0            [24] 1073 	mov	r7,b
      0003AC 74 02            [12] 1074 	mov	a,#0x02
      0003AE 2D               [12] 1075 	add	a,r5
      0003AF FD               [12] 1076 	mov	r5,a
      0003B0 E4               [12] 1077 	clr	a
      0003B1 3E               [12] 1078 	addc	a,r6
      0003B2 FE               [12] 1079 	mov	r6,a
      0003B3 8D 82            [24] 1080 	mov	dpl,r5
      0003B5 8E 83            [24] 1081 	mov	dph,r6
      0003B7 8F F0            [24] 1082 	mov	b,r7
      0003B9 12 05 BA         [24] 1083 	lcall	__gptrget
      0003BC 54 01            [12] 1084 	anl	a,#0x01
      0003BE F5 82            [12] 1085 	mov	dpl,a
      0003C0 22               [24] 1086 	ret
                                   1087 ;------------------------------------------------------------
                                   1088 ;Allocation info for local variables in function 'main'
                                   1089 ;------------------------------------------------------------
                                   1090 ;keys                      Allocated with name '_main_keys_65537_40'
                                   1091 ;i                         Allocated to registers r7 
                                   1092 ;i                         Allocated to registers r7 
                                   1093 ;j                         Allocated to registers r6 
                                   1094 ;i                         Allocated to registers r7 
                                   1095 ;------------------------------------------------------------
                                   1096 ;	uart.c:167: void main(void)
                                   1097 ;	-----------------------------------------
                                   1098 ;	 function main
                                   1099 ;	-----------------------------------------
      0003C1                       1100 _main:
                                   1101 ;	uart.c:169: timer0_init();
      0003C1 12 00 A2         [24] 1102 	lcall	_timer0_init
                                   1103 ;	uart.c:170: timer2_38khz_init();
      0003C4 12 01 02         [24] 1104 	lcall	_timer2_38khz_init
                                   1105 ;	uart.c:171: uart_init();
      0003C7 12 01 11         [24] 1106 	lcall	_uart_init
                                   1107 ;	uart.c:174: for( uint8_t i = 0; i < 10; i++ )
      0003CA 7F 00            [12] 1108 	mov	r7,#0x00
      0003CC                       1109 00126$:
      0003CC BF 0A 00         [24] 1110 	cjne	r7,#0x0a,00232$
      0003CF                       1111 00232$:
      0003CF 50 23            [24] 1112 	jnc	00123$
                                   1113 ;	uart.c:176: BTN_Init(&keys[i], (i%2)+2);
      0003D1 EF               [12] 1114 	mov	a,r7
      0003D2 75 F0 03         [24] 1115 	mov	b,#0x03
      0003D5 A4               [48] 1116 	mul	ab
      0003D6 24 17            [12] 1117 	add	a,#_main_keys_65537_40
      0003D8 FE               [12] 1118 	mov	r6,a
      0003D9 7D 00            [12] 1119 	mov	r5,#0x00
      0003DB 7C 40            [12] 1120 	mov	r4,#0x40
      0003DD 74 01            [12] 1121 	mov	a,#0x01
      0003DF 5F               [12] 1122 	anl	a,r7
      0003E0 24 02            [12] 1123 	add	a,#0x02
      0003E2 F5 35            [12] 1124 	mov	_BTN_Init_PARM_2,a
      0003E4 8E 82            [24] 1125 	mov	dpl,r6
      0003E6 8D 83            [24] 1126 	mov	dph,r5
      0003E8 8C F0            [24] 1127 	mov	b,r4
      0003EA C0 07            [24] 1128 	push	ar7
      0003EC 12 01 67         [24] 1129 	lcall	_BTN_Init
      0003EF D0 07            [24] 1130 	pop	ar7
                                   1131 ;	uart.c:174: for( uint8_t i = 0; i < 10; i++ )
      0003F1 0F               [12] 1132 	inc	r7
                                   1133 ;	uart.c:179: while(1)
      0003F2 80 D8            [24] 1134 	sjmp	00126$
      0003F4                       1135 00123$:
                                   1136 ;	uart.c:181: timer0_update();
      0003F4 12 00 B3         [24] 1137 	lcall	_timer0_update
                                   1138 ;	uart.c:182: count_100 += dtime;
      0003F7 AE 08            [24] 1139 	mov	r6,_dtime
      0003F9 7F 00            [12] 1140 	mov	r7,#0x00
      0003FB AC 0E            [24] 1141 	mov	r4,_count_100
      0003FD AD 0F            [24] 1142 	mov	r5,(_count_100 + 1)
      0003FF EE               [12] 1143 	mov	a,r6
      000400 2C               [12] 1144 	add	a,r4
      000401 FC               [12] 1145 	mov	r4,a
      000402 EF               [12] 1146 	mov	a,r7
      000403 3D               [12] 1147 	addc	a,r5
      000404 FD               [12] 1148 	mov	r5,a
      000405 8C 0E            [24] 1149 	mov	_count_100,r4
      000407 8D 0F            [24] 1150 	mov	(_count_100 + 1),r5
                                   1151 ;	uart.c:183: bitset(P3, 2);
      000409 43 B0 04         [24] 1152 	orl	_P3,#0x04
                                   1153 ;	uart.c:185: for( uint8_t i = 0; i < 5; i++ )
      00040C 7F 00            [12] 1154 	mov	r7,#0x00
      00040E                       1155 00132$:
      00040E BF 05 00         [24] 1156 	cjne	r7,#0x05,00234$
      000411                       1157 00234$:
      000411 50 61            [24] 1158 	jnc	00103$
                                   1159 ;	uart.c:187: bitclear(P2, cols[i]);
      000413 EF               [12] 1160 	mov	a,r7
      000414 24 10            [12] 1161 	add	a,#_cols
      000416 F9               [12] 1162 	mov	r1,a
      000417 87 06            [24] 1163 	mov	ar6,@r1
      000419 8E F0            [24] 1164 	mov	b,r6
      00041B 05 F0            [12] 1165 	inc	b
      00041D 74 01            [12] 1166 	mov	a,#0x01
      00041F 80 02            [24] 1167 	sjmp	00238$
      000421                       1168 00236$:
      000421 25 E0            [12] 1169 	add	a,acc
      000423                       1170 00238$:
      000423 D5 F0 FB         [24] 1171 	djnz	b,00236$
      000426 F4               [12] 1172 	cpl	a
      000427 52 A0            [12] 1173 	anl	_P2,a
                                   1174 ;	uart.c:188: for( uint8_t j = 0; j < 2; j++ )
      000429 7E 00            [12] 1175 	mov	r6,#0x00
      00042B                       1176 00129$:
      00042B BE 02 00         [24] 1177 	cjne	r6,#0x02,00239$
      00042E                       1178 00239$:
      00042E 50 2B            [24] 1179 	jnc	00102$
                                   1180 ;	uart.c:190: BTN_Update(&keys[i*2 + j], dtime);
      000430 8F 05            [24] 1181 	mov	ar5,r7
      000432 ED               [12] 1182 	mov	a,r5
      000433 2D               [12] 1183 	add	a,r5
      000434 FD               [12] 1184 	mov	r5,a
      000435 8E 04            [24] 1185 	mov	ar4,r6
      000437 EC               [12] 1186 	mov	a,r4
      000438 2D               [12] 1187 	add	a,r5
      000439 75 F0 03         [24] 1188 	mov	b,#0x03
      00043C A4               [48] 1189 	mul	ab
      00043D 24 17            [12] 1190 	add	a,#_main_keys_65537_40
      00043F FD               [12] 1191 	mov	r5,a
      000440 7C 00            [12] 1192 	mov	r4,#0x00
      000442 7B 40            [12] 1193 	mov	r3,#0x40
      000444 85 08 0A         [24] 1194 	mov	_BTN_Update_PARM_2,_dtime
      000447 8D 82            [24] 1195 	mov	dpl,r5
      000449 8C 83            [24] 1196 	mov	dph,r4
      00044B 8B F0            [24] 1197 	mov	b,r3
      00044D C0 07            [24] 1198 	push	ar7
      00044F C0 06            [24] 1199 	push	ar6
      000451 12 01 97         [24] 1200 	lcall	_BTN_Update
      000454 D0 06            [24] 1201 	pop	ar6
      000456 D0 07            [24] 1202 	pop	ar7
                                   1203 ;	uart.c:188: for( uint8_t j = 0; j < 2; j++ )
      000458 0E               [12] 1204 	inc	r6
      000459 80 D0            [24] 1205 	sjmp	00129$
      00045B                       1206 00102$:
                                   1207 ;	uart.c:192: bitset(P2, cols[i]);
      00045B EF               [12] 1208 	mov	a,r7
      00045C 24 10            [12] 1209 	add	a,#_cols
      00045E F9               [12] 1210 	mov	r1,a
      00045F 87 06            [24] 1211 	mov	ar6,@r1
      000461 8E F0            [24] 1212 	mov	b,r6
      000463 05 F0            [12] 1213 	inc	b
      000465 74 01            [12] 1214 	mov	a,#0x01
      000467 80 02            [24] 1215 	sjmp	00243$
      000469                       1216 00241$:
      000469 25 E0            [12] 1217 	add	a,acc
      00046B                       1218 00243$:
      00046B D5 F0 FB         [24] 1219 	djnz	b,00241$
      00046E FE               [12] 1220 	mov	r6,a
      00046F 42 A0            [12] 1221 	orl	_P2,a
                                   1222 ;	uart.c:185: for( uint8_t i = 0; i < 5; i++ )
      000471 0F               [12] 1223 	inc	r7
      000472 80 9A            [24] 1224 	sjmp	00132$
      000474                       1225 00103$:
                                   1226 ;	uart.c:196: for( uint8_t i = 0; i < 10; i++ )
      000474 7F 00            [12] 1227 	mov	r7,#0x00
      000476 7E 00            [12] 1228 	mov	r6,#0x00
      000478                       1229 00135$:
      000478 BE 0A 00         [24] 1230 	cjne	r6,#0x0a,00244$
      00047B                       1231 00244$:
      00047B 50 5E            [24] 1232 	jnc	00113$
                                   1233 ;	uart.c:198: if( BTN_IsDown(&keys[i]) )
      00047D EE               [12] 1234 	mov	a,r6
      00047E 75 F0 03         [24] 1235 	mov	b,#0x03
      000481 A4               [48] 1236 	mul	ab
      000482 24 17            [12] 1237 	add	a,#_main_keys_65537_40
      000484 FD               [12] 1238 	mov	r5,a
      000485 7C 00            [12] 1239 	mov	r4,#0x00
      000487 7B 40            [12] 1240 	mov	r3,#0x40
      000489 8D 82            [24] 1241 	mov	dpl,r5
      00048B 8C 83            [24] 1242 	mov	dph,r4
      00048D 8B F0            [24] 1243 	mov	b,r3
      00048F C0 07            [24] 1244 	push	ar7
      000491 C0 06            [24] 1245 	push	ar6
      000493 12 03 8B         [24] 1246 	lcall	_BTN_IsDown
      000496 E5 82            [12] 1247 	mov	a,dpl
      000498 D0 06            [24] 1248 	pop	ar6
      00049A D0 07            [24] 1249 	pop	ar7
      00049C 60 38            [24] 1250 	jz	00136$
                                   1251 ;	uart.c:200: bitclear(P3, 2);
      00049E 53 B0 FB         [24] 1252 	anl	_P3,#0xfb
                                   1253 ;	uart.c:201: if( i >= 7 )
      0004A1 BE 07 00         [24] 1254 	cjne	r6,#0x07,00247$
      0004A4                       1255 00247$:
      0004A4 40 1E            [24] 1256 	jc	00110$
                                   1257 ;	uart.c:203: switch (i)
      0004A6 BF 07 02         [24] 1258 	cjne	r7,#0x07,00249$
      0004A9 80 0A            [24] 1259 	sjmp	00104$
      0004AB                       1260 00249$:
      0004AB BF 08 02         [24] 1261 	cjne	r7,#0x08,00250$
      0004AE 80 0A            [24] 1262 	sjmp	00105$
      0004B0                       1263 00250$:
                                   1264 ;	uart.c:205: case 7: cmd = 0b01111110;break;
      0004B0 BF 09 28         [24] 1265 	cjne	r7,#0x09,00113$
      0004B3 80 0A            [24] 1266 	sjmp	00106$
      0004B5                       1267 00104$:
      0004B5 75 15 7E         [24] 1268 	mov	_cmd,#0x7e
                                   1269 ;	uart.c:206: case 8: cmd = 0b00111100;break;
      0004B8 80 21            [24] 1270 	sjmp	00113$
      0004BA                       1271 00105$:
      0004BA 75 15 3C         [24] 1272 	mov	_cmd,#0x3c
                                   1273 ;	uart.c:207: case 9: cmd = 0b01010101;break;
      0004BD 80 1C            [24] 1274 	sjmp	00113$
      0004BF                       1275 00106$:
      0004BF 75 15 55         [24] 1276 	mov	_cmd,#0x55
                                   1277 ;	uart.c:210: break;
      0004C2 80 17            [24] 1278 	sjmp	00113$
      0004C4                       1279 00110$:
                                   1280 ;	uart.c:212: cmd |= (1 << i);
      0004C4 8E 05            [24] 1281 	mov	ar5,r6
      0004C6 8D F0            [24] 1282 	mov	b,r5
      0004C8 05 F0            [12] 1283 	inc	b
      0004CA 74 01            [12] 1284 	mov	a,#0x01
      0004CC 80 02            [24] 1285 	sjmp	00254$
      0004CE                       1286 00252$:
      0004CE 25 E0            [12] 1287 	add	a,acc
      0004D0                       1288 00254$:
      0004D0 D5 F0 FB         [24] 1289 	djnz	b,00252$
      0004D3 FD               [12] 1290 	mov	r5,a
      0004D4 42 15            [12] 1291 	orl	_cmd,a
      0004D6                       1292 00136$:
                                   1293 ;	uart.c:196: for( uint8_t i = 0; i < 10; i++ )
      0004D6 0E               [12] 1294 	inc	r6
      0004D7 8E 07            [24] 1295 	mov	ar7,r6
      0004D9 80 9D            [24] 1296 	sjmp	00135$
      0004DB                       1297 00113$:
                                   1298 ;	uart.c:215: if( count_100 >= 60 )
      0004DB AE 0E            [24] 1299 	mov	r6,_count_100
      0004DD AF 0F            [24] 1300 	mov	r7,(_count_100 + 1)
      0004DF C3               [12] 1301 	clr	c
      0004E0 EE               [12] 1302 	mov	a,r6
      0004E1 94 3C            [12] 1303 	subb	a,#0x3c
      0004E3 EF               [12] 1304 	mov	a,r7
      0004E4 94 00            [12] 1305 	subb	a,#0x00
      0004E6 50 03            [24] 1306 	jnc	00255$
      0004E8 02 03 F4         [24] 1307 	ljmp	00123$
      0004EB                       1308 00255$:
                                   1309 ;	uart.c:217: count_100 %= 60;
      0004EB 75 35 3C         [24] 1310 	mov	__moduint_PARM_2,#0x3c
      0004EE 75 36 00         [24] 1311 	mov	(__moduint_PARM_2 + 1),#0x00
      0004F1 8E 82            [24] 1312 	mov	dpl,r6
      0004F3 8F 83            [24] 1313 	mov	dph,r7
      0004F5 12 05 44         [24] 1314 	lcall	__moduint
      0004F8 AE 82            [24] 1315 	mov	r6,dpl
      0004FA AF 83            [24] 1316 	mov	r7,dph
      0004FC 8E 0E            [24] 1317 	mov	_count_100,r6
      0004FE 8F 0F            [24] 1318 	mov	(_count_100 + 1),r7
                                   1319 ;	uart.c:218: if( cmd_end && cmd == 0 )
      000500 E5 16            [12] 1320 	mov	a,_cmd_end
      000502 60 0F            [24] 1321 	jz	00117$
      000504 E5 15            [12] 1322 	mov	a,_cmd
                                   1323 ;	uart.c:220: uart_write( 0x00 );
      000506 70 0B            [24] 1324 	jnz	00117$
      000508 F5 82            [12] 1325 	mov	dpl,a
      00050A 12 01 20         [24] 1326 	lcall	_uart_write
                                   1327 ;	uart.c:221: cmd_end = 0;
      00050D 75 16 00         [24] 1328 	mov	_cmd_end,#0x00
      000510 02 03 F4         [24] 1329 	ljmp	00123$
      000513                       1330 00117$:
                                   1331 ;	uart.c:223: else if( cmd != 0 )
      000513 E5 15            [12] 1332 	mov	a,_cmd
      000515 70 03            [24] 1333 	jnz	00258$
      000517 02 03 F4         [24] 1334 	ljmp	00123$
      00051A                       1335 00258$:
                                   1336 ;	uart.c:225: uart_write( cmd );
      00051A 85 15 82         [24] 1337 	mov	dpl,_cmd
      00051D 12 01 20         [24] 1338 	lcall	_uart_write
                                   1339 ;	uart.c:226: cmd = 0;
      000520 75 15 00         [24] 1340 	mov	_cmd,#0x00
                                   1341 ;	uart.c:227: cmd_end = 1;
      000523 75 16 01         [24] 1342 	mov	_cmd_end,#0x01
                                   1343 ;	uart.c:231: }
      000526 02 03 F4         [24] 1344 	ljmp	00123$
                                   1345 	.area CSEG    (CODE)
                                   1346 	.area CONST   (CODE)
                                   1347 	.area XINIT   (CODE)
                                   1348 	.area CABS    (ABS,CODE)
