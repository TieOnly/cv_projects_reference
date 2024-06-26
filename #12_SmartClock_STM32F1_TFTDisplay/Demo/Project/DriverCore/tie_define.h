#pragma once

#include "stdint.h"
#include "string.h"
#include "stdbool.h"
#include "stdio.h"

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


