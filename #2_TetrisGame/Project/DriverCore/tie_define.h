#pragma once

#include "stdint.h"
#include "string.h"
#include "stdbool.h"
#include "tie_driver.h"

typedef unsigned char	_uc;
typedef uint8_t			_u8;
typedef uint16_t		_ui;
typedef uint32_t		_ul;

#define bitRead(res, bit) 				(((res) >> (bit)) & 0x01)
#define bitSet(res, bit) 				((res) |= (1UL << (bit)))
#define bitClear(res, bit) 				((res) &= ~(1UL << (bit)))
#define bitToggle(res, bit) 			((res) ^= (1UL << (bit)))
#define bitWrite(res, bit, val)			((val) ? bitSet(res, bit) : bitClear(res, bit))

//Init
static const uint16_t size_init = 2;
static const uint16_t fre_init[] = { 466 };
static const uint16_t dura_init[] = { 50 };
static const uint16_t start_init[] = { 12, 62 };

//BornFire
static const uint16_t size_born_fire = 37;
static const uint16_t fre_born_fire[] = 
{ 
  587, 466, 523, 587, 523, 466, 587, 523, 466,
  523, 466, 523, 466, 622, 466, 622, 587, 466,
  587, 466, 523, 466, 392, 523, 587, 523, 622,
  587, 523, 466, 523, 466, 523, 587, 523, 466
};
static const uint16_t dura_born_fire[] = 
{ 
  175, 175, 162, 176, 187, 188, 212, 176, 200, 187, 163, 188, 162, 175, 
  175, 162, 200, 187, 175, 175, 200, 188, 187, 163, 187, 162, 175, 187, 
  188, 175, 187, 162, 187, 188, 188, 188 
};
static const uint16_t start_born_fire[] = 
{ 
  12, 362, 725, 887, 1063, 1250, 1788, 2150, 2501, 2864, 3226, 3589, 3777, 
  3939, 4114, 4640, 5002, 5365, 5703, 6078, 6415, 6615, 6803, 6990, 7516, 
  7866, 8229, 8404, 8591, 8954, 9280, 9655, 10005, 10380, 10730, 11080, 11268,  
};


//Take on me
static const uint16_t size_d = 25;
static const uint16_t fre_d[] = 
{
    493, 493, 392, 329, 329, 440, 440, 440, 
    554, 554, 587, 659, 587, 587, 587, 440, 
    392, 493, 493, 493, 440, 440, 493, 440, 
};
static const uint16_t dura_d[] = 
{
    163, 187, 200, 200, 200, 175, 189, 175, 
    175, 188, 188, 187, 201, 175, 213, 176, 
    187, 188, 188, 200, 200, 188, 200, 162, 
};
static const uint16_t start_d[] = 
{
    12, 200, 425, 637, 1026, 1476, 1863, 2302, 
    2527, 2715, 2915, 3116, 3328, 3541, 3741, 3966, 
    4380, 4805, 5206, 5632, 5832, 6045, 6258, 6471, 
    6633, 
};

//Guy
static const uint16_t size_guy = 35;
static const uint16_t fre_guy[] = 
{
    311, 392, 493, 392, 329, 311, 311, 311, 
    293, 311, 311, 392, 493, 392, 311, 554, 
    554, 493, 466, 349, 466, 293, 466, 349, 
    466, 349, 311, 293, 311, 311, 311, 311, 
    311, 311, 
};
static const uint16_t dura_guy[] = 
{
    188, 188, 200, 175, 188, 188, 188, 175, 
    175, 188, 175, 200, 175, 200, 188, 375, 
    176, 175, 188, 163, 188, 200, 162, 188, 
    200, 162, 200, 388, 175, 163, 176, 201, 
    175, 200, 
};
static const uint16_t start_guy[] = 
{
    12, 412, 600, 1025, 1413, 1613, 1826, 2039, 
    3028, 3203, 3416, 3603, 3816, 4204, 4617, 4830, 
    5230, 5406, 5618, 6431, 6832, 7020, 7445, 7820, 
    8221, 8634, 8821, 9209, 9634, 9834, 10022, 10811, 
    11036, 11249, 11449, 
};

//Grand Line
static const uint16_t size_op1 = 22;
static const uint16_t fre_op1[] = 
{
    329, 329, 392, 440, 392, 440, 587, 587, 
    783, 698, 587, 587, 523, 698, 523, 587, 
    587, 523, 493, 440, 392, 
};
static const uint16_t dura_op1[] = 
{
    388, 100, 138, 100, 125, 138, 751, 225, 
    488, 513, 500, 225, 238, 262, 501, 125, 
    126, 125, 113, 112, 500, 
};
static const uint16_t start_op1[] = 
{
    12, 400, 500, 638, 738, 863, 1001, 2265, 
    2515, 3003, 3516, 4279, 4504, 4742, 5004, 5743, 
    6005, 6256, 6506, 6619, 6731, 7231, 
};