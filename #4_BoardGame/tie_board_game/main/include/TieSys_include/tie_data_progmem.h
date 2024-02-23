#ifndef _TIE_DATA_PROGMEM_
#define _TIE_DATA_PROGMEM_

//////////////////////////////////////////////////////////////////////////////////
//Font
const unsigned char prog_font[27][8] PROGMEM = {
    {0x78, 0xfc, 0xcc, 0xcc, 0xfc, 0xfc, 0xcc, 0xcc},
    {0xf8, 0xcc, 0xc4, 0xf8, 0xcc, 0xc4, 0xfc, 0xfc},
    {0x78, 0xfc, 0xc4, 0xc0, 0xc0, 0xc4, 0xfc, 0x78},
    {0xf8, 0xfc, 0xcc, 0xcc, 0xcc, 0xcc, 0xfc, 0xf8},
    {0xfc, 0xfc, 0xc0, 0xf8, 0xf8, 0xc0, 0xfc, 0xfc},
    {0xfc, 0xfc, 0xc0, 0xf8, 0xf8, 0xc0, 0xc0, 0xc0},
    {0x78, 0xfc, 0xc4, 0xc0, 0xcc, 0xc4, 0xfc, 0x78},
    {0xcc, 0xcc, 0xcc, 0xfc, 0xfc, 0xcc, 0xcc, 0xcc},
    {0xfc, 0xfc, 0x30, 0x30, 0x30, 0x30, 0xfc, 0xfc},
    {0xfc, 0xfc, 0x18, 0x18, 0x18, 0x98, 0xf8, 0x70},
    {0xcc, 0xd8, 0xf0, 0xe0, 0xf0, 0xf8, 0xdc, 0xcc},
    {0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xfc},
    {0xc4, 0xec, 0xd4, 0xc4, 0xc4, 0xc4, 0xc4, 0xc4},
    {0xc4, 0xe4, 0xf4, 0xd4, 0xdc, 0xcc, 0xc4, 0xc4},
    {0x78, 0xfc, 0xcc, 0xcc, 0xcc, 0xcc, 0xfc, 0x78},
    {0xf8, 0xc4, 0xc4, 0xfc, 0xf8, 0xc0, 0xc0, 0xc0},
    {0x78, 0xfc, 0xcc, 0xcc, 0xec, 0xec, 0xf8, 0x7c},
    {0xf8, 0xcc, 0xc4, 0xcc, 0xf8, 0xf8, 0xdc, 0xcc},
    {0x78, 0xcc, 0xc4, 0x60, 0x38, 0x0c, 0xcc, 0x78},
    {0xfc, 0xfc, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30},
    {0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xfc, 0x78},
    {0x84, 0x84, 0x84, 0xc4, 0xcc, 0x68, 0x78, 0x30},
    {0x84, 0x84, 0x84, 0x94, 0xd4, 0xd4, 0x78, 0x28},
    {0x84, 0xcc, 0x68, 0x30, 0x78, 0xec, 0xcc, 0xcc},
    {0x84, 0xcc, 0x78, 0x30, 0x30, 0x60, 0xe0, 0xc0},
    {0xfc, 0xfc, 0x1c, 0x38, 0x70, 0xe0, 0xfc, 0xfc},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};
const unsigned char prog_number[10][8] PROGMEM = {
    {0x78, 0xfc, 0xcc, 0xcc, 0xcc, 0xcc, 0xfc, 0x78},
    {0x30, 0x70, 0x70, 0x30, 0x30, 0x30, 0xfc, 0xfc},
    {0x78, 0xfc, 0xcc, 0x0c, 0x38, 0x70, 0xfc, 0xfc},
    {0xfc, 0x0c, 0x0c, 0x38, 0x0c, 0xcc, 0xfc, 0x78},
    {0x18, 0x38, 0x78, 0xd8, 0x98, 0xfc, 0xfc, 0x18},
    {0xfc, 0xfc, 0x80, 0x78, 0x0c, 0x0c, 0xfc, 0x78},
    {0x78, 0xf8, 0x80, 0xf8, 0xfc, 0x84, 0xfc, 0x78},
    {0xfc, 0xfc, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x30},
    {0x78, 0xcc, 0xcc, 0x78, 0xfc, 0xcc, 0xfc, 0x78},
    {0x78, 0xcc, 0xcc, 0xfc, 0x7c, 0x0c, 0xfc, 0x78}
};

//////////////////////////////////////////////////////////////////////////////////
//Buzzer
const unsigned int size_bornfire = 37;
const unsigned int fre_bornfire[] PROGMEM = 
{ 
  587, 466, 523, 587, 523, 466, 587, 523, 466,
  523, 466, 523, 466, 622, 466, 622, 587, 466,
  587, 466, 523, 466, 392, 523, 587, 523, 622,
  587, 523, 466, 523, 466, 523, 587, 523, 466
};
const unsigned int dura_bornfire[] PROGMEM = 
{ 
  175, 175, 162, 176, 187, 188, 212, 176, 200, 187, 163, 188, 162, 175, 
  175, 162, 200, 187, 175, 175, 200, 188, 187, 163, 187, 162, 175, 187, 
  188, 175, 187, 162, 187, 188, 188, 188 
};
const unsigned int start_bornfire[] PROGMEM = 
{ 
  12, 362, 725, 887, 1063, 1250, 1788, 2150, 2501, 2864, 3226, 3589, 3777, 
  3939, 4114, 4640, 5002, 5365, 5703, 6078, 6415, 6615, 6803, 6990, 7516, 
  7866, 8229, 8404, 8591, 8954, 9280, 9655, 10005, 10380, 10730, 11080, 11268,  
};

