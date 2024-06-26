#pragma once

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <mtd/mtd-user.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>

#include <math.h>

typedef unsigned char	_uc;
typedef uint8_t			_u8;
typedef uint16_t		_ui;
typedef uint32_t		_ul;

#define bitRead(res, bit) 				(((res) >> (bit)) & 0x01)
#define bitSet(res, bit) 				((res) |= (1UL << (bit)))
#define bitClear(res, bit) 				((res) &= ~(1UL << (bit)))
#define bitToggle(res, bit) 			((res) ^= (1UL << (bit)))
#define bitWrite(res, bit, val)			((val) ? bitSet(res, bit) : bitClear(res, bit))