const unsigned int size_takeonme PROGMEM = 25;
const unsigned int fre_takeonme[] PROGMEM = 
{
    493, 493, 392, 329, 329, 440, 440, 440, 
    554, 554, 587, 659, 587, 587, 587, 440, 
    392, 493, 493, 493, 440, 440, 493, 440, 
};
const unsigned int dura_takeonme[] PROGMEM = 
{
    125, 137, 150, 150, 137, 125, 138, 124, 
    138, 138, 138, 137, 138, 138, 162, 126, 
    138, 151, 150, 151, 150, 138, 163, 125, 
};
const unsigned int start_takeonme[] PROGMEM = 
{
    12, 150, 312, 475, 775, 1112, 1400, 1726, 
    1888, 2038, 2188, 2339, 2501, 2651, 2802, 2977, 
    3290, 3603, 3904, 4229, 4380, 4542, 4692, 4855, 
    4980, 
};

const unsigned int size_howls_0 PROGMEM = 37;
const unsigned int fre_howls_0[] PROGMEM = 
{
    311, 415, 493, 622, 554, 493, 466, 493, 
    415, 493, 622, 830, 830, 830, 739, 659, 
    739, 493, 622, 739, 932, 830, 739, 698, 
    739, 830, 739, 698, 622, 554, 493, 554, 
    622, 554, 415, 466, 
};
const unsigned int dura_howls_0[] PROGMEM = 
{
    238, 250, 251, 500, 250, 250, 250, 500, 
    251, 225, 275, 463, 238, 225, 275, 251, 
    475, 237, 263, 225, 513, 238, 250, 238, 
    250, 500, 263, 225, 238, 262, 250, 250, 
    263, 250, 263, 500, 
};
const unsigned int start_howls_0[] PROGMEM = 
{
    12, 250, 500, 751, 1251, 1501, 1751, 2001, 
    2751, 3027, 3252, 3527, 4002, 4265, 4490, 4765, 
    5016, 5766, 6003, 6266, 6491, 7004, 7267, 7517, 
    7755, 8005, 8505, 8768, 9256, 9494, 9756, 10006, 
    10256, 10519, 10769, 11032, 11532, 
};

const unsigned int size_eat PROGMEM = 4;
const unsigned int fre_eat[] PROGMEM = 
{
    739, 783, 830,
};
const unsigned int dura_eat[] PROGMEM = 
{
    163, 187, 225,
};
const unsigned int start_eat[] PROGMEM = 
{
    12, 175, 362, 587,
};

const unsigned int size_rickroll PROGMEM = 19;
const unsigned int fre_rickroll[] PROGMEM = 
{
    554, 622, 415, 622, 698, 830, 739, 698, 
    554, 554, 622, 415, 415, 415, 466, 554, 
    466, 554, 
};
const unsigned int dura_rickroll[] PROGMEM = 
{
    338, 387, 200, 362, 375, 100, 100, 87, 
    88, 363, 363, 375, 62, 87, 100, 100, 
    113, 112, 
};
const unsigned int start_rickroll[] PROGMEM = 
{
    12, 550, 1125, 1500, 2075, 2613, 2713, 2813, 
    2900, 3000, 3588, 4126, 4864, 4939, 5026, 5126, 
    5226, 5339, 5451, 
};

const unsigned int size_guy PROGMEM = 35;
const unsigned int fre_guy[] PROGMEM = 
{
    311, 392, 493, 392, 329, 311, 311, 311, 
    293, 311, 311, 392, 493, 392, 311, 554, 
    554, 493, 466, 349, 466, 293, 466, 349, 
    466, 349, 311, 293, 311, 311, 311, 311, 
    311, 311, 
};
const unsigned int dura_guy[] PROGMEM = 
{
    188, 188, 200, 175, 188, 187, 188, 175, 
    175, 188, 175, 200, 175, 200, 188, 375, 
    175, 188, 187, 163, 188, 200, 163, 175, 
    200, 162, 200, 387, 175, 162, 162, 187, 
    188, 200, 
};
const unsigned int start_guy[] PROGMEM = 
{
    12, 412, 600, 1026, 1413, 1614, 1826, 2039, 
    3027, 3202, 3415, 3602, 3815, 4203, 4615, 4828, 
    5228, 5403, 5616, 6428, 6841, 7029, 7441, 7829, 
    8217, 8630, 8817, 9205, 9630, 9830, 10030, 10818, 
    11030, 11243, 11443, 
};

const unsigned int size_feelgood PROGMEM = 34;
const unsigned int fre_feelgood[] PROGMEM = 
{
    311, 349, 369, 415, 466, 554, 466, 415, 
    369, 311, 369, 415, 349, 311, 277, 277, 
    311, 349, 369, 415, 466, 554, 466, 415, 
    415, 369, 311, 369, 415, 369, 349, 277, 
    311, 
};
const unsigned int dura_feelgood[] PROGMEM = 
{
    163, 150, 162, 187, 162, 162, 362, 162, 
    163, 163, 175, 337, 176, 163, 150, 150, 
    175, 175, 200, 188, 162, 325, 175, 150, 
    301, 175, 175, 163, 313, 163, 137, 176, 
    162, 
};
const unsigned int start_feelgood[] PROGMEM = 
{
    12, 175, 500, 800, 1150, 1500, 1800, 2475, 
    2637, 2975, 3288, 3613, 4100, 4288, 4451, 4951, 
    5101, 5276, 5601, 5926, 6264, 6601, 6926, 7101, 
    7601, 7902, 8077, 8414, 8739, 9414, 9577, 9714, 
    9890, 10052, 
};


#endif//tie_data_progmem
